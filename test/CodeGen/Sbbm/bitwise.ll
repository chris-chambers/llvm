; RUN: llc < %s -mtriple=sbbm | FileCheck %s

; and reg-reg is recognized
define i32 @test1(i32 %a, i32 %b) {
entry:
; CHECK-LABEL: test1:
; CHECK: and r0, r1
  %x = and i32 %a, %b
  ret i32 %x
}

; orr reg-reg is recognized
define i32 @test2(i32 %a, i32 %b) {
entry:
; CHECK-LABEL: test2:
; CHECK: orr r0, r1
  %x = or i32 %a, %b
  ret i32 %x
}

; eor reg-reg is recognized
define i32 @test3(i32 %a, i32 %b) {
entry:
; CHECK-LABEL: test3:
; CHECK: eor r0, r1
  %x = xor i32 %a, %b
  ret i32 %x
}
