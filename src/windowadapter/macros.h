#ifndef MACROS_DEF
#define MACROS_DEF

#define NO_DEFAULT_CONSTRUCTOR(X)\
    X() = delete;\
    X( const X &) = delete;\
    X &operator = (const X &) = delete;

#endif
