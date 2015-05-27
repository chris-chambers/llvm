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

class SbbmAsmPrinter;

MCInst LowerInstruction(const SbbmAsmPrinter &Printer, const MachineInstr &MI);
std::tuple<bool, MCOperand> LowerOperand(
  const SbbmAsmPrinter &Printer, const MachineInstr &MI,
  const MachineOperand &MO);
MCOperand LowerSymbolOperand(
  const SbbmAsmPrinter &Printer, const MachineOperand &MO);

class SbbmAsmPrinter : public AsmPrinter {
public:
  SbbmAsmPrinter(TargetMachine &TM, MCStreamer &Streamer)
    : AsmPrinter(TM, Streamer)
  { }

  virtual const char *getPassName() const override {
    return "SBBM Assembly Printer";
  }

  virtual void EmitInstruction(const MachineInstr *MI) override {
    MCInst Lowered = LowerInstruction(*this, *MI);
    EmitToStreamer(OutStreamer, Lowered);
  }
};

MCInst LowerInstruction(const SbbmAsmPrinter &Printer, const MachineInstr &MI) {
  MCInst Out;
  Out.setOpcode(MI.getOpcode());

  for (const MachineOperand &MO : MI.operands()) {
    bool Lowered;
    MCOperand MCOp;
    std::tie(Lowered, MCOp) = LowerOperand(Printer, MI, MO);

    if (Lowered) {
      Out.addOperand(MCOp);
    }
  }
  return Out;
}

std::tuple<bool, MCOperand> LowerOperand(
  const SbbmAsmPrinter &Printer, const MachineInstr &MI,
  const MachineOperand &MO)
{
  switch (MO.getType()) {
  default:
    llvm_unreachable("LowerOperand: unknown operand type");
  case MachineOperand::MO_RegisterMask:
    // Ignore register masks.
    return std::make_tuple(false, MCOperand());
  case MachineOperand::MO_Register:
    // Ignore all implicit register operands.
    if (MO.isImplicit()) {
      return std::make_tuple(false, MCOperand());
    }
    return std::make_tuple(true, MCOperand::CreateReg(MO.getReg()));
  case MachineOperand::MO_Immediate:
    return std::make_tuple(true, MCOperand::CreateImm(MO.getImm()));
  case MachineOperand::MO_MachineBasicBlock:
  case MachineOperand::MO_GlobalAddress:
  case MachineOperand::MO_ExternalSymbol:
  case MachineOperand::MO_JumpTableIndex:
  case MachineOperand::MO_ConstantPoolIndex:
  case MachineOperand::MO_BlockAddress:
    return std::make_tuple(true, LowerSymbolOperand(Printer, MO));
  }
}

MCOperand LowerSymbolOperand(
  const SbbmAsmPrinter &Printer, const MachineOperand &MO)
{
  const MCSymbol *Symbol;

  switch (MO.getType()) {
  case MachineOperand::MO_MachineBasicBlock:
    Symbol = MO.getMBB()->getSymbol();
    break;
  case MachineOperand::MO_GlobalAddress:
    Symbol = Printer.getSymbol(MO.getGlobal());
    assert(MO.getOffset() == 0 && "Global addresses with offsets are not supported");
    break;
  case MachineOperand::MO_BlockAddress:
    Symbol = Printer.GetBlockAddressSymbol(MO.getBlockAddress());
    assert(MO.getOffset() == 0 && "Block addresses with offsets are not supported");
    break;
  case MachineOperand::MO_ExternalSymbol:
    Symbol = Printer.GetExternalSymbolSymbol(MO.getSymbolName());
    assert(MO.getOffset() == 0 && "External symbols with offsets are not supported");
    break;
  case MachineOperand::MO_JumpTableIndex:
    Symbol = Printer.GetJTISymbol(MO.getIndex());
    break;
  case MachineOperand::MO_ConstantPoolIndex:
    Symbol = Printer.GetCPISymbol(MO.getIndex());
    assert(MO.getOffset() == 0 && "Const pool indexes with offsets are not supported");
    break;
  default:
    llvm_unreachable("<unknown operand type>");
  }

  MCSymbolRefExpr::VariantKind Kind = MCSymbolRefExpr::VK_None;
  const MCSymbolRefExpr *MCSym = MCSymbolRefExpr::Create(Symbol, Kind, Printer.OutContext);
  return MCOperand::CreateExpr(MCSym);
}

} // anonymous namespace

extern "C" void LLVMInitializeSbbmAsmPrinter() {
  RegisterAsmPrinter<SbbmAsmPrinter> X(TheSbbmTarget);
}
