def i32simm : ImmLeaf<i32, [{}]>;
def i32posimm : ImmLeaf<i32, [{ return Imm > 0; }]>;
def i32negimm : ImmLeaf<i32, [{ return Imm < 0; }]>;
def brtarget    : Operand<OtherVT>;
def memsrc : Operand<i32> {
  let MIOperandInfo = (ops GRRegs);
  let PrintMethod = "printAddrModeMemSrc";
}

// Complex Pattern Definitions.
// NOTE: The second parameter here indicates how many nodes will be produced by
//       the method given in the third parameter.  And, the number and types
//       should match with an Operand<> type (the MIOperandInfo field) that
//       receives these nodes.  For instance, 'addr' matches with memsrc.
def addr : ComplexPattern<iPTR, 1, "SelectAddr", [frameindex], []>;
