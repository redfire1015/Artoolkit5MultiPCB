#include "Headers/SimulationHandler.h"


//Matrix Variables
double C = 470e-6; //Capacitance
double R = 10e3; //Resistance
double Vs = 12; // Source Voltage
//Vs = readSettings.getInputVoltage(); //TODO: 

spMATRIX A;
double A_aa[] = { 1,-1,R,1,-1,1,-1,-1,R,1,-1,1,-1,-R,1,-1,-1,1,-1,-1,R,1,-1,1,-1,-1,-R,1,-1,-1,1,-1,-R,1,-1,-1,1,-1,-1,R,1,-1,1,-1,-1,-R,1,-1,-1,1,-1,-1,-R,1,-1,-1,1,-1,-1,-R,1,-1,-1,1,-1,-1,-R,1,-1,-1,1,-1,-R,1,-1,-1, };
ML_INT A_ja[] = { 0,1,0,2,1,3,4,5,3,7,4,5,6,5,7,8,6,9,10,11,9,15,10,11,12,13,11,15,16,12,13,14,14,16,17,14,18,19,20,18,30,19,20,21,22,20,30,31,21,22,23,24,22,31,32,23,24,25,26,24,32,33,25,26,27,28,26,33,34,27,28,29,28,34,35,29, };
ML_INT A_ia[] = { 0,2,4,5,8,10,11,13,16,17,20,22,23,26,29,30,32,35,36,39,41,42,45,48,49,52,55,56,59,62,63,66,69,70,72,75,76, };


spMATRIX M;
double M_aa[] = { C,C,C,C,C,C,C,C,C,C,C,C, };
ML_INT M_ja[] = { 2,7,8,15,16,17,30,31,32,33,34,35, };
ML_INT M_ia[] = { 0,0,0,1,1,1,2,2,2,3,3,3,4,4,4,5,5,5,6,6,6,7,7,7,8,8,8,9,9,9,10,10,10,11,11,11,12, };

double original_b[] = { 0,Vs,0,0,Vs,0,0,0,0,0,Vs,0,0,0,0,0,0,0,0,Vs,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, };
double b[] = { 0,Vs,0,0,Vs,0,0,0,0,0,Vs,0,0,0,0,0,0,0,0,Vs,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, }; //Voltage inputs
//End Matrix Variables

//Variables for transient Solving
double hd = 1e-3;
double prevTimestep = hd;
spMATRIX LHS;

double* transientCurrentSolution = new double[36];
double* transientNextSolution = new double[36];

ML_INT mat_i, mat_j, mat_k1, mat_k2, mat_j1, mat_j2, mat_jl;
ML_INT* ia;
ML_INT* mind, * aind;
//End variables for transient Solving


//Timing Variables
std::chrono::system_clock::time_point currentTime;
std::chrono::system_clock::time_point previousTime;

double timeSinceSimulationStart = 0.0;

bool simulationStarted = false;
//End Timing Variables

