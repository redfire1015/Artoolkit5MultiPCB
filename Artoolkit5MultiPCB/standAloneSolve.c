/*		Virtual Prototyping for Power Electronics
 *
 * 		@file:				standAloneSolve.c
 * 		Author(s): 			P L Evans
 * 		Creation Date:		18/03/2020
 *
 * 		Description:	    Solvers that don't depend on the solve wrapper.
 *
 */
#include <math.h>
#include "Headers/standAloneSolve.h"
#include <math.h>

 /* standard GMRES */
ML_INT gmres(spMATRIX* A, double* B, double* x0, ML_INT maxIts, double tol) {
	ML_INT i, j, k;
	double b, dtemp1, dtemp2, norm;
	MATRIX V, R;
	VECTOR gk, y, c, s;

	if (!(V = matAlloc(maxIts + 1, A->nr + 1))
		|| !(R = matAlloc(maxIts + 1, maxIts + 1))
		|| !(gk = malloc((maxIts + 1) * sizeof(double)))
		|| !(y = malloc((maxIts + 1) * sizeof(double)))
		|| !(c = calloc(maxIts, sizeof(double)))
		|| !(s = calloc(maxIts, sizeof(double)))) {
		peprintf(PEPSTR_ERROR, NULL, "GMRES: Error allocating memory.\n");
		return -1;
	}

	/* get v0  */
	vecCopy(V[0], B, A->nr);
	spMatVec(A, x0, V[0]);

	for (i = 0; i < A->nr; i++)
		V[0][i] = B[i] - V[0][i];

	b = vecMag(V[0], A->nr);                      /* Beta in GMRES paper */
	norm = fabs(b);

	/* Normalise V[0] */
	vecScale(V[0], 1. / b, A->nr);

	/* set inital value for gk */
	gk[0] = b;

	/* GMRES iterations */
	for (k = 0; k < maxIts && norm > b * tol; k++) {
		/* Get next Arnoldi vector */
		spMatVec(A, V[k], V[k + 1]);
		//matVecCopy( A, V[k], V[k+1], size, size );

		/* Make next Arnoldi vector orthogonal to all previous vectors */
		for (j = 0; j <= k; j++) {
			R[k][j] = vecDot(V[k + 1], V[j], A->nr);
			vecSubScale(V[k + 1], V[j], R[k][j], A->nr);
		}

		/* H now contains Hk - extend new column to give H~k */
		R[k][k + 1] = vecMag(V[k + 1], A->nr);

		/* Normalise V[k+1] */
		vecScale(V[k + 1], 1. / R[k][k + 1], A->nr);

		/* Rotate last column of H~k (k+1 x k) by all previous rotations */
		for (j = 0; j < k; j++) {
			dtemp1 = R[k][j];
			dtemp2 = R[k][j + 1];
			R[k][j] = c[j] * dtemp1 - s[j] * dtemp2;
			R[k][j + 1] = s[j] * dtemp1 + c[j] * dtemp2;
		}

		/* Get new rotation matrix (k+1 x k+1) */
		dtemp1 = sqrt(R[k][k] * R[k][k] + R[k][k + 1] * R[k][k + 1]);
		c[k] = R[k][k] / dtemp1;
		s[k] = -R[k][k + 1] / dtemp1;

		/* Rotate augmented matrix (k+1 x k+1) by new rotation (Fk) to get R~k (k+1 x k+1) */
		dtemp1 = R[k][k];
		dtemp2 = R[k][k + 1];
		R[k][k] = c[k] * dtemp1 - s[k] * dtemp2;
		R[k][k + 1] = s[k] * dtemp1 + c[k] * dtemp2;   /* == 0 */

		/* Multiply b*e1 by new rotation matrix (k+1 x k+1) */
		dtemp1 = gk[k];
		dtemp2 = gk[k + 1];
		gk[k] = dtemp1 * c[k];/* (- dtemp2 * s[k]; ~= 0) */
		gk[k + 1] = dtemp1 * s[k];/* (+ dtemp2 * c[k]; ~= 0) */

		/* residual norm */
		norm = fabs(gk[k + 1]);
	}

	if (k - 1 == maxIts - 1 && norm > b * tol) {
		peprintf(PEPSTR_ERROR, NULL, "GMRES: Iteration limit was reached, GMRES has not converged.\n");
		return -1;
	}

	/* back substitute in the upper diagonal R to get y */
	for (j = k - 1; j >= 0; j--) {
		dtemp1 = gk[j];
		for (i = j + 1; i < k; i++)
			dtemp1 -= R[i][j] * y[i];
		y[j] = dtemp1 / R[j][j];
	}

	/* Form approximate solution, x in x0*/
	for (i = 0; i < A->nr; i++)
		for (j = 0; j < k; j++)
			x0[i] += V[j][i] * y[j];

	matFree(V);
	matFree(R);
	free(gk);
	free(y);
	free(c);
	free(s);
	return k;
}


