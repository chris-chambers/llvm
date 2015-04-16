// FIXME: Add standard header.

#include "SbbmInstrInfo.h"
#include "MCTargetDesc/SbbmMCTargetDesc.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"

#define GET_INSTRINFO_CTOR_DTOR
#include "SbbmGenInstrInfo.inc"

using namespace llvm;

SbbmInstrInfo::SbbmInstrInfo()
  : SbbmGenInstrInfo()
  , RI()
{ }

void SbbmInstrInfo::copyPhysReg(
  MachineBasicBlock &MBB, MachineBasicBlock::iterator I, DebugLoc DL,
  unsigned DestReg, unsigned SrcReg, bool KillSrc) const
{
  BuildMI(MBB, I, I->getDebugLoc(), get(Sbbm::MOVrr), DestReg)
    .addReg(SrcReg, getKillRegState(KillSrc));
}

void SbbmInstrInfo::anchor() { }
