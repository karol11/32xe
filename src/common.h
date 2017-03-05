#ifndef _32xe_common_h_
#define _32xe_common_h_

#include <stdint.h>

typedef uint8_t  byte;
typedef int8_t   sbyte;
typedef uint16_t word;
typedef int16_t  sword;

#ifndef MSVC
typedef uint8_t  bool;
#define false 0
#define true 1
#endif

void init(void);
void loop(void);
void reflash(void);

#endif
