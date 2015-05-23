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
entry5:
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

define void @test6() {
entry6:
; CHECK-LABEL: test6:
; CHECK: ;APP
; CHECK-NEXT: raw 1 2
; CHECK-NEXT: ;NO_APP
    call void @alfa1()
    call void @bravo1()
    call void @alfa2()
    call void @bravo2()
    call void asm sideeffect "raw %bravo1 %bravo2", ""()

; CHECK: ;APP
; CHECK-NEXT: raw 3 4
; CHECK-NEXT: ;NO_APP
; CHECK-NEXT: b lr
    call void @alfa3()
    call void @bravo1()
    call void @alfa4()
    call void @bravo2()
    call void asm sideeffect "raw %bravo1 %bravo2", ""()
    ret void
}

define void @alfa1() alwaysinline {
	call void asm sideeffect "def foo, 1", ""()
    ret void
}

define void @alfa2() alwaysinline {
	call void asm sideeffect "def foo, 2", ""()
    ret void
}

define void @alfa3() alwaysinline {
	call void asm sideeffect "def foo, 3", ""()
    ret void
}

define void @alfa4() alwaysinline {
	call void asm sideeffect "def foo, 4", ""()
    ret void
}

define void @bravo1() alwaysinline {
	call void asm sideeffect "def bravo1, %foo", ""()
    ret void
}

define void @bravo2() alwaysinline {
	call void asm sideeffect "def bravo2, %foo", ""()
    ret void
}

; Check that substitution does not eat backslashes.
define void @test7() {
entry7:
; CHECK-LABEL: test7:
; CHECK: ;APP
; CHECK-NEXT: raw \u9000
; CHECK-NEXT: ;NO_APP
    call void asm sideeffect "def foo, \5cu9000", ""()
    call void asm sideeffect "def bar, %foo", ""()
    call void asm sideeffect "def baz, %bar", ""()
    call void asm sideeffect "raw %baz", ""()
    ret void
}