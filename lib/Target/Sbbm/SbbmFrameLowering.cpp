// Copyright 2015, Christopher Chambers
// Distributed under the GNU GPL v3. See LICENSE.TXT for details.

#include "SbbmFrameLowering.h"
#include "SbbmInstrInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/Target/TargetInstrInfo.h"
#include "llvm/Target/TargetSubtargetInfo.h"

using namespace llvm;

namespace {

uint64_t computeStackSize(MachineFunction &MF, unsigned int StackAlign);

} // anonymous namespace

SbbmFrameLowering::SbbmFrameLowering()
  : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, 4, 0)
{ }

bool SbbmFrameLowering::hasFP(const MachineFunction &MF) const {
  return
    MF.getTarget().Options.DisableFramePointerElim(MF) ||
    MF.getFrameInfo()->hasVarSizedObjects();
}

void SbbmFrameLowering::emitPrologue(MachineFunction &MF) const {
  uint64_t StackSize = computeStackSize(MF, getStackAlignment());
  // No need for a prologue if the stack size is zero.
  if (!StackSize) {
    return;
  }

  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
  MachineBasicBlock &MBB = MF.front();
  MachineBasicBlock::iterator MBBI = MBB.begin();
  DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();

  BuildMI(MBB, MBBI, DL, TII.get(Sbbm::SUBri), Sbbm::SP)
      .addReg(Sbbm::SP)
      .addImm(StackSize)
      .setMIFlag(MachineInstr::FrameSetup);
}

void SbbmFrameLowering::emitEpilogue(
  MachineFunction &MF, MachineBasicBlock &MBB) const
{
  uint64_t StackSize = computeStackSize(MF, getStackAlignment());
  // No need for an epilogue if the stack size was zero.
  if (!StackSize) {
    return;
  }

  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
  DebugLoc DL = MBBI->getDebugLoc();

  BuildMI(MBB, MBBI, DL, TII.get(Sbbm::ADDri), Sbbm::SP)
      .addReg(Sbbm::SP)
      .addImm(StackSize)
      .setMIFlag(MachineInstr::FrameSetup);
}

namespace {

uint64_t computeStackSize(MachineFunction &MF, unsigned int StackAlign) {
  uint64_t StackSize = MF.getFrameInfo()->getStackSize();
  if (StackAlign > 0) {
    StackSize = RoundUpToAlignment(StackSize, StackAlign);
  }
  return StackSize;
}

} // anonymous namespace
