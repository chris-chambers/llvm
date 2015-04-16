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
