//Alexander Yu - CS202
//October 1st, 2021
//Lab 5: Bit Matrices

/*This lab focuses on the implementation of functions focusing on creation, storage and manipulation of bit matrices. Manipulation involves arithemetic such as addition and multiplication, as well as inverting matrices. Storage involves storing matrices in both PGM files and hash tables (which uses separate chaining). */

#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include "bitmatrix.hpp"
using namespace std;

unsigned int djb_hash(const string &s) //hash function used for hash table
{
  size_t i;
  unsigned int h;
  
  h = 5381;

  for (i = 0; i < s.size(); i++) {
    h = (h << 5) + h + s[i];
  }
  return h;
}

Bitmatrix::Bitmatrix(int rows, int cols) //constructor for bitmatrix that take twos parameters
{
//checks if rows and columns are valid
  if(rows < 1)
  {
	 throw((string) "Bad rows");
  }
  if(cols < 1)
  {
	 throw((string) "Bad cols");
  }
//"creates" a matrix by resizing M to number of rows with default value being a string size cols filled with 0s
  string s;
  s.resize(cols, '0');
  M.resize(rows, s);
}
           
Bitmatrix::Bitmatrix(const string &fn) //constructor for bitmatrix that reads from a file
{
  ifstream file;
  string s, temp;
  size_t i, columnSize; //loop variable and size of bitmatrix without whitespace
  file.open(fn.c_str());
  if(!file.is_open()) //checks if file is openable
  {
	 throw((string) "Can't open file");
  }
  getline(file, s); //gets first line from file to check columnSize
  for(i = 0; i < s.size(); i++)
  {
	if(s[i] == '0' || s[i] == '1') //adding only '0' and '1' skips white space
	{
		temp.push_back(s[i]);
	}
  }
  columnSize = temp.size();
  M.push_back(temp);
  temp = "";
  while(file >> s) //adds bits from file into matrix
  {
	 temp += s; //adding s into temp skips any white space that may be present
	 if(temp.size() == columnSize) //using the columnSize acquired from the first line, adds a string of bits should it match the length of the first line
	 {
		 M.push_back(temp);
		 temp = "";
	 }
  }
  if(temp.size() > 0) //temp has characters if spillover has occurred, meaning that at least one row in the file did not match the size of the rest of the rows
  {
	  throw((string) "Bad file format");
  }
  file.close();
}
        
Bitmatrix *Bitmatrix::Copy() const //creates a copy of a bitmatrix
{
  Bitmatrix *copy;
  copy = new Bitmatrix(1, 1); //creates a new blank bitmatrix with one square (to prevent seg. faults)
  copy->M.resize(M.size()); //resizes the blank bitmatrix to the size of the bitmatrix to be copied
  copy->M = M; //sets the blank's M to the M of the bitmatrix to be copied
  return copy;
}
                 
bool Bitmatrix::Write(const string &fn) const //writes a bitmatrix into a file
{
  ofstream file;
  size_t i; //loop variable
  file.open(fn.c_str());
  if(!file.is_open()) //checks if file is valid
  {
	return false;
  }
  else //writes every bit into the file, with correct formatting
  {
	  for(i = 0; i < M.size(); i++)
	  {
		  file << M[i] << "\n";
	  }
	  file.close();
	  return true;
  }
  return false; //this return statement should never happen
}
 
void Bitmatrix::Print(size_t w) const //prints bitmatrix on standard output
{
  size_t i, j; //loop variables
  if(w == 0) //if w is 0, no spacing is needed between a certain number of bits, so all bits are printed
  {
	  for(i = 0; i < M.size(); i++)
	  {
		  printf("%s\n", M[i].c_str());
	  }
  }
  else
  {
	  for(i = 0; i < M.size(); i++)
	  {
		  for(j = 0; j < M[i].size(); j++)
		  {
			  printf("%c", M[i][j]);
			  if(j < (M[i].size() - 1) && ((j + 1) % w == 0)) //prints a space after w bits are printed (this condition ignores the first bit to prevent excess spaces)
			  {
				  printf(" ");
			  }
		  }
		  printf("\n");
		  if(i < (M.size() - 1) && ((i + 1) % w == 0)) //prints a newline after w rows (ignoring first row)
		  {
			  printf("\n");
		  }
	  }
  }
}
                 
