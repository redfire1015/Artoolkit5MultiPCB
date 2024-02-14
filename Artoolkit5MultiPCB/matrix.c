/*		Virtual Prototyping for Power Electronics
 *
 * 		File:				matrix.c
 * 		Author(s): 			P L Evans
 * 		Creation Date:		2009
 *
 * 		Description:
 *		Basic library of matrix handling routines
 *
 */

#include "Headers/matrix.h"
#include <math.h>



void scaleSparseEquations(spMATRIX* M, spMATRIX* A, spMATRIX* B, spMATRIX* K, spMATRIX* C, spMATRIX* O, double s) {
	ML_INT i, j, split;
	double scale;

	if (!A) return;

	split = -1;
	for (i = 0; i < A->nr; i++) {
		scale = 0.;
		split = 0;
		for (j = A->ia[i]; j < A->ia[i + 1]; j++) {
			scale += fabs(A->aa[j]);
			split++;
		}
		scale /= split;

		for (j = A->ia[i]; j < A->ia[i + 1]; j++) A->aa[j] /= scale;
		if (M) 	for (j = M->ia[i]; j < M->ia[i + 1]; j++) M->aa[j] /= scale;
		if (B) 	for (j = B->ia[i]; j < B->ia[i + 1]; j++) B->aa[j] /= scale;
	}


	return;
}


ML_INT spGenerateIdentity(spMATRIX* mat, ML_INT size) {
	ML_INT i, j;

	spMatInit(mat);

	/* Create output matrix (node temperatures only) not including temperatures defined by boundary */
	if (!((mat->aa) = malloc((size) * sizeof(double)))
		|| !((mat->ja) = malloc((size) * sizeof(ML_INT)))
		|| !((mat->ia) = malloc((size + 1) * sizeof(ML_INT)))) {
		printf("Mem alloc, spGenerateIdentity (%d)\n", size);
		return ML_ERR_MALLOC;
	}

	mat->ia[0] = 0;
	for (i = 0; i < size; i++) {
		mat->ia[i + 1] = i + 1;
		mat->aa[i] = 1.;
		mat->ja[i] = i;
	}
	mat->nr = size;
	mat->nc = size;

	return ML_OK;
}

void spMatVec(spMATRIX* A, VECTOR v, VECTOR o) {
	ML_INT i, j;

#ifdef MATRIX_WARN_SPMATVEC_IO
	if (o == v) {
		printf("Warning: Input and output same for spMatVec\n");
	}
#endif

	for (i = 0; i < A->nr; i++)
	{
		o[i] = 0.;

		for (j = A->ia[i]; j < A->ia[i + 1]; j++)
			o[i] += A->aa[j] * v[A->ja[j]];
	}
}

void spMatVecOverwrite(spMATRIX* A, VECTOR v) {
	ML_INT i, j;
	VECTOR temp;

	if (!(temp = malloc(A->nr * sizeof(double)))) {
		printf("Malloc error in spMatVecOverwrite\n");
		return;
	}

	for (i = 0; i < A->nr; i++)
	{
		temp[i] = 0.;

		for (j = A->ia[i]; j < A->ia[i + 1]; j++)
			temp[i] += A->aa[j] * v[A->ja[j]];
	}

	vecCopy(v, temp, A->nr);
	free(temp);
	return;
}

void spMatVecT(spMATRIX* A, VECTOR v, VECTOR o) {
	ML_INT i, j;

#ifdef MATRIX_WARN_SPMATVEC_IO
	if (o == v) {
		printf("Warning: Input and output same for spMatVec\n");
	}
#endif

	for (i = 0; i < A->nc; i++) {
		o[i] = 0.;
	}

	for (i = 0; i < A->nr; i++) {
		for (j = A->ia[i]; j < A->ia[i + 1]; j++)
			o[A->ja[j]] += A->aa[j] * v[i];
	}
}

void spMatVecSum(spMATRIX* A, VECTOR v, VECTOR o) {
	ML_INT i, j;

#ifdef MATRIX_WARN_SPMATVEC_IO
	if (o == v) {
		printf("Warning: Input and output same for spMatVec\n");
	}
#endif

	for (i = 0; i < A->nr; i++) {
		for (j = A->ia[i]; j < A->ia[i + 1]; j++)
			o[i] += A->aa[j] * v[A->ja[j]];
	}
}

