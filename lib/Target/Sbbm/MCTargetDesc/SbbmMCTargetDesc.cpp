// FIXME: Add standard header.

#include "SbbmMCTargetDesc.h"
#include "SbbmMCAsmInfo.h"
#include "TargetInfo/SbbmTargetInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/TargetRegistry.h"

#define GET_INSTRINFO_MC_DESC
#include "SbbmGenInstrInfo.inc"

#define GET_REGINFO_MC_DESC
#include "SbbmGenRegisterInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "SbbmGenSubtargetInfo.inc"

using namespace llvm;

namespace {

static MCAsmInfo *createMCAsmInfo(const MCRegisterInfo &MRI, StringRef TT) {
  return new SbbmMCAsmInfo(TT);
}

static MCInstrInfo *createMCInstrInfo() {
  MCInstrInfo *InstrInfo = new MCInstrInfo();
  InitSbbmMCInstrInfo(InstrInfo);
  return InstrInfo;
}

static MCRegisterInfo *createMCRegInfo(StringRef TT) {
  MCRegisterInfo *RegisterInfo = new MCRegisterInfo();
  InitSbbmMCRegisterInfo(RegisterInfo, Sbbm::LR);
  return RegisterInfo;
}

static MCSubtargetInfo *createMCSubtargetInfo(
  StringRef TT, StringRef CPU, StringRef FS)
{
  MCSubtargetInfo *SubtargetInfo = new MCSubtargetInfo();
  InitSbbmMCSubtargetInfo(SubtargetInfo, TT, CPU, FS);
  return SubtargetInfo;
}

} // anonymous namespace

extern "C" void LLVMInitializeSbbmTargetMC() {
  TargetRegistry::RegisterMCAsmInfo(TheSbbmTarget, createMCAsmInfo);
  TargetRegistry::RegisterMCInstrInfo(TheSbbmTarget, createMCInstrInfo);
  TargetRegistry::RegisterMCRegInfo(TheSbbmTarget, createMCRegInfo);
  TargetRegistry::RegisterMCSubtargetInfo(TheSbbmTarget, createMCSubtargetInfo);
}
