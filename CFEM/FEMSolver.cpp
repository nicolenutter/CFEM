#include "CFEMTypes_Global.h"
#include "FEMSolver.h"
#include "PhyElement.h"
#include "LAFuncs.h"
#include "PhyGlobal.h"
#include "CFEMTypes_Global.h"
#include "PhyNode.h"

// in C++ do not write friend again (similar to virtual)
void FEMSolver::Input(istream& in)
{
	int tmpi;
	long double tmpd;	
	
	in.ignore(INT_MAX,' ');
	in >> tmpi; 
	dim = tmpi;
	in.ignore(INT_MAX, '\n');
	in.ignore(INT_MAX, ' ');
	in >> tmpi;
	ndofpn = tmpi;
	in.ignore(INT_MAX, '\n');
	in.ignore(INT_MAX, '\n');
	in.ignore(INT_MAX, ' ');
	in >> tmpi;
	nNodes = tmpi;
	in.ignore(INT_MAX, '\n');
	in.ignore(INT_MAX, '\n');
	
	for (int i = 0; i < nNodes; ++i)
	{	
		in >> tmpi;
		nodes[i].id = tmpi;
		if (tmpi != (i + 1))
		{
			THROW("incorrect id");
		}
		nodes[i].coordinate.resize(dim);
		for (int J = 0; J < dim; ++J)
		{
			in.ignore(INT_MAX, ' ');
			in >> tmpd;
			nodes[i].coordinate(J) = tmpd;
		}
		in.ignore(INT_MAX, '\n');
	}
	
	in.ignore(INT_MAX, '\n');
	in.ignore(INT_MAX, ' ');
	in >> tmpi;
	ne = tmpi;
	in.ignore(INT_MAX, '\n');
	in.ignore(INT_MAX, '\n'); 
	
	ElementType eType;
	int matID;
	int nNodeInElement;
	PhyElement* pe;
	for (int i = 0; i < ne; ++i)
	{
		in >> tmpi;
		if (tmpi != (i + 1))
			THROW("incorrect id");

		in >> tmpi;
		eType = (ElementType)tmpi;

		pes[i] = PhyElementFactory(eType);
		pes[i]->id = tmpi;

		// 1. we don't need to use pe instead of pes[i]. It just makes read and write simpler
		// 2. we can use pe instead pes[i] because it's a pointer (it's an address).
		// 3. OBVIOUSLY we cannot do this trick with nonpointer data

		pe = pes[i]; 
		in >> pe->matID;		// longer way which was fine: in >> pes[i]->matID;
								// another way not recommended (*pe).matID
								// ptr			*ptr	object
							// object		&object	address of the object
		
		int nNodeInElement;
		in >> nNodeInElement;
		vector<int> eNodesTmp(nNodeInElement);
		vector <PhyNode*> eNodePtrsTmp(nNodeInElement);
		for (int j = 0; j < nNodeInElement; ++j)
		{
		 in >> eNodesTmp[j];
		 --eNodesTmp[j];
		 eNodePtrsTmp[j] = &nodes[eNodesTmp[j]]; // safe here because nodes size never is going to change. If not this causes a very nasty bug to fix ... 
		}
		pe->setNodeConnectivity_Sizes(nNodeInElement, ndofpn, eNodesTmp, eNodePtrsTmp); 
	}

	in.ignore(INT_MAX, '\n');
	in.ignore(INT_MAX, '\n');
	in.ignore(INT_MAX, ' ');
	in >> tmpi;
	np = tmpi;
	in.ignore(INT_MAX, '\n');
	int tmp2i, tmp3i;
	for (int i = 0; i < np; ++i)
	{
		in.ignore(INT_MAX, '\n');
		in >> tmpi;
		in.ignore(INT_MAX, ' ');
		in >> tmp2i;
		in.ignore(INT_MAX, ' ');
		in >> tmp3i;
		nodes[tmpi].ndof[tmp2i].p = true; //Set prescribed dof & value
		nodes[tmpi].ndof[tmp2i].v = tmp3i;
		
	}
	in.ignore(INT_MAX, '\n');
	in.ignore(INT_MAX, '\n');
	in.ignore(INT_MAX, ' ');
	in >> tmpi;
	in.ignore(INT_MAX, '\n');
	for (int i = 0; i < nf; ++i)
	{
		in.ignore(INT_MAX, '\n');
		in >> tmpi;
		in.ignore(INT_MAX, ' ');
		in >> tmp2i;
		in.ignore(INT_MAX, ' ');
		in >> tmp3i;
		nodes[tmpi].ndof[tmp2i].p = false;  //set free dof boolean
		nodes[tmpi].ndof[tmp2i].f = tmp3i; //Set force on Free DOF
	}
	in.ignore(INT_MAX, '\n');
	in.ignore(INT_MAX, '\n');
	in.ignore(INT_MAX, ' ');
	in >> tmpi;
	nmats = tmpi;
	in.ignore(INT_MAX, '\n');
	for (int i = 0; i < nmats; ++i)
	{
		in.ignore(INT_MAX, '\n');
		in >> tmpi;
		in.ignore(INT_MAX, ' ');
		in >> tmp2i;
		for (int j = 0; j < tmp2i; ++j)
		{
			in.ignore(INT_MAX, ' ');
			in >> tmp3i;
			mats[tmpi].paras(j) = tmp3i; //Set Mat Props
		}
	}

	for (int e = 0; e < ne; ++e)
	{
		pe = pes[e];
		pe->setGeometry();
		matID = pe->matID;
		pe->setInternalMaterialProperties(&mats[matID]);
	}
//	return in;
}

