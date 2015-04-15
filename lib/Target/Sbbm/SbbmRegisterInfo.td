// FIXME: Add standard header.

class SbbmReg<bits<16> Enc, string n> : Register<n> {
    let HWEncoding = Enc;
    let Namespace = "Sbbm";
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

// Register classes
def GRRegs : RegisterClass<"Sbbm", [i32], 32,
    (add
        // Return values and arguments: caller saved
        R0, R1, R2, R3,
        // General use: callee-saved
        R4, R5, R6, R7, R8, R9, R10, R11,
        // Special: callee-saved
        SP, LR)>;
