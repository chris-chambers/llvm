; RUN: llc < %s -mtriple=sbbm | FileCheck %s

define i32 @test1(i32* %a) {
entry1:
; CHECK-LABEL: test1:
; CHECK: ldr r0, [r0]
; CHECK-NEXT: b lr
  %res = load i32* %a
  ret i32 %res
}
