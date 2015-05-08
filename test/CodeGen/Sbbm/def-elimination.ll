; RUN: opt -O2 < %s -mtriple=sbbm | llc | FileCheck %s

target triple = "sbbm"

define void @test1() {
entry1:
; CHECK-LABEL: test1:
; CHECK: ;APP
; CHECK-NEXT: raw scoreboard objectives remove foo
; CHECK-NEXT: ;NO_APP
; CHECK-NEXT: b lr
  call void asm sideeffect "def obj, foo", ""()
  call void asm sideeffect "raw scoreboard objectives remove %obj", ""()
  ret void
}


define void @test2() {
entry2:
; CHECK-LABEL: test2:
; CHECK: ;APP
; CHECK-NEXT: raw scoreboard objectives add foo bar baz
; CHECK-NEXT: ;NO_APP
; CHECK-NEXT: b lr
    call void asm sideeffect "def obj, foo", ""()
    call void asm sideeffect "def crit, bar", ""()
    call void asm sideeffect "def disp, baz", ""()
    call void asm sideeffect "raw scoreboard objectives add %obj %crit %disp", ""()
    ret void
}

define void @test3() {
entry3:
	call void asm sideeffect "def sel, @e[score_iter_main_45_22_min=1,score_iter_main_45_22=1]", ""()
	call void asm sideeffect "def obj, Hydration", ""()
	call void asm sideeffect "mov r3, %sel, %obj", ""()
    ret void
}

define void @foo() alwaysinline {
	call void asm sideeffect "def obj, foo", ""()
    ret void
}

define void @test4(i32 %a) {
entry4:
; CHECK-LABEL: test4:
; CHECK: ;APP
; CHECK-NEXT: raw scoreboard objectives remove foo
; CHECK-NEXT: ;NO_APP
; CHECK-NEXT: b lr
    call void @foo()
	call void asm sideeffect "raw scoreboard objectives remove %obj", ""()
    ret void
}

define void @test5() {
entry3:
; CHECK-LABEL: test5:
; CHECK: ;APP
; CHECK-NEXT: raw 1 2
; CHECK-NEXT: ;NO_APP
; CHECK-NEXT: b lr
	call void asm sideeffect "def foo, 1", ""()
	call void asm sideeffect "def bar, %foo", ""()
	call void asm sideeffect "def foo, 2", ""()
	call void asm sideeffect "def baz, %foo", ""()
	call void asm sideeffect "raw %bar %baz", ""()
    ret void
}