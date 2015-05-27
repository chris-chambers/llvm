// Copyright 2015, Christopher Chambers
// Distributed under the GNU GPL v3. See LICENSE.TXT for details.

#include "llvm/Support/CodeGen.h"

namespace llvm {

class FunctionPass;
class SbbmTargetMachine;

/// createSbbmISelDag - This pass converts a legalized DAG into an
/// Sbbm-specific DAG, ready for instruction scheduling.
FunctionPass *createSbbmISelDag(
  SbbmTargetMachine &TM, CodeGenOpt::Level OptLevel);

extern char &SbbmDefExpanderID;
extern char &SbbmDefRemoverID;

} // namespace llvm
