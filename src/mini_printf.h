#ifndef __mini_prinf_h__
#define __mini_prinf_h__

#include <stdarg.h>
//return the number of printable character produced
int mini_printf(const char* formatString, ...);
int mini_snscanf(char buffer[], unsigned int bufferSize, const char* formatString, ...);

#endif