void spMatVecTSum(spMATRIX* A, VECTOR v, VECTOR o) {
	ML_INT i, j;

#ifdef MATRIX_WARN_SPMATVEC_IO
	if (o == v) {
		printf("Warning: Input and output same for spMatVec\n");
	}
#endif

	for (i = 0; i < A->nr; i++) {
		for (j = A->ia[i]; j < A->ia[i + 1]; j++)
			o[A->ja[j]] += A->aa[j] * v[i];
	}
}

void spMatVecSub(spMATRIX* A, VECTOR v, VECTOR o) {
	ML_INT i, j;

#ifdef MATRIX_WARN_SPMATVEC_IO
	if (o == v) {
		printf("Warning: Input and output same for spMatVec\n");
	}
#endif

	for (i = 0; i < A->nr; i++) {
		for (j = A->ia[i]; j < A->ia[i + 1]; j++)
			o[i] -= A->aa[j] * v[A->ja[j]];
	}
}

void spMatVecTSub(spMATRIX* A, VECTOR v, VECTOR o) {
	ML_INT i, j;

#ifdef MATRIX_WARN_SPMATVEC_IO
	if (o == v) {
		printf("Warning: Input and output same for spMatVec\n");
	}
#endif

	for (i = 0; i < A->nr; i++) {
		for (j = A->ia[i]; j < A->ia[i + 1]; j++)
			o[A->ja[j]] -= A->aa[j] * v[i];
	}
}


void spMatVecScale(spMATRIX* A, VECTOR v, VECTOR o, double s) {
	ML_INT i, j;

#ifdef MATRIX_WARN_SPMATVEC_IO
	if (o == v) {
		printf("Warning: Input and output same for spMatVec\n");
	}
#endif
	for (i = 0; i < A->nr; i++)
	{
		o[i] = 0.;

		for (j = A->ia[i]; j < A->ia[i + 1]; j++)
			o[i] += s * A->aa[j] * v[A->ja[j]];
	}
}

void spMatVecScaleSum(spMATRIX* A, VECTOR v, VECTOR o, double s) {
	ML_INT i, j;

#ifdef MATRIX_WARN_SPMATVEC_IO
	if (o == v) {
		printf("Warning: Input and output same for spMatVec\n");
	}
#endif
	for (i = 0; i < A->nr; i++)
	{
		for (j = A->ia[i]; j < A->ia[i + 1]; j++)
			o[i] += s * A->aa[j] * v[A->ja[j]];
	}
}


ML_INT spMatTrans(spMATRIX* A, MATRIX H, MATRIX V, ML_INT nVecs) {
	ML_INT i, j, k;
	VECTOR vtemp;

	if (!(vtemp = malloc(A->nr * sizeof(double))))
		return ML_ERR_MALLOC;

	/* calculate each column of V^T*A*V */
	for (i = 0; i < nVecs; i++)
	{
		/* get column i of A*V */
		spMatVec(A, V[i], vtemp);

		/* get column i of V^T*A*V */
		for (j = 0; j < nVecs; j++) {
			H[j][i] = 0.;
			for (k = 0; k < A->nr; k++)
				H[j][i] += V[j][k] * vtemp[k];
		}
	}

	free(vtemp);
	return ML_OK;
}

ML_INT spMatGetCol(spMATRIX* A, ML_INT col, double* x) {
	int i, j;

	/* Set column to zer */
	for (i = 0; i < A->nr; i++) {
		x[i] = 0.;
	}

	if (col >= A->nc) {
		printf("Cannot return matrix col, out of range (%d > %d)\n", col, A->nc);
		return ML_OK;
	}

	/* For each row in column, check for non-zero entry */
	for (i = 0; i < A->nr; i++) {
		for (j = A->ia[i]; j < A->ia[i + 1]; j++) {
			if (A->ja[j] == col) {
				x[i] = A->aa[j];
				break;
			}
		}
	}
	return ML_OK;
}

ML_INT spMatGetRow(spMATRIX* A, ML_INT row, double* x) {
	int i, j;

	/* Set column to zer */
	for (i = 0; i < A->nc; i++) {
		x[i] = 0.;
	}

	if (row >= A->nr) return ML_OK;

	/* For each row in column, check for non-zero entry */
	for (j = A->ia[row]; j < A->ia[row + 1]; j++) {
		x[A->ja[j]] = A->aa[j];
	}

	return ML_OK;
}


