// Copyright 2015, Christopher Chambers
// Distributed under the GNU GPL v3. See LICENSE.TXT for details.

def SbbmRetFlag : SDNode<
    "Sbbm::ISD::RET_FLAG", SDTNone,
    [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

def SbbmHaltFlag : SDNode<
    "Sbbm::ISD::HALT_FLAG", SDTNone,
    [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

def SbbmWrapper : SDNode<"Sbbm::ISD::WRAPPER", SDTIntUnaryOp>;

def SDT_SbbmCallSeqStart : SDCallSeqStart<[SDTCisVT<0, i32>]>;
def SDT_SbbmCallSeqEnd   : SDCallSeqEnd<[
  SDTCisVT<0, i32>, SDTCisVT<1, i32>
]>;

def callseq_start : SDNode<
    "ISD::CALLSEQ_START", SDT_SbbmCallSeqStart,
    [SDNPHasChain, SDNPOutGlue]>;
def callseq_end : SDNode<
    "ISD::CALLSEQ_END", SDT_SbbmCallSeqEnd,
    [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;

def SDT_SbbmCall  : SDTypeProfile<0, -1, [SDTCisPtrTy<0>]>;

// Custom SDNodes
def sbbm_call : SDNode<
    "Sbbm::ISD::CALL", SDT_SbbmCall,
    [ SDNPHasChain, SDNPOptInGlue, SDNPOutGlue, SDNPVariadic ]>;
