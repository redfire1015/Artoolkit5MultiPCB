/*		Virtual Prototyping for Power Electronics
 *
 * 		@file:				gmres.c
 * 		Author(s): 			P L Evans
 * 		Creation Date:		18/03/2020
 *
 * 		Description:	    Solver implementation.
 *
 */

#include <math.h>

#include "Headers/msgmres.h"

#ifdef MS_HAVE_KLU
#include <klu.h>
#endif

#include "Headers/matrix.h"
#include "Headers/msolve.h"
#include "Headers/solverdata.h"

MS_Solver MS_initSolver_gmres(MS_Solver wd, spMATRIX* LHS, void** params) {

	if (params) {
		wd->GMRES_iters = (int)*((ML_INT*)params[0]);
		wd->GMRES_tol = (double)*((double*)params[1]);
	}
	else {
		wd->GMRES_iters = LHS->nr;
		wd->GMRES_tol = 1e-6;
	}

	wd->GMRES_size = LHS->nr;
	if (!(wd->GMRES_V = matAlloc(wd->GMRES_iters + 1, wd->GMRES_size + 2))
		|| !(wd->GMRES_R = matAlloc(wd->GMRES_iters + 1, wd->GMRES_iters + 1))
		|| !(wd->GMRES_gk = malloc((wd->GMRES_iters + 1) * sizeof(double)))
		|| !(wd->GMRES_y = malloc((wd->GMRES_iters + 1) * sizeof(double)))
		|| !(wd->GMRES_c = malloc((wd->GMRES_iters + 1) * sizeof(double)))
		|| !(wd->GMRES_s = malloc((wd->GMRES_iters + 1) * sizeof(double))))
		return NULL;


	if (MS_modifyLHS(wd, LHS, NULL)) {
		return wd;
	}
	else {
		MS_cleanWD(wd);
		return NULL;
	}
}

ML_INT MS_modifyLHS_gmres(MS_Solver wd, spMATRIX* LHS, void** params) {
	if (params) {
		wd->GMRES_iters = (int)*((ML_INT*)params[0]);
		wd->GMRES_tol = (double)*((double*)params[1]);
	}
	wd->LHS = LHS;
	return 1;
}

ML_INT MS_freeSolver_gmres(MS_Solver wd) {
	if (wd->GMRES_V) free(wd->GMRES_V);
	if (wd->GMRES_R) free(wd->GMRES_R);
	if (wd->GMRES_gk) free(wd->GMRES_gk);
	if (wd->GMRES_y) free(wd->GMRES_y);
	if (wd->GMRES_c) free(wd->GMRES_c);
	if (wd->GMRES_s) free(wd->GMRES_s);
}

