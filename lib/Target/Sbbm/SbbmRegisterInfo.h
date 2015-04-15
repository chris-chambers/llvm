// FIXME: Add standard header.

#ifndef SBBM_REGISTERINFO_H
#define SBBM_REGISTERINFO_H

#include "llvm/Target/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "SbbmGenRegisterInfo.inc"

namespace llvm {

struct SbbmRegisterInfo : public SbbmGenRegisterInfo {
public:
  SbbmRegisterInfo();

  const uint16_t *getCalleeSavedRegs(const MachineFunction *MF = nullptr)
    const override;

  unsigned getFrameRegister(const MachineFunction &MF) const;

  BitVector getReservedRegs(const MachineFunction &MF) const override;

  void eliminateFrameIndex(
    MachineBasicBlock::iterator II, int SPAdj, unsigned FIOperandNum,
    RegScavenger *RS = nullptr)
    const override;

};

} // namespace llvm

#endif // include guard
