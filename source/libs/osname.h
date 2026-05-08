/*
 *  Defines OS name and type
 *
 *  (c) 2026  Martin TÁBOR
 *  GNU GPL v3.0+
 *
 */


#ifndef OSNAME_H
#define OSNAME_H


#ifndef OSNAME
  #if defined(__ANDROID__)
    #define OSNAME "Android"
    #define OS_UNIX
    #define OS_ANDROID
  #elif defined (__EMSCRIPTEN__)
    #define OSNAME "Web"
    #define OS_WEB
    #include <emscripten/emscripten.h>
  #elif defined(__DOS__) || defined(__MSDOS__) || defined(MSDOS)
    #define OSNAME  "DOS"
    #define OS_DOS
  #elif defined(_WIN32) || defined(_WIN64)
    #define OSNAME  "Windows"
    #define OS_WIN
  #elif defined(__APPLE__)
    #define OSNAME  "macOS"
    #define OS_UNIX
    #define OS_MAC
  #elif defined(__linux__)
    #define OSNAME  "Linux"
    #define OS_UNIX
    #define OS_LINUX
  #elif defined(__FreeBSD__)
    #define OSNAME  "FreeBSD"
    #define OS_UNIX
    #define OS_BSD
  #elif defined(__unix__)
    #define OSNAME  "UN*X"
    #define OS_UNIX
  #else
    #define OSNAME  "?"
  #endif
#endif


#endif  /* OSNAME_H */
