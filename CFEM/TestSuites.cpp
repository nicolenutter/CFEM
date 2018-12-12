#include "TestSuites.h"


void testMatrixInversion()
{
	// sample how a matrix is formed and solved in the form Ka = F with our specific definitions of MATRIX, VECTOR, and LUSolve.
	// NOTE: These functions and classes (VECTOR, MATRIX, and LUSolve) are NOT C++ built-in classes or functions.
	MATRIX K(2, 2);
	K = 1;
	// NOTE: unlike Fortran and Matlab indices often start from 0 in c++
	K(0, 1) = 2.0;
	K(1, 0) = 3.0;
	cout << "K\n" << K << '\n';

	VECTOR a(2);		// since LUSolves overwrited the solution on F (RHS) we directly store RHS F in unknown solution a
	a(0) = 4;
	a(1) = 7;
	cout << "F (RHS) is \n" << a << '\n';

	int singular = LUsolve(K, a);

	if (singular == 0)
	{
		// solution is computed and overwritten on a
		cout << "a (LHS unknown) is \n" << a << '\n';
	}
}