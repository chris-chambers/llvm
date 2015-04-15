// FIXME: Add standard header.

#ifndef SBBM_SUBTARGET_H
#define SBBM_SUBTARGET_H

#include "SbbmInstrInfo.h"
#include "SbbmTargetLowering.h"
#include "llvm/Target/TargetSubtargetInfo.h"
#include <string>

#define GET_SUBTARGETINFO_HEADER
#include "SbbmGenSubtargetInfo.inc"

namespace llvm {

class SbbmTargetMachine;
class StringRef;

class SbbmSubtarget : public SbbmGenSubtargetInfo {
public:
  SbbmSubtarget(
    const std::string &TT, const std::string &CPU, const std::string &FS,
    const SbbmTargetMachine &TM);

  virtual const DataLayout *getDataLayout() const override;
  virtual const SbbmInstrInfo *getInstrInfo() const override;
  virtual const SbbmRegisterInfo *getRegisterInfo() const override;
  virtual const SbbmTargetLowering *getTargetLowering() const override {
    return &TargetLowering;
  }

  /// Parses features string setting specified subtarget options.
  // The implementation of ParseSubtargetFeatures is provided by tblgen, and
  // is part of the include of SbbmGenSubtargetInfo.inc above.
  void ParseSubtargetFeatures(StringRef CPU, StringRef FS);

private:
  virtual void anchor();

  const SbbmTargetLowering TargetLowering;
};

} // namespace llvm

#endif // include guard
