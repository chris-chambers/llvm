# FIXME: Add standard header.

add_llvm_target(SbbmCodeGen
  SbbmTargetMachine.cpp
  )

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
