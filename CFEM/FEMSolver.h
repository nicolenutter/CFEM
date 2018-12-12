#ifndef FEM_SOLVER__H
#define FEM_SOLVER__H

#include "CFEMTypes_Global.h"
#include "LAFuncs.h"
#include "PhyMaterial.h"
#include "PhyNode.h"
#include <cstring>

class PhyElement;	// code should compile because PhyElement is declared in this file as a pointer
					// still in FEMSolver we need to include PhyElement

class FEMSolver
{
public:
	// All steps together:
	void FEMSolve(string& runName, bool verboseIn);

	// input function
	// 1. friend? so the following function can access private members in this class
	//			both classes and functions can be declared as friends. 
	//			for example we can write
	// friend PhyElement;
	//	if all data/functions in this class is public, we don't need to declare this as friend
	//	this is not a function of the class. Let's say we have 
	//	FEMSolver solver;
	//	to call assemble on solve rwe need to write:	solver.Assemble();
	//	but the following is not a member function; IT'S ONLY DECLARED HERE TO MAKE A MAKE IT A FRIEND

	// 2. operator overload:	operator>>
	// in C++ there are many operations that can be "overloaded". For example you define a class of Polynomials
	// Polynomial p1, p2
	// Polynomial p3 = p1 + p2;
	// then you overload + operators
	//		>>			is an operator for readig in data 
	// How it's used
	//	FEMSolver solver;
	//	fstream in(name, ios::in);
	//	in >> solver;

	//	why do we return istream?
	// (istream& in, FEMSolver& dat) :			in >> dat
	//	in >> a >> b:		(in >> a) -> (returns in)		in >> b
	// this type of format is why input and output cascade in C++
	void Input(istream& in);

	// constructors and destructors DO NOT RETURN any values

	// don't need constructor; just a sample
	FEMSolver(int dimIn = 2);		// works as function with one or zero argument
	// destructors don't have input arguments
	~FEMSolver();

	// Step 3
	void setSizes();
	// Step 4; set prescribed dofs: already done when reading the input file
	// Step 5: Set global free nodal dof: already done when reading the input file 
	// Step 6 and Step 7: dof positions; Step 7: Set F
	void setPositions_F();
	// Step 8: Element dof maps Me
	// Step 9: Set element dofs ae
	void setElementDofMap_ae();
	// Step 10: Compute element stiness
	void Calculate_ElementStiffness_Force();

	// Step 11: Assembly from local to global system
	void Assemble();
	// Step 12: Solve global (free) dof a from Ka = F
	// successful solution returns true
	bool Solve_Dofs();
	// Step 13: Assign a to nodes and elements
	void Assign_dof();
	// Step 14: Compute prescribed dof forces
	void UpdateFpNodalPrescribedForces();


//private:
//	int ndofpn;		// for covering all topic we define this parameter as global (so nodes, ... can easily access it)
	int dim;
	int ndofpn;
	int nNodes;
	int ne; //number of elements
	vector<PhyNode> nodes;
	vector<PhyElement*>	pes;	// we do not use a vector of PhyElements because then virtualization (calling the subclass level function) would not work
	VECTOR dofs;
	int nf;	// number of free dofs
	int np;	// number of prescribed dofs
	int ndof; // ndof = nf + np
	MATRIX K; //stiffness matrix
	VECTOR F; // Force vector
	VECTOR Fp;	// vector of prescribed dof forces

	int nmats;	// number of materials
	map<int, PhyMaterial> mats;
};

istream& operator>>(istream& input, FEMSolver& dat);
ostream& operator<<(ostream& out, const FEMSolver& dat);

#endif