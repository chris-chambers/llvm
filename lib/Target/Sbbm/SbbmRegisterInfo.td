// FIXME: Add standard header.

class SbbmReg<bits<16> Enc, string n> : Register<n> {
    let HWEncoding = Enc;
    let Namespace = "Sbbm";
}

class SbbmPredReg<bits<16> Enc, string n> : SbbmReg<Enc, n>;
class SbbmNPredReg<bits<16> Enc, string n, Register P> : SbbmReg<Enc, n> {
    let Aliases = [P];
}

// CPU registers
def R0  : SbbmReg< 0, "r0">;
def R1  : SbbmReg< 1, "r1">;
def R2  : SbbmReg< 2, "r2">;
def R3  : SbbmReg< 3, "r3">;
def R4  : SbbmReg< 4, "r4">;
def R5  : SbbmReg< 5, "r5">;
def R6  : SbbmReg< 6, "r6">;
def R7  : SbbmReg< 7, "r7">;
def R8  : SbbmReg< 8, "r8">;
def R9  : SbbmReg< 9, "r9">;
def R10 : SbbmReg<10, "r10">;
def R11 : SbbmReg<11, "r11">;

def SP  : SbbmReg<13, "sp">;
def LR  : SbbmReg<14, "lr">;

def P0  : SbbmPredReg<0, "p0">;
def P1  : SbbmPredReg<1, "p1">;
def P2  : SbbmPredReg<2, "p2">;
def P3  : SbbmPredReg<3, "p3">;

def N0  : SbbmNPredReg<0, "n0", P0>;
def N1  : SbbmNPredReg<1, "n1", P1>;
def N2  : SbbmNPredReg<2, "n2", P2>;
def N3  : SbbmNPredReg<3, "n3", P3>;

// Register classes
def GRRegs : RegisterClass<"Sbbm", [i32], 32,
    (add
        // Return values and arguments: caller saved
        R0, R1, R2, R3,
        // General use: callee-saved
        R4, R5, R6, R7, R8, R9, R10, R11,
        // Special: callee-saved
        SP, LR)>;

def PredRegs : RegisterClass<"Sbbm", [i32], 32,
    (add P0, P1, P2, P3)>;

def NPredRegs : RegisterClass<"Sbbm", [i32], 32,
    (add N0, N1, N2, N3)>;

def AllPredRegs : RegisterClass<"Sbbm", [i32], 32,
    (add P0, P1, P2, P3, N0, N1, N2, N3)>;
