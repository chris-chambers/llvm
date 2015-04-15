// FIXME: Add standard header.

#include "SbbmTargetLowering.h"
#include "SbbmTargetMachine.h"
#include "MCTargetDesc/SbbmMCTargetDesc.h"

using namespace llvm;

SbbmTargetLowering::SbbmTargetLowering(const SbbmTargetMachine &SbbmTM)
  : TargetLowering(SbbmTM)
{
  // Set up the register classes.
  addRegisterClass(MVT::i32, &Sbbm::GRRegsRegClass);

  // Compute derived properties from the register classes
  computeRegisterProperties();

  setStackPointerRegisterToSaveRestore(Sbbm::SP);

  setSchedulingPreference(Sched::RegPressure);
}