/* preconditioned GMRES */
ML_INT gmresp(spMATRIX* A, spMATRIX* M, ML_INT* uptr, double* B, double* x0, ML_INT maxIts, double tol) {
	ML_INT i, j, k;
	double b, dtemp1, dtemp2, norm;
	MATRIX V, R;
	VECTOR gk, y, c, s;

	if (!(V = matAlloc(maxIts + 1, A->nr + 1))
		|| !(R = matAlloc(maxIts + 1, maxIts + 1))
		|| !(gk = malloc((maxIts + 1) * sizeof(double)))
		|| !(y = malloc((maxIts + 1) * sizeof(double)))
		|| !(c = calloc(maxIts, sizeof(double)))
		|| !(s = calloc(maxIts, sizeof(double))))
	{
		peprintf(PEPSTR_ERROR, NULL, "GMRES: Error allocating memory.\n");
		return -1;
	}

	/* get v0  */
	vecCopy(V[0], B, A->nr);
	//vtemp = matVecNew( A, x0, size, size );
	spMatVec(A, x0, V[0]);
	for (i = 0; i < A->nr; i++)
		V[0][i] = B[i] - V[0][i];
	MS_LUSolve(M, uptr, V[0], V[0]);        /* M^(-1).(b - A.x0) */
	//vecSub( V[0], vtemp, size );
	b = vecMag(V[0], A->nr);                      /* Beta in GMRES paper */
	norm = fabs(b);
	/* Normalise V[0] */
	vecScale(V[0], 1. / b, A->nr);
	/* set inital value for gk */
	gk[0] = b;    //printf( "GK[0] = %f\n", gk[0] );

	/* GMRES iterations */
	for (k = 0; k < maxIts && norm > b * tol; k++) {
		/* Get next Arnoldi vector */
		spMatVec(A, V[k], V[k + 1]);
		MS_LUSolve(M, uptr, V[k + 1], V[k + 1]);  /* v[k+1] = M^(-1).(A.vk) */

		/* Make next Arnoldi vector orthogonal to all previous vectors */
		for (j = 0; j <= k; j++) {
			R[k][j] = vecDot(V[k + 1], V[j], A->nr);
			vecSubScale(V[k + 1], V[j], R[k][j], A->nr);
		}

		/* H now contains Hk - extend new column to give H~k */
		R[k][k + 1] = vecMag(V[k + 1], A->nr);
		/* Normalise V[k+1] */
		vecScale(V[k + 1], 1. / R[k][k + 1], A->nr);
		/* Rotate last column of H~k (k+1 x k) by all previous rotations */
		for (j = 0; j < k; j++) {
			dtemp1 = R[k][j];
			dtemp2 = R[k][j + 1];
			R[k][j] = c[j] * dtemp1 - s[j] * dtemp2;
			R[k][j + 1] = s[j] * dtemp1 + c[j] * dtemp2;
		}

		/* Get new rotation matrix (k+1 x k+1) */
		dtemp1 = sqrt(R[k][k] * R[k][k] + R[k][k + 1] * R[k][k + 1]);
		c[k] = R[k][k] / dtemp1;
		s[k] = -R[k][k + 1] / dtemp1;

		/* Rotate augmented matrix (k+1 x k+1) by new rotation (Fk) to get R~k (k+1 x k+1) */
		dtemp1 = R[k][k];
		dtemp2 = R[k][k + 1];
		R[k][k] = c[k] * dtemp1 - s[k] * dtemp2;
		R[k][k + 1] = s[k] * dtemp1 + c[k] * dtemp2;   /* == 0 */

		/* Multiply b*e1 by new rotation matrix (k+1 x k+1) */
		dtemp1 = gk[k];
		dtemp2 = gk[k + 1];
		gk[k] = dtemp1 * c[k];/* (- dtemp2 * s[k]; ~= 0) */
		gk[k + 1] = dtemp1 * s[k];/* (+ dtemp2 * c[k]; ~= 0) */

		/* residual norm */
		norm = fabs(gk[k + 1]);
	}

	if (k - 1 == maxIts - 1 && norm > b * tol)
	{
		peprintf(PEPSTR_ERROR, NULL, "GMRES: Iteration limit was reached, GMRES has not fully converged.\n");
		return -1;
	}

	/* back substitute in the upper diagonal R to get y */
	for (j = k - 1; j >= 0; j--) {
		dtemp1 = gk[j];
		for (i = j + 1; i < k; i++)
			dtemp1 -= R[i][j] * y[i];
		y[j] = dtemp1 / R[j][j];
	}

	/* Form approximate solution, x in x0*/
	for (i = 0; i < A->nr; i++)
		for (j = 0; j < k; j++)
			x0[i] += V[j][i] * y[j];

	matFree(V);
	matFree(R);
	free(gk);
	free(y);
	free(c);
	free(s);

	return k;
}



