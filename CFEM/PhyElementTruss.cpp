#include "PhyElementTruss.h"
#include "PhyMaterial.h"
#include "PhyNode.h"

void PhyElementTruss::setInternalMaterialProperties(PhyMaterial* pMat)
{
	E = pMat->paras(mpb_E); //CC Ref PhyElementFrame, R. Abedi
	A = pMat->paras(mpb_A); //CC Ref PhyElementFrame, R. Abedi
}

void PhyElementTruss::setGeometry()
{
	VECTOR *crd0, *crd1; 
	crd1 = &eNodePtrs[1]->coordinate;
	crd0 = &eNodePtrs[0]->coordinate;

	int sz = crd1->size();
	{
		THROW("implementation only for 2D truss");
	}
	double delX, delY;
	delX = (*crd1)(0) - (*crd0)(0); 
	delY = (*crd1)(1) - (*crd0)(1); 
	L = sqrt(delX * delX + delY * delY);
	c = delX / L;
	s = delY / L;
}


void PhyElementTruss::Calculate_ElementStiffness_Force()
{
	ke.resize(4, 4); //CC Ref R. Abedi ME517_20150417_x264.mp4 ->
	double factor = A * E / L;
	for (int I = 0; I < 2; ++I)
	{
		for (int J = 0; J < 2; ++J)
		{
			double f2 = factor;
			if ((I + J) % 2 != 0)
			{
				f2 = -factor;
			}
			ke(I, J) = pow(c, 2)* f2;
			ke(I + 1, J) = c * s * f2;
			ke(I + 1, J + 1) = pow(s, 2) * f2;
		} //CC <- Ref R. Abedi ME517_20150417_x264.mp4
	}
}
}

void PhyElementTruss::SpecificOutput(ostream& out) const
{
	double T = A * E / L * (c * (edofs(2) - edofs(0)) + s * (edofs(3) - edofs(1))); 
	out << T;
}
