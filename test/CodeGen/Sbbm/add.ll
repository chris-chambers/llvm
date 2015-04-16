; RUN: llc < %s -mtriple=sbbm | FileCheck %s

; add reg-reg is recognized
define i32 @test1(i32 %a, i32 %b) {
entry:
; CHECK-LABEL: test1:
; CHECK: add r0, r1
  %x = add i32 %a, %b
  ret i32 %x
}

; add reg-imm is recognized
define i32 @test2(i32 %a) {
entry:
; CHECK-LABEL: test2:
; CHECK: add r0, #37
  %x = add i32 %a, 37
  ret i32 %x
}

; add imm-reg is commutable to reg-imm
define i32 @test3(i32 %a) {
; CHECK-LABEL: test3:
; CHECK: add r0, #15
  %x = add i32 15, %a
  ret i32 %x
}
