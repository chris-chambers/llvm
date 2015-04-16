// FIXME: Add standard header.

#ifndef SBBM_TARGETLOWERING_H
#define SBBM_TARGETLOWERING_H

#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/Target/TargetLowering.h"

namespace llvm {

class SbbmTargetMachine;

class SbbmTargetLowering : public TargetLowering {
public:
  explicit SbbmTargetLowering(const SbbmTargetMachine &TM);
};

} // namespace llvm

#endif // include guard
