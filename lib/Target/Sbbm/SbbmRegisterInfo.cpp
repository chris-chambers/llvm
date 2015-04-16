// FIXME: Add standard header.

#include "SbbmRegisterInfo.h"
#include "MCTargetDesc/SbbmMCTargetDesc.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/IR/Function.h"

#define GET_REGINFO_TARGET_DESC
#include "SbbmGenRegisterInfo.inc"

using namespace llvm;

SbbmRegisterInfo::SbbmRegisterInfo()
  : SbbmGenRegisterInfo(Sbbm::LR)
{ }

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

void SbbmRegisterInfo::eliminateFrameIndex(
  MachineBasicBlock::iterator II, int SPAdj, unsigned FIOperandNum,
  RegScavenger *RS) const
{
  llvm_unreachable("eliminateFrameIndex not implemented");
}
