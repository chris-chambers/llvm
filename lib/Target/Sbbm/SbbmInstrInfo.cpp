// FIXME: Add standard header.

#include "SbbmInstrInfo.h"
#include "MCTargetDesc/SbbmMCTargetDesc.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include <algorithm>

#define GET_INSTRINFO_CTOR_DTOR
#define GET_INSTRMAP_INFO
#include "SbbmGenInstrInfo.inc"

using namespace llvm;

namespace {

SmallVector<MachineBasicBlock::iterator, 2>
getRelevantTerminators(MachineBasicBlock &MBB)
{
  SmallVector<MachineBasicBlock::iterator, 2> Terms;
  auto Iter = MBB.end();
  if (Iter == MBB.begin()) {
    return Terms;
  }

  do {
    --Iter;
  } while (Iter != MBB.begin() && Iter->isDebugValue());

  while (Iter->isTerminator()) {
    Terms.push_back(Iter);
    if (Iter == MBB.begin()) {
      break;
    }
    --Iter;
  }

  std::reverse(Terms.begin(), Terms.end());
  return Terms;
}

} // anonymous namespace

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

bool SbbmInstrInfo::expandPostRAPseudo(MachineBasicBlock::iterator MI) const {
  if (MI->getOpcode() == Sbbm::SEL) {
    DebugLoc DL = MI->getDebugLoc();
    MachineBasicBlock &MBB = *MI->getParent();

    const MachineOperand &Dst = MI->getOperand(0);
    const MachineOperand &Cond = MI->getOperand(1);
    const MachineOperand &TVal = MI->getOperand(2);
    const MachineOperand &FVal = MI->getOperand(3);

    if (Dst.getReg() != TVal.getReg()) {
      BuildMI(MBB, MI, DL, get(Sbbm::MOVrr_P))
        .addReg(Dst.getReg())
        .addReg(TVal.getReg())
        .addReg(Cond.getReg());
    }

    if (Dst.getReg() != FVal.getReg()) {
      BuildMI(MBB, MI, DL, get(Sbbm::MOVrr_P))
        .addReg(Dst.getReg())
        .addReg(FVal.getReg())
        .addReg(ReversePredReg(Cond.getReg()));
    }

    MBB.erase(MI);
    return true;
  }

  return false;
}

bool SbbmInstrInfo::AnalyzeBranch(
  MachineBasicBlock &MBB, MachineBasicBlock *&TBB, MachineBasicBlock *&FBB,
  SmallVectorImpl<MachineOperand> &Cond, bool AllowModify = false)
  const
{
  auto Terms = getRelevantTerminators(MBB);

  // Remove consecutive unconditional branches from the end, leaving only a
  // single final unconditional branch.
  while (AllowModify && Terms.size() > 1) {
    auto NextToLast = *(Terms.end() - 2);
    if (Terms.back()->isUnconditionalBranch() && NextToLast->isUnconditionalBranch()) {
      Terms.back()->eraseFromParent();
      Terms.pop_back();
    } else {
      break;
    }
  }

  switch (Terms.size()) {
  case 0:
    return false;
  case 1:
    switch (Terms[0]->getOpcode()) {
    default:
      return true;
    case Sbbm::B:
      // If branch is unconditional set the true block to its operand.
      TBB = Terms[0]->getOperand(0).getMBB();
      return false;
    case Sbbm::B_LR:
      return true;
    case Sbbm::BCOND:
      TBB = Terms[0]->getOperand(1).getMBB();
      auto PredReg = Terms[0]->getOperand(0);
      PredReg.clearParent();
      Cond.push_back(PredReg);
      return false;
    }
  case 2:
    if (Terms[0]->getOpcode() == Sbbm::BCOND && Terms[1]->getOpcode() == Sbbm::B) {
      TBB = Terms[0]->getOperand(1).getMBB();
      auto PredReg = Terms[0]->getOperand(0);
      PredReg.clearParent();
      Cond.push_back(PredReg);
      FBB = Terms[1]->getOperand(0).getMBB();
      return false;
    }
    return true;
  default:
    //llvm_unreachable("TODO: Handle blocks that end with two branch instructions.");
    return true;
  }
}

unsigned SbbmInstrInfo::RemoveBranch(MachineBasicBlock &MBB) const {
  auto Terms = getRelevantTerminators(MBB);
  unsigned count = 0;
  while (!Terms.empty() && Terms.back()->isBranch()) {
    Terms.back()->eraseFromParent();
    Terms.pop_back();
    count++;
  }
  return count;
}

unsigned SbbmInstrInfo::InsertBranch(
  MachineBasicBlock &MBB, MachineBasicBlock *TBB, MachineBasicBlock *FBB,
  const SmallVectorImpl<MachineOperand> &Cond, DebugLoc DL) const
{
  // Shouldn't be a fall through.
  assert(TBB && "InsertBranch must not be told to insert a fallthrough");

  if (!FBB) {
    if (Cond.empty()) {
      // Unconditional branch
      BuildMI(&MBB, DL, get(Sbbm::B)).addMBB(TBB);
    } else {
      //instantiateCondBranch(MBB, DL, TBB, Cond);
      BuildMI(&MBB, DL, get(Sbbm::BCOND))
        .addOperand(Cond[0])
        .addMBB(TBB);
    }
    return 1;
  }

  // Two-way conditional branch.
  //instantiateCondBranch(MBB, DL, TBB, Cond);
  BuildMI(&MBB, DL, get(Sbbm::BCOND))
    .addOperand(Cond[0])
    .addMBB(TBB);
  BuildMI(&MBB, DL, get(Sbbm::B)).addMBB(FBB);
  return 2;
}


bool SbbmInstrInfo::PredicateInstruction(
  MachineInstr *MI, const SmallVectorImpl<MachineOperand> &Pred)
  const
{
  const int PredOpCode = Sbbm::getPredicated(MI->getOpcode());
  if (PredOpCode != -1) {
    MI->setDesc(get(PredOpCode));
  }

  for (const auto& Op : Pred) {
    MI->addOperand(Op);
  }
  return true;
}

void SbbmInstrInfo::anchor() { }
