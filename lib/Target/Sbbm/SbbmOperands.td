def i32simm : ImmLeaf<i32, [{}]>;
def i32posimm : ImmLeaf<i32, [{ return Imm > 0; }]>;
def i32negimm : ImmLeaf<i32, [{ return Imm < 0; }]>;
def brtarget    : Operand<OtherVT>;
