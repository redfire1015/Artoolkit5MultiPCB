/*		Virtual Prototyping for Power Electronics
 *
 * 		File:				msolve.c
 * 		Author(s): 			P L Evans
 * 		Creation Date:		2009
 *
 * 		Description:		Interface to Matrix Solvers
 *
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#ifdef MS_HAVE_KLU
#include <msklu.h>
#endif



#include "Headers/matrix.h"
#include "Headers/msolve.h"
#include "Headers/solverdata.h"



MS_Solver MS_initSolver(spMATRIX* LHS, ML_INT solver, void** params) {
	MS_Solver wd;

	if (!(wd = malloc(sizeof(MS_WData)))) {
		printf("ERROR: Error allocating memory (WD)\n");
		return NULL;
	}

	MS_initWD(wd);
	wd->solver = solver;

	switch (solver) {
		//case MS_SOLVER_SLU:
		//	return MS_initSolver_slu(wd, LHS, params);
	case MS_SOLVER_GMRES:
		return MS_initSolver_gmres(wd, LHS, params);
		//case MS_SOLVER_PGMRES:
			//	return MS_initSolver_gmresp(wd, LHS, params);
			//case MS_SOLVER_KLU:	/* use KLU solver */
			//	return MS_initSolver_klu(wd, LHS, params);
			//case MS_SOLVER_KLU_CX:	// use KLU solver and assume complex input
			//	return MS_initSolver_klucx(wd, LHS, params);
			//case MS_SOLVER_PGMRES_GMV:
			//	return MS_initSolver_gmresp_gmv(wd, LHS, params);
			//case MS_SOLVER_LAPACK_DENSELT:
			//	return MS_initSolver_lapack_denselt(wd, LHS, params);
			//case MS_SOLVER_LAPACK_DENSEGE:
			//	return MS_initSolver_lapack_densege(wd, LHS, params);
			//case MS_SOLVER_BLOCKDIAG:
			//	return MS_initSolver_blockdiaglu(wd, LHS, params);
	default:
		peprintf(PEPSTR_WARN, NULL, "WARNING: Choice of Solver not recognised, equations will not be solved. (init)\n");
		return NULL;
		break;
	}
	return NULL;
}

ML_INT MS_modifyLHS(MS_Solver wd, spMATRIX* LHS, void** params) {
	ML_INT i;

	if (!wd) return 0;

	switch (wd->solver) {
	case MS_SOLVER_GMRES:
		return MS_modifyLHS_gmres(wd, LHS, params);
		//case MS_SOLVER_PGMRES:
		//	return MS_modifyLHS_gmresp(wd, LHS, params);
		//case MS_SOLVER_SLU:
		//	return MS_modifyLHS_slu(wd, LHS, params);
		//case MS_SOLVER_KLU:
		//	return MS_modifyLHS_klu(wd, LHS, params);
		//case MS_SOLVER_KLU_CX:
		//	return MS_modifyLHS_klucx(wd, LHS, params);
		//case MS_SOLVER_PGMRES_GMV:
		//	return MS_modifyLHS_gmresp_gmv(wd, LHS, params);
		//case MS_SOLVER_LAPACK_DENSELT:
		//	return MS_modifyLHS_lapack_denselt(wd, LHS, params);
		//case MS_SOLVER_LAPACK_DENSEGE:
		//	return MS_modifyLHS_lapack_densege(wd, LHS, params);
		//case MS_SOLVER_BLOCKDIAG:
		//	return MS_modifyLHS_blockdiaglu(wd, LHS, params);
	default:
		peprintf(PEPSTR_WARN, NULL, "WARNING: Choice of Solver not recognised, equations will not be solved. (modify)\n");
		return 0;
	}
}

/* change structure of LHS */
ML_INT MS_changeLHS(MS_Solver wd, spMATRIX* LHS, void** params) {
	if (!wd) return 0;

	switch (wd->solver) {

	case MS_SOLVER_GMRES:
		//case MS_SOLVER_PGMRES:
		//case MS_SOLVER_SLU:
		//	return MS_modifyLHS(wd, LHS, params);
		//case MS_SOLVER_KLU:
		//	return MS_changeLHS_klu(wd, LHS, params);
		//case MS_SOLVER_KLU_CX:
		//	return MS_changeLHS_klucx(wd, LHS, params);
		//case MS_SOLVER_PGMRES_GMV:
		//	return MS_changeLHS_gmresp_gmv(wd, LHS, params);
		//case MS_SOLVER_LAPACK_DENSELT:
		//	return MS_changeLHS_lapack_denselt(wd, LHS, params);
		//case MS_SOLVER_LAPACK_DENSEGE:
		//	return MS_changeLHS_lapack_densege(wd, LHS, params);
		//case MS_SOLVER_BLOCKDIAG:
		//	return MS_changeLHS_blockdiaglu(wd, LHS, params);
	default:
		peprintf(PEPSTR_WARN, NULL, "WARNING: Choice of Solver not recognised, equations will not be solved. (change)\n");
		return 0;
		break;
	}
}