ML_INT spMatPopulateFromDense(spMATRIX* M, MATRIX m, int nr, int nc, double tol) {
	ML_INT i, j, k, ind = 0, alloc;

	if (nr > 5)   i = 5 * nc;
	else           i = nr * nc;

	if (!(M->aa = realloc(M->aa, i * sizeof(double)))
		|| !(M->ja = realloc(M->ja, i * sizeof(ML_INT)))
		|| !(M->ia = realloc(M->ia, (nr + 1) * sizeof(ML_INT)))) {
		printf("M malloc error when creating sparse matrix (%d, %d)\n", nr, i);
		return ML_ERR_MALLOC;
	}

	alloc = i;

	for (i = 0; i < nr - 5; ) {
		/* Block of 5 rows */
		for (k = 0; k < 5; k++) {
			M->ia[i] = ind;
			for (j = 0; j < nc; j++) {
				if (fabs(m[i][j]) > tol) {
					M->aa[ind] = m[i][j];
					M->ja[ind] = j;
					ind++;
				}
			}
			i++;
		}

		/* Check allocation */
		if (alloc - ind < 5 * nc) {
			alloc += 5 * nc;
			if (!(M->aa = realloc(M->aa, alloc * sizeof(double)))
				|| !(M->ja = realloc(M->ja, alloc * sizeof(ML_INT)))) {
				printf("M malloc error when creating sparse matrix (%d, %d)\n", nr, i);
				return ML_ERR_MALLOC;
			}
		}
	}

	/* Finish off */
	for (; i < nr; i++) {
		M->ia[i] = ind;
		for (j = 0; j < nc; j++) {
			if (fabs(m[i][j]) > tol) {
				M->aa[ind] = m[i][j];
				M->ja[ind] = j;
				ind++;
			}
		}
	}

	M->ia[i] = ind;
	M->nr = nr;
	M->nc = nc;

	return ML_OK;
}


ML_INT spMatPopulateFromDenseT(spMATRIX* M, MATRIX m, int nr, int nc, double tol) {
	ML_INT i, j, k, ind = 0, alloc;

	if (nr > 5)   i = 5 * nc;
	else           i = nr * nc;

	if (!(M->aa = realloc(M->aa, i * sizeof(double)))
		|| !(M->ja = realloc(M->ja, i * sizeof(ML_INT)))
		|| !(M->ia = realloc(M->ia, (nr + 1) * sizeof(ML_INT)))) {
		printf("M malloc error when creating sparse matrix (%d, %d)\n", nr, i);
		return ML_ERR_MALLOC;
	}

	alloc = i;

	for (i = 0; i < nr - 5; ) {
		/* Block of 5 rows */
		for (k = 0; k < 5; k++) {
			M->ia[i] = ind;
			for (j = 0; j < nc; j++) {
				if (fabs(m[j][i]) > tol) {
					M->aa[ind] = m[j][i];
					M->ja[ind] = j;
					ind++;
				}
			}
			i++;
		}

		/* Check allocation */
		if (alloc - ind < 5 * nc) {
			alloc += 5 * nc;
			if (!(M->aa = realloc(M->aa, alloc * sizeof(double)))
				|| !(M->ja = realloc(M->ja, alloc * sizeof(ML_INT)))) {
				printf("M malloc error when creating sparse matrix (%d, %d)\n", nr, i);
				return ML_ERR_MALLOC;
			}
		}
	}

	/* Finish off */
	for (; i < nr; i++) {
		M->ia[i] = ind;
		for (j = 0; j < nc; j++) {
			if (fabs(m[j][i]) > tol) {
				M->aa[ind] = m[j][i];
				M->ja[ind] = j;
				ind++;
			}
		}
	}

	M->ia[i] = ind;
	M->nr = nr;
	M->nc = nc;

	return ML_OK;
}




MATRIX matAlloc(ML_INT rows, ML_INT cols)
{
	ML_INT i;
	MATRIX m;

	if (!(m = malloc(sizeof(double*) * rows)) || !(*m = calloc(cols * rows, sizeof(double))))
	{
		return NULL;
	}

	for (i = 1; i < rows; i++)
		m[i] = (double*)&(m[0][i * cols]);

	return m;
}


MATRIX matRealloc(MATRIX m, ML_INT rows, ML_INT cols)
{
	ML_INT i;

	/* allocate memory in single block - makes deallocation much faster */
	if (!(m = realloc(m, sizeof(double*) * rows)) || !(*m = realloc(*m, cols * rows * sizeof(double))))
	{
		return NULL;
	}

	for (i = 1; i < rows; i++)
		m[i] = (double*)&(m[0][i * cols]);

	return m;
}

