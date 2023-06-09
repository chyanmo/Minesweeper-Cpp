#pragma once

// 字符集问题
#ifdef UNICODE
#undef UNICODE
#endif // UNICODE

#include <easyx.h>
#include <string>

using u16_t = uint16_t;

// #define DEBUG