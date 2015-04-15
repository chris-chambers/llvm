// FIXME: Add standard header.

#define DEBUG_TYPE "sbbm-asm-printer"

#include "SbbmSubtarget.h"
#include "TargetInfo/SbbmTargetInfo.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

namespace {

class SbbmAsmPrinter : public AsmPrinter {
public:
  SbbmAsmPrinter(TargetMachine &TM, MCStreamer &Streamer)
    : AsmPrinter(TM, Streamer)
  { }

  virtual const char *getPassName() const override {
    return "SBBM Assembly Printer";
  }
};

} // anonymous namespace

extern "C" void LLVMInitializeSbbmAsmPrinter() {
  RegisterAsmPrinter<SbbmAsmPrinter> X(TheSbbmTarget);
}