void matZero(MATRIX m, ML_INT rows, ML_INT cols) {
	int i, j;

	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			m[i][j] = 0.;
		}
	}
}

void vecZero(VECTOR v, ML_INT n) {
	int i;

	for (i = 0; i < n; i++) {
		v[i] = 0.;
	}
}



VECTOR vecAlloc(ML_INT size)
{
	VECTOR v;

	if (!(v = calloc(size, sizeof(double))))
	{
		return NULL;
	}

	return v;
}


void matFree(MATRIX m)
{
	if (!m) return;

	free(m[0]);
	free(m);
}


void vecFree(VECTOR v)
{
	free(v);
}


inline void matCopy(MATRIX m1, MATRIX m2, ML_INT rows, ML_INT cols)
{
	ML_INT i, j;

	for (i = 0; i < rows; i++)
		for (j = 0; j < cols; j++)
			m1[i][j] = m2[i][j];

}

inline void matAdd(MATRIX m1, MATRIX m2, ML_INT rows, ML_INT cols)
{
	ML_INT i, j;

	for (i = 0; i < rows; i++)
		for (j = 0; j < cols; j++)
			m1[i][j] += m2[i][j];
}


inline void matSub(MATRIX m1, MATRIX m2, ML_INT rows, ML_INT cols)
{
	ML_INT i, j;

	for (i = 0; i < rows; i++)
		for (j = 0; j < cols; j++)
			m1[i][j] -= m2[i][j];
}


ML_INT matSqMult(MATRIX m1, MATRIX m2, ML_INT size)
{
	ML_INT i, j, k;
	double* d;

	if (!(d = calloc(size, sizeof(double))))
	{
		return ML_ERR_MALLOC;
	}

	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			d[j] = 0.;
			for (k = 0; k < size; k++)
				d[j] += m1[i][k] * m2[k][j];
		}
		for (j = 0; j < size; j++)
			m1[i][j] = d[j];
	}

	free(d);

	return ML_OK;
}


MATRIX matMultNew(MATRIX m1, MATRIX m2, ML_INT r1, ML_INT inner, ML_INT c2)
{
	MATRIX res;
	ML_INT i, j, k;

	if (!(res = (MATRIX)matAlloc(r1, c2)))
	{
		return NULL;
	}

	for (i = 0; i < r1; i++)
		for (j = 0; j < c2; j++)
		{
			res[i][j] = 0.;

			for (k = 0; k < inner; k++)
				res[i][j] += m1[i][k] * m2[k][j];
		}

	return res;
}


ML_INT  matMult(MATRIX m1, MATRIX m2, ML_INT r1, ML_INT inner, ML_INT c2)
{
	ML_INT i, j, k;

	double* d;

	if (!(d = calloc(c2, sizeof(double))))
	{
		return ML_ERR_MALLOC;
	}

	for (i = 0; i < r1; i++)
	{
		for (j = 0; j < c2; j++)
		{
			d[j] = 0.;

			for (k = 0; k < inner; k++)
				d[j] += m1[i][k] * m2[k][j];
		}

		for (j = 0; j < c2; j++)
			m1[i][j] = d[j];
	}

	free(d);

	return ML_OK;
}

ML_INT matMultCopy(MATRIX m1, MATRIX m2, MATRIX res, ML_INT r1, ML_INT inner, ML_INT c2)
{
	ML_INT i, j, k;

	for (i = 0; i < r1; i++)
	{
		for (j = 0; j < c2; j++)
		{
			res[i][j] = 0.;

			for (k = 0; k < inner; k++)
				res[i][j] += m1[i][k] * m2[k][j];
		}
	}

	return ML_OK;
}

double vecDot(VECTOR v1, VECTOR v2, ML_INT size)
{
	ML_INT i;
	double res = 0.;

	for (i = 0; i < size; i++)
	{
		res += v1[i] * v2[i];
	}

	return res;
}

ML_INT matVec(MATRIX m, VECTOR v, ML_INT mCols, ML_INT vSize)
{
	ML_INT i, j;
	VECTOR temp;

	if (!(temp = vecAlloc(vSize)))
	{
		return ML_ERR_MALLOC;
	}

	for (i = 0; i < vSize; i++)
	{
		temp[i] = 0.;

		for (j = 0; j < mCols; j++)
		{
			temp[i] += v[j] * m[i][j];
		}
	}

	vecCopy(v, temp, vSize);

	vecFree(temp);

	return ML_OK;
}

