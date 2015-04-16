// FIXME: Add standard header.

#define DEBUG_TYPE "sbbm-asm-printer"

#include "SbbmSubtarget.h"
#include "TargetInfo/SbbmTargetInfo.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Support/TargetRegistry.h"
#include <tuple>

using namespace llvm;

namespace {

MCInst LowerInstruction(const MachineInstr &MI);
std::tuple<bool, MCOperand> LowerOperand(
  const MachineInstr &MI, const MachineOperand &MO);

class SbbmAsmPrinter : public AsmPrinter {
public:
  SbbmAsmPrinter(TargetMachine &TM, MCStreamer &Streamer)
    : AsmPrinter(TM, Streamer)
  { }

  virtual const char *getPassName() const override {
    return "SBBM Assembly Printer";
  }

  virtual void EmitInstruction(const MachineInstr *MI) override {
    MCInst Lowered = LowerInstruction(*MI);
    EmitToStreamer(OutStreamer, Lowered);
  }
};

MCInst LowerInstruction(const MachineInstr &MI) {
  MCInst Out;
  Out.setOpcode(MI.getOpcode());

  for (const MachineOperand &MO : MI.operands()) {
    bool Lowered;
    MCOperand MCOp;
    std::tie(Lowered, MCOp) = LowerOperand(MI, MO);

    if (Lowered) {
      Out.addOperand(MCOp);
    }
  }
  return Out;
}

std::tuple<bool, MCOperand> LowerOperand(const MachineInstr &MI, const MachineOperand &MO) {
  switch (MO.getType()) {
  default:
    llvm_unreachable("unknown operand type");
  case MachineOperand::MO_Register:
    // Ignore all implicit register operands.
    if (MO.isImplicit()) {
      return std::make_tuple(false, MCOperand());
    }
    return std::make_tuple(true, MCOperand::CreateReg(MO.getReg()));
  case MachineOperand::MO_Immediate:
    return std::make_tuple(true, MCOperand::CreateImm(MO.getImm()));
  }
}

} // anonymous namespace

extern "C" void LLVMInitializeSbbmAsmPrinter() {
  RegisterAsmPrinter<SbbmAsmPrinter> X(TheSbbmTarget);
}
