/*		Virtual Prototyping for Power Electronics
 *
 * 		File:				matrixio.h
 * 		Author(s): 			P L Evans
 * 		Creation Date:		2009
 *
 * 		Description:
 *		Routines for printing / saving matrices
 *
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "Headers/matrix.h"


ML_UINT writeSmallMatrix(char* filename, char* delim, MATRIX mat, ML_UINT rows, ML_UINT cols) {
	ML_INT i, j;
	FILE* fp;
	char* cptr;

	if (!(fp = fopen(filename, "w"))) {
		return 1;
	}

	cptr = &filename[strlen(filename) - 1];
	while (cptr > filename && *cptr != '.') cptr--;
	if (cptr > filename) cptr--;

	fprintf(fp, "%c=[", *cptr);

	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			fprintf(fp, "%e%s", mat[i][j], delim);
		}
		if (i != rows - 1)
			fprintf(fp, ";\n");
		else
			fprintf(fp, "];\n");
	}

	fclose(fp);

	return 0;
}

ML_UINT writeSmallMatrixT(char* filename, char* delim, MATRIX mat, ML_UINT rows, ML_UINT cols) {
	ML_UINT i, j;
	FILE* fp;

	if (!(fp = fopen(filename, "w"))) {
		return 1;
	}

	fprintf(fp, "%s=[", filename);

	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			fprintf(fp, "%g%s", mat[j][i], delim);
		}
		if (i != rows - 1)
			fprintf(fp, ";\n");
		else
			fprintf(fp, "];\n");
	}

	fclose(fp);

	return 0;
}

ML_UINT writeSmallSparse(char* filename, spMATRIX A) {
	ML_INT i, il, iu, j;
	double** dptr;
	char* cptr;
	FILE* fp;

	if (A.nc <= 0 || A.nr <= 0) return 0;

	if (!(dptr = malloc(A.nc * sizeof(double*)))) {
		printf("Unable to allocate ptrs in writeSmallSparse (%d bytes)\n", A.nc * sizeof(double));
		getchar();
		exit(1);
	}

	if (!(fp = fopen(filename, "w"))) {
		printf("Unable open file in writeSmallSparse\n");
		getchar();
		exit(1);
	}

	cptr = &filename[strlen(filename) - 1];
	while (cptr > filename && *cptr != '.') cptr--;
	if (cptr > filename) cptr--;

	fprintf(fp, "%c = [ ", *cptr);

	for (i = 0; i < A.nr; i++) {
		il = A.ia[i];
		iu = A.ia[i + 1];

		for (j = 0; j < A.nc; j++)
			dptr[j] = NULL;

		/* set pointers to non-zero elements */
		for (j = il; j < iu; j++)
			dptr[A.ja[j]] = &(A.aa[j]);

		/* write row */
		for (j = 0; j < A.nc; j++)
			fprintf(fp, "%.8g ", dptr[j] ? *dptr[j] : 0.);

		fprintf(fp, "\n");
	}

	fprintf(fp, "]\n");

	fclose(fp);

	free(dptr);

	return 1;
}

ML_UINT printSmallSparse(spMATRIX A) {
	ML_INT i, il, iu, j;
	double** dptr;

	if (!(dptr = malloc(A.nc * sizeof(double*)))) {
		printf("Unable to allocate ptrs in writeSmallSparse (%d bytes)\n", A.nc * sizeof(double));
		getchar();
		exit(1);
	}


	for (i = 0; i < A.nr; i++) {
		il = A.ia[i];
		iu = A.ia[i + 1];

		for (j = 0; j < A.nc; j++)
			dptr[j] = NULL;

		/* set pointers to non-zero elements */
		for (j = il; j < iu; j++)
			dptr[A.ja[j]] = &(A.aa[j]);

		/* write row */
		for (j = 0; j < A.nc; j++)
			printf("%.8g ", dptr[j] ? *dptr[j] : 0.);

		printf("\n");
	}



	free(dptr);

	return 1;
}

