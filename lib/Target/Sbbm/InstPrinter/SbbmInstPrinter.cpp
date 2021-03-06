// Copyright 2015, Christopher Chambers
// Distributed under the GNU GPL v3. See LICENSE.TXT for details.

#include "SbbmInstPrinter.h"
#include "SbbmInstrInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/raw_ostream.h"

#define DEBUG_TYPE "sbbm-inst-printer"

using namespace llvm;

#include "SbbmGenAsmWriter.inc"

void SbbmInstPrinter::printInst(
  const MCInst *MI, raw_ostream &O, StringRef Annot)
{
  printPredicate(MI, O);
  printInstruction(MI, O);
  printAnnotation(O, Annot);
}

void SbbmInstPrinter::printPredicate(const MCInst *MI, raw_ostream &O) {
  const auto FirstVarArg = MII.get(MI->getOpcode()).getNumOperands();
  bool First = true;
  for (unsigned int i = FirstVarArg, e = MI->getNumOperands(); i < e; ++i) {
    const MCOperand &Op = MI->getOperand(i);
    if (Op.isReg()) {
      const unsigned int Reg = Op.getReg();
      bool PredSense;
      if (Sbbm::PredRegsRegClass.contains(Reg)) {
        PredSense = true;
      } else if (Sbbm::NPredRegsRegClass.contains(Reg)) {
        PredSense = false;
      } else {
        continue;
        //llvm_unreachable("invalid predicate register");
      }

      if (First) {
        First = false;
        O << '\t';
      }

      O << "{";
      if (PredSense == true) {
        printRegName(O, Reg);
        O << ", #1, #1";
      } else {
        printRegName(O, SbbmInstrInfo::ReversePredReg(Reg));
        O << ", #0, #0";
      }
      O << "}";
    }
  }
}

void SbbmInstPrinter::printOperand(
  const MCInst *MI, unsigned OpNo, raw_ostream &O)
{
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg()) {
    unsigned int Reg = Op.getReg();
    if (Sbbm::NPredRegsRegClass.contains(Reg)) {
      Reg = SbbmInstrInfo::ReversePredReg(Reg);
    }
    printRegName(O, Reg);
    return;
  }

  if (Op.isImm()) {
    O << "#" << Op.getImm();
    return;
  }

  if (Op.isExpr()) {
    const MCExpr *Expr = Op.getExpr();
    if (auto SRE = dyn_cast<MCSymbolRefExpr>(Expr)) {
      O << '=' << SRE->getSymbol();
      return;
    }

    llvm_unreachable("printOperand: unhandled operand expression");
  }

  llvm_unreachable("printOperand: unhandled operand kind");
}

void SbbmInstPrinter::printAddrModeMemSrc(
  const MCInst *MI, unsigned OpNum, raw_ostream &O)
{
  const MCOperand &Op1 = MI->getOperand(OpNum);
  if (Op1.isReg()) {
    O << "[";
    printRegName(O, Op1.getReg());
    O << "]";
  } else if (Op1.isExpr()) {
    const MCExpr *Expr = Op1.getExpr();
    switch (Expr->getKind()) {
    case MCExpr::SymbolRef:
      O << '=' << cast<MCSymbolRefExpr>(Expr)->getSymbol();
      break;
    default:
      llvm_unreachable("printAddrModeMemSrc: invalid expression kind");
    }
  } else {
    llvm_unreachable("printAddrModeMemSrc: invalid operand");
  }
}

void SbbmInstPrinter::printRegName(raw_ostream &OS, unsigned RegNo) const {
  OS << StringRef(getRegisterName(RegNo)).lower();
}
