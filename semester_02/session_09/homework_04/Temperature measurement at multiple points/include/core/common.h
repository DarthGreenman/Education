/// common.h

#ifndef COMMON_H_FOR_THE_COMPONENT_LIBRARY_PROJECT
#define COMMON_H_FOR_THE_COMPONENT_LIBRARY_PROJECT

#if defined(_MSC_VER)
#define FORCEINLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#define FORCEINLINE inline __attribute__((always_inline))
#else
#define FORCEINLINE inline
#endif

#endif /// !defined(COMMON_H_FOR_THE_COMPONENT_LIBRARY_PROJECT)

