// FIXME: Add standard header.

#include "SbbmMCTargetDesc.h"
#include "SbbmMCAsmInfo.h"
#include "InstPrinter/SbbmInstPrinter.h"
#include "TargetInfo/SbbmTargetInfo.h"
#include "llvm/MC/MCCodeGenInfo.h"
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

MCCodeGenInfo *createMCCodeGenInfo(
  StringRef TT, Reloc::Model RM, CodeModel::Model CM, CodeGenOpt::Level OL)
{
  auto GenInfo = new MCCodeGenInfo();
  GenInfo->InitMCCodeGenInfo(RM, CM, OL);
  return GenInfo;
}

static MCInstrInfo *createMCInstrInfo() {
  MCInstrInfo *InstrInfo = new MCInstrInfo();
  InitSbbmMCInstrInfo(InstrInfo);
  return InstrInfo;
}

static MCInstPrinter *createMCInstPrinter(
  const Target &T, unsigned SyntaxVariant, const MCAsmInfo &MAI,
  const MCInstrInfo &MII, const MCRegisterInfo &MRI, const MCSubtargetInfo &STI)
{
  return new SbbmInstPrinter(MAI, MII, MRI);
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
  TargetRegistry::RegisterMCCodeGenInfo(TheSbbmTarget, createMCCodeGenInfo);
  TargetRegistry::RegisterMCInstrInfo(TheSbbmTarget, createMCInstrInfo);
  TargetRegistry::RegisterMCInstPrinter(TheSbbmTarget, createMCInstPrinter);
  TargetRegistry::RegisterMCRegInfo(TheSbbmTarget, createMCRegInfo);
  TargetRegistry::RegisterMCSubtargetInfo(TheSbbmTarget, createMCSubtargetInfo);
}
