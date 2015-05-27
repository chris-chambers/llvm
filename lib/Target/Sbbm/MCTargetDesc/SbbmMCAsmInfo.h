// Copyright 2015, Christopher Chambers
// Distributed under the GNU GPL v3. See LICENSE.TXT for details.

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
