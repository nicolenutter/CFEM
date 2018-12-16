#include "PhyElement.h"
#include "PhyElementBar.h"
#include "PhyElementTruss.h"
#include "PhyElementFrame.h" //CC
#include "PhyNode.h"
#include "PhyDof.h"
#include "CFEMTypes_Global.h"
#include "PhyGlobal.h"

PhyElement* PhyElementFactory(ElementType eTypeIn)
{
	PhyElement* pePtr = NULL;
	switch (eTypeIn)
	{
//	case etDefault:
//		pePtr = new PhyElement();
//		break;
	case etBar:
		pePtr = new PhyElementBar();
		break;
	case etTruss:
		pePtr = new PhyElementTruss();
		break;
	case etFrame: //CC	
		pePtr = new PhyElementFrame(); //CC
		break; //CC
	default:
		THROW("the type is not defined");
	}
	if (pePtr != NULL)
		pePtr->eType = eTypeIn;
	return pePtr;
}

ostream& operator<<(ostream& out, const PhyElement& dat)
{
	// id ElementType
	out << dat.id << '\t' << dat.eType << '\n';
	if (verbose == true)
	{
		for (int i = 0; i < dat.nedof; ++i)
			out << dat.fee(i) << '\t';
	}
	out << '\n';
	dat.SpecificOutput(out);
	out << '\n';
	return out;
}

void PhyElement::setNodeConnectivity_Sizes(int nNodeInElement, int ndofpnIn, vector<int>& eNodesIn, vector <PhyNode*>& eNodePtrsIn)
{
	neNodes = nNodeInElement;
	eNodes.resize(neNodes);
	eNodes = eNodesIn;

	eNodePtrs.resize(neNodes);
	eNodePtrs = eNodePtrsIn;
	// resizing members in PhyElement

	ndofpn = ndofpnIn;

	// Complete
}


void PhyElement::setElementDofMap_ae(int ndofpn)
{
	edofs.resize(nedof);
	int gn;
	for (int jj = 0; jj < neNodes; jj++)
	{
		gn = eNodes[jj]; // global node number for element node en

		for (int kk = 0; kk < ndofpn; kk++)
		{
			if (eNodes[jj].pos == true)
			{
				edofs(kk) -> eNodes[jj].v;
			}
			eNodes[jj].v -> dofMap[kk];
		}
	}
} // Doesn't work but it's an attempt to recreate the psuedocode and make it run in C++


void PhyElement::AssembleStiffnessForce(MATRIX& globalK, VECTOR& globalF)
{
	// fde = ke*ae, fr = 0, fn = 0
	// free columns are used for k assembly but prescribed columns not needed
	MATRIX K;
	for (int ii = 0; ii < nedof; ii++)
	{
		double I = dofMap[ii];
		if (I < 0)
		{
			continue;
		}
		else
		{
			I = foe(ii);
		}
		for (int jj = 0; jj < nedof; jj++)
		{
			double J = dofMap[jj];
			if (J < 0)
			{
				continue;
			}
			else
			{
				J = foe(jj);
				K(ii, jj) += ke(ii, jj);
				fde(ii) += ke(ii, jj)*edofs(jj);
			}
		}
		fee(ii) = foe(ii) - fde(ii);		
	}
}

void PhyElement::UpdateElementForces_GlobalFp(VECTOR& Fp)
{
	for (int ii = 0; ii < nedof; ++ii)
		Fp(ii) += fee(ii);

}
