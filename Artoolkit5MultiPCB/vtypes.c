//
// Created by ezzpe on 16/04/2020.
//

#include "Headers/math/vtypes.h"

#ifdef MS_HAVE_PEPRINTF
#include "peprintf.h"
#else
#define peprintf( type, ptr, printfArgs) printf( printfArgs )
#endif

/* TODO: The 64 bit file seek/tell functions are causing problems in mechanicalmodel.c, particularly _ftelli64 returns
 * an arbitrary negative file offset after a single byte is read from the file. The same code reading the same file
 * works in other files. The issue is probably a 32-64 bit conversion / overflow somewhere.
 * 64 bit file offsets are needed for larger models so this needs fixing.
 */
ML_INT VP_ftell(FILE* fp) {
	//ML_INT pos = ftell( fp );
	ML_INT pos = _ftelli64(fp);

	if (pos < 0) {
		peprintf(PEPSTR_ERROR, NULL, "ftell returned negative file offset (%d) (%s)\n", pos, strerror(errno));
	}

	return pos;
}

ML_INT VP_fseek(FILE* fp, ML_INT offset, ML_INT pos) {
	//return fseek( fp, offset, pos );
	return _fseeki64(fp, offset, pos);
}