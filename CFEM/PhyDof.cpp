#include "PhyDof.h"

PhyDof::PhyDof()
{
	p = false;
	pos = -1;	// so the code definitely crashes if it goes unassigned
	v = 0.0;
	f = 0.0;	// this is the one that by default is zero
}
