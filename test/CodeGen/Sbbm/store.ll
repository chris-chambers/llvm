define i32* @foo() {
  %addr = alloca i32
  %addr2 = alloca i16
  ret i32* %addr
}
