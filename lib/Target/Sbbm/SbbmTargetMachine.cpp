// FIXME: Add standard header.

#include "SbbmTargetMachine.h"
#include "TargetInfo/SbbmTargetInfo.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

SbbmTargetMachine::SbbmTargetMachine(
  const Target &T, StringRef TT, StringRef CPU, StringRef FS,
  const TargetOptions &Options, Reloc::Model RM, CodeModel::Model CM,
  CodeGenOpt::Level OL)
  : LLVMTargetMachine(T, TT, CPU, FS, Options, RM, CM, OL)
  , Subtarget(TT, CPU, FS)
{ }

const TargetSubtargetInfo *SbbmTargetMachine::getSubtargetImpl() const {
  return &Subtarget;
}

extern "C" void LLVMInitializeSbbmTarget() {
  RegisterTargetMachine<SbbmTargetMachine> X(TheSbbmTarget);
}
