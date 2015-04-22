// FIXME: Add standard header.

#ifndef SBBM_INSTRUCTIONINFO_H
#define SBBM_INSTRUCTIONINFO_H

#include "TargetInfo/SbbmRegisterInfo.h"
#include "MCTargetDesc/SbbmMCTargetDesc.h"
#include "llvm/Target/TargetInstrInfo.h"

// FIXME: DO NOT INCLUDE THIS.  MOVE THAT STUFF TO THE .cpp
#include "llvm/CodeGen/MachineInstrBuilder.h"

#define GET_INSTRINFO_HEADER
#include "SbbmGenInstrInfo.inc"

namespace llvm {

class SbbmInstrInfo : public SbbmGenInstrInfo {
public:
  SbbmInstrInfo();

  /// getRegisterInfo - TargetInstrInfo is a superset of MRegister info.  As
  /// such, whenever a client has an instance of instruction info, it should
  /// always be able to get register info as well (through this method).
  const SbbmRegisterInfo &getRegisterInfo() const { return RI; }

  virtual bool expandPostRAPseudo(MachineBasicBlock::iterator MI) const override;

  virtual bool AnalyzeBranch(
    MachineBasicBlock &MBB, MachineBasicBlock *&TBB, MachineBasicBlock *&FBB,
    SmallVectorImpl<MachineOperand> &Cond, bool AllowModify)
    const override;

  virtual unsigned RemoveBranch(MachineBasicBlock &MBB) const override;

  virtual unsigned InsertBranch(
    MachineBasicBlock &MBB, MachineBasicBlock *TBB, MachineBasicBlock *FBB,
    const SmallVectorImpl<MachineOperand> &Cond, DebugLoc DL) const override;

  virtual void copyPhysReg(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator I, DebugLoc DL,
    unsigned DestReg, unsigned SrcReg, bool KillSrc) const override;

  virtual bool isProfitableToIfCvt(
    MachineBasicBlock &MBB, unsigned NumCycles, unsigned ExtraPredCycles,
    const BranchProbability &Probability) const override
  {
    return true;
  }

  virtual bool isProfitableToIfCvt(
    MachineBasicBlock &TMBB, unsigned NumTCycles, unsigned ExtraTCycles,
    MachineBasicBlock &FMBB, unsigned NumFCycles, unsigned ExtraFCycles,
    const BranchProbability &Probability) const override
  {
    return true;
  }

  virtual bool isProfitableToDupForIfCvt(
    MachineBasicBlock &MBB, unsigned NumCycles,
    const BranchProbability &Probability) const override
  {
    return true;
  }

  virtual bool isPredicated(const MachineInstr *MI) const override {
    // FIXME: Really implement isPredicated.
    return false;
  }

  virtual bool PredicateInstruction(
    MachineInstr *MI, const SmallVectorImpl<MachineOperand> &Pred)
    const override;

  virtual bool SubsumesPredicate(
    const SmallVectorImpl<MachineOperand> &Pred1,
    const SmallVectorImpl<MachineOperand> &Pred2)
    const override
  {
    return false;
  }

  virtual bool DefinesPredicate(
    MachineInstr *MI, std::vector<MachineOperand> &Pred) const override
  {
    switch (MI->getOpcode()) {
    case Sbbm::SRNG:
    case Sbbm::SRNG_NOT:
    case Sbbm::URNG:
      Pred.push_back(MI->getOperand(0));
      break;
    }

    return !Pred.empty();
  }

  virtual bool ReverseBranchCondition(
    SmallVectorImpl<MachineOperand> &Cond) const override
  {
    for (MachineOperand& Op : Cond) {
      unsigned int Reg = Op.getReg();
      if (!Sbbm::AllPredRegsRegClass.contains(Reg)) {
        return true;
      }
      Op.setReg(ReversePredReg(Reg));
    }
    return false;
  }

  static unsigned int ReversePredReg(unsigned int PredReg)
  {
    switch (PredReg) {
    case Sbbm::P0: return Sbbm::N0;
    case Sbbm::P1: return Sbbm::N1;
    case Sbbm::P2: return Sbbm::N2;
    case Sbbm::P3: return Sbbm::N3;
    case Sbbm::N0: return Sbbm::P0;
    case Sbbm::N1: return Sbbm::P1;
    case Sbbm::N2: return Sbbm::P2;
    case Sbbm::N3: return Sbbm::P3;
    // FIXME: Emit a better error message.
    default: llvm_unreachable("unknown predicate register");
    }
  }

private:
  virtual void anchor();

  const SbbmRegisterInfo RI;
};

} // namespace llvm

#endif // include guard
