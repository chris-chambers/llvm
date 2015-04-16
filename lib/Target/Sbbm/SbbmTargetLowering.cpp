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
  if (!Outs.empty()) {
    llvm_unreachable("LowerReturn not implemented for more than zero return values");
  }

  SmallVector<SDValue, 4> RetOps(1, Chain);
  if (DAG.getMachineFunction().getFunction()->hasFnAttribute(llvm::Attribute::NoReturn)) {
    return SDValue(DAG.getMachineNode(Sbbm::HALT, DL, MVT::Other, RetOps), 0);
  }

  llvm_unreachable("LowerReturn is only implemented for functions marked noreturn");
}
