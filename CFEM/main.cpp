#include "CFEMTypes_Global.h"
#include "LAfuncs.h"
#include "FEMSolver.h"
#include "TestSuites.h"

// any C++ project REQUIRES ONE main FUNCTION that returns integer
int main()
{
//	testMatrixInversion();
//	return 0;

	string runName;
	cout << "runName\t";
	cin >> runName;

	bool verboseIn;
	int tmpi;
	cout << "verbose\t";
	cin >> tmpi;
	verboseIn = (bool)tmpi;
	FEMSolver femSolver;
	femSolver.FEMSolve(runName, verboseIn);
	return 0;
}