ML_INT MS_solve(MS_Solver wd, double* b, double* x) {
	if (!wd) return 0;

	switch (wd->solver) {
	case MS_SOLVER_GMRES:
		return MS_gmres(wd, wd->LHS, b, x, wd->GMRES_iters, wd->GMRES_tol);
		//case MS_SOLVER_PGMRES:
		//	return MS_gmresp(wd, wd->LHS, &wd->LU, wd->uptr, b, x, wd->GMRES_iters, wd->GMRES_tol);
		//case MS_SOLVER_SLU:
		//	return spLUPSolve(wd, &wd->LU, wd->uptr, b, x, wd->rp, wd->cp, wd->scale);
		//case MS_SOLVER_KLU:
		//	return MS_solve_klu(wd, b, x);
		//case MS_SOLVER_KLU_CX:
		//	return MS_solve_klucx(wd, b, x);
		//case MS_SOLVER_PGMRES_GMV:
		//	return MS_gmresp_gmv(wd, b, x, wd->GMRES_iters, wd->GMRES_tol);
		//case MS_SOLVER_LAPACK_DENSELT:
		//	return MS_solve_lapack_denselt(wd, b, x);
		//case MS_SOLVER_LAPACK_DENSEGE:
		//	return MS_solve_lapack_densege(wd, b, x);
		//case MS_SOLVER_BLOCKDIAG:
		//	return MS_solve_blockdiaglu(wd, b, x);
	default:
		peprintf(PEPSTR_WARN, NULL, "WARNING: Choice of Solver not recognised, equations will not be solved. (solve)\n");
		return 0;
		break;
	}
}



ML_INT MS_freeSolver(MS_Solver wd) {
	ML_INT i;

	if (!wd) return 0;

	switch (wd->solver) {
	case MS_SOLVER_GMRES:
		MS_freeSolver_gmres(wd);
		break;
		//case MS_SOLVER_PGMRES:
		//	MS_freeSolver_gmresp(wd);
		//	break;
		//case MS_SOLVER_SLU:
		//	MS_freeSolver_slu(wd);
		//	break;
		//case MS_SOLVER_KLU:
		//case MS_SOLVER_KLU_CX:
		//	MS_freeSolver_klu(wd);
		//	break;
		//case MS_SOLVER_PGMRES_GMV:
		//	MS_freeSolver_gmresp_gmv(wd);
		//	break;
		//case MS_SOLVER_LAPACK_DENSELT:
		//	MS_freeSolver_lapack_denselt(wd);
		//	break;
		//case MS_SOLVER_LAPACK_DENSEGE:
		//	MS_freeSolver_lapack_densege(wd);
		//	break;
		//case MS_SOLVER_BLOCKDIAG:
		//	MS_freeSolver_blockdiaglu(wd);
		//	break;
	default:
		peprintf(PEPSTR_WARN, NULL, "WARNING: Choice of Solver not recognised, equations will not be solved. (free)\n");
		return 0;
		break;
	}
	MS_initWD(wd);
	return 1;
}



void MS_cleanWD(MS_Solver wd) {
	ML_INT i;

	if (!wd) return;

	if (wd->GMRES_V) matFree(wd->GMRES_V);
	if (wd->GMRES_gk)free(wd->GMRES_gk);
	if (wd->GMRES_y) free(wd->GMRES_y);
	if (wd->GMRES_R) matFree(wd->GMRES_R);
	if (wd->GMRES_c) free(wd->GMRES_c);
	if (wd->GMRES_s) free(wd->GMRES_s);

	if (wd->ILUT_w) free(wd->ILUT_w);
	if (wd->ILUT_wp) free(wd->ILUT_wp);
	if (wd->ILUT_uptrm) free(wd->ILUT_uptrm);

	if (wd->ILUT_vv) free(wd->ILUT_vv);
	if (wd->ILUT_diag) free(wd->ILUT_diag);
	if (wd->ILUT_col) free(wd->ILUT_col);
	if (wd->ILUT_rows) free(wd->ILUT_rows);

	if (wd->ILUT_data) {
		for (i = 0; i < wd->ILUT_all; i++) free(wd->ILUT_data[i]);
		free(wd->ILUT_data);
	}

	MS_initWD(wd);
}


/* global vars now contained in working data structure */
void MS_initWD(MS_Solver wd) {

	if (!wd) return;

	/* intialise all variables to NULL to allow active pointers to be identified on cleanup */
	wd->GMRES_V = NULL;
	wd->GMRES_gk = NULL;
	wd->GMRES_y = NULL;
	wd->GMRES_R = NULL;
	wd->GMRES_c = NULL;
	wd->GMRES_s = NULL;
	wd->GMRES_size = 0;

	wd->ILUT_w = NULL;
	wd->ILUT_wp = NULL;
	wd->ILUT_uptrm = NULL;
	wd->ILUT_size = 0;
	wd->ILUT_all = 0;
	wd->ILUT_bs = 0;

	wd->ILUT_vv = NULL;
	wd->ILUT_data = NULL;
	wd->ILUT_diag = NULL;
	wd->ILUT_col = NULL;
	wd->ILUT_rows = NULL;

	wd->uptr = NULL;
	wd->rp = NULL;
	wd->cp = NULL;
	wd->scale = NULL;

	spMatInit(&wd->LU);

	//wd->KLU_symbolic = NULL;
	//wd->KLU_numeric = NULL;

}








