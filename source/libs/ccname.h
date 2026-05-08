/*
 *  Defines compiler name
 *
 *  (c) 2026  Martin TÁBOR
 *  GNU GPL v3.0+
 *
 */


#ifndef CCNAME_H
#define CCNAME_H


#ifndef CCNAME
  #if defined (__EMSCRIPTEN__)
    #define CCNAME  "Emscripten"
    #define CCVER   __EMSCRIPTEN_MAJOR__
    #define CCMINOR __EMSCRIPTEN_MINOR__
    #define CCTINY  __EMSCRIPTEN_TINY__
  #elif defined(_MSC_VER)
    #define CCNAME  "MSVC"
    #define CCVER   _MSC_VER
  #elif defined(__WATCOMC__)
    #define CCNAME  "Watcom"
    #ifdef WATCOMC_VER
      #define CCVER  WATCOMC_VER
    #else
      #define CCVER  __WATCOMC__
    #endif
  #elif defined(__clang__)
    #define CCNAME   "Clang"
    #define CCVER    __clang_major__
    #define CCMINOR  __clang_minor__
    #define CCTINY   __clang_patchlevel__
  #elif defined(__GNUC__)
    #if defined(__DJGPP__)
      #define CCNAME  "DJGPP"
    #else
      #define CCNAME  "GCC"
    #endif
    #define CCVER    __GNUC__
    #define CCMINOR  __GNUC_MINOR__
    #define CCTINY   __GNUC_PATCHLEVEL__
  #else
    #define CCNAME  "?"
    #define CCVER  (-1)
  #endif

  #ifndef CCMINOR
    #define CCMINOR  (-1)
  #endif
  #ifndef CCTINY
    #define CCTINY  (-1)
  #endif
#else
  #ifndef CCVER
    #define CCVER  (-1)
  #endif
#endif


#endif  /* CCNAME_H */
