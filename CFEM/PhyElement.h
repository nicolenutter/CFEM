#ifndef PHY_ELEMENT__H
#define PHY_ELEMENT__H

#include "CFEMTypes_Global.h"
#include "LAFuncs.h"

// One interesting thing in this file is the notation of polymorphism and subclasses.
// To better understand subclasses consider the shape example again:

// Shape is the parent class
// polygon is a subclass of shape becuase it is a shape but more specific with potentially more data members and functions (here a list of corner vertices are additional data)
// subclasses are 
//					bigger (often more functions and data)
//					more specific (subset of class)

// If we continue this square are triangle are two examples of subclasses of polygon


// Polymorphism, virtual function
// is a mechanism to define the same operation for different types of realization of a class.
// For example we can define a virtual function called computeArea for 2D shapes
// a general shape does not know how to compute circumferene
// but square (l  = 4 a) and triangle (l = a + b + c) know how to compute area and they have DIFFERENT implementation.
// If we define a shape (by pointer) and call computeArea computer recognizes which type of shape it is and calls the right function.
// polymorphism (calling the right function based on the specific of subclass) can only be achieved by CALLING A VIRTUAL FUNCTION ON A POINTER
// More details will be given in the class.

class PhyNode; // because only the pointer is used in the header file
class PhyMaterial;

class PhyElement
{
	friend ostream& operator<<(ostream& out, const PhyElement& dat);
public:
	virtual void setGeometry() = 0;
	virtual void setInternalMaterialProperties(PhyMaterial* pMat) = 0;
	void setNodeConnectivity_Sizes(int nNodeInElement, int ndofpnIn, vector<int>& eNodesIn, vector <PhyNode*>& eNodePtrsIn); 

	void print(ostream& out);
	// Step 8: Element dof maps Me
	// Step 9: Set element dofs ae
	void setElementDofMap_ae(int ndofpn);

	// Step 10: Compute element stiness/force (ke, foe (fre: source term; fNe: Neumann BC))
	virtual void Calculate_ElementStiffness_Force() = 0;

	// Step 11: Assembly from local to global system
	void AssembleStiffnessForce(MATRIX& globalK, VECTOR& globalF);

	// Step 14: Compute prescribed dof forces
	void UpdateElementForces_GlobalFp(VECTOR& Fp);

	// Step 15: Compute/output element specific data
	virtual void SpecificOutput(ostream& out) const {THROW("does not have implementation");};


	int id;
	int neNodes; // # element nodes
	vector<int> eNodes; // element node vector
	vector <PhyNode*> eNodePtrs; 
	int nedof; // # element dof
	VECTOR edofs; // element dofs
	vector<int> dofMap;
	ElementType eType;
	int matID;
	MATRIX ke; // element stiffness matrix
	VECTOR foe; // element force vector from all sources other than essential BC
	VECTOR fde;	// element essential BC force
	VECTOR fee; // all element forces
};


// Defining types (e.g. here bar, beam, truss, frame elements) and returning the right choice of the parent class by "factory" functions is often used in C++ programming.
// For example consider a function that based on shape name creates the right shape and return them all as shape (pointer) but when compute area is called on these pointers the right area calculation function is called.
// Refer to the implementation of the function for reference.
PhyElement* PhyElementFactory(ElementType eTypeIn);


#endif