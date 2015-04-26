; RUN: llc < %s -mtriple=sbbm | FileCheck %s

; branch, eq, reg-imm
define i32 @test1(i32 %a) {
entry1:
; CHECK-LABEL: test1:
; CHECK: rng p0, r0, #10, #10
; CHECK-NEXT: mov r0, #2 {p0, #0, #0}
; CHECK-NEXT: mov r0, #1 {p0, #1, #1}
; CHECK-NEXT: b lr
  %0 = icmp eq i32 %a, 10
  br i1 %0, label %t, label %f
t:
  ret i32 1
f:
  ret i32 2
}

; branch, ne, reg-imm
define i32 @test2(i32 %a) {
entry2:
; CHECK-LABEL: test2:
; CHECK: rng p0, r0, #10, #10
; CHECK-NEXT: mov r0, #2 {p0, #1, #1}
; CHECK-NEXT: mov r0, #1 {p0, #0, #0}
; CHECK-NEXT: b lr
  %0 = icmp ne i32 %a, 10
  br i1 %0, label %t, label %f
t:
  ret i32 1
f:
  ret i32 2
}

; branch, sge, reg-imm
define i32 @test3(i32 %a) {
entry3:
; CHECK-LABEL: test3:
; CHECK: rng p0, r0, #-2147483648, #9
; CHECK-NEXT: mov r0, #2 {p0, #1, #1}
; CHECK-NEXT: mov r0, #1 {p0, #0, #0}
; CHECK-NEXT: b lr
  %0 = icmp sge i32 %a, 10
  br i1 %0, label %t, label %f
t:
  ret i32 1
f:
  ret i32 2
}

; branch, sgt, reg-imm
define i32 @test4(i32 %a) {
entry4:
; CHECK-LABEL: test4:
; CHECK: rng p0, r0, #-2147483648, #10
; CHECK-NEXT: mov r0, #2 {p0, #1, #1}
; CHECK-NEXT: mov r0, #1 {p0, #0, #0}
; CHECK-NEXT: b lr
  %0 = icmp sgt i32 %a, 10
  br i1 %0, label %t, label %f
t:
  ret i32 1
f:
  ret i32 2
}

; branch, sle, reg-imm
define i32 @test5(i32 %a) {
entry5:
; CHECK-LABEL: test5:
; CHECK: rng p0, r0, #11, #2147483647
; CHECK-NEXT: mov r0, #2 {p0, #1, #1}
; CHECK-NEXT: mov r0, #1 {p0, #0, #0}
; CHECK-NEXT: b lr
  %0 = icmp sle i32 %a, 10
  br i1 %0, label %t, label %f
t:
  ret i32 1
f:
  ret i32 2
}

; branch, slt, reg-imm
define i32 @test6(i32 %a) {
entry6:
; CHECK-LABEL: test6:
; CHECK: rng p0, r0, #10, #2147483647
; CHECK-NEXT: mov r0, #2 {p0, #1, #1}
; CHECK-NEXT: mov r0, #1 {p0, #0, #0}
; CHECK-NEXT: b lr
  %0 = icmp slt i32 %a, 10
  br i1 %0, label %t, label %f
t:
  ret i32 1
f:
  ret i32 2
}

; branch, eq, reg-reg
define i32 @test7(i32 %a, i32 %b) {
entry7:
; CHECK-LABEL: test7:
; CHECK: sub r0, r1
; CHECK-NEXT: rng p0, r0, #0, #0
; CHECK-NEXT: mov r0, #2 {p0, #0, #0}
; CHECK-NEXT: mov r0, #1 {p0, #1, #1}
; CHECK-NEXT: b lr
  %0 = icmp eq i32 %a, %b
  br i1 %0, label %t, label %f
t:
  ret i32 1
f:
  ret i32 2
}

; branch, ne, reg-reg
define i32 @test8(i32 %a, i32 %b) {
entry8:
; CHECK-LABEL: test8:
; CHECK: sub r0, r1
; CHECK-NEXT: rng p0, r0, #0, #0
; CHECK-NEXT: mov r0, #2 {p0, #1, #1}
; CHECK-NEXT: mov r0, #1 {p0, #0, #0}
; CHECK-NEXT: b lr
  %0 = icmp ne i32 %a, %b
  br i1 %0, label %t, label %f
t:
  ret i32 1
f:
  ret i32 2
}

; branch, sge, reg-reg
define i32 @test9(i32 %a, i32 %b) {
entry9:
; CHECK-LABEL: test9:
; CHECK: sub r0, r1
; CHECK-NEXT: rng p0, r0, #-2147483648, #-1
; CHECK-NEXT: mov r0, #2 {p0, #1, #1}
; CHECK-NEXT: mov r0, #1 {p0, #0, #0}
; CHECK-NEXT: b lr
  %0 = icmp sge i32 %a, %b
  br i1 %0, label %t, label %f
t:
  ret i32 1
f:
  ret i32 2
}

; branch, sgt, reg-reg
define i32 @test10(i32 %a, i32 %b) {
entry10:
; CHECK-LABEL: test10:
; CHECK: sub r0, r1
; CHECK-NEXT: rng p0, r0, #-2147483648, #0
; CHECK-NEXT: mov r0, #2 {p0, #1, #1}
; CHECK-NEXT: mov r0, #1 {p0, #0, #0}
; CHECK-NEXT: b lr
  %0 = icmp sgt i32 %a, %b
  br i1 %0, label %t, label %f
t:
  ret i32 1
f:
  ret i32 2
}

; branch, sle, reg-reg
define i32 @test11(i32 %a, i32 %b) {
entry11:
; CHECK-LABEL: test11:
; CHECK: sub r0, r1
; CHECK-NEXT: rng p0, r0, #1, #2147483647
; CHECK-NEXT: mov r0, #2 {p0, #1, #1}
; CHECK-NEXT: mov r0, #1 {p0, #0, #0}
; CHECK-NEXT: b lr
  %0 = icmp sle i32 %a, %b
  br i1 %0, label %t, label %f
t:
  ret i32 1
f:
  ret i32 2
}

; branch, slt, reg-reg
define i32 @test12(i32 %a, i32 %b) {
entry12:
; CHECK-LABEL: test12:
; CHECK: sub r0, r1
; CHECK-NEXT: rng p0, r0, #0, #2147483647
; CHECK-NEXT: mov r0, #2 {p0, #1, #1}
; CHECK-NEXT: mov r0, #1 {p0, #0, #0}
; CHECK-NEXT: b lr
  %0 = icmp slt i32 %a, %b
  br i1 %0, label %t, label %f
t:
  ret i32 1
f:
  ret i32 2
}
