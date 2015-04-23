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
    setOperationAction(ISD::GlobalAddress, VT, Custom);

    for (auto Op : {
      ISD::BR_CC,
      ISD::SELECT_CC,

      ISD::SHL_PARTS,
      ISD::SRL_PARTS,
      ISD::SRA_PARTS,

      ISD::SMUL_LOHI,
      ISD::UMUL_LOHI,

      ISD::MULHS,
      ISD::MULHU,

      ISD::CTPOP,
      ISD::CTLZ,
      ISD::CTLZ_ZERO_UNDEF,
      ISD::CTTZ,
      ISD::CTTZ_ZERO_UNDEF,

      ISD::BSWAP,
    }) {
      setOperationAction(Op, VT, Expand);
    }

    // TODO: Sign extension in register can be implemented directly.
    setOperationAction(ISD::SIGN_EXTEND_INREG, VT, Expand);
  }
  setOperationAction(ISD::BR_JT, MVT::Other, Expand);
  setOperationAction(ISD::BRIND, MVT::Other, Expand);
}

const char *SbbmTargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (Opcode) {
  case Sbbm::ISD::CALL: return "Call";
  case Sbbm::ISD::WRAPPER: return "Wrapper";
  case Sbbm::ISD::HALT_FLAG: return "HaltFlag";
  case Sbbm::ISD::RET_FLAG: return "RetFlag";
  default: return TargetLowering::getTargetNodeName(Opcode);
  }
}

SDValue SbbmTargetLowering::LowerCall(
  TargetLowering::CallLoweringInfo &CLI, SmallVectorImpl<SDValue> &InVals) const
{
  SelectionDAG &DAG = CLI.DAG;
  SDLoc &dl = CLI.DL;
  SmallVectorImpl<ISD::OutputArg> &Outs = CLI.Outs;
  SmallVectorImpl<SDValue> &OutVals = CLI.OutVals;
  SmallVectorImpl<ISD::InputArg> &Ins = CLI.Ins;
  SDValue Chain = CLI.Chain;
  SDValue Callee = CLI.Callee;
  bool &isTailCall = CLI.IsTailCall;
  CallingConv::ID CallConv = CLI.CallConv;
  bool isVarArg = CLI.IsVarArg;

  const MachineFunction &MachineFunction = DAG.getMachineFunction();
  const Function &Function = *MachineFunction.getFunction();

  if (isVarArg) {
    llvm_unreachable("var args are not implemented");
  }

  if (isTailCall) {
    llvm_unreachable("tail calls are not implemented");
  }

  // Analyze operands of the call, assigning locations to each operand.
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, isVarArg, DAG.getMachineFunction(), ArgLocs,
                 *DAG.getContext());
  CCInfo.AnalyzeCallOperands(Outs, CC_Sbbm);

  // Get the size of the outgoing arguments stack space requirement.
  const unsigned NumBytes = CCInfo.getNextStackOffset();

  Chain =
      DAG.getCALLSEQ_START(Chain, DAG.getIntPtrConstant(NumBytes, true), dl);

  SmallVector<std::pair<unsigned, SDValue>, 8> RegsToPass;
  SmallVector<SDValue, 8> MemOpChains;

  // Walk the register/memloc assignments, inserting copies/loads.
  for (unsigned i = 0, realArgIdx = 0, e = ArgLocs.size(); i != e;
       ++i, ++realArgIdx) {
    CCValAssign &VA = ArgLocs[i];
    SDValue Arg = OutVals[realArgIdx];

    // We only handle fully promoted arguments.
    assert(VA.getLocInfo() == CCValAssign::Full && "Unhandled loc info");

    if (!VA.isRegLoc()) {
      llvm_unreachable("Only passing paramters via registers");
    }

    RegsToPass.push_back(std::make_pair(VA.getLocReg(), Arg));
  }

  // Build a sequence of copy-to-reg nodes chained together with token chain
  // and flag operands which copy the outgoing args into the appropriate regs.
  SDValue InFlag;

  for (auto &Reg : RegsToPass) {
    Chain = DAG.getCopyToReg(Chain, dl, Reg.first, Reg.second, InFlag);
    InFlag = Chain.getValue(1);
  }

  std::vector<SDValue> Ops;
  Ops.push_back(Chain);

  if (GlobalAddressSDNode *G = dyn_cast<GlobalAddressSDNode>(Callee)) {
    Ops.push_back(DAG.getTargetGlobalAddress(G->getGlobal(), dl, MVT::i32));
  } else if (ExternalSymbolSDNode *ES = dyn_cast<ExternalSymbolSDNode>(Callee)) {
    Ops.push_back(SDValue(ES, 0));
  } else {
    llvm_unreachable("unsupported call type");
  }

  // Add argument registers to the end of the list so that they are known live
  // into the call.
  for (auto &Reg : RegsToPass) {
    Ops.push_back(DAG.getRegister(Reg.first, Reg.second.getValueType()));
  }

  // Add a register mask operand representing the call-preserved registers.
  const uint32_t *Mask;
  const TargetRegisterInfo *TRI =
      getTargetMachine().getSubtargetImpl(Function)->getRegisterInfo();
  Mask = TRI->getCallPreservedMask(CallConv);

  assert(Mask && "Missing call preserved mask for calling convention");
  Ops.push_back(DAG.getRegisterMask(Mask));

  if (InFlag.getNode()) {
    Ops.push_back(InFlag);
  }

  SDVTList NodeTys = DAG.getVTList(MVT::Other, MVT::Glue);

  // Returns a chain and a flag for retval copy to use.
  Chain = DAG.getNode(Sbbm::ISD::CALL, dl, NodeTys, Ops);
  InFlag = Chain.getValue(1);

  Chain = DAG.getCALLSEQ_END(Chain, DAG.getIntPtrConstant(NumBytes, true),
                             DAG.getIntPtrConstant(0, true), InFlag, dl);
  if (!Ins.empty()) {
    InFlag = Chain.getValue(1);
  }

  // Handle result values, copying them out of physregs into vregs that we
  // return.
  return LowerCallResult(Chain, InFlag, CallConv, isVarArg, Ins, dl, DAG,
                         InVals);
}

