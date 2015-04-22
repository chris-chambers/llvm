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

  virtual bool requiresRegisterScavenging(const MachineFunction &MF)
    const override;

  virtual const uint16_t *getCalleeSavedRegs(const MachineFunction *MF = nullptr)
    const override;

  virtual const uint32_t *getCallPreservedMask(CallingConv::ID) const override;

  virtual unsigned getFrameRegister(const MachineFunction &MF) const override;

  virtual BitVector getReservedRegs(const MachineFunction &MF) const override;

  virtual void eliminateFrameIndex(
    MachineBasicBlock::iterator II, int SPAdj, unsigned FIOperandNum,
    RegScavenger *RS = nullptr)
    const override;

};

} // namespace llvm

#endif // include guard
