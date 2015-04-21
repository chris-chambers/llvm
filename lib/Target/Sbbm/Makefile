# FIXME: Add standard header.

LEVEL = ../../..
LIBRARYNAME = LLVMSbbmCodeGen
TARGET = Sbbm

# Make sure that tblgen is run.
BUILT_SOURCES = \
	SbbmGenAsmWriter.inc \
	SbbmGenDAGISel.inc \
	SbbmGenCallingConv.inc \
	SbbmGenInstrInfo.inc \
	SbbmGenRegisterInfo.inc \
	SbbmGenSubtargetInfo.inc

DIRS = InstPrinter TargetInfo MCTargetDesc

include $(LEVEL)/Makefile.common
