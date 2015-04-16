; RUN: llc < %s -mtriple=sbbm | FileCheck %s

; ordinary sdiv is recognized
define i32 @test1(i32 %a, i32 %b) {
entry:
; CHECK-LABEL: test1:
; CHECK: sdiv r0, r1
  %x = sdiv i32 %a, %b
  ret i32 %x
}

; sdiv is not commutable
define i32 @test2(i32 %a, i32 %b) {
entry:
; CHECK-LABEL: test2:
; CHECK: sdiv r1, r0
; CHECK-NEXT: mov r0, r1
  %x = sdiv i32 %b, %a
  ret i32 %x
}

; sdiv constant folding works
define i32 @test3() {
entry:
; CHECK-LABEL: test3:
; CHECK: mov r0, #7
  %x = sdiv i32 35, 5
  ret i32 %x
}
