; RUN: llc < %s -mtriple=sbbm | FileCheck %s

; ret => bl r
define void @test1() {
entry:
; CHECK-LABEL: test1:
; CHECK: b lr
  ret void
}

; noreturn + ret => halt
define void @test2() noreturn {
entry:
; CHECK-LABEL: test2:
; CHECK: halt
  ret void
}
