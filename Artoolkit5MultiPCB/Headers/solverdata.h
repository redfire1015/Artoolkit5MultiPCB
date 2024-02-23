
/*		Virtual Prototyping for Power Electronics
 *
 * 		File:				solverdata.h
 * 		Author(s): 			P L Evans
 * 		Creation Date:		5 May 2015
 *
 * 		Description:
 * 		Header file for solvers containing definitions that do not
 * 		need to be known by calling code
 */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef SOLVERDATA_H_
#define SOLVERDATA_H_

	//#include "klu.h"

#include "matrix.h"
#include "msolve.h"

#define MEM_BLOCK_SIZE 12

	extern char* KLU_ERRORTEXT[];

	/** Prints a KLU error message based on a KLU error code
	 * @param e is the KLU error code
	 */
#define PRINTKLUERROR( e ) ( printf( "%s\n",(e>=0)? KLU_ERRORTEXT[e]:KLU_ERRORTEXT[-e]) )

	 /** Returns a KLU error message based on a KLU error code
	  * @param e is the KLU error code
	  */
#define KLUERRORSTR( e ) ( (e>=0)? KLU_ERRORTEXT[e]:KLU_ERRORTEXT[-e] )


	  /** Temporary structure used for constructing sparse LU factors by MS_getLUTP_cs() */
	typedef struct _spMatrixElement {
		ML_INT col;						/**< Column of matrix entry */
		double val;						/**< Matrix entry value */
		struct _spMatrixElement* n;		/**< use very simple linked list to allow fast row insertions */
	} spMatrixElement;


	/** Main solver data structure for the MS_Solver wrapper.
	 * contains data storage pointers for all solver types. Structure
	 * allows memory to be allocated once for repetitive calls of solver
	 * and provides abstraction from solver type choice.
	 */
	typedef struct _MS_WData {
		ML_INT alloc;			/**< */
		ML_INT solver;			/**< solver type */

		/* working data used while factorising / solving  */
		/* gmres global variables -> so mem allocation only needs to happen once */
		double** GMRES_V;				/**< GMRES projection matrix, V */
		double* GMRES_gk;				/**< GMRES residual vector */
		double* GMRES_y;				/**< */
		double** GMRES_R;				/**< GMRES coefficient matrix */
		double* GMRES_c;				/**< GMRES rotation coefficients */
		double* GMRES_s;				/**< GMRES rotation coefficients */
		ML_UINT GMRES_size;		/**< GMRES allocation size */

		double* ILUT_w;			/**< ILUT working data */
		double* ILUT_vv;		/**< ILUT working data */
		ML_INT* ILUT_wp;			/**< ILUT working data */
		ML_INT* ILUT_uptrm;		/**< ILUT diagonal indeces */
		ML_INT ILUT_size;			/**< ILUT matrix dimension */
		ML_INT ILUT_all;			/**< ILUT memory allocation size */
		ML_INT ILUT_bs;			/**< ILUT memory block size */
		ML_INT ILUT_ne;			/**< */
		ML_INT ILUT_ma;			/**< */
		spMatrixElement** ILUT_rows;	/** ILUT temporary matrix rows */
		spMatrixElement** ILUT_col;		/** ILUT temporary matrix columns */
		spMatrixElement** ILUT_data;	/** ILUT temporary matrix data */
		spMatrixElement** ILUT_diag;	/** ILUT temporary matrix diagonals */

		/* factorised LHS data */
		spMATRIX LU;			/**< Contains LU factored matrix / LU preconditioner */
		spMATRIX* LHS;			/**< original matrix to solve */
		ML_INT* uptr;			/**< Diagonal indeces in combined LU matrix */
		ML_INT* rp;				/**< Row permutations for pivoting */
		ML_INT* cp;				/**< Columnn permutations for pivoting */
		double* scale;			/**< Row scaling */

		double params[4];		/**< Solver options/parameters */

		double ILUT_ztol;		/**< zero tolerance (matrix values smaller than this are set to zero) */
		double ILUT_ptol;		/**< */
		double GMRES_tol;		/**< GMRES convergence tolerance */
		double ILUT_tol;		/**< ILUT threshold */
		ML_INT GMRES_iters;		/**< max GMRES iterations */

		/* for the KLU solver */
		//klu_l_symbolic* KLU_symbolic;		/**< KLU result of symbolic factorisation */
		//klu_l_numeric* KLU_numeric;		/**< KLU result of numerical factorisation */
		//klu_l_common KLU_common;			/**< KLU options */

		/* Callbacks */
		void (*matVec)(double*, double*, void**);        /**< Callback for custom matrix-vector product in case of interative solver */
		void (*matVecPre)(double*, double*, void**);     /**< Callback for custom preconditioner matrix-vector product in case of interative solver */
		void* cbarg;
	} MS_WData;


#endif /* SOLVERDATA_H_ */

#ifdef __cplusplus
}
#endif