ML_UINT writeSparseStruct(char* filename, spMATRIX A) {
	ML_INT i, il, iu, j;
	double** dptr;
	FILE* fp;

	if (!(dptr = malloc(A.nc * sizeof(double*)))) {
		printf("Unable to allocate ptrs in writeSmallSparse (%d bytes)\n", A.nc * sizeof(double));
		getchar();
		exit(1);
	}

	if (!(fp = fopen(filename, "w"))) {
		printf("Unable open file in writeSmallSparse\n");
		getchar();
		exit(1);
	}

	fprintf(fp, "%2.2s = [ ", filename);

	for (i = 0; i < A.nr; i++)
	{
		il = A.ia[i];
		iu = A.ia[i + 1];

		for (j = 0; j < A.nc; j++)
			dptr[j] = NULL;

		/* set pointers to non-zero elements */
		for (j = il; j < iu; j++)
			dptr[A.ja[j]] = &(A.aa[j]);

		/* write row */
		for (j = 0; j < A.nc; j++)
			fprintf(fp, "%c ", dptr[j] ? 'X' : '.');

		fprintf(fp, "\n");
	}

	fprintf(fp, "]\n");

	fclose(fp);

	free(dptr);

	return 1;
}

ML_UINT writeLargeSparse(char* filename, spMATRIX A) {
	ML_INT i, j;
	FILE* fp;

	if (!(fp = fopen(filename, "w"))) {
		printf("Unable open file in writeSmallSparse\n");
		return 0;
	}

	fprintf(fp, "rows = [ ");
	for (i = 0; i < A.nr; i++)
		for (j = A.ia[i]; j < A.ia[i + 1]; j++)
			fprintf(fp, "%d ", i + 1);
	fprintf(fp, "];\n");

	fprintf(fp, "cols = [ ");
	for (i = 0; i < A.nr; i++)
		for (j = A.ia[i]; j < A.ia[i + 1]; j++)
			fprintf(fp, "%d ", A.ja[j] + 1);
	fprintf(fp, "];\n");

	fprintf(fp, "vals = [ ");
	for (i = 0; i < A.nr; i++)
		for (j = A.ia[i]; j < A.ia[i + 1]; j++)
			fprintf(fp, "%f ", A.aa[j]);
	fprintf(fp, "];\n");

	fprintf(fp, "%2.2s = sparse( rows, cols, vals, %d, %d );", filename, A.nr, A.nc);

	fclose(fp);

	return 1;
}

ML_UINT writeLargeSparseMatrices(char* filename, ML_INT num, ...) {
	ML_INT i, j, mnum;
	FILE* fp;
	va_list args;
	spMATRIX* mat;
	char* mname;

	if (num < 1) {
		printf("Need at least one matrix to add to file\n");
		return 0;
	}

	if (!(fp = fopen(filename, "w"))) {
		printf("Unable open file in writeSmallSparse\n");
		return 0;
	}

	va_start(args, num);

	for (mnum = 0; mnum < num; mnum++) {
		mat = va_arg(args, spMATRIX*);
		mname = va_arg(args, char*);

		if (!mat->ia || !mat->ja || !mat->aa) continue;

		fprintf(fp, "rows = [ ");
		for (i = 0; i < mat->nr; i++)
			for (j = mat->ia[i]; j < mat->ia[i + 1]; j++)
				fprintf(fp, "%d ", i + 1);
		fprintf(fp, "];\n");

		fprintf(fp, "cols = [ ");
		for (i = 0; i < mat->nr; i++)
			for (j = mat->ia[i]; j < mat->ia[i + 1]; j++)
				fprintf(fp, "%d ", mat->ja[j] + 1);
		fprintf(fp, "];\n");

		fprintf(fp, "vals = [ ");
		for (i = 0; i < mat->nr; i++)
			for (j = mat->ia[i]; j < mat->ia[i + 1]; j++)
				fprintf(fp, "%e ", mat->aa[j]);
		fprintf(fp, "];\n");

		fprintf(fp, "%s = sparse( rows, cols, vals, %d, %d );\n\n", mname, mat->nr, mat->nc);
	}

	fclose(fp);

	va_end(args);

	return 1;
}

