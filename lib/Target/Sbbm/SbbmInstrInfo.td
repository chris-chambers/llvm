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

// FIXME: MOVra may be the wrong name here.  This moves an address of some kind
//        into a register (just the address, not the contents).
defm MOVra : SbbmPredInstr<
    "MOVra",
    (outs GRRegs:$dst),
    (ins memsrc:$addr),
    "mov $dst, $addr",
    [(set i32:$dst, addr:$addr)]>;

def : Pat<(SbbmWrapper tglobaladdr:$addr),
          (MOVra $addr)>;

let isMoveImm = 1 in {
defm MOVri : SbbmPredInstr<
    "MOVri",
    (outs GRRegs:$dst),
    (ins i32imm:$imm),
    "mov $dst, $imm",
    [(set i32:$dst, i32simm:$imm)]>;
}

defm LDR : SbbmPredInstr<
    "LDR",
    (outs GRRegs:$dst),
    (ins memsrc:$addr),
    "ldr $dst, $addr",
    [(set i32:$dst, (load addr:$addr))]>;

defm LDRH : SbbmPredInstr<
    "LDRH",
    (outs GRRegs:$dst),
    (ins memsrc:$addr),
    "ldrh $dst, $addr",
    [(set i32:$dst, (zextloadi16 addr:$addr))]>;

defm LDRSH : SbbmPredInstr<
    "LDRSH",
    (outs GRRegs:$dst),
    (ins memsrc:$addr),
    "ldrsh $dst, $addr",
    [(set i32:$dst, (sextloadi16 addr:$addr))]>;

defm LDRB : SbbmPredInstr<
    "LDRB",
    (outs GRRegs:$dst),
    (ins memsrc:$addr),
    "ldrb $dst, $addr",
    [(set i32:$dst, (zextloadi8 addr:$addr))]>;

def : Pat<(extloadi16 addr:$addr),
          (LDRH $addr)>;
def : Pat<(extloadi16 (SbbmWrapper tglobaladdr:$addr)),
          (LDRH $addr)>;

def : Pat<(extloadi8 addr:$addr),
          (LDRB $addr)>;
def : Pat<(extloadi8 (SbbmWrapper tglobaladdr:$addr)),
          (LDRB $addr)>;

defm LDRSB : SbbmPredInstr<
    "LDRSB",
    (outs GRRegs:$dst),
    (ins memsrc:$addr),
    "ldrsb $dst, $addr",
    [(set i32:$dst, (sextloadi8 addr:$addr))]>;

defm STR : SbbmPredInstr<
    "STR",
    (outs),
    (ins GRRegs:$src, memsrc:$addr),
    "str $src, $addr",
    [(store i32:$src, addr:$addr)]>;

defm STRH : SbbmPredInstr<
    "STRH",
    (outs),
    (ins GRRegs:$src, memsrc:$addr),
    "strh $src, $addr",
    [(truncstorei16 i32:$src, addr:$addr)]>;

defm STRB : SbbmPredInstr<
    "STRB",
    (outs),
    (ins GRRegs:$src, memsrc:$addr),
    "strb $src, $addr",
    [(truncstorei8 i32:$src, addr:$addr)]>;

defm SRNG : SbbmPredInstr<
    "SRNG",
    (outs PredRegs:$dst),
    (ins GRRegs:$src, i32imm:$low, i32imm:$high),
    "srng $dst, $src, $low, $high">;

defm SRNG_NOT : SbbmPredInstr<
    "SRNG_NOT",
    (outs NPredRegs:$dst),
    (ins GRRegs:$src, i32imm:$low, i32imm:$high),
    "srng $dst, $src, $low, $high">;

defm URNG : SbbmPredInstr<
    "URNG",
    (outs PredRegs:$dst),
    (ins GRRegs:$src, i32imm:$low, i32imm:$high),
    "urng $dst, $src, $low, $high">;

let isTerminator = 1, isBranch = 1 in {

def BCOND : SbbmInstr<
    (outs),
    (ins AllPredRegs:$cond, brtarget:$dst),
    "; BCOND needs to be eliminated">
{
    let isPseudo = 1;
}

let isBarrier = 1 in {
defm B : SbbmPredInstr<
    "B",
    (outs), (ins brtarget:$target),
    "b $target",
    [(br bb:$target)]>;
}

}

def SEL : SbbmInstr<
    (outs GRRegs:$dst),
    (ins AllPredRegs:$cond, GRRegs:$T, GRRegs:$F),
    "; SEL needs to be eliminated">
{
    let isPseudo = 1;
}

