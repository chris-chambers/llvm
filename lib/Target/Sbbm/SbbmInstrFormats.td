// Copyright 2015, Christopher Chambers
// Distributed under the GNU GPL v3. See LICENSE.TXT for details.

// Instruction format superclass
class SbbmInstr<dag outs, dag ins, string asmstr, list<dag> pattern = []>
    : Instruction
{
    let Namespace = "Sbbm";
    string BaseOpcode = ?;
    dag OutOperandList = outs;
    dag InOperandList = ins;
    let AsmString   = asmstr;
    let Pattern = pattern;
}

class SbbmPredRel;

class SbbmUnpredInstrBase<dag outs, dag ins, string asmstr, list<dag> pattern>
    : SbbmInstr<outs, ins, asmstr, pattern>, SbbmPredRel
{
    let isPredicable = 1;
    string PredMarker = "unpredicated";
}

class SbbmPredInstrBase<dag outs, dag ins, string asmstr, list<dag> pattern>
    : SbbmInstr<outs, ins, asmstr, pattern>, SbbmPredRel
{
    let isPredicable = 1;
    string PredMarker = "predicated";
}

multiclass SbbmPredInstr<
    string baseOp,
    dag outs, dag unpred_ins,
    string asmstr,
    list<dag> pattern = []>
{
    let BaseOpcode = baseOp in {

    def NAME : SbbmUnpredInstrBase<outs, unpred_ins, asmstr, pattern>;
    def NAME#_P : SbbmPredInstrBase<
        outs, !con(unpred_ins, (ins variable_ops)), asmstr, []>;

    }
}

def getPredicated : InstrMapping {
    let FilterClass = "SbbmPredRel";
    let RowFields = ["BaseOpcode"];
    let ColFields = ["PredMarker"];
    let KeyCol = ["unpredicated"];
    let ValueCols = [["predicated"]];
}
