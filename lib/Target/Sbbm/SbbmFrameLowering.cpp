// FIXME: Add standard header.

#include "SbbmFrameLowering.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
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

  llvm_unreachable("prologue with non-zero stack size not yet implemented.");
}

void SbbmFrameLowering::emitEpilogue(
  MachineFunction &MF, MachineBasicBlock &MBB) const
{
  uint64_t StackSize = computeStackSize(MF, getStackAlignment());
  // No need for an epilogue if the stack size was zero.
  if (!StackSize) {
    return;
  }

  llvm_unreachable("epilogue with non-zero stack size not yet implemented.");
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
