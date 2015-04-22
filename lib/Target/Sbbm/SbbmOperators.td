// FIXME: Add standard header.

def SbbmRetFlag : SDNode<
    "Sbbm::ISD::RET_FLAG", SDTNone,
    [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

def SbbmHaltFlag : SDNode<
    "Sbbm::ISD::HALT_FLAG", SDTNone,
    [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

def SbbmWrapper : SDNode<"Sbbm::ISD::WRAPPER", SDTIntUnaryOp>;
