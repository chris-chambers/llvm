; RUN: llc < %s -mtriple=sbbm | FileCheck %s

; sub reg-reg is recognized
define i32 @test1(i32 %a, i32 %b) {
entry:
; CHECK-LABEL: test1:
; CHECK: sub r0, r1
  %x = sub i32 %a, %b
  ret i32 %x
}

; sub reg-imm is recognized
define i32 @test2(i32 %a) {
entry:
; CHECK-LABEL: test2:
; CHECK: sub r0, #37
  %x = sub i32 %a, 37
  ret i32 %x
}

; sub constant folding works
define i32 @test3() {
; CHECK-LABEL: test3:
; CHECK: mov r0, #-1
  %x = sub i32 1, 2
  ret i32 %x
}

;; sub reg-imm is not commutable
;define i32 @test4(i32 %a) {
;; XXCHECK-LABEL: test4:
;; XXCHECK:      mov r1, #15
;; XXCHECK-NEXT: sub r1, r0
;; XXCHECK-NEXT: mov r0, r1
;  %x = sub i32 15, %a
;  ret i32 %x
;}

; sub reg-imm with negative constant is converted to add reg-imm
define i32 @test5(i32 %a) {
; CHECK-LABEL: test5:
; CHECK: add r0, #20
  %x = sub i32 %a, -20
  ret i32 %x
}
