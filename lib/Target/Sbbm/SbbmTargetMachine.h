// Copyright 2015, Christopher Chambers
// Distributed under the GNU GPL v3. See LICENSE.TXT for details.

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

  virtual TargetLoweringObjectFile *getObjFileLowering()
    const override;

  virtual const TargetSubtargetInfo *getSubtargetImpl() const override;

  virtual void addAnalysisPasses(PassManagerBase &PM) override;

  virtual bool addPassesToEmitFile(
    PassManagerBase &PM, formatted_raw_ostream &Out, CodeGenFileType FileType,
    bool DisableVerify = true, AnalysisID StartAfter = nullptr,
    AnalysisID StopAfter = nullptr) override;

  virtual TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

private:
  SbbmSubtarget Subtarget;
};

} // namespace llvm

#endif // include guard