VECTOR matVecNew(MATRIX m, VECTOR v, ML_INT mCols, ML_INT vSize)
{
	ML_INT i, j;
	VECTOR ret;

	if (!(ret = vecAlloc(vSize)))
	{
		return NULL;
	}

	for (i = 0; i < vSize; i++)
	{
		ret[i] = 0.;

		for (j = 0; j < mCols; j++)
		{
			ret[i] += v[j] * m[i][j];
		}
	}

	return ret;
}

ML_INT matVecCopy(MATRIX m, VECTOR v, VECTOR res, ML_INT mCols, ML_INT vSize) {
	ML_INT i, j;

	for (i = 0; i < vSize; i++) {
		res[i] = 0.;

		for (j = 0; j < mCols; j++) {
			res[i] += v[j] * m[i][j];
		}
	}

	return ML_OK;
}

inline void matAddI(MATRIX m, double d, ML_INT size) {
	ML_INT i;

	for (i = 0; i < size; i++) {
		m[i][i] += d;
	}
}

inline void matSubI(MATRIX m, double d, ML_INT size) {
	ML_INT i;

	for (i = 0; i < size; i++) {
		m[i][i] -= d;
	}
}

inline void matAddVectI(MATRIX m, VECTOR v, ML_INT size) {
	ML_INT i;

	for (i = 0; i < size; i++) {
		m[i][i] += v[i];
	}
}

inline void matSubVectI(MATRIX m, VECTOR v, ML_INT size) {
	ML_INT i;

	for (i = 0; i < size; i++) {
		m[i][i] += v[i];
	}
}

inline void vecCopy(VECTOR v1, VECTOR v2, ML_INT size) {
	ML_INT i;

	for (i = 0; i < size; i++) {
		v1[i] = v2[i];
	}
}


void vecNormalise(VECTOR v, ML_INT size) {
	ML_INT i;
	double mag = 0.;

	for (i = 0; i < size; i++) {
		mag += v[i] * v[i];
	}

	mag = sqrt(mag);

	for (i = 0; i < size; i++) {
		v[i] /= (v[i] > 0.) ? mag : -mag;
	}
}

double vecMag(VECTOR v, ML_INT size) {
	ML_INT i;
	double mag = 0.;

	for (i = 0; i < size; i++) {
		mag += v[i] * v[i];
	}

	return sqrt(mag);
}

void vecScale(VECTOR v, double val, ML_INT size) {
	ML_INT i;

	for (i = 0; i < size; i++) {
		v[i] *= val;
	}
}

void vecSub(VECTOR v1, VECTOR v2, ML_INT size) {
	ML_INT i;

	for (i = 0; i < size; i++)
		v1[i] -= v2[i];
}

void vecAdd(VECTOR v1, VECTOR v2, ML_INT size) {
	ML_INT i;

	for (i = 0; i < size; i++)
		v1[i] += v2[i];
}

void vecSubScale(VECTOR v1, VECTOR v2, double scale, ML_INT size) {
	ML_INT i;

	for (i = 0; i < size; i++) {
		v1[i] -= v2[i] * scale;
	}
}

double matMax(MATRIX A, ML_INT rows, ML_INT cols) {
	ML_INT i, j;
	double ret = 0.;

	for (i = 0; i < rows; i++)
		for (j = 0; j < cols; j++)
			if (fabs(A[i][j]) > ret)
				ret = fabs(A[i][j]);

	return ret;
}

double vecMax(VECTOR A, ML_INT size) {
	ML_INT i;
	double ret = 0.;

	for (i = 0; i < size; i++)
		if (fabs(A[i]) > ret)
			ret = fabs(A[i]);

	return ret;
}

ML_INT matTranspose(MATRIX m, ML_INT size) {
	ML_INT i, j;
	MATRIX mold;

	if (!(mold = matAlloc(size, size)))
		return ML_ERR_MALLOC;;

	matCopy(mold, m, size, size);

	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			m[i][j] = mold[j][i];

	free(mold);
	return ML_OK;
}

double spRow2Norm(spMATRIX* A, ML_INT row) {
	ML_INT j1, j2;
	double norm = 0.;

	j2 = A->ia[row + 1];

	for (j1 = A->ia[row]; j1 < j2; j1++)
		norm += A->aa[j1] * A->aa[j1];

	return (norm == 0.) ? 0. : sqrt(norm);
}

