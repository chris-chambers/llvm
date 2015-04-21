// FIXME: Add standard header.

#include "llvm/Support/CodeGen.h"

namespace llvm {

class FunctionPass;
class SbbmTargetMachine;

/// createSbbmISelDag - This pass converts a legalized DAG into an
/// Sbbm-specific DAG, ready for instruction scheduling.
FunctionPass *createSbbmISelDag(
  SbbmTargetMachine &TM, CodeGenOpt::Level OptLevel);

} // namespace llvm