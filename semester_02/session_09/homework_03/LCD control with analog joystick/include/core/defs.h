// defs.h

#ifndef DEFS_H_FOR_THE_ARDUINO_UNO_PROJECT
#define DEFS_H_FOR_THE_ARDUINO_UNO_PROJECT

#if defined(_MSC_VER)
#define FORCEINLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#define FORCEINLINE inline __attribute__((always_inline))
#else
#define FORCEINLINE inline
#endif

#endif /// !DEFS_H_FOR_THE_ARDUINO_UNO_PROJECT