bool Bitmatrix::PGM(const string &fn, int p, int border) const //stores bitmatrix in a PGM file
{
  ofstream file;
  string pixel; //stores pixel value to be used
  size_t i, j, k, l, borderWidth, borderHeight; //loop variables and border sizes
  file.open(fn.c_str());
  if(!file.is_open() || p <= 0) //checks if file and parameter are valid
  {
	  return false;
  }
  /*size of border is determined by p, border, and M.size(). It includes each bit p times.
   The border is determined by the length of the matrix + 1 because of the first edge of the PGM file.*/
  borderWidth = (M.size() * p) + (border * (M.size() + 1)); 
  borderHeight = (M[0].size() * p) + (border * (M[0].size() + 1));
  
  /*PGM Header*/
  file << "P2" << endl;
  file << borderHeight << " " << borderWidth << endl;
  file << "255" << endl;

  /*loop for printing if there is no border*/
  if(border == 0)
  {
	  file << "\n"; //formatting requires a newline after header if there is no border
	  for(i = 0; i < M.size(); i++) 
	  {  
		  for(j = 0; (int) j < p; j++)
		  {
			if(M[i][0] == '0')
			{
				pixel = "255";
			}
			else if(M[i][0] == '1')
			{
				pixel = "100";
			}
			file << pixel; //adds first pixel of the first bit into PGM, since no space should go before it
			pixel.insert(0, " "); //adds a space for the rest of the first bit
			for(k = 1; (int) k < p; k++)
			{
			  file << pixel;
			}
			for(k = 1; k < M[i].size(); k++) //adds in the rest of the row of bits p times
			{
				if(M[i][k] == '0')
				{
					pixel = " 255";
				}
				else if(M[i][k] == '1')
				{
					pixel = " 100";
				}
				for(l = 0; (int) l < p; l++)
				{
					file << pixel;
				}
			}
			file << "\n";
		  }
	  }
  }
  else //loop for printing if there is a border
  {
	  for(i = 0; (int) i < border; i++) //loop for top border
	  {
		file << "0"; //first pixel of row should have no space before it, so it is added separately
		for(j = 1; j < borderHeight; j++) //adds the rest of the border
		{
			file << " 0";
		}
		file << "\n";
      }
	  file << "\n";
	  for(i = 0; i < M.size(); i++) //loop for rest of pixels
	  {
		  for(j = 0; (int) j < p; j++)
		  {	  
			file << "0"; //first pixel of row is added separately for no space
			for(k = 1; (int) k < border; k++) //rest of border is added
			{
				file << " 0";
			}
		    for(k = 0; k < M[i].size(); k++) //pixels are added
			{
				if(M[i][k] == '0')
				{
					pixel = " 255";
				}
				if(M[i][k] == '1')
				{
					pixel = " 100";
				}
				for(l = 0; (int) l < p; l++)
				{
					file << pixel;
				}
				for(l = 0; (int) l < border; l++) //border is added
				{
					file << " 0";
				}
			}
			file << "\n";
	      }
		  for(j = 0; (int) j < border; j++) //another border is created after every p rows
		  {
			  file << "0";
			  for(k = 1; k < borderHeight; k++)
			  {
				file << " 0";
			  }
			  file << "\n";
		  }
		  file << "\n";
	  }
  }
  return true;
}

int Bitmatrix::Rows() const //returns number of rows in bitmatrix
{
  return (int) M.size();
}
                            
int Bitmatrix::Cols() const //returns number of cols in bitmatrix
{
  return (int) M[0].size();
}
                            
char Bitmatrix::Val(int row, int col) const //returns value from bitmatrix at (row, col)
{
  if(row < 0 || col < 0 || row > (int) M.size() || col > (int) M[0].size()) //checks if parameters are within bitmatrix
  {
	return 'x';
  }
  else
  {
	return M[row][col];
  }
}
            
bool Bitmatrix::Set(int row, int col, char val) //sets value in bitmatrix at (row, col) to val
{
  if(row < 0 || col < 0 || row > (int) M.size() || col > (int) M[0].size() || (val != '0' && val != '1')) //checks if parameters are within bitmatrix, and checks val for valid bit
  {
	return false;
  }
  else
  {
	  M[row][col] = val;
	  return true;
  }
}
        
