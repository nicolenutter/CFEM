#ifndef PHY_NODE__H
#define PHY_NODE__H

#include "CFEMTypes_Global.h"
#include "LAFuncs.h"
#include "PhyDof.h"

class PhyNode
{
	// no need to have friend specification, because all members are public
	// in fact, we can even declare the function outside the class scope (because we don't need to give friendship)
	// remember friendship is only given inside the class
//	friend ostream& operator<<(ostream& out, PhyNode& node);
//	friend ostream& operator<<(ostream& out, PhyNode& node);

public:
	void set_nndof(int nndofIn);
	void UpdateNodePrescribedDofForces(VECTOR& Fp);
	ID id;
	VECTOR coordinate;	
	vector <PhyDof> ndof;
	int nndof;	// number of dofs
};

ostream& operator<<(ostream& out, const PhyNode& node);

#endif