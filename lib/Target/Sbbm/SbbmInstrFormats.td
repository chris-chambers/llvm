// FIXME: Add standard header.

// Instruction format superclass
class SbbmInstr<dag outs, dag ins, string asmstr, list<dag> pattern = []>
    : Instruction
{
    let Namespace = "Sbbm";
    dag OutOperandList = outs;
    dag InOperandList = ins;
    let AsmString   = asmstr;
    let Pattern = pattern;
}
