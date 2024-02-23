/**		Virtual Prototyping for Power Electronics
 *
 * 		@file:				types.h
 * 		Author(s): 			P L Evans
 * 		Creation Date:		16 Apr 2019
 *
 * 		Description:
 *		Define integer types and associated functions
 *
 */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef VP_MATH_TYPES_H
#define VP_MATH_TYPES_H

#ifndef ML_INT_DEFINED
#define ML_INT_DEFINED
#include <stdint.h>
#include <stdio.h>
	typedef int64_t     ML_INT;
	typedef uint64_t    ML_UINT;
#endif

	ML_INT VP_ftell(FILE* fp);

	ML_INT VP_fseek(FILE* fp, ML_INT offset, ML_INT pos);

#if defined(WIN32) || defined (WIN64)
#include <windows.h>
#define WINDOWS

#else
	/* TODO: Would need updating with64-bit compatible functions for other os */

#endif

#endif

#ifdef __cplusplus
}
#endif
