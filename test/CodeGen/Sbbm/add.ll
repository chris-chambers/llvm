; RUN: llc < %s -mtriple=sbbm | FileCheck %s

; add reg-reg is recognized
define i32 @test1(i32 %a, i32 %b) {
entry:
; CHECK-LABEL: test1:
; CHECK: add r0, r1
  %x = add i32 %a, %b
  ret i32 %x
}