bool Bitmatrix::Swap_Rows(int r1, int r2) //swaps two rows in a bitmatrix
{
  string temp; //temporary variable to store a row
  if(r1 < 0 || r2 < 0 || r1 > (int) M.size() || r2 > (int) M.size()) //checks if parameters are within bitmatrix
  {
	  return false;
  }
  else
  {
	temp = M[r1]; //stores row1 into temp
	M[r1] = M[r2]; //sets row1 to value at row2
	M[r2] = temp; //sets row2 to value in temp (which is currently row1)
	return true;
  }
}
              
bool Bitmatrix::R1_Plus_Equals_R2(int r1, int r2) //sums two rows in a bitmatrix, then replaces first row with sum
{
  size_t i; //loop variable
  string s; //sum variable
  if(r1 < 0 || r2 < 0 || r1 >= (int) M.size() || r2 >= (int) M.size()) //checks if parameters are within bitmatrix
  {
	  return false;
  }
  else
  {
    for(i = 0; i < M[r1].size(); i++) //sums rows using XOR rules
    {
	  if(M[r1][i] == M[r2][i])
	  {
		  s.push_back('0');
	  }
	  else
	  {
		  s.push_back('1');
	  }
    }
    M[r1] = s;
    return true;
  }
}
      

Bitmatrix *Sum(const Bitmatrix *a1, const Bitmatrix *a2) //creates a new bitmatrix that is the sum of two other bitmatrices
{
  Bitmatrix *sum;
  int i, j; //loop variables
  unsigned char x; //holds summed value
  if(a1->Rows() != a2->Rows() || a1->Cols() != a2->Cols()) //checks if bitmatrices are the same size
  {
	  return NULL;
  }
  sum = a1->Copy(); //copies a1 into sum
  for(i = 0; i < sum->Rows(); i++)
  {
	  for(j = 0; j < sum->Cols(); j++) //sums bits at the same location in a1 and a2 using XOR
	  {
		  x = (a1->Val(i, j)) ^ (a2->Val(i, j));
		  x |= 48; //because char is the data type used, '0' == 48 and '1' == 49, so OR is used to set correct char values
		  sum->Set(i, j, x);
	  }
  }
  return sum;
}

Bitmatrix *Product(const Bitmatrix *a1, const Bitmatrix *a2) //creates a new bitmatrix that is the product of two other bitmatrices
{
  Bitmatrix *product;
  int i, j, k; //loop variables
  unsigned char val; //hold multiplied value
  val = '0'; //sets value to 0 for later use
  if(a1->Cols() != a2->Rows()) //checks if cols of first matrix is equal to rows of second matrix
  {
	  return NULL;
  }
  product = new Bitmatrix(a1->Rows(), a2->Cols());
  for(i = 0; i < a1->Rows(); i++)
  {
	 for(j = 0; j < a2->Cols(); j++)
	 {	 
		for(k = 0; k < a2->Rows(); k++) //mutiplies values at same location using AND rules, then adds into val
		{
			val += (a1->Val(i, k) & 1) & (a2->Val(k, j) & 1); //values are ANDed with 1 to remove excess bits
		}
		val %= 2; //modulus 2 as per matrix multiplication
		val |= 48; //OR 48 to set value back to correct char
		product->Set(i, j, val);
		val = '0'; //reset val for later use
	 }
  }
  return product;
}

Bitmatrix *Sub_Matrix(const Bitmatrix *a1, const vector <int> &rows) //creates a new matrix that has specfic rows from one matrix
{
  Bitmatrix *sub;
  int i, j; //loop variables
  if(rows.size() == 0) //checks for valid parameters
  {
	  return NULL;
  }
  for(i = 0; i < (int) rows.size(); i++)
  {
	  if(rows[i] < 0 || rows[i] > a1->Rows()) //checks for valid parameters
	  {
		  return NULL;
	  }
  }
  sub = new Bitmatrix(rows.size(), a1->Cols());
  for(i = 0; i < (int) rows.size(); i++)
  {
     for(j = 0; j < a1->Cols(); j++) //creates submatrix using specified rows
	 {
		 sub->Set(i, j, a1->Val(rows[i], j));
	 }
  }
  return sub;
}

