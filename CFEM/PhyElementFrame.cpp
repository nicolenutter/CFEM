#include "PhyElementFrame.h"
#include "PhyMaterial.h"
#include "PhyNode.h"

void PhyElementFrame::setInternalMaterialProperties(PhyMaterial* pMat)
{
	E = pMat->paras(mpb_E);
	A = pMat->paras(mpb_A);
	I = pMat->paras(mpb_I);
//	eType = 4 Frame matfeng = [E A I] (454d)
}

void PhyElementFrame::setGeometry()
{
	// Complete
}


void PhyElementFrame::Calculate_ElementStiffness_Force()
{
	a1 = E * A / L;
	a2 = E * I / pow(L, 3);

	//! 1. stiffness matrix in local coordinate system
	// kbar
	kLocalCoordinate.resize(6, 6);
	kLocalCoordinate = 0.0;
	kLocalCoordinate(0, 0) = a1;
	kLocalCoordinate(0, 3) = -a1;
	kLocalCoordinate(3, 0) = -a1;
	kLocalCoordinate(3, 3) = a1;
	kLocalCoordinate(1, 1) = 12.0 * a2;

	//! 2. Transformation matrix T
	T.resize(6, 6);
	T(0, 0) = c;
	T(0, 1) = s;
	T(1, 0) = -s;
	T(1, 1) = c;
	T(0, 0) = c;
	T(0, 1) = s;
	T(2, 2) = 1;
	// complete
}

void PhyElementFrame::SpecificOutput(ostream& out) const
{
	// Complete
}