ML_UINT writeLargeSparseMatrices_DenseMAT(char* filename, char* prefix, ML_INT num, ...) {

	typedef struct {
		__int32 type;
		__int32 mrows;
		__int32 ncols;
		__int32 imagf;
		__int32 namelen;
	} Fmatrix;

	ML_INT i, j, mnum, col;
	__int32 k;
	FILE* fp;
	va_list args;
	spMATRIX* mat;
	char* mname;
	Fmatrix matHeader;
	char matName[256];
	char* matSuffix;
	__int32 size = 1;
	ML_INT row = 0;
	double* colVals = NULL;

	if (num < 1) {
		printf("Need at least one matrix to add to file\n");
		return 0;
	}

	if (!(fp = fopen(filename, "wb"))) {
		printf("Unable open file in writeSmallSparse\n");
		return 0;
	}

	strcpy(matName, prefix);
	strcat(matName, "_");
	matSuffix = matName + strlen(matName);

	matHeader.imagf = 0;

	va_start(args, num);

	for (mnum = 0; mnum < num; mnum++) {
		mat = va_arg(args, spMATRIX*);
		mname = va_arg(args, char*);

		if (!(colVals = realloc(colVals, mat->nr * sizeof(double)))) {
			fclose(fp);
			return 0;
		}

		matHeader.type = 0000;
		matHeader.mrows = mat->nr;
		matHeader.ncols = mat->nc;
		strcpy(matSuffix, mname);
		matHeader.namelen = strlen(matName) + 1;

		fwrite(&matHeader, sizeof(Fmatrix), 1, fp);
		fwrite(matName, sizeof(char), matHeader.namelen, fp);

		for (col = 0; col < mat->nc; col++) {
			memset(colVals, 0, mat->nr * sizeof(double));
			row = 0;
			for (row = 0; row < mat->nr; row++) {
				for (j = mat->ia[row]; j < mat->ia[row + 1]; j++) {
					if (mat->ja[j] == col) {
						colVals[row] = mat->aa[j];
						break;
					}
				}
			}
			fwrite(colVals, sizeof(double), mat->nr, fp);
		}
	}

	fclose(fp);

	va_end(args);

	return 1;
}



ML_UINT writeLargeSparseMatrix_MAT(char* filename, char* name, spMATRIX* mat) {

	typedef struct {
		__int32 type;
		__int32 mrows;
		__int32 ncols;
		__int32 imagf;
		__int32 namelen;
	} Fmatrix;

	ML_INT i, j;
	FILE* fp;
	va_list args;
	Fmatrix matHeader;
	char matName[256];
	char* matSuffix;
	double* rows;


	if (!(fp = fopen(filename, "wb"))) {
		printf("Unable open file in writeSmallSparse\n");
		return 0;
	}

	strcpy(matName, name);
	strcat(matName, "_");
	matSuffix = matName + strlen(matName);

	if (!(rows = malloc(mat->ia[mat->nr] * sizeof(int)))) {
		printf("Malloc error when processing matrix in writeLargeSparseMatrix_MAT\n");
		return 0;
	}

	matHeader.imagf = 0;


	for (i = 0; i < mat->nr; i++) {
		for (j = mat->ia[i]; j < mat->ia[i + 1]; j++) {
			rows[j] = (double)i;
		}
	}

	matHeader.type = 0000;
	matHeader.mrows = mat->ia[mat->nr];
	matHeader.ncols = 1;
	strcpy(matName, "row");
	matHeader.namelen = 3;
	fwrite(&matHeader, sizeof(Fmatrix), 1, fp);
	fwrite(matName, sizeof(char), matHeader.namelen, fp);
	fwrite(rows, sizeof(double), mat->ia[mat->nr], fp);

	for (j = 0; j < mat->ia[mat->nr]; j++) {
		rows[j] = (double)mat->ja[j];
	}

	matHeader.type = 0000;
	matHeader.mrows = mat->ia[mat->nr];
	matHeader.ncols = 1;
	strcpy(matName, "col");
	matHeader.namelen = 3;
	fwrite(&matHeader, sizeof(Fmatrix), 1, fp);
	fwrite(matName, sizeof(char), matHeader.namelen, fp);
	fwrite(rows, sizeof(double), mat->ia[mat->nr], fp);

	matHeader.type = 0000;
	matHeader.mrows = mat->ia[mat->nr];
	matHeader.ncols = 1;
	strcpy(matName, "val");
	matHeader.namelen = 3;
	fwrite(&matHeader, sizeof(Fmatrix), 1, fp);
	fwrite(matName, sizeof(char), matHeader.namelen, fp);
	fwrite(mat->aa, sizeof(double), mat->ia[mat->nr], fp);

	fclose(fp);

	return 1;
}

