; RUN: llc < %s -mtriple=sbbm | FileCheck %s

; ordinary mul is recognized
define i32 @test1(i32 %a, i32 %b) {
entry:
; CHECK-LABEL: test1:
; CHECK: mul r0, r1
  %x = mul i32 %a, %b
  ret i32 %x
}

; mul is commutable
define i32 @test2(i32 %a, i32 %b) {
entry:
; CHECK-LABEL: test2:
; CHECK: mul r0, r1
  %x = mul i32 %b, %a
  ret i32 %x
}

; mul constant folding works
define i32 @test3() {
entry:
; CHECK-LABEL: test3:
; CHECK: mov r0, #50
  %x = mul i32 5, 10
  ret i32 %x
}
