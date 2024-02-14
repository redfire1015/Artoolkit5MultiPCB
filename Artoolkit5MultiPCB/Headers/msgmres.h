/*		Virtual Prototyping for Power Electronics
 *
 * 		@file:				gmres.h
 * 		Author(s): 			P L Evans
 * 		Creation Date:		18/03/2020
 *
 * 		Description:	    Solver implementation.
 *
 */

#ifndef VPPE3_MSGMRES_H
#define VPPE3_MSGMRES_H

#include "msolve.h"

/**
 *
 * @param LHS
 * @param solver
 * @param params
 * @return
 */
MS_Solver MS_initSolver_gmres( MS_Solver wd, spMATRIX *LHS, void** params ) ;

/**
 *
 * @param wd
 * @param LHS
 * @param params
 * @return
 */
ML_INT MS_modifyLHS_gmres( MS_Solver wd, spMATRIX *LHS, void **params );

/**
 *
 * @param wd
 * @return
 */
ML_INT MS_freeSolver_gmres( MS_Solver wd );

/** GMRES Solver.
 * USED INTERNALLY ONLY BY MS_solve()
 * @param wd is an initialised MS_Solver
 * @param A is A in Ax = b
 * @param B is b in Ax = b
 * @param x0 is x in Ax = b and also the initial guess for the iterations. It is important that this is not zero! A unit vector [1,0,0,0,...] tends to work well.
 * @param maxIts is the maximum number of iterations that will be attempted. Set to dimension of A for guaranteed solution. Set through params passed to MS_initSolver(), MS_modifyLHS() and MS_changeLHS().
 * @param tol is the convergence tolerance. 1e-6 tends to work well. Set through params passed to MS_initSolver(), MS_modifyLHS() and MS_changeLHS()
 * @return 1 for success, 0 for fail.
 */
ML_INT MS_gmres( MS_Solver wd, spMATRIX *A, double* B, double* x0, ML_INT maxIts, double tol ) ;



#endif //VPPE3_MSGMRES_H
