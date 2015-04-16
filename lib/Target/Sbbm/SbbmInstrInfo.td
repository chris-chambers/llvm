// FIXME: Add standard header.

include "SbbmInstrFormats.td"
include "SbbmOperands.td"
include "SbbmOperators.td"
include "SbbmXForms.td"

let Constraints = "$src1 = $dst" in {

class SbbmBinOp<string opstr, SDNode opnode> : SbbmInstr<
    (outs GRRegs:$dst),
    (ins GRRegs:$src1, GRRegs:$src2),
    opstr#" $dst, $src2",
    [(set i32:$dst, (opnode i32:$src1, i32:$src2))]>;

class SbbmBinImm<string opstr, SDNode opnode> : SbbmInstr<
    (outs GRRegs:$dst),
    (ins GRRegs:$src1, i32imm:$src2),
    opstr#" $dst, $src2",
    [(set i32:$dst, (add i32:$src1, i32posimm:$src2))]>;
}

let isCommutable = 1 in {

def ADDrr : SbbmBinOp<"add", add>;
def ADDri : SbbmBinImm<"add", add>;

}

def SUBrr : SbbmBinOp<"sub", sub>;
def SUBri : SbbmBinImm<"sub", sub>;

// Convert ADDri with negative immediate to SUBri
def : Pat<(add GRRegs:$lhs, i32negimm:$rhs),
          (SUBri $lhs, (negate $rhs))>;

// Convert SUBri with negative immediate to ADDri
def : Pat<(sub GRRegs:$lhs, i32negimm:$rhs),
          (ADDri $lhs, (negate $rhs))>;

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
