// FIXME: Add standard header.

#ifndef SBBM_FRAMELOWERING_H
#define SBBM_FRAMELOWERING_H

#include "llvm/Target/TargetFrameLowering.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class SbbmSubtarget;

class SbbmFrameLowering : public TargetFrameLowering {
public:
  SbbmFrameLowering();

  void emitPrologue(MachineFunction &MF) const;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const;
  bool hasFP(const MachineFunction &MF) const;
};

} // namespace llvm

#endif // include guard
