// FIXME: Add standard header.

include "SbbmInstrFormats.td"

let isTerminator = 1, isReturn = 1, isBarrier = 1 in {

def HALT : SbbmInstr<(outs), (ins), "halt">;

}
