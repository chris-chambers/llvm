// FIXME: Add standard header.

#include "SbbmInstPrinter.h"
#include "SbbmInstrInfo.h"
#include "llvm/Support/raw_ostream.h"

#define DEBUG_TYPE "sbbm-inst-printer"

using namespace llvm;

#include "SbbmGenAsmWriter.inc"

void SbbmInstPrinter::printInst(
  const MCInst *MI, raw_ostream &O, StringRef Annot)
{
  printInstruction(MI, O);
  printAnnotation(O, Annot);
}

void SbbmInstPrinter::printOperand(
  const MCInst *MI, unsigned OpNo, raw_ostream &O)
{
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg()) {
    printRegName(O, Op.getReg());
    return;
  }

  if (Op.isImm()) {
    O << "#" << Op.getImm();
    return;
  }

  llvm_unreachable(
    "printOperand: only register and immediate operands are currently supported");
}

void SbbmInstPrinter::printRegName(raw_ostream &OS, unsigned RegNo) const {
  OS << StringRef(getRegisterName(RegNo)).lower();
}
