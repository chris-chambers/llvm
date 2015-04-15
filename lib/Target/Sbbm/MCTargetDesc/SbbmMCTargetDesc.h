// FIXME: Add standard header.

#ifndef SBBM_MCTARGETDESC_H
#define SBBM_MCTARGETDESC_H

#include "llvm/Support/DataTypes.h"

// Defines symbolic names for SBBM registers.  This defines a mapping from
// register name to register number.
#define GET_REGINFO_ENUM
#include "SbbmGenRegisterInfo.inc"

// Defines symbolic names for the SBBM instructions.
#define GET_INSTRINFO_ENUM
#include "SbbmGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "SbbmGenSubtargetInfo.inc"

#endif // include guard
