#include "PhyElementBar.h"
#include "PhyMaterial.h"
#include "PhyNode.h"


void PhyElementBar::setGeometry()
{
	L = fabs(eNodePtrs[1]->coordinate(0) - eNodePtrs[0]->coordinate(0));
}

void PhyElementBar::setInternalMaterialProperties(PhyMaterial* pMat)
{
	A = pMat->paras(mpb_A);
	E = pMat->paras(mpb_E);
}

void PhyElementBar::Calculate_ElementStiffness_Force()
{
	// compute stiffness matrix:
	ke.resize(2, 2);
	double factor = A * E / L;
	ke(0, 0) = ke(1, 1) = factor;
	ke(1, 0) = ke(0, 1) = -factor;
}

void PhyElementBar::SpecificOutput(ostream& out) const
{
	double T = A * E / L * (edofs(1) - edofs(0)); 
	out << T;
}
