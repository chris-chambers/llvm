; RUN: opt -O2 < %s -mtriple=sbbm | llc | FileCheck %s

target triple = "sbbm"

;define void @test1() {
;entry1:
;; CHECK-LABEL: test1:
;; CHECK: ;APP
;; CHECK-NEXT: raw scoreboard objectives remove foo
;; CHECK-NEXT: ;NO_APP
;; CHECK-NEXT: b lr
;  call void asm sideeffect "def obj, foo", ""()
;  call void asm sideeffect "raw scoreboard objectives remove %obj", ""()
;  ret void
;}
;
;
;define void @test2() {
;entry2:
;    call void asm sideeffect "def obj, Hydration", ""()
;    call void asm sideeffect "def crit, dummy", ""()
;    call void asm sideeffect "def disp, Hydration", ""()
;    call void asm sideeffect "raw scoreboard objectives add %obj %crit %disp", ""()
;    ret void
;}
;
;define void @test3() {
;entry3:
;	call void asm sideeffect "def sel, @e[score_iter_main_45_22_min=1,score_iter_main_45_22=1]", ""()
;	call void asm sideeffect "def obj, Hydration", ""()
;	call void asm sideeffect "mov r3, %sel, %obj", ""()
;    ret void
;}
;
;define void @test4(i32 %a) {
;entry4:
;    %cond = icmp eq i32 %a, 10
;    br i1 %cond, label %t, label %f
;t:
;	call void asm sideeffect "def sel, @e[score_iter_main_45_22_min=1,score_iter_main_45_22=1]", ""()
;	call void asm sideeffect "def obj, Foo", ""()
;	call void asm sideeffect "mov r3, %sel, %obj", ""()
;    ret void
;f:
;	call void asm sideeffect "def sel, @e[score_iter_main_45_22_min=1,score_iter_main_45_22=1]", ""()
;	call void asm sideeffect "def obj, Bar", ""()
;	call void asm sideeffect "mov r3, %sel, %obj", ""()
;    ret void
;}
;
;
;define void @test5(i32 %a) {
;entry5:
;	call void asm sideeffect "def obj, Stop", ""()
;	call void asm sideeffect "def crit, dummy", ""()
;	call void asm sideeffect "def disp, ", ""()
;	call void asm sideeffect "raw scoreboard objectives add %obj %crit %disp", ""()
;    ret void
;}

define void @foo() alwaysinline {
	call void asm sideeffect "def obj, Stop", ""()
    ret void
}

define void @test6(i32 %a) {
entry6:
    call void @foo()
	call void asm sideeffect "raw scoreboard objectives remove %obj", ""()
    ret void
}