void populateMatrix() {
	// ******************* 36 * 36 Matrix *******************
	A.nr = 36;
	A.nc = 36;

	A.ia = A_ia;
	A.ja = A_ja;
	A.aa = A_aa;

	M.nr = 36;
	M.nc = 36;


	M.ia = M_ia;
	M.ja = M_ja;
	M.aa = M_aa;

	//TODO: Add definition for b in here


	transientCurrentSolution = new double[A.nc];
	vecZero(transientCurrentSolution, A.nc);
	//transientCurrentSolution[0] = 0.;

	transientNextSolution = new double[A.nc];
	vecZero(transientNextSolution, A.nc);
	transientNextSolution[0] = 1.;				//Sets our initial guess

	//std::cout << "************** Printing M Matrix * *************" << std::endl;
	//printSmallSparse(M);

	//std::cout << "************** Printing A Matrix * *************" << std::endl;
	//printSmallSparse(A);

	/* allocate LHS and setup standard matrix for 1st step */

	if (!((LHS.aa) = (double*)malloc((A.ia[A.nr] + M.ia[M.nr]) * sizeof(double)))
		|| !((LHS.ja) = (ML_INT*)malloc((A.ia[A.nr] + M.ia[M.nr]) * sizeof(ML_INT)))
		|| !((LHS.ia) = (ML_INT*)malloc((A.nr + 1) * sizeof(ML_INT)))
		|| !((ia) = (ML_INT*)malloc((A.nr + 1) * sizeof(ML_INT)))
		|| !((mind) = (ML_INT*)malloc((M.ia[M.nr]) * sizeof(ML_INT)))
		|| !((aind) = (ML_INT*)malloc((A.ia[A.nr]) * sizeof(ML_INT)))) {
		peprintf(PEPSTR_ERROR, NULL, "Error allocating memory for timesolve\n");
		return;
	}

	/* The following code generates arrays that map the sparse matrices A and M to a new sparse matrix LHS = M/h - A
	 * This allows LHS to be reconstructed very efficiently if M or A change during the simulation
	 */
	mat_j = 0;
	for (mat_i = 0; mat_i < A.nr; mat_i++) {      /* For each row in A and M */
		mat_k1 = A.ia[mat_i + 1];
		mat_k2 = M.ia[mat_i + 1];
		mat_j1 = A.ia[mat_i];
		mat_j2 = M.ia[mat_i];
		ia[mat_i] = LHS.ia[mat_i] = mat_j;

		/* Cannot guarantee A, M have ordered column indeces
		 * Cannot re-order A, M as will break links/pointers used by models for update
		 * So must do this ML_INT way...
		 */
		for (mat_jl = 0; mat_jl < A.nc; mat_jl++) {       /* For each potential column entry, mat_jl, on this row */
			for (mat_j1 = A.ia[mat_i]; mat_j1 < mat_k1; mat_j1++) if (A.ja[mat_j1] == mat_jl) break;      /* Is there a matching column entry in A matrix? */
			for (mat_j2 = M.ia[mat_i]; mat_j2 < mat_k2; mat_j2++) if (M.ja[mat_j2] == mat_jl) break;      /* Is there a matching column entry in M matrix? */

			if (mat_j1 == mat_k1 && mat_j2 == mat_k2) {
				continue;     /* This column will be zero */
			}
			else if (mat_j1 < mat_k1 && mat_j2 < mat_k2) {
				/* An entry was found in both A and M, entry in LHS matrix will be M[mat_i,mat_jl]+ (h*A[mat_i,mat_jl]) */
				LHS.aa[mat_j] = M.aa[mat_j2] + (hd * A.aa[mat_j1]);
				LHS.ja[mat_j] = M.ja[mat_j2];
				mind[mat_j2] = mat_j;         /* Entry mat_j2 in M->aa is mapped to entry mat_j in LHS->aa */
				aind[mat_j1] = mat_j;         /* Entry mat_j1 in A->aa is mapped to entry mat_j in LHS->aa */
				mat_j++;
			}
			else if (mat_j1 < mat_k1) {
				/* Entry was found only in A matrix, entry in LHS matrix will be 0 + h * A[mat_i,mat_jl] */
				LHS.aa[mat_j] = hd * A.aa[mat_j1];
				LHS.ja[mat_j] = A.ja[mat_j1];
				aind[mat_j1] = mat_j;         /* Entry mat_j1 in A->aa is mapped to entry mat_j in LHS->aa */
				mat_j++;
			}
			else if (mat_j2 < mat_k2) {
				/* Entry was found only in M matrix, entry in LHS matrix will be M[mat_i,mat_jl] */
				LHS.aa[mat_j] = M.aa[mat_j2];
				LHS.ja[mat_j] = M.ja[mat_j2];
				mind[mat_j2] = mat_j;         /* Entry mat_j2 in M->aa is mapped to entry mat_j in LHS->aa */
				mat_j++;
			}
		}
	}
	ia[mat_i] = LHS.ia[mat_i] = mat_j;

	LHS.nr = LHS.nc = A.nr;
	LHS.aa = (double*)realloc(LHS.aa, mat_j * sizeof(double));
	LHS.ja = (ML_INT*)realloc(LHS.ja, mat_j * sizeof(ML_INT));

	//Generate RHS aka b
	//spMatVec(&B, u, b);							// B*U -- For Now b = bu
	vecScale(b, hd, A.nr);							//now h(BU)

	//Print LHS
	std::cout << "************** Printing LHS Matrix * *************" << std::endl;
	printSmallSparse(LHS);

	//Print h*bu
	std::cout << "************** Printing h*bu * *************" << std::endl;
	for (int m = 0; m < A.nc; m++)
	{
		std::cout << b[m] << std::endl;
	}
}

int iterationsSimulation = 0;

void runSimulation(double currentTimestep)
{
	iterationsSimulation++;

	currentTimestep *= 1e-3;

	if (prevTimestep != currentTimestep)
	{
		//Reformat Matrix for new time step
		memcpy(LHS.ia, ia, (LHS.nr + 1) * sizeof(ML_INT));
		memset(LHS.aa, 0x00, LHS.ia[LHS.nr] * sizeof(double));

		for (mat_i = 0; mat_i < M.ia[M.nr]; mat_i++) {
			LHS.aa[mind[mat_i]] = M.aa[mat_i];
			LHS.ja[mind[mat_i]] = M.ja[mat_i];
		}
		for (mat_i = 0; mat_i < A.ia[A.nr]; mat_i++) {
			LHS.aa[aind[mat_i]] += A.aa[mat_i] * currentTimestep;
			LHS.ja[aind[mat_i]] = A.ja[mat_i];
		}

		//Restore b and recalculate  h *bu
		std::memcpy(b, original_b, sizeof(b));
		vecScale(b, currentTimestep, A.nr);

		prevTimestep = currentTimestep;
	}

	//Solve the matrix with time
	double* m_xn = new double[A.nc];
	vecZero(m_xn, A.nc);

	spMatVecSum(&M, transientCurrentSolution, m_xn);		//Now do m*xn

	//Now h(BU) + m(xn)
	double* B = new double[A.nc];
	vecZero(B, A.nc);
	for (int l = 0; l < A.nc; l++)
	{
		B[l] = b[l] + m_xn[l];
	}

	ML_INT itr = gmres(&LHS, B, transientNextSolution, LHS.nr, 1e-6); // Solve transient resposne for xn + 1

	for (int m = 0; m < A.nc; m++)
	{
		transientCurrentSolution[m] = transientNextSolution[m];
	}

}