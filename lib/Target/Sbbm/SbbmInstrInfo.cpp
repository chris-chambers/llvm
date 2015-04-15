// FIXME: Add standard header.

#include "SbbmInstrInfo.h"

#define GET_INSTRINFO_CTOR_DTOR
#include "SbbmGenInstrInfo.inc"

using namespace llvm;

SbbmInstrInfo::SbbmInstrInfo()
  : SbbmGenInstrInfo()
  , RI()
{ }

void SbbmInstrInfo::anchor() { }
