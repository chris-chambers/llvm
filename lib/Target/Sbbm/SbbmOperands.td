def i32simm : ImmLeaf<i32, [{}]>;
def i32posimm : ImmLeaf<i32, [{ return Imm > 0; }]>;
def i32negimm : ImmLeaf<i32, [{ return Imm < 0; }]>;
def brtarget    : Operand<OtherVT>;
def memsrc : Operand<i32> {
  let MIOperandInfo = (ops GRRegs, i32imm);
  let PrintMethod = "printAddrModeMemSrc";
  let EncoderMethod = "getMemSrcValue";
}

// Complex Pattern Definitions.
def addr : ComplexPattern<iPTR, 2, "SelectAddr", [], []>;
