// Copyright 2015, Christopher Chambers
// Distributed under the GNU GPL v3. See LICENSE.TXT for details.

#include "SbbmTargetMachine.h"
#include "SbbmPasses.h"
#include "TargetInfo/SbbmTargetInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/PassManager.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/Pass.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Transforms/Scalar.h"

using namespace llvm;

SbbmTargetMachine::SbbmTargetMachine(
  const Target &T, StringRef TT, StringRef CPU, StringRef FS,
  const TargetOptions &Options, Reloc::Model RM, CodeModel::Model CM,
  CodeGenOpt::Level OL)
  : LLVMTargetMachine(T, TT, CPU, FS, Options, RM, CM, OL)
  , Subtarget(TT, CPU, FS, *this)
{
  initAsmInfo();
}

TargetLoweringObjectFile* SbbmTargetMachine::getObjFileLowering() const {
  // FIXME: We don't really want ELF format.  We want a very bare format.
  static TargetLoweringObjectFileELF tlof;
  return &tlof;
}

const TargetSubtargetInfo *SbbmTargetMachine::getSubtargetImpl() const {
  return &Subtarget;
}

void SbbmTargetMachine::addAnalysisPasses(PassManagerBase &PM) {
  LLVMTargetMachine::addAnalysisPasses(PM);
  PM.add(Pass::createPass(&SbbmDefExpanderID));
}

bool SbbmTargetMachine::addPassesToEmitFile(
  PassManagerBase &PM, formatted_raw_ostream &Out, CodeGenFileType FileType,
  bool DisableVerify, AnalysisID StartAfter, AnalysisID StopAfter)
{
  PM.add(Pass::createPass(&SbbmDefRemoverID));
  return LLVMTargetMachine::addPassesToEmitFile(
    PM, Out, FileType, DisableVerify, StartAfter, StopAfter);
}

TargetPassConfig *SbbmTargetMachine::createPassConfig(PassManagerBase &PM) {
  class SbbmPassConfig : public TargetPassConfig {
  public:
    SbbmPassConfig(SbbmTargetMachine *TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM)
    { }
    virtual void addIRPasses() override {
      addPass(createTailCallEliminationPass());
      addPass(createAtomicExpandPass(this->TM));
    }
    virtual bool addPreISel() override { return false; }
    virtual bool addInstSelector() override {
      addPass(createSbbmISelDag(getTM<SbbmTargetMachine>(), getOptLevel()));
      return false;
    }
    virtual void addPreSched2() override {
      if (getOptLevel() > CodeGenOpt::Less) {
        addPass(&IfConverterID);
      }
    }
    virtual void addPreEmitPass() override { }
  };

  return new SbbmPassConfig(this, PM);
}


extern "C" void LLVMInitializeSbbmTarget() {
  RegisterTargetMachine<SbbmTargetMachine> X(TheSbbmTarget);
}
