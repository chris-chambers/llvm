// FIXME: Add standard header.

#ifndef SBBM_TARGETLOWERING_H
#define SBBM_TARGETLOWERING_H

#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/Target/TargetLowering.h"

namespace llvm {

namespace Sbbm {
namespace ISD {

enum NodeType {
  FIRST_NUMBER = llvm::ISD::BUILTIN_OP_END,
  HALT_FLAG,
  RET_FLAG,
};

}} // namespace Sbbm::ISD

class SbbmTargetMachine;

class SbbmTargetLowering : public TargetLowering {
public:
  explicit SbbmTargetLowering(const SbbmTargetMachine &TM);

  virtual SDValue LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, SDLoc DL, SelectionDAG &DAG,
    SmallVectorImpl<SDValue> &InVals) const override;

  virtual SDValue LowerReturn(
    SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
    const SmallVectorImpl<ISD::OutputArg> &Outs,
    const SmallVectorImpl<SDValue> &OutVals, SDLoc DL, SelectionDAG &DAG)
    const override;
};

} // namespace llvm

#endif // include guard