ML_INT MS_LUSolve(spMATRIX* M, ML_INT* uptr, double* b, double* x) {
	ML_INT i, j;

	static double* temp = NULL;
	static ML_INT alloc = 0;
	//printf( "LUS 1, mianr = %d\n", M->ia[M->nr] );
	if (alloc < M->ia[M->nr]) {
		if (!(temp = realloc(temp, M->ia[M->nr] * sizeof(double))))
		{
			printf("error\n"); getchar(); exit(1);
		}
		alloc = M->ia[M->nr];
	}

	for (i = 0; i < M->nr; i++) {
		temp[i] = b[i];

		for (j = M->ia[i]; j < uptr[i]; j++)
			temp[i] -= M->aa[j] * temp[M->ja[j]];

	}

	for (i = M->nr - 1; i >= 0; i--) {
		x[i] = temp[i];
		for (j = M->ia[i + 1] - 1; j > uptr[i]; j--)
			x[i] -= M->aa[j] * x[M->ja[j]];
		x[i] /= M->aa[uptr[i]];
	}

	return 1;
}


/* LU factorisation with controlled fill-in */
ML_INT getILUT(spMATRIX* A, spMATRIX* M, ML_INT* ILUT_uptrm, double t) {
	double rowTol, cmax = 0.;
	ML_INT i, j, k, index, wpi = 0, ti, dset;

	double mVal;
	ML_INT mInd;

	double* ILUT_w;			/* ILUT working data */
	ML_INT* ILUT_wp;		/* ILUT working data */

	ML_INT ILUT_bs = 4;
	ML_INT ILUT_all = ILUT_bs * A->nr;


	if (!(ILUT_w = malloc((A->nr + 2) * sizeof(double)))
		|| !(ILUT_wp = malloc((A->nr + 2) * sizeof(ML_INT)))) {
		peprintf(PEPSTR_ERROR, NULL, "Error allocating for ILUT temporary storage\n");
		return 0;
	}

	if (!(M->ia = realloc(M->ia, (A->nr + 2) * sizeof(ML_INT)))
		|| !(M->ja = realloc(M->ja, (ILUT_all) * sizeof(ML_INT)))
		|| !(M->aa = realloc(M->aa, (ILUT_all) * sizeof(double)))) {
		peprintf(PEPSTR_ERROR, NULL, "Error allocating for ILUT matrix\n");
		return 0;
	}

	for (i = 0; i < A->ia[A->nr]; i++) if (fabs(A->aa[i]) > cmax)
		cmax = fabs(A->aa[i]);

	cmax *= 0.1;
	index = 0;

	for (i = 0; i < A->nr; i++) {
		ILUT_uptrm[i] = -1;
		/* zero w */
		for (j = 0; j < A->nr; j++) {
			ILUT_w[j] = 0.;
		}

		M->ia[i] = index;
		rowTol = spRow2Norm(A, i) * t;
		/* copy working row */
		mVal = 0.;
		mInd = -1;
		wpi = 0;

		for (j = A->ia[i]; j < A->ia[i + 1]; j++) {
			if (A->ja[j] > i && ILUT_w[i] == 0.) {
				/* add an element on diagonal if required */
				ILUT_w[i] = cmax;
				ILUT_wp[wpi] = i;    /* keep track of non-zero elements */
				wpi++;
			}
			ILUT_w[A->ja[j]] = A->aa[j];
			if ((A->ja[j] > i && fabs(ILUT_w[A->ja[j]]) > mVal)) {
				mVal = fabs(ILUT_w[A->ja[j]]);                 // adj 1
				mInd = A->ja[j];
			}
			ILUT_wp[wpi] = A->ja[j];    /* keep track of non-zero elements */
			wpi++;
		}

		/* k loop - up to diagonal through non-zeros in w */
		for (k = 0; ILUT_wp[k] < i && k < wpi; k++) {
			ILUT_w[ILUT_wp[k]] /= M->aa[ILUT_uptrm[ILUT_wp[k]]];

			if (fabs(ILUT_w[ILUT_wp[k]] * M->aa[ILUT_uptrm[ILUT_wp[k]]]) < rowTol
				&& ILUT_wp[k] != mInd && ILUT_wp[k] != i) {
				ILUT_w[ILUT_wp[k]] = 0.;
			}
			else {
				/* j loop - from k+1 to n through non-zeros in U(k,*) */
				/* find first non-zero element after diagonal */
				for (j = M->ia[ILUT_wp[k]]; M->ja[j] <= ILUT_wp[k] && j < M->ia[ILUT_wp[k] + 1]; j++);

				/* What happens if reached end of row here - i.e. j == M->ia[ILUT_wp[k]+1]?
				 * - This is happening sometimes. */
				for (; j < M->ia[ILUT_wp[k] + 1]; j++) {
					if (ILUT_w[M->ja[j]] == 0.) {
						/* keep an ordered list of pointers */
						ti = wpi;
						wpi++;
						while (ILUT_wp[ti - 1] > M->ja[j]) {
							ILUT_wp[ti] = ILUT_wp[ti - 1];
							ti--;
						}
						ILUT_wp[ti] = M->ja[j];
					}
					ILUT_w[M->ja[j]] -= ILUT_w[ILUT_wp[k]] * M->aa[j];
				}
			}
		}

		dset = 0;
		for (k = 0; k < wpi; k++) {
			if (!(fabs(ILUT_w[ILUT_wp[k]]) < rowTol) || (ILUT_wp[k] < i && !(fabs(ILUT_w[ILUT_wp[k]]) < t)) || ILUT_wp[k] == i || ILUT_wp[k] == mInd) {
				if (ILUT_wp[k] > i && !dset) {
					// force a small non-zero value for diagonal element
					peprintf(PEPSTR_WARN, NULL, "WARNING: Forcing non-zero diagonal on row %d of preconditioner to avoid singularity\n", i);
					M->aa[index] = 1e-9;
					M->ja[index++] = i;
					ILUT_uptrm[i] = index;
					dset = 1;
				}

				M->aa[index] = ILUT_w[ILUT_wp[k]];
				M->ja[index] = ILUT_wp[k];
				if (ILUT_wp[k] == i) {
					dset = 1;
					ILUT_uptrm[i] = index;

					/* catch any diagonal elements that are close to zero and adjust to avoid problems */
					if (fabs(M->aa[index]) < 1e-9) {
						peprintf(PEPSTR_WARN, NULL, "WARNING: Preconditioner modified on row %d to avoid potential singularity (val = %E), convergence may be affected\n", i, M->aa[index]);
						M->aa[index] = M->aa[index] < 0 ? -1e-9 : 1e-9;
					}
				}
				index++;
			}
			ILUT_w[ILUT_wp[k]] = 0.;
		}

		if (ILUT_uptrm[i] == -1) {
			peprintf(PEPSTR_ERROR, NULL, "ILUT preconditioning failed on row %d\n", i);
			return 0;
		}

		/* see if need to allocate more memory */
		if (index > ILUT_all) {											// this should never happen now row length is used to determine allocation
			peprintf(PEPSTR_ERROR, NULL, "ILUT error, increse blocksize (index = %d, allocated = %d)\n", index, ILUT_all);
			return 0;
		}

		if (ILUT_all - index < ILUT_bs * A->nr) {
			ILUT_all += ILUT_bs * A->nr;    /*  wd->ILUT_bs is big to minimise realloc calls */
			if (!(M->aa = realloc(M->aa, ILUT_all * sizeof(double)))
				|| !(M->ja = realloc(M->ja, ILUT_all * sizeof(ML_INT)))) {
				peprintf(PEPSTR_ERROR, NULL, "ILUT realloc error\n");
				return 0;
			}
		}
	}

	M->ia[i] = index;
	M->nr = A->nr;
	M->nc = A->nc;

	free(ILUT_w);
	free(ILUT_wp);

	return 1;
}

