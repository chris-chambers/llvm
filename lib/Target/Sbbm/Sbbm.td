// Copyright 2015, Christopher Chambers
// Distributed under the GNU GPL v3. See LICENSE.TXT for details.

include "llvm/Target/Target.td"

// Register, instruction, and calling convention descriptions
include "SbbmRegisterInfo.td"
include "SbbmInstrInfo.td"
include "SbbmCallingConv.td"

def SbbmInstrInfo : InstrInfo;

// Processors SBBM supports
class Proc<string Name, list<SubtargetFeature> Features>
    : Processor<Name, NoItineraries, Features>;

def : Proc<"generic", []>;

// The SBBM Target
def Sbbm : Target {
    let InstructionSet = SbbmInstrInfo;
}
