// Copyright 2015, Christopher Chambers
// Distributed under the GNU GPL v3. See LICENSE.TXT for details.

#include "SbbmRegisterInfo.h"
#include "MCTargetDesc/SbbmMCTargetDesc.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/IR/Function.h"
#include "llvm/Target/TargetSubtargetInfo.h"
#include "llvm/Target/TargetInstrInfo.h"

#define GET_REGINFO_TARGET_DESC
#include "SbbmGenRegisterInfo.inc"

using namespace llvm;

SbbmRegisterInfo::SbbmRegisterInfo()
  : SbbmGenRegisterInfo(Sbbm::LR)
{ }

bool
SbbmRegisterInfo::requiresRegisterScavenging(const MachineFunction &MF) const
{
  // FIXME: Could be more conservative here.
  return true;
}

const uint16_t *
SbbmRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const
{
  static const MCPhysReg EmptySaveList[] = { 0 };

  if (MF->getFunction()->hasFnAttribute(llvm::Attribute::NoReturn)) {
      return EmptySaveList;
  }
  return CC_Save_SaveList;
}

const uint32_t *SbbmRegisterInfo::getCallPreservedMask(CallingConv::ID) const {
  return CC_Save_RegMask;
}

unsigned SbbmRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return Sbbm::SP;
}

BitVector SbbmRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  Reserved
    .set(Sbbm::SP)
    .set(Sbbm::LR);
  return Reserved;
}

void ExpandFrameIndex(
  const MachineOperand &FrameIndexOp, MachineBasicBlock::iterator II, int SPAdj,
  unsigned int DstReg)
{
  MachineBasicBlock &BB = *II->getParent();
  MachineFunction &MF = *BB.getParent();
  const MachineFrameInfo &MFI = *MF.getFrameInfo();
  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
  const int FrameIndex = FrameIndexOp.getIndex();

  //  MOVrr $dst, sp
  BuildMI(BB, II, II->getDebugLoc(), TII.get(Sbbm::MOVrr), DstReg)
    .addReg(Sbbm::SP);

  //  (ADDri|SUBri) $tmp, offset
  int Offset = MFI.getObjectOffset(FrameIndex) + MFI.getStackSize() + SPAdj;
  if (Offset > 0) {
    BuildMI(BB, II, II->getDebugLoc(), TII.get(Sbbm::ADDri), DstReg)
      .addReg(DstReg)
      .addImm(Offset);
  } else if (Offset < 0) {
    // Can Offset be less than zero?  Is SPAdj ever sufficiently negative to
    // make this happen?
    BuildMI(BB, II, II->getDebugLoc(), TII.get(Sbbm::SUBri), DstReg)
      .addReg(DstReg)
      .addImm(-Offset);
  }
}

void SbbmRegisterInfo::eliminateFrameIndex(
  MachineBasicBlock::iterator II, int SPAdj, unsigned FIOperandNum,
  RegScavenger *RS) const
{
  MachineInstr &MI = *II;
  MachineOperand &FrameIndexOp = MI.getOperand(FIOperandNum);

  switch (MI.getOpcode()) {
  case Sbbm::MOVra: case Sbbm::MOVra_P:
    ExpandFrameIndex(FrameIndexOp, II, SPAdj, MI.getOperand(0).getReg());
    MI.removeFromParent();
    break;
  case Sbbm::LDR:   case Sbbm::LDR_P:
  case Sbbm::LDRH:  case Sbbm::LDRH_P:
  case Sbbm::LDRSH: case Sbbm::LDRSH_P:
  case Sbbm::LDRB:  case Sbbm::LDRB_P:
  case Sbbm::LDRSB: case Sbbm::LDRSB_P: {
    unsigned int Dst = MI.getOperand(0).getReg();
    ExpandFrameIndex(FrameIndexOp, II, SPAdj, Dst);
    FrameIndexOp.ChangeToRegister(Dst, false, false, true);
    break;
  }
  case Sbbm::STR:  case Sbbm::STR_P:
  case Sbbm::STRH: case Sbbm::STRH_P:
  case Sbbm::STRB: case Sbbm::STRB_P: {
    unsigned int Tmp = RS->FindUnusedReg(&Sbbm::GRRegsRegClass);
    if (Tmp == 0) {
      report_fatal_error("Could not find an unused register to use when eliminating a frame index.");
    }
    ExpandFrameIndex(FrameIndexOp, II, SPAdj, Tmp);
    FrameIndexOp.ChangeToRegister(Tmp, false, false, true);
    break;
  }
  default:
    report_fatal_error("frame index elimination is not supported for this instruction");
  }
}
