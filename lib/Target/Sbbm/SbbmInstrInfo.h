// FIXME: Add standard header.

#ifndef SBBM_INSTRUCTIONINFO_H
#define SBBM_INSTRUCTIONINFO_H

#include "SbbmRegisterInfo.h"
#include "llvm/Target/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "SbbmGenInstrInfo.inc"

namespace llvm {

class SbbmInstrInfo : public SbbmGenInstrInfo {
public:
  SbbmInstrInfo();

  /// getRegisterInfo - TargetInstrInfo is a superset of MRegister info.  As
  /// such, whenever a client has an instance of instruction info, it should
  /// always be able to get register info as well (through this method).
  const SbbmRegisterInfo &getRegisterInfo() const { return RI; }

  virtual void copyPhysReg(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator I, DebugLoc DL,
    unsigned DestReg, unsigned SrcReg, bool KillSrc) const override;

private:
  virtual void anchor();

  const SbbmRegisterInfo RI;
};

} // namespace llvm

#endif // include guard
