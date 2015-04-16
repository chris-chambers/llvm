def negate : SDNodeXForm<imm, [{
    uint32_t value = N->getZExtValue();
    return CurDAG->getTargetConstant(-value, MVT::i32);
}]>;
