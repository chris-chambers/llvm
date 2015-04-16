// FIXME: Add standard header.

// SBBM Argument Calling Conventions
def CC_Sbbm : CallingConv<[
  // Promote i8/i16 arguments to i32.
  CCIfType<[i8, i16], CCPromoteToType<i32>>,

  // The first 4 integer arguments are passed in integer registers.
  CCIfType<[i32], CCAssignToReg<[R0, R1, R2, R3]>>,

  // Integer values get stored in stack slots that are 4 bytes in
  // size and 4-byte aligned.
  CCIfType<[i32], CCAssignToStack<4, 4>>
]>;

// SBBM Return Value Calling Convention
def RetCC_Sbbm : CallingConv<[
    // i32 are returned in registers R0, R1, R2, R3
    CCIfType<[i32], CCAssignToReg<[R0, R1, R2, R3]>>,

    // Integer values get stored in stack slots that are 4 bytes in
    // size and 4-byte aligned.
    CCIfType<[i32], CCAssignToStack<4, 4>>
]>;

def CC_Save : CalleeSavedRegs<(add R4, R5, R6, R7, R8, R9, R10, R11, LR)>;
