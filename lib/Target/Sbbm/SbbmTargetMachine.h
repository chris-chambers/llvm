// FIXME: Add standard header.

#ifndef SBBM_TARGETMACHINE_H
#define SBBM_TARGETMACHINE_H

#include "SbbmSubtarget.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class SbbmTargetMachine : public LLVMTargetMachine {
public:
  SbbmTargetMachine(
    const Target &T, StringRef TT, StringRef CPU, StringRef FS,
    const TargetOptions &Options, Reloc::Model RM, CodeModel::Model CM,
    CodeGenOpt::Level OL);

  virtual const TargetSubtargetInfo *getSubtargetImpl() const override;

  virtual TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

private:
  SbbmSubtarget Subtarget;
};

} // namespace llvm

#endif // include guard
