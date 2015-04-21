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

  for (auto VT : MVT::integer_valuetypes()) {
    setOperationAction(ISD::BR_CC, VT, Expand);
  }
}

SDValue SbbmTargetLowering::LowerFormalArguments(
  SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
  const SmallVectorImpl<ISD::InputArg> &Ins, SDLoc DL, SelectionDAG &DAG,
  SmallVectorImpl<SDValue> &InVals) const
{
  if (isVarArg) {
    llvm_unreachable("LowerFormalArguments: varargs are not yet supported");
  }

  MachineFunction &MF = DAG.getMachineFunction();
  MachineRegisterInfo &RegInfo = MF.getRegInfo();

  // Assign locations to all of the incoming arguments.
  const auto ArgLocs = [&]() {
    SmallVector<CCValAssign, 16> ArgLocs;
    CCState CCInfo(CallConv, isVarArg, MF, ArgLocs, *DAG.getContext());
    CCInfo.AnalyzeFormalArguments(Ins, CC_Sbbm);
    return ArgLocs;
  }();

  for (const auto &VA : ArgLocs) {
    if (!VA.isRegLoc()) {
      llvm_unreachable(
        "LowerFormalArguments: only arguments passed in registers are currently "
        "supported");
    }

    SDValue ArgIn;
    EVT RegVT = VA.getLocVT();
    switch (RegVT.getSimpleVT().SimpleTy) {
    default:
      llvm_unreachable("Unhandled type for register");
    case MVT::i32: {
      unsigned int VReg = RegInfo.createVirtualRegister(&Sbbm::GRRegsRegClass);
      RegInfo.addLiveIn(VA.getLocReg(), VReg);
      ArgIn = DAG.getCopyFromReg(Chain, DL, VReg, RegVT);
      break;
    }
    }

    InVals.push_back(ArgIn);
  }

  return Chain;
}

SDValue SbbmTargetLowering::LowerReturn(
  SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
  const SmallVectorImpl<ISD::OutputArg> &Outs,
  const SmallVectorImpl<SDValue> &OutVals, SDLoc DL, SelectionDAG &DAG)
  const
{
  if (isVarArg) {
    llvm_unreachable("LowerReturn: varags are not yet supported");
  }

  MachineFunction &MF = DAG.getMachineFunction();

  // Represent the assignment of the return value to a location.
  const auto RVLocs = [&]() {
    SmallVector<CCValAssign, 16> RVLocs;
    CCState CCInfo(CallConv, isVarArg, MF, RVLocs, *DAG.getContext());
    CCInfo.AnalyzeReturn(Outs, RetCC_Sbbm);
    return RVLocs;
  }();

  SDValue Flag(nullptr, 0);
  SmallVector<SDValue, 4> RetOps(1, Chain);

  // Copy the result values into the output locations.
  for (unsigned i = 0; i < RVLocs.size(); ++i) {
    const CCValAssign &VA = RVLocs[i];
    if (!VA.isRegLoc()) {
      llvm_unreachable(
        "LowerReturn: returning into non-register locations is not yet "
        "supported");
    }

    Chain = DAG.getCopyToReg(Chain, DL, VA.getLocReg(), OutVals[i], Flag);
    Flag = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(VA.getLocReg(), VA.getLocVT()));
  }

  // Chain is updated while processing return values, so update RetOps with  the
  // (possibly) new value.
  RetOps[0] = Chain;

  // Add the flag if we have it.
  if (Flag.getNode()) {
    RetOps.push_back(Flag);
  }

  if (DAG.getMachineFunction().getFunction()->hasFnAttribute(llvm::Attribute::NoReturn)) {
    return DAG.getNode(Sbbm::ISD::HALT_FLAG, DL, MVT::Other, RetOps);
  }

  return DAG.getNode(Sbbm::ISD::RET_FLAG, DL, MVT::Other, RetOps);
}
