// begininning of all header files we use the following "macros" it says if CFEM_TYPES__H is not already defined define it and then the rest of the file gets included.
// #endif at the end of the file is the end of this #ifnded. The next time another place in the code tries to include this file CFEM_TYPES__H is already defined and the if check fails.
// so the rest of the file is not read another time avoiding having several declaration of the same function.
// macros are used in various forms for activating machine specific parts or debug statement (e.g. DB below) to be compiled or not, including the files (#include below), macros in general sense (shorthand for long statement like EXIT below), ...
#ifndef CFEM_TYPES__H
#define CFEM_TYPES__H

#include <iostream> /* for input and output using >> and << operator */
#include <fstream>  /* file streams for input and output */
#include <assert.h>     /* assert */
// for C++ built-in template vector class. Template means we can change the type of components of the vector. For example we can have a vector of integer, double, etc.
#include <vector>  

// for map data structure. For data corresponding to ids (material ids, geometry ids, ...) they may better be stored in maps rather than vectors
#include <map>	 

// many classes and functions are within "namespaces". This is similar to having streets within a city. Ney York::Broadway Street; Chicago::Broadway, etc.
// To point to these specific functions, classes, etc. we need to use their containing namespace followed by "resolution operator" and finally the function or class 
// for example to print on screen we write std::cout because cout belongs to std namespace
// If we write "using namespace X" even if we do not write std:: C++ can find cout because we have already requested to look within that namespace (similar to giving addresses within a city)
// std is common namespace for many C++ build-in functions and classes
#include <string>  // for string class (words)
#include <cmath>   // for certain math operators (fabs, sqrt, ...)


using namespace std;	

//#define USE_BLITZ	1



#ifndef EXIT
#define EXIT exit(1);getchar();getchar();
#endif

// to exit the code with useful information when something is wrong. It provides file and line number with a message if a check is incorrect.
// example
//					if (denominator == 0) THROW("cannot divide be zero!\n");
#ifndef THROW
#define THROW(msg){{char tmpstr[255];sprintf(tmpstr, "In %s, line %d : %s \n", __FILE__, __LINE__, msg);	cerr<<tmpstr;getchar(); getchar(); throw(tmpstr); }}
#endif


//You can put debug statements in DB for example
// DB( cout << "the value of the matrix is a\n" << a << '\n';)
// if DEBUG_MODE is 1 the value of a is printed through the above statement, otherwise it won't.
// see double& MATRIX::operator()(unsigned int i, unsigned int j  for example
#define DEBUG_MODE	1
#if DEBUG_MODE
#define DB(x) x
#else
#define DB(x) 
#endif

// Other examples of macro for Min Max operators
#ifndef Min
#define Min(a,b)    ((a) < (b) ? (a) : (b))
#endif

#ifndef Max
#define Max(a,b)    ((a) > (b) ? (a) : (b))
#endif


// This is the list of element we want to support. Instead of using simple integers such as 1 for bar 2 for beam 3 for truss and 4 for frame
// we commonly use enumerations.
// The benefits are
//		The code is more readable
//		It's much easier to find a certain option (e.g. etBar) in the code and for example if needed replace it with something else compared to finding and replacing integers that are prevalent
typedef enum {etDefault, etBar, etBeam, etTruss, etFrame} ElementType;

// The following is a common C++ practice that we create another name for a build-in C++ type (or user specified class) for better readability.
// egain something like ID better suggests we are referring to an id 
// it also makes changing this type (e.g. from long to long long) much easier as the change only needs to be done here.
typedef long ID;

typedef enum {mpb_E, mpb_A, mpb_I} mP_barTruss;




#endif