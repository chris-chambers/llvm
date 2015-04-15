// FIXME: Add standard header.

#include "SbbmFrameLowering.h"

using namespace llvm;

SbbmFrameLowering::SbbmFrameLowering()
  : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, 4, 0)
{ }

bool SbbmFrameLowering::hasFP(const MachineFunction &MF) const {
  llvm_unreachable("hasFP not implemented");
}

void SbbmFrameLowering::emitPrologue(MachineFunction &MF) const {
  llvm_unreachable("emitPrologue not implemented");
}

void SbbmFrameLowering::emitEpilogue(
  MachineFunction &MF, MachineBasicBlock &MBB) const
{
  llvm_unreachable("emitEpilogue not implemented");
}
