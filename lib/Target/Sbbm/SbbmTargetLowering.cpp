// FIXME: Add standard header.

#include "SbbmTargetLowering.h"
#include "SbbmTargetMachine.h"
#include "MCTargetDesc/SbbmMCTargetDesc.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/Function.h"

using namespace llvm;

#include "SbbmGenCallingConv.inc"

SbbmTargetLowering::SbbmTargetLowering(const SbbmTargetMachine &SbbmTM)
  : TargetLowering(SbbmTM)
{
  // Set up the register classes.
  addRegisterClass(MVT::i32, &Sbbm::GRRegsRegClass);

  // Compute derived properties from the register classes
  computeRegisterProperties();

  setStackPointerRegisterToSaveRestore(Sbbm::SP);

  setSchedulingPreference(Sched::RegPressure);
}

SDValue SbbmTargetLowering::LowerFormalArguments(
  SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
  const SmallVectorImpl<ISD::InputArg> &Ins, SDLoc DL, SelectionDAG &DAG,
  SmallVectorImpl<SDValue> &InVals) const
{
  if (!Ins.empty()) {
    llvm_unreachable("LowerFormalArguments not implemented for more than zero arguments");
  }

  return Chain;
}

SDValue SbbmTargetLowering::LowerReturn(
  SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
  const SmallVectorImpl<ISD::OutputArg> &Outs,
  const SmallVectorImpl<SDValue> &OutVals, SDLoc DL, SelectionDAG &DAG)
  const
{
  if (!Outs.empty()) {
    llvm_unreachable("LowerReturn not implemented for more than zero return values");
  }

  SmallVector<SDValue, 4> RetOps(1, Chain);
  if (DAG.getMachineFunction().getFunction()->hasFnAttribute(llvm::Attribute::NoReturn)) {
    return SDValue(DAG.getMachineNode(Sbbm::HALT, DL, MVT::Other, RetOps), 0);
  }

  llvm_unreachable("LowerReturn is only implemented for functions marked noreturn");
}