SDValue SbbmTargetLowering::LowerCallResult(
    SDValue Chain, SDValue InGlue, CallingConv::ID CallConv, bool isVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, SDLoc dl, SelectionDAG &DAG,
    SmallVectorImpl<SDValue> &InVals) const {
  assert(!isVarArg && "Unsupported");

  // Assign locations to each value returned by this call.
  SmallVector<CCValAssign, 16> RVLocs;
  CCState CCInfo(CallConv, isVarArg, DAG.getMachineFunction(), RVLocs,
                 *DAG.getContext());

  CCInfo.AnalyzeCallResult(Ins, RetCC_Sbbm);

  // Copy all of the result registers out of their specified physreg.
  for (auto &Loc : RVLocs) {
    Chain = DAG.getCopyFromReg(Chain, dl, Loc.getLocReg(), Loc.getValVT(),
                               InGlue).getValue(1);
    InGlue = Chain.getValue(2);
    InVals.push_back(Chain.getValue(0));
  }

  return Chain;
}

SDValue SbbmTargetLowering::LowerOperation(SDValue Op, SelectionDAG &DAG) const {
  if (Op.getOpcode() == ISD::GlobalAddress) {
    const GlobalValue *GV = cast<GlobalAddressSDNode>(Op.getNode())->getGlobal();
    SDLoc DL(Op);
    SDValue G = DAG.getTargetGlobalAddress(GV, DL, getPointerTy());
    return DAG.getNode(Sbbm::ISD::WRAPPER, DL, getPointerTy(), G);
  }
  return TargetLowering::LowerOperation(Op, DAG);
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
