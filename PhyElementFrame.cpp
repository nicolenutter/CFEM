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
	VECTOR *crd0, *crd1; //CC Ref PhyElementTruss, R. Abedi ->
	crd1 = &eNodePtrs[1]->coordinate;
	crd0 = &eNodePtrs[0]->coordinate;

	int sz = crd1->size();
	if (sz != 2)
	{
		THROW("implementation only for 2D frame");
	}
	double delX, delY;
	delX = (*crd1)(0) - (*crd0)(0);
	delY = (*crd1)(1) - (*crd0)(1);
	L = sqrt(delX * delX + delY * delY);
	c = delX / L;
	s = delY / L; //CC <- Ref PhyElementTruss, R. Abedi
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

	T(3, 3) = c; //CC
	T(3, 4) = s; //CC
	T(4, 3) = -s; //CC
	T(4, 4) = c; //CC
	T(3, 3) = c; //CC
	T(4, 4) = s; //CC
	T(5, 5) = 1; //CC

	MATRIX TT;	//CC
	for (int I = 0; I < 6; ++I)	//CC
	{
		for (int J = 0; J < 6; ++J)	//CC
		{
			TT(I, J) = T(J, I);	//CC
		}
	}

	ke.resize(6, 6); //CC
	ke = 0.0; //CC
	
	//ke = TT*kLocalCoordinate*T 
	for (int I = 0; I < 6; ++I) //CC
	{
		for (int J = 0; J < 6; ++J) //CC
		{
			for (int K = 0; K < 6; ++K) //CC
			{
				for (int L = 0; L < 6; ++L) //CC
				{
					ke(I, J) += TT(I, K) * kLocalCoordinate(K, L) * T(L, J); //CC
				}
			}
		}
	}
}

void PhyElementFrame::SpecificOutput(ostream& out) const
{
	// Complete for T, y, theta, M & V
}
