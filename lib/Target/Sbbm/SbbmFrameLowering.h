// Copyright 2015, Christopher Chambers
// Distributed under the GNU GPL v3. See LICENSE.TXT for details.

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
