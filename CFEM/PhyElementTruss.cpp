#include "PhyElementTruss.h"
#include "PhyMaterial.h"
#include "PhyNode.h"

void PhyElementTruss::setInternalMaterialProperties(PhyMaterial* pMat)
{
	// Complete
}

void PhyElementTruss::setGeometry()
{
	VECTOR *crd0, *crd1; 
	crd1 = &eNodePtrs[1]->coordinate;
	crd0 = &eNodePtrs[0]->coordinate;

	int sz = crd1->size();
	if (sz != 2) 
		THROW("implementation only for 2D truss");
	double delX, delY;
	delX = (*crd1)(0) - (*crd0)(0); 
	delY = (*crd1)(1) - (*crd0)(1); 
	L = sqrt(delX * delX + delY * delY);
	c = delX / L;
	s = delY / L;
}


void PhyElementTruss::Calculate_ElementStiffness_Force()
{
	// Complete
}

void PhyElementTruss::SpecificOutput(ostream& out) const
{
	double T = A * E / L * (c * (edofs(2) - edofs(0)) + s * (edofs(3) - edofs(1))); 
	out << T;
}
