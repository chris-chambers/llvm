// Copyright 2015, Christopher Chambers
// Distributed under the GNU GPL v3. See LICENSE.TXT for details.

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