/* standard GMRES */
ML_INT MS_gmres(MS_Solver wd, spMATRIX* A, double* B, double* x0, ML_INT maxIts, double tol) {
	ML_INT i, j, k;
	double b, dtemp1, dtemp2, norm;

	if (!wd) {
		peprintf(PEPSTR_ERROR, NULL, "GMRES solver not initialised\n");
		return 0;
	}

	if (maxIts == 0) {
		peprintf(PEPSTR_ERROR, NULL, "GMRES max iterations set to 0\n");
		return 0;
	}

	/*
	norm = vecMag( x0, A->nr );
	if( norm < 1e-13 ) {
		x0[0] = -1.0;
	}
	 */

	printf("GMRES: A->nr = %d, A->aa[0] = %g, B[0] = %g, |B| = %g, |x0| = %g\n", A->nr, A->aa[0], B[0], vecMag(B, A->nr), vecMag(x0, A->nr));

	/* get v0  */
	vecCopy(wd->GMRES_V[0], B, A->nr);
	spMatVec(A, x0, wd->GMRES_V[0]);

	for (i = 0; i < A->nr; i++)
		wd->GMRES_V[0][i] = B[i] - wd->GMRES_V[0][i];

	b = vecMag(wd->GMRES_V[0], A->nr);                      /* Beta in GMRES paper */
	norm = fabs(b);

	/* Normalise V[0] */
	vecScale(wd->GMRES_V[0], 1. / b, A->nr);

	/* set inital value for gk */
	wd->GMRES_gk[0] = b;

	/* GMRES iterations */
	for (k = 0; k < maxIts && norm > b * tol; k++) {
		/* Get next Arnoldi vector */
		spMatVec(A, wd->GMRES_V[k], wd->GMRES_V[k + 1]);

		/* Make next Arnoldi vector orthogonal to all previous vectors */
		for (j = 0; j <= k; j++) {
			wd->GMRES_R[k][j] = vecDot(wd->GMRES_V[k + 1], wd->GMRES_V[j], A->nr);
			vecSubScale(wd->GMRES_V[k + 1], wd->GMRES_V[j], wd->GMRES_R[k][j], A->nr);
		}

		/* H now contains Hk - extend new column to give H~k */
		wd->GMRES_R[k][k + 1] = vecMag(wd->GMRES_V[k + 1], A->nr);

		/* Normalise V[k+1] */
		vecScale(wd->GMRES_V[k + 1], 1. / wd->GMRES_R[k][k + 1], A->nr);

		/* Rotate last column of H~k (k+1 x k) by all previous rotations */
		for (j = 0; j < k; j++) {
			dtemp1 = wd->GMRES_R[k][j];
			dtemp2 = wd->GMRES_R[k][j + 1];
			wd->GMRES_R[k][j] = wd->GMRES_c[j] * dtemp1 - wd->GMRES_s[j] * dtemp2;
			wd->GMRES_R[k][j + 1] = wd->GMRES_s[j] * dtemp1 + wd->GMRES_c[j] * dtemp2;
		}

		/* Get new rotation matrix (k+1 x k+1) */
		dtemp1 = sqrt(wd->GMRES_R[k][k] * wd->GMRES_R[k][k] + wd->GMRES_R[k][k + 1] * wd->GMRES_R[k][k + 1]);
		wd->GMRES_c[k] = wd->GMRES_R[k][k] / dtemp1;
		wd->GMRES_s[k] = -wd->GMRES_R[k][k + 1] / dtemp1;

		/* Rotate augmented matrix (k+1 x k+1) by new rotation (Fk) to get R~k (k+1 x k+1) */
		dtemp1 = wd->GMRES_R[k][k];
		dtemp2 = wd->GMRES_R[k][k + 1];
		wd->GMRES_R[k][k] = wd->GMRES_c[k] * dtemp1 - wd->GMRES_s[k] * dtemp2;
		wd->GMRES_R[k][k + 1] = wd->GMRES_s[k] * dtemp1 + wd->GMRES_c[k] * dtemp2;   /* == 0 */

		/* Multiply b*e1 by new rotation matrix (k+1 x k+1) */
		dtemp1 = wd->GMRES_gk[k];
		dtemp2 = wd->GMRES_gk[k + 1];
		wd->GMRES_gk[k] = dtemp1 * wd->GMRES_c[k];/* (- dtemp2 * s[k]; ~= 0) */
		wd->GMRES_gk[k + 1] = dtemp1 * wd->GMRES_s[k];/* (+ dtemp2 * c[k]; ~= 0) */

		/* residual norm */
		norm = fabs(wd->GMRES_gk[k + 1]);
	}

	//peprintf( PEPSTR_INFO, NULL, "GMRES matSize = %d, its = %d, resid = %d, residTol = %d\n", A->nr, k, norm, b*tol );

	if (k - 1 == maxIts - 1 && norm > b * tol) {
		peprintf(PEPSTR_ERROR, NULL, "GMRES: Iteration limit was reached, GMRES has not fully converged.\n");
		return -1;
	}

#ifdef MS_SOLVE_GMRES_PRINTCONV
	printf("GMRES: converged at iteration %d (resid = %g, tol = %g).\n", k, norm, b * tol);
#endif

	/* back substitute in the upper diagonal R to get y */
	for (j = k - 1; j >= 0; j--) {
		dtemp1 = wd->GMRES_gk[j];
		for (i = j + 1; i < k; i++)
			dtemp1 -= wd->GMRES_R[i][j] * wd->GMRES_y[i];
		wd->GMRES_y[j] = dtemp1 / wd->GMRES_R[j][j];
	}

	/* Form approximate solution, x in x0*/
	for (i = 0; i < A->nr; i++)
		for (j = 0; j < k; j++)
			x0[i] += wd->GMRES_V[j][i] * wd->GMRES_y[j];

	return k;
}

