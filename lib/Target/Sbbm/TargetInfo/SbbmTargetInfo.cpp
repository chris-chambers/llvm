// FIXME: Add standard header

#include "SbbmTargetInfo.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

// Initialize the one and only SBBM Target.
Target llvm::TheSbbmTarget;

extern "C" void LLVMInitializeSbbmTargetInfo() {
  RegisterTarget<Triple::sbbm> X(TheSbbmTarget, "sbbm", "SBBM");
}