Bitmatrix *Inverse(const Bitmatrix *m) //creates a new matrix that is an inverted version of given matrix
{
  Bitmatrix *inv, *mcopy, *id;
  int i, j; //loop variables
  if(m->Rows() != m->Cols()) //checks if matrix is a square
  {
	  return NULL;
  }
  id = new Bitmatrix(m->Rows(), m->Cols()); //creates an identity matrix
  for(i = 0; i < id->Rows(); i++)
  {
	  id->Set(i, i, '1');
  }
  inv = id->Copy(); //copies identity for inversion
  mcopy = m->Copy(); //copies m for inversion
  for(i = 0; i < (inv->Rows()) - 1; i++) //creates upper-triangular matrix
  {
	  if(mcopy->Val(i, i) != '1') //if (i, i) is not 1, swaps row for a row in which (i, i) is 1
	  {
		  for(j = i + 1; j < inv->Rows(); j++)
		  {
			  if(mcopy->Val(j, i) == '1')
			  {
				  mcopy->Swap_Rows(i, j);
				  inv->Swap_Rows(i, j);
				  break;
			  }
			  if(j == (inv->Rows()) - 1) //if loops proceeds completely, there is no row where (i, i) is 1, therefore matrix is not invertible
			  {
				  return NULL;
			  }
		  }
	  }
	  for(j = i + 1; j < inv->Rows(); j++) //if row j has a 1 where it shouldn't be, replaces row j with the sum of row j and row i
	  {
		  if(mcopy->Val(j, i) == '1') 
		  {
			  mcopy->R1_Plus_Equals_R2(j, i);
			  inv->R1_Plus_Equals_R2(j, i);
		  }
	  }
  }
  for(i = (inv->Rows()) - 2; i >= 0; i--) //finishes inverted matrix
  {
	  for(j = i + 1; j < inv->Rows(); j++) //if row i has a 1 where it shouldn't be, replaces row i with the sum of row i and row j
	  {
		  if(mcopy->Val(i, j) == '1')
		  {
			  mcopy->R1_Plus_Equals_R2(i, j);
			  inv->R1_Plus_Equals_R2(i, j);
		  }
	  }
  }
  for(i = 0; i < mcopy->Rows(); i++) //compares mcopy with identity matrix; if they are not equal, inversion has failed
  {
	  for(j = 0; j < mcopy->Cols(); j++)
	  {
		  if(mcopy->Val(i, j) != id->Val(i, j))
		  {
			  return NULL;
		  }
	  }
  }
  return inv;
}



BM_Hash::BM_Hash(int size) //creates hash table with specific size
{
	if(size <= 0) //checks if size is valid
	{
		throw((string) "Bad size");
	}
	Table.resize(size);
}

bool BM_Hash::Store(const string &key, Bitmatrix *bm) //stores a bitmatrix into hash table with specific key
{
  HTE entry;
  unsigned int index; //holds position in which key should be
  size_t i; //loop variable
  if(Table.size() == 0 || key == "" || bm->Rows() < 1 || bm->Cols() < 1) //checks if hash table is initialized, key is valid, and bitmatrix is empty
  {
	  return false;
  }
  index = djb_hash(key) % Table.size(); //finds possible position using hash function
  if(Table[index].size() != 0) //if position is filled, checks if key already exists
  {
	for(i = 0; i < Table[index].size(); i++)
	{
		if(Table[index][i].key == key)
		{
			return false;
		}
	}
  }	
  entry.key = key;
  entry.bm = bm;
  Table[index].push_back(entry); //inserts key into hash table
  return true;
}
    
Bitmatrix *BM_Hash::Recall(const string &key) const //finds and returns bitmatrix corresponding to key
{
  unsigned int index; //holds possible position
  size_t i; //loop variable
  if(Table.size() == 0 || key == "") //checks if table is initialized and key is valid
  {
	  return NULL;
  }
  index = djb_hash(key) % Table.size(); //finds possible position using hash function
  if(Table[index].size() == 0) //checks if position is empty
  {
	  return NULL;
  }
  else //checks if key is in position
  {
	  for(i = 0; i < Table[index].size(); i++)
	  {
		  if(Table[index][i].key == key)
		  {
			  return Table[index][i].bm;
		  }
	  }
  }
  return NULL; //returns nothing if key is not found
}
      
vector <HTE> BM_Hash::All() const //returns all entries in order of hash table. If two keys have same position, returns in order in which they were added
{
  vector <HTE> rv;
  size_t i, j; //loop variables
  for(i = 0; i < Table.size(); i++) //adds hash table entry into return value
  {
	  if(Table[i].size() != 0)
	  {
		  for(j = 0; j < Table[i].size(); j++)
		  {
			  rv.push_back(Table[i][j]);
		  }
	  }
  }
  return rv;
}
