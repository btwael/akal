#ifndef BOUTGLAY_AKAL_CORE_TYPES_HH
#define BOUTGLAY_AKAL_CORE_TYPES_HH

//*-- NULL
#define NULL (0L)

//*-- i8
typedef char i8;
//*-- u8
typedef unsigned char u8;

//*-- i16
typedef short i16;
//*-- u16
typedef unsigned short u16;

//*-- i32
typedef int i32;
//*-- u32
typedef unsigned int u32;

//*-- i64
typedef long i64;
//*-- u64
typedef unsigned long u64;

//*-- Size
#ifdef AKAL_APPLICATION_TARGET_32BIT
typedef u32 Size;
#elif AKAL_APPLICATION_TARGET_64BIT
typedef u64 Size;
#endif


#endif // BOUTGLAY_AKAL_CORE_TYPES_HH