istream& operator>>(istream& input, FEMSolver& dat)
{
	dat.Input(input);
	return input;
}

ostream& operator<<(ostream& out, const FEMSolver& dat)
{
	out << "Nodes\n";
	out << "nNodes\t" << dat.nNodes << '\n';
	out << "id\tcrd\n";
	out << "values\nforces\n";
	if (verbose == true)
	{
		out << "position(verbose)\n";
		out << "prescribed_boolean(verbose)\n";
	}
	for (int node = 0; node < dat.nNodes; ++node)
		out << dat.nodes[node] << '\n';
	for (int element = 0; element < dat.ne; ++element) 
		out << (*dat.pes[element]) << '\n';
	return out;
}

FEMSolver::FEMSolver(int dimIn)
{
	dim = dimIn;
}

FEMSolver::~FEMSolver()
{
	// ne should be equal to pes.size()
	// still a better practice is
	for (int i = 0; i < pes.size(); ++i)
		delete pes[i];
}


void FEMSolver::FEMSolve(string& runName, bool verboseIn)
{
	verbose = verboseIn;
	string inputFileName;
	inputFileName = runName + ".txt";
	fstream in(inputFileName.c_str(), ios::in);
	if (in.is_open() == false)
	{
		cout << "input file name\t" << inputFileName << " does not exist\n";
		THROW("file does not exist\n");
	}
	// reading data
	Input(in);
	// can do it as
//	in >> (*this);
	in.close();
	
	/////////////////////////////////////////////////////////////////////////
	// steps

	// Step 3
	setSizes();
	// Step 4; set prescribed dofs: already done when reading the input file
	// Step 5: Set global free nodal dof: already done when reading the input file 
	// Step 6 and Step 7: dof positions; Step 7: Set F
	setPositions_F();
	// Step 8: Element dof maps Me
	// Step 9: Set element dofs ae
	setElementDofMap_ae();
	// Step 10: Compute element stiness
	Calculate_ElementStiffness_Force();

	// Step 11: Assembly from local to global system
	Assemble();
	// Step 12: Solve global (free) dof a from Ka = F
	// successful solution returns true
	if (Solve_Dofs() == false)
		THROW("Matrix solve failed\n");
	// Step 13: Assign a to nodes and elements
	Assign_dof();
	// Step 14: Compute prescribed dof forces
	UpdateFpNodalPrescribedForces();

	/////////////////////////////////////////////////////////////////////////
	// output
	string outputFileName;
	outputFileName = runName + "Output.txt";
	fstream out(outputFileName.c_str(), ios::out);
	out << (*this);
}

void FEMSolver::setSizes()
{
	// Initializes dofs, K, and F by setting the sizes and all values to 0
	int n = ndofpn*nNodes;
	int nf = n - np;
	K.resize(np,nf);
	K=0.0;
	dofs.resize(nf);
	dofs=0.0;
	F.resize(np);
	F=0.0;
}

void FEMSolver::setPositions_F()  
{
	int posf=0;
	int posp=0;
	for (int i=0; i<nNodes; i++)
	{
		for (int j=0; j<ndof; j++)
		{	if (nodes[i].ndof[j].p==true)
			{	posp = posp-1;
				nodes[i].ndof[j].pos=posp;
			}
		 	else
				{	posf=posf+1;
				 	nodes[i].ndof[j].pos=posf;
				 	F(posn)=nodes[i].ndof[j].f;	//These are the free DOF forces...not sure if F(posn) is correct
				}
		}
	}
	
}

void FEMSolver::setElementDofMap_ae()
{
	PhyElement* pe;
	for (int e = 0; e < ne; ++e)
		pes[e]->setElementDofMap_ae(ndofpn);
}


void FEMSolver::Calculate_ElementStiffness_Force()
{
	PhyElement* pe;
	for (int e = 0; e < ne; ++e)
		pes[e]->Calculate_ElementStiffness_Force();
}

void FEMSolver::Assemble()
{
	PhyElement* pe;
	for (int e = 0; e < ne; ++e)
		pes[e]->AssembleStiffnessForce(K, F);
}

bool FEMSolver::Solve_Dofs()
{
	dofs.resize(nf);
	dofs = F;
	int isNonsingular;
	if (verbose)
	{
		// to save K for verbose output
		MATRIX Kbk;
		Kbk = K;
		isNonsingular = !LUsolve(Kbk, dofs);
	}
	else
		isNonsingular = !LUsolve(K, dofs);
	return (isNonsingular != 0);
}


void FEMSolver::Assign_dof()
{
	for(int i=0; i<nNodes; i++)
	{	for (int j=0; j<ndofpn; j++)
		{	if (nodes[i].ndof[j].p == false)
			{	nodes[i].ndof[j].v = nodes[i].ndof[j].p;  //finishes the assignment of free DOF
			}
		}
	}
	/*Still requires the Phyelement step 8 to be carried out by completing SetElementDofMap_ae() before to loop over the element map
	and complete edof by using the displacement values for the non-prescribed dofs.
	
	Pseudo Code, complete once PhyElement is completed
	for (int e=0; e<ne; e++)
	{	for (int i=0; i<element(e).nedof; i++)
		{	posn = element(e)dofMap(i)
			if (posn > 0)  //Free dof
			{
				element(e).edofs(i) = dofs(posn);
			}
		}
	}
	*/
}	

void FEMSolver::UpdateFpNodalPrescribedForces()
{
	// Complete
}
