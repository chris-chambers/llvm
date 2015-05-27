# Copyright 2015, Christopher Chambers
# Distributed under the GNU GPL v3. See LICENSE.TXT for details.

LEVEL = ../../..
LIBRARYNAME = LLVMSbbmCodeGen
TARGET = Sbbm

# Make sure that tblgen is run.
BUILT_SOURCES = \
	SbbmGenAsmWriter.inc \
	SbbmGenCallingConv.inc \
	SbbmGenDAGISel.inc \
	SbbmGenInstrInfo.inc \
	SbbmGenRegisterInfo.inc \
	SbbmGenSubtargetInfo.inc

DIRS = InstPrinter TargetInfo MCTargetDesc

include $(LEVEL)/Makefile.common
