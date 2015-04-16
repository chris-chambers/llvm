; RUN: llc < %s -mtriple=sbbm | FileCheck %s

; noreturn + ret => halt
define void @test1() noreturn {
entry:
; CHECK-LABEL: test1:
; CHECK: halt
  ret void
}
