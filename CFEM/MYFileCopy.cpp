#include "MYFileCopy.h"

void MYFileCopy( const char *from, const char *to )
{
const unsigned int BUFF_SZ=16000;

char            *buf;
FILE            *fi;
FILE            *fo;
unsigned        amount;
unsigned        written;
int             result;
int 			PASS = 0;
int 			FAIL = -1;

buf = new char[BUFF_SZ];

fi = fopen( from, "rb" );
fo = fopen( to, "wb" );

result = PASS;
if((fi == NULL) || (fo == NULL) )
  {
  result = FAIL;
  if (fi != NULL) fclose(fi);
  if (fo != NULL) fclose(fo);
  }

if(fi == NULL)
	std::cout<<std::endl<<"Cannot open file = "<<from<<" to read from."<<std::endl;
	
if(fo == NULL)
	std::cout<<std::endl<<"Cannot open file = "<<to<<" to write to."<<std::endl;	

if (result == PASS)
  {
  do
    {
    amount = fread( buf, sizeof(char), BUFF_SZ, fi );
    if (amount)
      {
      written = fwrite( buf, sizeof(char), amount, fo );
      if (written != amount)
        {
        result = FAIL; // Why would this happen?
        }
      }
    } // when amount read is < BUFF_SZ, copy is done
  while ((result == PASS) && (amount == BUFF_SZ));
  fclose(fi);
  fclose(fo);
  }
delete [] buf;
}
