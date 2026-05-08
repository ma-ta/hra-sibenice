/*
 *  Defines architecture name
 *
 *  (c) 2026  Martin TÁBOR
 *  GNU GPL v3.0+
 *
 */


#ifndef ARCHNAME_H
#define ARCHNAME_H


#if defined(__EMSCRIPTEN__)
  #if defined(__wasm64__)
    #define ARCH  "wasm64"
  #elif defined(__wasm32__)
    #define ARCH  "wasm32"
  #elif defined(__wasm__)
    #define ARCH  "Wasm"
  #else
    #define ARCH  "?"
  #endif
#elif defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64)
  #define ARCH  "x86-64"
#elif defined(_M_I86)  /* 16-bit: MSVC, Watcom */
  #define ARCH  "x86 16-bit"
#elif defined(__i386__) || defined(_M_IX86)
  #define ARCH  "x86"
#elif defined(__aarch64__) || defined(_M_ARM64)
  #define ARCH  "arm64"
#elif defined(__arm__) || defined(_M_ARM)
  #define ARCH  "arm 32-bit"
#elif defined(__riscv)
  #if __riscv_xlen == 64
    #define ARCH  "rv64"
  #elif __riscv_xlen == 32
    #define ARCH  "rv32"
  #else
    #define ARCH  "risc-v"
  #endif
#else
  #define ARCH  "?"
#endif


#endif  /* ARCHNAME_H */