double spRow2NormC(spMATRIX* A, ML_INT row, double C) {
	ML_INT j1, j2;
	double norm = 0.;

	j2 = A->ia[row + 1];

	for (j1 = A->ia[row]; j1 < j2; j1++)
		norm += A->aa[j1] * A->aa[j1] * C * C;

	return sqrt(norm);
}

void spMatFree(spMATRIX* m) {

	if (!m) return;

	if (m->aa) free(m->aa);
	//if( m->ai ) free( m->ai );
	if (m->ja) free(m->ja);
	if (m->ia) free(m->ia);

	spMatInit(m);
}

void spMatInit(spMATRIX* m) {
	m->aa = NULL;
	//m->ai = NULL;
	m->ja = NULL;
	m->ia = NULL;
	m->nr = 0;
	m->nc = 0;
}

/* very basic LU factorisation, non-sparse & no pivoting */
ML_INT LUFactor(MATRIX A, ML_INT size) {
	ML_INT i, j, k;

	for (i = 0; i < size; i++)
		for (k = 0; k < i; k++) {
			A[i][k] /= A[k][k];
			for (j = k + 1; j < size; j++)
				A[i][j] -= A[i][k] * A[k][j];
		}
	return ML_OK;
}

ML_INT HLUFactor(MATRIX A, ML_INT size) {
	ML_INT i, j, k;

	for (i = 0; i < size; i++)
		for (k = i - 1 >= 0 ? i - 1 : 0; k < i; k++) {
			A[i][k] /= A[k][k];
			for (j = k + 1; j < size; j++)
				A[i][j] -= A[i][k] * A[k][j];
		}
	return ML_OK;
}



ML_INT LUSweep(MATRIX M, ML_INT size, VECTOR b, VECTOR x) {
	ML_INT i, j;
	double* temp;

	if (!(temp = malloc(size * sizeof(double))))
		return ML_ERR_MALLOC;

	for (i = 0; i < size; i++) {
		temp[i] = b[i];
		for (j = 0; j < i; j++)
			temp[i] -= M[i][j] * temp[j];
	}

	for (i = size - 1; i >= 0; i--) {
		x[i] = temp[i];
		for (j = size - 1; j > i; j--)
			x[i] -= M[i][j] * x[j];
		x[i] /= M[i][i];
	}

	free(temp);

	return ML_OK;
}

ML_INT HLUSweep(MATRIX M, ML_INT size, VECTOR b, VECTOR x) {
	ML_INT i, j;
	double* temp;

	if (!(temp = malloc(size * sizeof(double))))
		return ML_ERR_MALLOC;

	for (i = 0; i < size; i++) {
		temp[i] = b[i];
		for (j = 0; j < i; j++)
			temp[i] -= M[i][j] * temp[j];
	}

	for (i = size - 1; i >= 0; i--) {
		x[i] = temp[i];
		for (j = size - 1; j > i; j--)
			x[i] -= M[i][j] * x[j];
		x[i] /= M[i][i];
	}

	free(temp);

	return ML_OK;
}

ML_INT** imatAlloc(ML_INT rows, ML_INT cols)
{
	ML_INT i;
	ML_INT** m;


	if (!(m = malloc(sizeof(ML_INT*) * rows)) || !(*m = calloc(cols * rows, sizeof(ML_INT)))) {
		return NULL;
	}

	for (i = 1; i < rows; i++)
		m[i] = (ML_INT*)&(m[0][i * cols]);

	return m;
}


void imatFree(ML_INT** m) {
	free(m[0]);
	free(m);
}


ML_INT orderSparseMatrix(spMATRIX* A) {
	ML_INT i, j, j1, k, * uptr;
	double* temp, b;

	if (!((temp) = malloc(A->ia[A->nr] * sizeof(double)))
		|| !((uptr) = malloc(A->ia[A->nr] * sizeof(ML_INT))))
		return ML_ERR_MALLOC;
	j = 0;

	for (i = 0; i < A->nr; i++) {
		A->ia[i] = j;
		for (j1 = 0; j1 < A->nc; j1++) {
			b = 0.;
			for (k = A->ia[i]; k < A->ia[i + 1]; k++) {
				if (A->ja[k] == j1) {
					b += A->aa[k];
					break;
				}
			}

			if (b != 0.) {
				temp[j] = b;
				uptr[j] = j1;
				j++;
			}
		}
	}

	A->ia[i] = j;
	free(A->aa);
	A->aa = temp;
	free(A->ja);
	A->ja = uptr;

	return ML_OK;
}


