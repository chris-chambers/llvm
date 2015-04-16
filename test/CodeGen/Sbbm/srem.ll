; RUN: llc < %s -mtriple=sbbm | FileCheck %s

; ordinary srem is recognized
define i32 @test1(i32 %a, i32 %b) {
entry:
; CHECK-LABEL: test1:
; CHECK: srem r0, r1
  %x = srem i32 %a, %b
  ret i32 %x
}

; srem is not commutable
define i32 @test2(i32 %a, i32 %b) {
entry:
; CHECK-LABEL: test2:
; CHECK: srem r1, r0
; CHECK-NEXT: mov r0, r1
  %x = srem i32 %b, %a
  ret i32 %x
}

; srem constant folding works
define i32 @test3() {
entry:
; CHECK-LABEL: test3:
; CHECK: mov r0, #2
  %x = srem i32 37, 5
  ret i32 %x
}
