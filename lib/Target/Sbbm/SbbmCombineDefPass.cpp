#include "SbbmPasses.h"
#include "SbbmTargetMachine.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InlineAsm.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/Regex.h"

using namespace llvm;

#define DEBUG_TYPE "sbbm-def-combine"

namespace {

class DefExpander : public FunctionPass {
public:
  DefExpander() : FunctionPass(ID) { }
  static char ID;

  virtual bool runOnFunction(Function &F) {
    bool MadeChange = false;
    for (auto& BB : F) {
      StringMap<std::string> Defs;
      MadeChange |= ProcessBB(BB, Defs);
    }
    return MadeChange;
  }

  bool ProcessBB(BasicBlock &BB, StringMap<std::string> &Defs)
  {
    // FIXME: Initialize this regular expression as a field, at construction time.
    // FIXME: Should use SbbmAsmParser.  It will be more reliable than a regex.
    Regex DefRe("def[ \t]+([^,]+),[ \t]*(.*)");

    bool MadeChange = false;

    for (auto &Inst : BB) {
      CallInst* CI = dyn_cast<CallInst>(&Inst);
      if (!CI) {
        continue;
      }

      if (Function *F = CI->getCalledFunction()) {
        if (F->hasFnAttribute(Attribute::AlwaysInline)) {
          if (F->size() == 1) {
            // FIXME: Guard against cycles!
            ProcessBB(*F->begin(), Defs);
          }
        }
        continue;
      }

      if (!CI->isInlineAsm()) {
        continue;
      }

      InlineAsm* IA = cast<InlineAsm>(CI->getCalledValue());
      const std::string& AsmStr = IA->getAsmString();

      if (AsmStr.compare(0, 3, "def") == 0) {
        SmallVector<StringRef, 3> Matches;
        if (!DefRe.match(AsmStr, &Matches)) {
          // FIXME: Do better.
          report_fatal_error("incorrect def!");
        }

        Defs.insert(std::make_pair(Matches[1], Matches[2]));
      } else {
        // Expand def references, mark defs as consumed.
        std::string Expanded(AsmStr);
        StringMap<bool> UsedDefs;

        // TODO: Move this to its own function.
        for (const auto& Entry : Defs) {
          std::string Pattern = "%" + std::string(Entry.first());
          Regex Re(Pattern);
          std::string S = Re.sub(Entry.second, Expanded);
          if (S.compare(Expanded) != 0) {
            DEBUG(dbgs() << "Expand: " << Expanded << " => " << S << "\n");

            UsedDefs.insert(std::make_pair(Entry.first(), true));
            Expanded = S;
          }
        }

        if (!UsedDefs.empty()) {
          for (const auto& Entry : UsedDefs) {
            Defs.erase(Entry.first());
          }

          DEBUG(dbgs()
            << "Removed " << UsedDefs.size() << " consumed def(s). "
            << "Remaining: " << Defs.size() << "\n\n");

          InlineAsm* NewIA = InlineAsm::get(
            IA->getFunctionType(), Expanded, IA->getConstraintString(),
            IA->hasSideEffects(), IA->isAlignStack(), IA->getDialect());

          CI->setCalledFunction(NewIA);
          MadeChange = true;
        }
      }
    }
    return MadeChange;
  }
};

class DefRemover : public FunctionPass {
public:
  DefRemover() : FunctionPass(ID) { }
  static char ID;

  virtual bool runOnFunction(Function &F) {
    bool MadeChange = false;
    for (auto &BB : F) {
      auto II = BB.begin();
      while (II != BB.end()) {
        if (CallInst* CI = dyn_cast<CallInst>(II)) {
          if (CI->isInlineAsm()) {
            InlineAsm* IA = cast<InlineAsm>(CI->getCalledValue());
            if (IA->getAsmString().compare(0, 3, "def") == 0) {
              ++II;
              CI->eraseFromParent();
              MadeChange = true;
              continue;
            }
          }
        }
        ++II;
      }
    }
    return MadeChange;
  }
};

} // anonymous namespace

char DefExpander::ID = 0;
char &llvm::SbbmDefExpanderID = DefExpander::ID;
static RegisterPass<DefExpander> X("sbbm-def-expand", "SBBM Def Expand Pass", false, false);

char DefRemover::ID = 0;
char &llvm::SbbmDefRemoverID = DefRemover::ID;
static RegisterPass<DefRemover> Y("sbbm-def-remove", "SBBM Def Remove Pass", false, false);
