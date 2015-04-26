# FIXME: Add standard header.

set(LLVM_TARGET_DEFINITIONS Sbbm.td)

tablegen(LLVM SbbmGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM SbbmGenCallingConv.inc -gen-callingconv)
tablegen(LLVM SbbmGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM SbbmGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM SbbmGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM SbbmGenSubtargetInfo.inc -gen-subtarget)
add_public_tablegen_target(SbbmCommonTableGen)

add_llvm_target(SbbmCodeGen
  SbbmAsmPrinter.cpp
  SbbmCombineDefPass.cpp
  SbbmFrameLowering.cpp
  SbbmInstrInfo.cpp
  SbbmISelDAGToDAG.cpp
  SbbmSubtarget.cpp
  SbbmTargetLowering.cpp
  SbbmTargetMachine.cpp
  )

add_subdirectory(InstPrinter)
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