ML_UINT writeLargeSparseCXMatrices(char* filename, ML_INT num, ...) {
	ML_INT i, j, mnum;
	FILE* fp;
	va_list args;
	spMATRIX* mat;
	char* mname;

	if (num < 1) {
		printf("Need at least one matrix to add to file\n");
		return 0;
	}

	if (!(fp = fopen(filename, "w"))) {
		printf("Unable open file in writeSmallSparse\n");
		return 0;
	}

	va_start(args, num);

	for (mnum = 0; mnum < num; mnum++) {
		mat = va_arg(args, spMATRIX*);
		mname = va_arg(args, char*);

		fprintf(fp, "rows = [ ");
		for (i = 0; i < mat->nr; i++)
			for (j = mat->ia[i]; j < mat->ia[i + 1]; j++)
				fprintf(fp, "%d ", i + 1);
		fprintf(fp, "];\n");

		fprintf(fp, "cols = [ ");
		for (i = 0; i < mat->nr; i++)
			for (j = mat->ia[i]; j < mat->ia[i + 1]; j++)
				fprintf(fp, "%d ", mat->ja[j] + 1);
		fprintf(fp, "];\n");

		fprintf(fp, "vals = [ ");
		for (i = 0; i < mat->nr; i++)
			for (j = mat->ia[i]; j < mat->ia[i + 1]; j++)
				fprintf(fp, "%e%+ei, ", mat->aa[2 * j], mat->aa[2 * j + 1]);
		fprintf(fp, "];\n");

		fprintf(fp, "%s = sparse( rows, cols, vals, %d, %d );\n\n", mname, mat->nr, mat->nc);
	}

	fclose(fp);

	va_end(args);

	return 1;
}


ML_UINT printSmallMatrixT(MATRIX mat, ML_UINT rows, ML_UINT cols) {
	ML_UINT i, j;

	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			printf("% .2e ", mat[j][i]);
		}
		printf("\n");
	}

	return ML_OK;
}


ML_UINT printSmallMatrix(MATRIX mat, ML_UINT rows, ML_UINT cols) {
	ML_INT i, j;

	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			printf("%.2e ", mat[i][j]);
		}
		printf("\n");
	}

	return ML_OK;
}




ML_UINT writeSmallVector(char* filename, VECTOR v, ML_UINT size) {
	ML_INT i;
	FILE* fp;

	if (!(fp = fopen(filename, "w"))) {
		return -1;
	}

	fprintf(fp, "%.4s=[", filename);

	for (i = 0; i < size; i++) {
		fprintf(fp, "%g\n", v[i]);
	}

	fprintf(fp, "];\n");

	fclose(fp);

	return ML_OK;
}

ML_UINT writeSmallCXVector(char* filename, VECTOR v, ML_UINT size) {
	ML_UINT i;
	FILE* fp;

	if (!(fp = fopen(filename, "w"))) {
		return -1;
	}

	fprintf(fp, "%.4s=[", filename);
	for (i = 0; i < size; i++) {
		fprintf(fp, "%g+%gi\n", v[2 * i], v[2 * i + 1]);
	}
	fprintf(fp, "];\n");

	fclose(fp);

	return ML_OK;
}

ML_UINT printMatStruct(MATRIX m, ML_INT rows, ML_INT cols) {
	ML_INT i, j;

	for (i = 0; i < rows; i++) {
		printf("|");
		for (j = 0; j < cols; j++)
			printf("%c", (m[i][j] < ML_ZERO_TOL) ? ' ' : 'x');
		printf("|\n");
	}
	return ML_OK;
}


ML_UINT writeMatStruct(char* filename, MATRIX m, ML_INT rows, ML_INT cols) {
	ML_INT i, j;
	FILE* fp;

	if (!(fp = fopen(filename, "w"))) return 0;

	for (i = 0; i < rows; i++)
	{
		fprintf(fp, "|");
		for (j = 0; j < cols; j++)
			fprintf(fp, "%c", (m[i][j] < ML_ZERO_TOL) ? ' ' : 'x');
		fprintf(fp, "|\n");
	}
	fclose(fp);
	return ML_OK;
}


ML_UINT writeSmallDenseLapackMatrix(const char* filename, spMATRIX A) {
	ML_INT i, j, ind = 0;
	char* cptr;
	FILE* fp;

	if (A.nc <= 0 || A.nr <= 0) return 0;

	if (!(fp = fopen(filename, "w"))) {
		printf("Unable open file in writeSmallSparse\n");
		getchar();
		exit(1);
	}

	cptr = &filename[strlen(filename) - 1];
	while (cptr > filename && *cptr != '.') cptr--;
	if (cptr > filename) cptr--;

	fprintf(fp, "%c = [ ", *cptr);

	for (i = 0; i < A.nr; i++) {
		/* write row */
		for (j = 0; j < A.nc; j++)
			fprintf(fp, "%.8g ", A.aa[i * A.nc + j]);

		fprintf(fp, "\n");
	}

	fprintf(fp, "]\n");

	fclose(fp);

	return 1;
}