/*		Virtual Prototyping for Power Electronics
 *
 * 		File:				matrix.h
 * 		Author(s): 			P L Evans
 * 		Creation Date:		2009
 *
 * 		Description:
 *		Basic library of matrix routines
 *
 */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef FTE_MATRIX_H
#define FTE_MATRIX_H

#include <stdlib.h>
#include <stdio.h>

#include "math/vtypes.h"

#define MATRIX_WARN_SPMATVEC_IO

#ifdef MS_HAVE_PEPRINTF
#include "peprintf.h"
#else
#define peprintf( type, ptr, printfArgs) printf( printfArgs )
#endif

#define ML_OK            0
#define ML_ERR_MALLOC    1
#define ML_ERR_SOLVOPT   2
#define ML_ERROR         3

#define ML_ZERO_TOL		1e-13

	/* matrix & vector */
	typedef double** MATRIX;
	typedef double* VECTOR;

	/* CSR sparse matrix */
	typedef struct _spMATRIX
	{
		double* aa;
		ML_INT* ja;
		ML_INT* ia;
		ML_INT             	nr;
		ML_INT             	nc;
	} spMATRIX;


	/* linked list for matrix assembly - dont think this was ever actually used */
	typedef struct _spMATENTRY_LL {
		double                  val;
		ML_INT                  col;
		struct _spMATENTRY_LL* n;
	} spMATENTRY_LL;

	/* real */

	/* matrix.c */
	void scaleSparseEquations(spMATRIX* M, spMATRIX* A, spMATRIX* B, spMATRIX* K, spMATRIX* C, spMATRIX* O, double s);
	ML_INT spGenerateIdentity(spMATRIX* mat, ML_INT size);
	void spMatVec(spMATRIX* A, VECTOR v, VECTOR o);
	void spMatVecT(spMATRIX* A, VECTOR v, VECTOR o);
	void spMatVecScale(spMATRIX* A, VECTOR v, VECTOR o, double s);
	void spMatVecSum(spMATRIX* A, VECTOR v, VECTOR o);
	void spMatVecTSum(spMATRIX* A, VECTOR v, VECTOR o);
	void spMatVecSub(spMATRIX* A, VECTOR v, VECTOR o);
	void spMatVecTSub(spMATRIX* A, VECTOR v, VECTOR o);
	void spMatVecScaleSum(spMATRIX* A, VECTOR v, VECTOR o, double s);
	ML_INT spMatGetCol(spMATRIX* A, ML_INT col, double* x);
	ML_INT spMatGetRow(spMATRIX* A, ML_INT row, double* x);
	ML_INT spMatTrans(spMATRIX* A, MATRIX H, MATRIX V, ML_INT nVecs);
	ML_INT spMatPopulateFromDense(spMATRIX* M, MATRIX m, int nr, int nc, double tol);
	ML_INT spMatPopulateFromDenseT(spMATRIX* M, MATRIX m, int nr, int nc, double tol);
	MATRIX matAlloc(ML_INT rows, ML_INT cols);
	MATRIX matRealloc(MATRIX m, ML_INT rows, ML_INT cols);
	void matZero(MATRIX m, ML_INT rows, ML_INT cols);
	void vecZero(VECTOR v, ML_INT n);
	VECTOR vecAlloc(ML_INT size);
	void matFree(MATRIX m);
	void vecFree(VECTOR v);
	void matCopy(MATRIX m1, MATRIX m2, ML_INT rows, ML_INT cols);
	void matAdd(MATRIX m1, MATRIX m2, ML_INT rows, ML_INT cols);
	void matSub(MATRIX m1, MATRIX m2, ML_INT rows, ML_INT cols);
	ML_INT matSqMult(MATRIX m1, MATRIX m2, ML_INT size);
	MATRIX matMultNew(MATRIX m1, MATRIX m2, ML_INT r1, ML_INT inner, ML_INT c2);
	ML_INT  matMult(MATRIX m1, MATRIX m2, ML_INT r1, ML_INT inner, ML_INT c2);
	ML_INT matMultCopy(MATRIX m1, MATRIX m2, MATRIX res, ML_INT r1, ML_INT inner, ML_INT c2);
	double vecDot(VECTOR v1, VECTOR v2, ML_INT size);
	ML_INT matVec(MATRIX m, VECTOR v, ML_INT mCols, ML_INT vSize);
	VECTOR matVecNew(MATRIX m, VECTOR v, ML_INT mCols, ML_INT vSize);
	ML_INT matVecCopy(MATRIX m, VECTOR v, VECTOR res, ML_INT mCols, ML_INT vSize);
	void matAddI(MATRIX m, double d, ML_INT size);
	void matSubI(MATRIX m, double d, ML_INT size);
	void matAddVectI(MATRIX m, VECTOR v, ML_INT size);
	void matSubVectI(MATRIX m, VECTOR v, ML_INT size);
	void vecCopy(VECTOR v1, VECTOR v2, ML_INT size);
	void vecNormalise(VECTOR v, ML_INT size);
	double vecMag(VECTOR v, ML_INT size);
	void vecScale(VECTOR v, double val, ML_INT size);
	void vecSub(VECTOR v1, VECTOR v2, ML_INT size);
	void vecAdd(VECTOR v1, VECTOR v2, ML_INT size);
	void vecSubScale(VECTOR v1, VECTOR v2, double scale, ML_INT size);
	double matMax(MATRIX A, ML_INT rows, ML_INT cols);
	double vecMax(VECTOR A, ML_INT size);
	ML_INT matTranspose(MATRIX m, ML_INT size);
	double spRow2Norm(spMATRIX* A, ML_INT row);
	double spRow2NormC(spMATRIX* A, ML_INT row, double C);
	void spMatFree(spMATRIX* m);
	void spMatInit(spMATRIX* m);
	ML_INT LUFactor(MATRIX A, ML_INT size);
	ML_INT HLUFactor(MATRIX A, ML_INT size);
	ML_INT LUSweep(MATRIX M, ML_INT size, VECTOR b, VECTOR x);
	ML_INT HLUSweep(MATRIX M, ML_INT size, VECTOR b, VECTOR x);
	ML_INT** imatAlloc(ML_INT rows, ML_INT cols);
	void imatFree(ML_INT** m);
	ML_INT orderSparseMatrix(spMATRIX* A);
	void MS_getQ(MATRIX A, MATRIX Q, VECTOR rd, ML_INT n, ML_INT m);
	ML_INT MS_getQT(MATRIX A, MATRIX Q, VECTOR rd, ML_INT m, ML_INT n);
	void MS_3x3Solve(MATRIX m, VECTOR y, VECTOR x);

	/** Check to make sure indeces used are appropriate and consistent
	 *
	 * @param m
	 * @return
	 */
	ML_INT checkSparseMatrix_verbose(spMATRIX* m);

	/** Add m2*scale to m1 for elements that exist in both matrix structures.
	 *
	 * @param m1
	 * @param m2
	 * @param scale
	 * @return number of elements added.
	 */
	ML_INT spAddScaledOverlaps(spMATRIX* m1, spMATRIX* m2, double scale);


	/** Copy m2 into m1, reallocating storage in m1 to accomodate
	 *
	 * @param m1
	 * @param m2
	 * @return 1 on success, 0 on failure. Failure will be due to malloc error or invalid m2 structure.
	 */
	ML_INT spMatCopy(spMATRIX* m1, spMATRIX* m2);


	/* matrixio.c functions */
	ML_UINT writeSmallMatrix(char* filename, char* delim, MATRIX mat, ML_UINT rows, ML_UINT cols);
	ML_UINT writeSmallMatrixT(char* filename, char* delim, MATRIX mat, ML_UINT rows, ML_UINT cols);
	ML_UINT writeSmallSparse(char* filename, spMATRIX A);
	ML_UINT printSmallSparse(spMATRIX A);
	ML_UINT writeSparseStruct(char* filename, spMATRIX A);
	ML_UINT writeLargeSparse(char* filename, spMATRIX A);
	ML_UINT writeLargeSparseMatrices(char* filename, ML_INT num, ...);
	ML_UINT writeLargeSparseCXMatrices(char* filename, ML_INT num, ...);
	ML_UINT printSmallMatrixT(MATRIX mat, ML_UINT rows, ML_UINT cols);
	ML_UINT printSmallMatrix(MATRIX mat, ML_UINT rows, ML_UINT cols);
	ML_UINT writeSmallVector(char* filename, VECTOR v, ML_UINT size);
	ML_UINT writeSmallCXVector(char* filename, VECTOR v, ML_UINT size);
	ML_UINT printMatStruct(MATRIX m, ML_INT rows, ML_INT cols);
	ML_UINT writeMatStruct(char* filename, MATRIX m, ML_INT rows, ML_INT cols);
	ML_UINT writeLargeSparseMatrices_DenseMAT(char* filename, char* prefix, ML_INT num, ...);
	ML_UINT writeLargeSparseMatrix_MAT(char* filename, char* name, spMATRIX* mat);
	ML_UINT writeSmallDenseLapackMatrix(const char* filename, spMATRIX A);


#endif

#ifdef __cplusplus
}
#endif
