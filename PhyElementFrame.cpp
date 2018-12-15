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
	// T = k * q
	out << T; 
	// equations used
	// y(eta) = n1(eta)*1e + n2(eta)*a2e + n3(eta)*a3e + n4(eta)*a4e
	// theta(eta) = dn1/deta(eta)*a1+dn2/deta(eta)*a2 dn3/deta(eta)*a3 + dn4/deta(eta)*a4 -> dy/deta
	// M(eta) = E*I(b1(eta)*a1 + b2(eta)*a2 + b3(eta)*a3 + b4(eta)*a4 -> d2y/deta2
	// V(eta) = E*I(db1/deta(eta)*a1 + db2/deta(eta)*a2 + db3/deta(eta)*a3 + db4/deta(eta)*a4 -> d3y/deta3
	// n1e = 1/4(2 - 3*eta + eta^3)                        b1e = 6*eta/Le^2
	// n2e = Le/8(1 - eta - eta^2 + eta^3)                 b2e = (-1 + 3*eta)/Le 
	// n3e = 1/4(2 + 3*eta - eta^3)                        b3e = -6*eta/Le^2
	// n4e = Le/8(-1 - eta + eta^2 + eta^3)                b4e = (1+3*eta)/Le
	double eta;
	// Is eta supposed to be half the length or a combo of c and s or something else entirely?
	double y = 1/4*(2-3*eta+pow(eta, 3))*edofs(0) + L/8*(1-eta-pow(eta,2)+pow(eta,3))*edofs(1) + 1/4*(2+3*eta-pow(eta,3))*edofs(2) + L/8*(-1-eta+pow(eta,2)+pow(eta,3))*edofs(3);
	out << y;
	// Should we do it this way (chain rule) or implement a finite difference or some other method for deriving?
	double theta = 3/(2*L)*(1+pow(eta,2))*edofs(0) + 1/4*(1-2*eta+3*pow(eta,2))*edofs(1) + 3/(2*L)*(1-pow(eta,2))*edofs(2) + 1/4*(-1+2*eta+3*pow(eta,2))*edofs(3);
	out << theta;
	double M = E*I*(6*eta/pow(L,2)*edofs(0) + (-1+3*eta)/L*edofs(1) - 6*eta/pow(L,2)*edofs(2) + (1+3*eta)/L*edofs(3));
	out << M;
	double V = E*I*(12/pow(L,3)*edofs(0) + 6/pow(L,2)*edofs(1) - 12/pow(L,3)*edofs(2) + 6/pow(L,2)*edofs(3));
	out << V;
}
