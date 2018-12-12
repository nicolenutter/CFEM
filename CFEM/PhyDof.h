#ifndef PHY_DOF__H
#define PHY_DOF__H

#include "CFEMTypes_Global.h"

class PhyDof
{
public:

	PhyDof();

	bool p;	// boolean: whether the dof is prescribed
	int pos;	// position in the global system (for free and prescribed)
	double v;	// value of dof
	double f;	// force corresponding to dof

//	F can be stress i can be (0, 1) sigma_{01}	
//	Field	F;
//	INDEX i;		
};

#endif