// FIXME: Add standard header.

include "SbbmInstrFormats.td"
include "SbbmOperands.td"
include "SbbmOperators.td"
include "SbbmXForms.td"

multiclass SbbmBinOp<string opstr, SDNode opnode> : SbbmPredInstr<
    NAME,
    (outs GRRegs:$dst),
    (ins GRRegs:$src1, GRRegs:$src2),
    opstr#" $dst, $src2",
    [(set i32:$dst, (opnode i32:$src1, i32:$src2))]>;

multiclass SbbmBinImm<string opstr, SDNode opnode> : SbbmPredInstr<
    NAME,
    (outs GRRegs:$dst),
    (ins GRRegs:$src1, i32imm:$src2),
    opstr#" $dst, $src2",
    [(set i32:$dst, (opnode i32:$src1, i32posimm:$src2))]>;


// FIXME: It would be nicer to declare Constraints as part of SbbmBinOp and
//        SbbmBinImm, but multiclass does not capture let .. in declarations.
let Constraints = "$src1 = $dst" in {
let isCommutable = 1 in {

defm ADDrr : SbbmBinOp<"add", add>;
defm ADDri : SbbmBinImm<"add", add>;

defm MULrr : SbbmBinOp<"mul", mul>;

}

defm SUBrr : SbbmBinOp<"sub", sub>;
defm SUBri : SbbmBinImm<"sub", sub>;

defm SDIVrr : SbbmBinOp<"sdiv", sdiv>;
defm SREMrr : SbbmBinOp<"srem", srem>;

// Bitwise and, or, exclusive-or
defm ANDrr : SbbmBinOp<"and", and>;
defm ORRrr : SbbmBinOp<"orr", or>;
defm EORrr : SbbmBinOp<"eor", xor>;

// Logical shift left, arithmetic shift right, logical shift right
defm LSLrr : SbbmBinOp<"lsl", shl>;
defm ASRrr : SbbmBinOp<"asr", sra>;
defm LSRrr : SbbmBinOp<"lsr", srl>;

} // Constraints "$src1 = $dst"

// Convert ADDri with negative immediate to SUBri
def : Pat<(add GRRegs:$lhs, i32negimm:$rhs),
          (SUBri $lhs, (negate $rhs))>;

// Convert SUBri with negative immediate to ADDri
def : Pat<(sub GRRegs:$lhs, i32negimm:$rhs),
          (ADDri $lhs, (negate $rhs))>;

defm MOVrr : SbbmPredInstr<
    "MOVrr",
    (outs GRRegs:$dst),
    (ins GRRegs:$src),
    "mov $dst, $src">;

let isMoveImm = 1 in {
defm MOVri : SbbmPredInstr<
    "MOVri",
    (outs GRRegs:$dst),
    (ins i32imm:$imm),
    "mov $dst, $imm",
    [(set i32:$dst, i32simm:$imm)]>;
}

defm RNG : SbbmPredInstr<
    "RNG",
    (outs PredRegs:$dst),
    (ins GRRegs:$src, i32imm:$low, i32imm:$high),
    "rng $dst, $src, $low, $high">;

defm NRNG : SbbmPredInstr<
    "NRNG",
    (outs NPredRegs:$dst),
    (ins GRRegs:$src, i32imm:$low, i32imm:$high),
    "rng $dst, $src, $low, $high">;

let isTerminator = 1, isBranch = 1 in {

def BCOND : SbbmInstr<
    (outs),
    (ins AllPredRegs:$cond, brtarget:$dst),
    "; BCOND needs to be eliminated">;

let isBarrier = 1 in {
defm B : SbbmPredInstr<
    "B",
    (outs), (ins brtarget:$target),
    "b $target",
    [(br bb:$target)]>;
}

}

def : Pat<(i32 (seteq GRRegs:$lhs, i32simm:$rhs)),
          (i32 (RNG $lhs, imm:$rhs, imm:$rhs))>;
def : Pat<(i32 (setne GRRegs:$lhs, i32simm:$rhs)),
          (i32 (NRNG $lhs, imm:$rhs, imm:$rhs))>;
def : Pat<(i32 (setlt GRRegs:$lhs, i32simm:$rhs)),
          (i32 (RNG $lhs, -2147483648, (dec imm:$rhs)))>;
def : Pat<(i32 (setle GRRegs:$lhs, i32simm:$rhs)),
          (i32 (RNG $lhs, -2147483648, imm:$rhs))>;
def : Pat<(i32 (setgt GRRegs:$lhs, i32simm:$rhs)),
          (i32 (RNG $lhs, (inc imm:$rhs), 2147483647))>;
def : Pat<(i32 (setge GRRegs:$lhs, i32simm:$rhs)),
          (i32 (RNG $lhs, imm:$rhs, 2147483647))>;

def : Pat<(brcond AllPredRegs:$cond, bb:$dst),
          (BCOND $cond, $dst)>;

let isTerminator = 1, isReturn = 1, isBarrier = 1 in {

defm B_LR : SbbmPredInstr<"B_LR", (outs), (ins variable_ops), "b lr", [(SbbmRetFlag)]>;
defm HALT : SbbmPredInstr<"HALT", (outs), (ins variable_ops), "halt", [(SbbmHaltFlag)]>;

}
