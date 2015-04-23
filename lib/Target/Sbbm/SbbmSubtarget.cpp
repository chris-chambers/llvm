// FIXME: Add standard header.

#include "SbbmSubtarget.h"
#include "SbbmFrameLowering.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Target/TargetSelectionDAGInfo.h"

#define DEBUG_TYPE "sbbm-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "SbbmGenSubtargetInfo.inc"

using namespace llvm;

namespace {
  // See: http://llvm.org/docs/LangRef.html#data-layout
  static const DataLayout DL(
    "e-"
    "p:32:32-"
    "i1:8:32-"
    "i8:8:32-"
    "i16:16:32-"
    "i32:32:32-"
    "i64:32-"
    "a:0:32-"
    "n32");
}

SbbmSubtarget::SbbmSubtarget(
  const std::string &TT, const std::string &CPU, const std::string &FS,
  const SbbmTargetMachine &TM)
  : SbbmGenSubtargetInfo(TT, CPU, FS)
  , TargetLowering(TM)
{ }

const DataLayout *SbbmSubtarget::getDataLayout() const {
  return &DL;
}

const TargetFrameLowering *SbbmSubtarget::getFrameLowering() const {
  static const SbbmFrameLowering FrameLowering;

  return &FrameLowering;
}

const SbbmInstrInfo *SbbmSubtarget::getInstrInfo() const {
  static const SbbmInstrInfo InstrInfo;

  return &InstrInfo;
}

const SbbmRegisterInfo *SbbmSubtarget::getRegisterInfo() const {
  static const SbbmRegisterInfo RegisterInfo;

  return &RegisterInfo;
}

const TargetSelectionDAGInfo *SbbmSubtarget::getSelectionDAGInfo() const {
  static const TargetSelectionDAGInfo TSI(&DL);

  return &TSI;
}

void SbbmSubtarget::anchor() {}
