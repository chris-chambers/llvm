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
  CALL,
  WRAPPER,
  HALT_FLAG,
  RET_FLAG,
};

}} // namespace Sbbm::ISD

class SbbmTargetMachine;

class SbbmTargetLowering : public TargetLowering {
public:
  explicit SbbmTargetLowering(const SbbmTargetMachine &TM);

  virtual const char *getTargetNodeName(unsigned Opcode) const override;

  virtual SDValue LowerCall(
    TargetLowering::CallLoweringInfo &CLI, SmallVectorImpl<SDValue> &InVals)
    const override;

  virtual SDValue LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, SDLoc DL, SelectionDAG &DAG,
    SmallVectorImpl<SDValue> &InVals) const override;

  virtual SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;

  virtual SDValue LowerReturn(
    SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
    const SmallVectorImpl<ISD::OutputArg> &Outs,
    const SmallVectorImpl<SDValue> &OutVals, SDLoc DL, SelectionDAG &DAG)
    const override;

private:
  SDValue LowerCallResult(
    SDValue Chain, SDValue InGlue, CallingConv::ID CallConv, bool isVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, SDLoc dl, SelectionDAG &DAG,
    SmallVectorImpl<SDValue> &InVals)
    const;
};

} // namespace llvm

#endif // include guard