/* return Q of QR Factorisation for rectangular matrix */
void MS_getQ(MATRIX A, MATRIX Q, VECTOR rd, ML_INT n, ML_INT m) {
	ML_INT i, j, k;
	double s, fak;
	//printf( "GETQ Start\n" );
	for (j = 0; j < n; j++) {
		s = 0.;
		for (i = 0; i < m; i++) Q[j][i] = 0.;
		Q[j][j] = 1.;
		for (i = j; i < n; i++) s += A[i][j] * A[i][j];
		s = sqrt(s);
		rd[j] = (A[j][j] > 0.) ? -s : s;
		fak = sqrt(s * (s + fabs(A[j][j])));
		A[j][j] -= rd[j];

		for (k = j; k < n; k++)    A[k][j] /= fak;
		for (i = j + 1; i < m; i++) {
			s = 0.;
			for (k = j; k < n; k++) s += A[k][j] * A[k][i];
			for (k = j; k < n; k++) A[k][i] -= A[k][j] * s;
		}
	}

	for (i = 0; i < m; i++) {
		for (j = m - 1; j >= 0; j--) {
			s = 0.;
			for (k = j; k < n; k++) s += A[k][j] * Q[k][i];
			for (k = j; k < n; k++) Q[k][i] -= A[k][j] * s;
		}
	}
	//printf( "GETQ End\n" );
}

/* return Q factor from QR Factorisation of rectangular matrix A
	Both Q and A are stored in transpose form */
ML_INT MS_getQT(MATRIX A, MATRIX Q, VECTOR rd, ML_INT m, ML_INT n) {
	ML_INT i, j, k;
	double s, fak;

	for (j = 0; j < n; j++) {
		s = 0.;
		for (i = 0; i < m; i++) Q[j][i] = 0.;
		Q[j][j] = 1.;
		for (i = j; i < m; i++) s += A[j][i] * A[j][i];
		s = sqrt(s);
		rd[j] = (A[j][j] > 0.) ? -s : s;
		fak = sqrt(s * (s + fabs(A[j][j])));
		A[j][j] -= rd[j];

		for (k = j; k < m; k++)    A[j][k] /= fak;
		for (i = j + 1; i < n; i++) {
			s = 0.;
			for (k = j; k < m; k++) s += A[j][k] * A[i][k];
			for (k = j; k < m; k++) A[i][k] -= A[j][k] * s;
		}
	}

	for (i = 0; i < n; i++) {
		for (j = n - 1; j >= 0; j--) {
			s = 0.;
			for (k = j; k < m; k++) s += A[j][k] * Q[i][k];
			for (k = j; k < m; k++) Q[i][k] -= A[j][k] * s;
		}
	}
	return 1;
}

void MS_3x3Solve(MATRIX m, VECTOR y, VECTOR x) {
	double d, dx, dy, dz;


	d = (m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
		m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
		m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));

	dx = y[0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
		m[0][1] * (y[1] * m[2][2] - m[1][2] * y[2]) +
		m[0][2] * (y[1] * m[2][1] - m[1][1] * y[2]);

	dy = m[0][0] * (y[1] * m[2][2] - m[1][2] * y[2]) -
		y[0] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
		m[0][2] * (m[1][0] * y[2] - y[1] * m[2][0]);

	dz = m[0][0] * (m[1][1] * y[2] - y[1] * m[2][1]) -
		m[0][1] * (m[1][0] * y[2] - y[1] * m[2][0]) +
		y[0] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);

	//peprintf( PEPSTR_INFO, NULL, "d = %g, dx = %g, dy = %g, dz = %g\n", d, dx, dy, dz );
	d = 1. / d;
	x[0] = dx * d;
	x[1] = dy * d;
	x[2] = dz * d;
}

#undef small

