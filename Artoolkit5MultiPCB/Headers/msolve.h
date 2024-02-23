/*		Virtual Prototyping for Power Electronics
 *
 * 		@file:				msolve.h
 * 		Author(s): 			P L Evans
 * 		Creation Date:		2009
 *
 * 		Description:
 *		Interface to matrix solvers
 *
 */
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef MSOLVE_H
#define MSOLVE_H

	/** Print num of iterations to convergence and final residual information in GMRES algorithms */
#define MS_SOLVE_GMRES_PRINTCONV

#ifdef MS_HAVE_KLU
#include <klu.h>
#endif

#include "matrix.h"

/** The contents of MS_WData are defined in a header which is only included by solvers.
 * User only needs to be able to define pointer to this structure.
 * If user were to know contents, would be required to import range of headers from KLU libraries etc
 */
#define MS_Solver struct _MS_WData*


 /* solver types, parameters and parameter recommended values */
#define MS_SOLVER_UNDEF             0
#define MS_SOLVER_GMRES			    1	/**< Solver type GMRES, params: p0 = maxits (300), p1 = gmres tol (1e-6) */
#define MS_SOLVER_PGMRES		    2	/**< Solver type Preconditioned GMRES, params: p0 = blocksize (10), p1 = maxits(300), p2 = gmres tol (1e-6), p3 = ilut tol (1e-2) */
#define MS_SOLVER_SLU			    3	/**< Solver type Basic sparse LU (DO NOT USE), params: p0 = blocksize (10), p1 = fill in zero tol (0), p2 = relative pivot size tolerance (1) */
#define MS_SOLVER_KLU			    4   /**< Solver type KLU sparse LU factorisation, params: */
#define MS_SOLVER_KLU_CX		    5	/**< Solver type KLU sparse LU factorisation for complex matrices, params: */
#define MS_SOLVER_PGMRES_GMV	    6	/**< Optionally preconditioned GMRES with external callbacks for matrix operations, params: MS_gmresp_gmv_opts* */
#define MS_SOLVER_LAPACK_DENSELT	7	/**< Dense symmetric matrix stored as packed rowmajor lower triangular in LHS.aa. Use lapack solver. if params != NULL then copy of LHS factored, otherwise factored in-situ. */
#define MS_SOLVER_LAPACK_DENSEGE	8	/**< Dense gereral matrix stored as rowmajor array in LHS.aa. Use lapack solver. if params != NULL then copy of LHS factored, otherwise factored in-situ. */
#define MS_SOLVER_BLOCKDIAG         9   /**< Block diagonal matrix with dense blocks. aa is array of values, ja is starting point of each block in aa array, ia is cummulative size of blocks (size of block i = ia[i+1]=ia[i], nr contains number of rows, nc contains number of blocks */
//#define KLU_DISPLAY_CONDEST

#include "msgmres.h"
//#include "msgmresp.h"
//#include "msklu.h"
//#include "msklucx.h"
//#include "msslu.h"
//#include "msgmresp_gmv.h"
//#include "mslapack.h"
//#include "msblockdiaglu.h"
//#include "solveUtils.h"
#include "standAloneSolve.h"

/** Initialise a MS_Solver for use with preconditioned GMRES and an incomplete LU factorisation.
 *
 * @param LHS is a pointer to a sparse matrix (A in Ax = b)
 * @param solver is a an integer code representing the solver type, see above.
 * @param params is an array of pointers to parameters for the solver, required parameters are indicated above in the solver types.
 *
 * @return a MS_Solver (a pointer to a MS_WData structure)
 */
	MS_Solver MS_initSolver(spMATRIX* LHS, ML_INT solver, void** params);


	/** Modify the matrix being used by the solver (left hand side of the equation Ax=b)
	 * This differs to MS_changeLHS() in that it assumes the structure of the LHS matrix remaining unchanged (only minor numerical
	 * value changes have occured), this is significant for KLU since it may not need to re-analyse the matrix.
	 *
	 * @param wd is an initialised MS_Solver
	 * @param LHS is a pointer to the new left hand side in sparse matrix format
	 * @param params is an array of pointers to parameters for the solver, required parameters are indicated above in the solver types.
	 * If this is NULL, the parameters supplied when the solver was initialised will continue to be used.
	 * @return 1 for success, 0 for fail
	 */
	ML_INT MS_modifyLHS(MS_Solver wd, spMATRIX* LHS, void** params);


	/** Change the matrix being used by the solver (left hand side of the equation Ax=b)
	 * This differs to MS_modifyLHS() in that it assumes the new LHS matrix is completely different to the old.
	 * This is significant for KLU since it needs to re-analyse the matrix.
	 *
	 * @param wd is an initialised MS_Solver
	 * @param LHS is a pointer to the new left hand side in sparse matrix format
	 * @param params is an array of pointers to parameters for the solver, required parameters are indicated above in the solver types.
	 * If this is NULL, the parameters supplied when the solver was initialised will continue to be used.
	 * @return 1 for success, 0 for fail
	 */
	ML_INT MS_changeLHS(MS_Solver wd, spMATRIX* LHS, void** params);


	/** Solve Ax = b using an initialised MS_Solver.
	 * Note A, the matrix, has already been supplied through either MS_initSolver(), MS_changeLHS() or MS_modifyLHS().
	 *
	 * @param wd is an initialised MS_Solver
	 * @param b is b in Ax = b
	 * @param x is the solution, x in Ax = b
	 * @return 1 for success, 0 for fail
	 */
	ML_INT MS_solve(MS_Solver wd, double* b, double* x);


	/** Initialises a MS_Solver.
	 * Just sets all internal storage pointers to NULL and all indeces/sizes to zero
	 * @param wd is the solver to initialise.
	 */
	void MS_initWD(MS_Solver wd);


	/** Frees a MS_Solver.
	 * Deallocates all memory used by the solver, only attempts to free variables used by solver.
	 * @param wd is the solver to free.
	 * @return
	 */
	ML_INT MS_freeSolver(MS_Solver wd);


	/** Frees a MS_Solver.
	 * Deallocates all memory used by the solver, attempts to free everything.
	 * @param wd is the solver to free.
	 * @return
	 */
	void MS_cleanWD(MS_Solver wd);

#endif

#ifdef __cplusplus
}
#endif
