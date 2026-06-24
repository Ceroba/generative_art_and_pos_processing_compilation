#pragma once
#include "utl.h"
typedef uint32_t uint32;
typedef uint8_t uint8;
typedef uint16_t uint16;
#define GBL static inline
namespace gbl{
	GBL float dt = 0;
	GBL float dt_max = 0;
	GBL float time = 0;
}