// Signed
def : Pat<(i32 (seteq GRRegs:$lhs, i32simm:$rhs)),
          (i32 (SRNG $lhs, imm:$rhs, imm:$rhs))>;
def : Pat<(i32 (setne GRRegs:$lhs, i32simm:$rhs)),
          (i32 (SRNG_NOT $lhs, imm:$rhs, imm:$rhs))>;
def : Pat<(i32 (setlt GRRegs:$lhs, i32simm:$rhs)),
          (i32 (SRNG $lhs, -2147483648, (dec imm:$rhs)))>;
def : Pat<(i32 (setle GRRegs:$lhs, i32simm:$rhs)),
          (i32 (SRNG $lhs, -2147483648, imm:$rhs))>;
def : Pat<(i32 (setgt GRRegs:$lhs, i32simm:$rhs)),
          (i32 (SRNG $lhs, (inc imm:$rhs), 2147483647))>;
def : Pat<(i32 (setge GRRegs:$lhs, i32simm:$rhs)),
          (i32 (SRNG $lhs, imm:$rhs, 2147483647))>;
// Unsigned
def : Pat<(i32 (setult GRRegs:$lhs, i32simm:$rhs)),
          (i32 (URNG $lhs, 0, (dec imm:$rhs)))>;
def : Pat<(i32 (setule GRRegs:$lhs, i32simm:$rhs)),
          (i32 (URNG $lhs, 0, imm:$rhs))>;
def : Pat<(i32 (setugt GRRegs:$lhs, i32simm:$rhs)),
          (i32 (URNG $lhs, (inc imm:$rhs), 4294967295))>;
def : Pat<(i32 (setuge GRRegs:$lhs, i32simm:$rhs)),
          (i32 (URNG $lhs, imm:$rhs, 4294967295))>;

// Signed
def : Pat<(i32 (seteq GRRegs:$lhs, GRRegs:$rhs)),
          (i32 (SRNG (SUBrr $lhs, $rhs), 0, 0))>;
def : Pat<(i32 (setne GRRegs:$lhs, GRRegs:$rhs)),
          (i32 (SRNG_NOT (SUBrr $lhs, $rhs), 0, 0))>;
def : Pat<(i32 (setlt GRRegs:$lhs, GRRegs:$rhs)),
          (i32 (SRNG (SUBrr $lhs, $rhs), -2147483648, -1))>;
def : Pat<(i32 (setle GRRegs:$lhs, GRRegs:$rhs)),
          (i32 (SRNG (SUBrr $lhs, $rhs), -2147483648, 0))>;
def : Pat<(i32 (setgt GRRegs:$lhs, GRRegs:$rhs)),
          (i32 (SRNG (SUBrr $lhs, $rhs), 1, 2147483647))>;
def : Pat<(i32 (setge GRRegs:$lhs, GRRegs:$rhs)),
          (i32 (SRNG (SUBrr $lhs, $rhs), 0, 2147483647))>;
// Unsigned
// FIXME: These unsigned patterns are wrong.  You can't do signed 32-bit
//        subtraction on unsigned 32-bit operands and expect it to work.
def : Pat<(i32 (setult GRRegs:$lhs, GRRegs:$rhs)),
          (i32 (SRNG (SUBrr $lhs, $rhs), -2147483648, -1))>;
def : Pat<(i32 (setule GRRegs:$lhs, GRRegs:$rhs)),
          (i32 (SRNG (SUBrr $lhs, $rhs), -2147483648, 0))>;
def : Pat<(i32 (setugt GRRegs:$lhs, GRRegs:$rhs)),
          (i32 (SRNG (SUBrr $lhs, $rhs), 1, 2147483647))>;
def : Pat<(i32 (setuge GRRegs:$lhs, GRRegs:$rhs)),
          (i32 (URNG (SUBrr $lhs, $rhs), 0, 2147483647))>;

def : Pat<(brcond AllPredRegs:$cond, bb:$dst),
          (BCOND $cond, $dst)>;

def : Pat<(select AllPredRegs:$cond, GRRegs:$T, GRRegs:$F),
          (SEL $cond, $T, $F)>;

let isTerminator = 1, isReturn = 1, isBarrier = 1 in {

defm B_LR : SbbmPredInstr<"B_LR", (outs), (ins variable_ops), "b lr", [(SbbmRetFlag)]>;
defm HALT : SbbmPredInstr<"HALT", (outs), (ins variable_ops), "halt", [(SbbmHaltFlag)]>;

}
