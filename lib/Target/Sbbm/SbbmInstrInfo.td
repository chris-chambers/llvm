// FIXME: Add standard header.

include "SbbmInstrFormats.td"
include "SbbmOperators.td"

def i32simm : ImmLeaf<i32, [{}]>;
def i32posimm : ImmLeaf<i32, [{ return Imm > 0; }]>;

let Constraints = "$src1 = $dst", isCommutable = 1 in {

def ADDrr : SbbmInstr<
    (outs GRRegs:$dst),
    (ins GRRegs:$src1, GRRegs:$src2),
    "add $dst, $src2",
    [(set i32:$dst, (add i32:$src1, i32:$src2))]>;

def ADDri : SbbmInstr<
    (outs GRRegs:$dst),
    (ins GRRegs:$src1, i32imm:$src2),
    "add $dst, $src2",
    [(set i32:$dst, (add i32:$src1, i32posimm:$src2))]>;

}

def MOVri : SbbmInstr<
    (outs GRRegs:$dst),
    (ins i32imm:$imm),
    "mov $dst, $imm",
    [(set i32:$dst, i32simm:$imm)]> {
    let isMoveImm = 1;
}

let isTerminator = 1, isReturn = 1, isBarrier = 1 in {

def B_LR : SbbmInstr<(outs), (ins variable_ops), "b lr", [(SbbmRetFlag)]>;
def HALT : SbbmInstr<(outs), (ins variable_ops), "halt", [(SbbmHaltFlag)]>;

}
