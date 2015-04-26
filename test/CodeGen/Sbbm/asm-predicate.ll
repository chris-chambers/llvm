; RUN: llc < %s -mtriple=sbbm | FileCheck %s

; branch, eq, reg-imm
define void @test1(i32 %a) {
entry1:
  %0 = icmp eq i32 %a, 10
  br i1 %0, label %t, label %f
t:
  call void asm sideeffect "raw say hi", ""()
  ret void
f:
  call void asm sideeffect "raw say bye", ""()
  ret void
}
