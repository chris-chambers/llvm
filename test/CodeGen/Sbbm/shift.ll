; RUN: llc < %s -mtriple=sbbm | FileCheck %s

; shl reg-reg is recognized
define i32 @test1(i32 %a, i32 %b) {
entry:
; CHECK-LABEL: test1:
; CHECK: lsl r0, r1
  %x = shl i32 %a, %b
  ret i32 %x
}

; ashr reg-reg is recognized
define i32 @test2(i32 %a, i32 %b) {
entry:
; CHECK-LABEL: test2:
; CHECK: asr r0, r1
  %x = ashr i32 %a, %b
  ret i32 %x
}

; lshr reg-reg is recognized
define i32 @test3(i32 %a, i32 %b) {
entry:
; CHECK-LABEL: test3:
; CHECK: lsr r0, r1
  %x = lshr i32 %a, %b
  ret i32 %x
}
