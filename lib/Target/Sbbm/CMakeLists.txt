# FIXME: Add standard header.

set(LLVM_TARGET_DEFINITIONS Sbbm.td)

tablegen(LLVM SbbmGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM SbbmGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM SbbmGenSubtargetInfo.inc -gen-subtarget)
add_public_tablegen_target(SbbmCommonTableGen)

add_llvm_target(SbbmCodeGen
  SbbmAsmPrinter.cpp
  SbbmInstrInfo.cpp
  SbbmRegisterInfo.cpp
  SbbmSubtarget.cpp
  SbbmTargetLowering.cpp
  SbbmTargetMachine.cpp
  )

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
