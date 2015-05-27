// Copyright 2015, Christopher Chambers
// Distributed under the GNU GPL v3. See LICENSE.TXT for details.

#include "SbbmTargetInfo.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

// Initialize the one and only SBBM Target.
Target llvm::TheSbbmTarget;

extern "C" void LLVMInitializeSbbmTargetInfo() {
  RegisterTarget<Triple::sbbm> X(TheSbbmTarget, "sbbm", "SBBM");
}