ML_INT checkSparseMatrix_verbose(spMATRIX* m) {
	ML_INT i, j, err, rowmax, colmax, rowmin, colmin;
	double min, max, small;

	printf("Checking [%d x %d] sparse matrix...\n", m->nr, m->nc);

	if (m->nr < 0) {
		printf(" - Negative row count\n");
		return 0;
	}

	if (m->nc < 0) {
		printf(" - Negative column count\n");
		return 0;
	}

	printf(" - Checking allocation...");
	if (!m->aa) {
		printf("\n    - aa vector is NULL");
	}
	if (!m->ja) {
		printf("\n    - ja vector is NULL");
	}
	if (!m->aa) {
		printf("\n    - ia vector is NULL");
	}

	if (!m->aa || !m->ja || !m->ia) {
		printf("\nCannot continue with check\n");
		return 0;
	}

	printf("ok\n");

	printf(" - Checking row index sequence...");
	err = 0;
	for (i = 1; i <= m->nr; i++) {
		if (m->ia[i] < m->ia[i - 1]) {
			printf("\n    - index for row %d (%d) < index for row %d (%d)", i, m->ia[i], i - 1, m->ia[i - 1]);
			err = 1;
		}
	}

	if (err == 1) {
		printf("\nCannot continue with check\n");
		return 0;
	}

	printf("ok\n - Checking column index limits...");
	for (i = 0; i < m->ia[m->nr]; i++) {
		if (m->ja[i] >= m->nc) {
			printf("\n    - column index for entry %d out of range (%d)", i, m->ja[i]);
			err = 1;
		}
	}

	printf("ok\n - Checking for valid numbers in matrix...");
	min = max = 0.;
	small = m->aa[0];
	rowmax = 0;
	colmax = m->ja[0];
	for (i = 0; i < m->nr; i++) {
		for (j = m->ia[i]; j < m->ia[i + 1]; j++) {
			if (isnan(m->aa[j]) || isinf(m->aa[j])) {
				printf("\n    - Invalid value at (%d,%d) = %g\n", i, m->ja[j], m->aa[j]);
				err = 1;
			}
			else {
				if (m->aa[j] > max) {
					max = m->aa[j];
					rowmax = i;
					colmax = m->ja[j];
				}
				if (m->aa[j] < min) {
					min = m->aa[j];
					rowmin = i;
					colmin = m->ja[j];
				}
				if (fabs(m->aa[j]) < small) small = fabs(m->aa[j]);
			}
		}
	}

	if (err == 1) {
		printf("\nCheck failed\n");
		return 0;
	}

	printf("ok\n");

	printf(" - Max = %g @(%d,%d), Min = %g @(%d,%d), Small = %g\n", max, rowmax, colmax, min, rowmin, colmin, small);

	return 1;
}


ML_INT spAddScaledOverlaps(spMATRIX* m1, spMATRIX* m2, double scale) {
	ML_INT i, j1, j2, count = 0;

	if (m1->nr != m2->nr || m1->nc != m2->nc) return 0;

	for (i = 0; i < m1->nr; i++) {
		j2 = m2->ia[i];
		/* for each non-zero col in m1 on this row */
		for (j1 = m1->ia[i]; j1 < m1->ia[i + 1]; j1++) {
			/* find next col in j2 that isnt before current col in j1 */
			while (j2 < m2->ia[i + 1] && m2->ja[j2] < m1->ja[j1]) j2++;

			/* check for last non-zero col in m2 on this row */
			if (j2 == m2->ia[i + 1]) break;

			/* check to see if col in m1 also exists in m2 */
			if (m1->ja[j1] == m2->ja[j2]) {
				/* add m2 val to m2 */
				m1->aa[j1] += m2->aa[j2] * scale;
				count++;
			}
		}
	}
	return count;
}

ML_INT spMatCopy(spMATRIX* m1, spMATRIX* m2) {

	if (!m2->ia || !m2->ja || !m2->aa || m2->nr <= 0 || m2->nc <= 0 || m2->ia[m2->nr] <= 0) return 0;

	if (!(m1->aa = realloc(m1->aa, m2->ia[m2->nr] * sizeof(double)))
		|| !(m1->ja = realloc(m1->ja, m2->ia[m2->nr] * sizeof(ML_INT)))
		|| !(m1->ia = realloc(m1->ia, (m2->nr + 1) * sizeof(ML_INT)))) {
		return 0;
	}

	memcpy(m1->aa, m2->aa, m2->ia[m2->nr] * sizeof(double));
	memcpy(m1->ja, m2->ja, m2->ia[m2->nr] * sizeof(ML_INT));
	memcpy(m1->ia, m2->ia, (m2->nr + 1) * sizeof(ML_INT));

	m1->nr = m2->nr;
	m1->nc = m2->nc;

	return 1;
}