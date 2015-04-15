// FIXME: Add standard header.

#ifndef SBBM_TARGETASMINFO_H
#define SBBM_TARGETASMINFO_H

#include "llvm/MC/MCAsmInfo.h"

namespace llvm {

class StringRef;

class SbbmMCAsmInfo : public MCAsmInfo {
public:
  explicit SbbmMCAsmInfo(StringRef TT);

private:
  virtual void anchor();
};

} // namespace llvm

#endif
