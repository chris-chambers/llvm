// FIXME: Add standard header.

#include "SbbmMCAsmInfo.h"

using namespace llvm;

SbbmMCAsmInfo::SbbmMCAsmInfo(StringRef TT) {
  SupportsDebugInformation = false;
  HasDotTypeDotSizeDirective = false;
  CommentString = ";";
  SeparatorString = "|";

  HiddenVisibilityAttr = MCSA_Invalid;
  HiddenDeclarationVisibilityAttr = MCSA_Invalid;
  ProtectedVisibilityAttr = MCSA_Invalid;
}

void SbbmMCAsmInfo::anchor() { }
