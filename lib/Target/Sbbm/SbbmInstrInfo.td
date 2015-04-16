// FIXME: Add standard header.

include "SbbmInstrFormats.td"
include "SbbmOperators.td"

def ADDrr : SbbmInstr<
    (outs GRRegs:$dst),
    (ins GRRegs:$src1, GRRegs:$src2),
    "add $dst, $src2",
    [(set i32:$dst, (add i32:$src1, i32:$src2))]> {

    let Constraints = "$src1 = $dst";
    let isCommutable = 1;
}

let isTerminator = 1, isReturn = 1, isBarrier = 1 in {

def B_LR : SbbmInstr<(outs), (ins variable_ops), "b lr", [(SbbmRetFlag)]>;
def HALT : SbbmInstr<(outs), (ins variable_ops), "halt", [(SbbmHaltFlag)]>;

}
