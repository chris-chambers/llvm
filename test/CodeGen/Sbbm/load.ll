; RUN: llc < %s -mtriple=sbbm | FileCheck %s

define i32 @test1(i32* %a) {
entry1:
; CHECK-LABEL: test1:
; CHECK: ldr r0, [r0]
; CHECK-NEXT: b lr
  %res = load i32* %a
  ret i32 %res
}

define i32 @test2(i16* %a) {
entry2:
; CHECK-LABEL: test2:
; CHECK: ldrh r0, [r0]
; CHECK-NEXT: b lr
  %tmp = load i16* %a
  %res = zext i16 %tmp to i32
  ret i32 %res
}

define i32 @test3(i16* %a) {
entry3:
; CHECK-LABEL: test3:
; CHECK: ldrsh r0, [r0]
; CHECK-NEXT: b lr
  %tmp = load i16* %a
  %res = sext i16 %tmp to i32
  ret i32 %res
}

define i32 @test4(i8* %a) {
entry4:
; CHECK-LABEL: test4:
; CHECK: ldrb r0, [r0]
; CHECK-NEXT: b lr
  %tmp = load i8* %a
  %res = zext i8 %tmp to i32
  ret i32 %res
}

define i32 @test5(i8* %a) {
entry5:
; CHECK-LABEL: test5:
; CHECK: ldrsb r0, [r0]
; CHECK-NEXT: b lr
  %tmp = load i8* %a
  %res = sext i8 %tmp to i32
  ret i32 %res
}
