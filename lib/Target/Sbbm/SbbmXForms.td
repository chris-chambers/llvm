// Copyright 2015, Christopher Chambers
// Distributed under the GNU GPL v3. See LICENSE.TXT for details.

def negate : SDNodeXForm<imm, [{
    uint32_t value = N->getZExtValue();
    return CurDAG->getTargetConstant(-value, MVT::i32);
}]>;
def inc : SDNodeXForm<imm, [{
    uint32_t value = N->getZExtValue();
    return CurDAG->getTargetConstant(value + 1, MVT::i32);
}]>;
def dec : SDNodeXForm<imm, [{
    uint32_t value = N->getZExtValue();
    return CurDAG->getTargetConstant(value - 1, MVT::i32);
}]>;
