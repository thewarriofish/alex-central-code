/* Alex Yu */
/* MIPS Machine Lab */
/* TAs: Tanner Fry and Shivam Patel*/
/* Completed 3/13/2021 */
//Ün Ün Ün https://www.youtube.com/watch?v=TnFR3ohg26M

#include <iostream>
#include <sstream>
#include <cstdio>
#include <string>
using namespace std;

//array of register names in MIPS
const string reg_names[] = {
	    "$zero",
		"$at",
		"$v0",
		"$v1",
		"$a0",
		"$a1",
		"$a2",
		"$a3",
		"$t0",
		"$t1",
		"$t2",
		"$t3",
		"$t4",
		"$t5",
		"$t6",
		"$t7",
		"$s0",
		"$s1",
		"$s2",
		"$s3",
		"$s4",
		"$s5",
		"$s6",
		"$s7",
		"$t8",
		"$t9",
		"$k0",
		"$k1",
		"$gp",
		"$sp",
		"$fp",
		"$ra"
};

//struct containing the Machine; opcodes and subopcodes contain supported instructions
struct Machine {
	unsigned int regs[32] = {0};
	unsigned int opcodes[3] = {4,5,8};
	unsigned int subopcodes[7] = {0,2,3,32,36,37,42};
	unsigned int pc = 0;
	unsigned int num_instructions = 0;
	unsigned int *instructions;
};

//function to calculate opcode
int opcode(Machine m, int step)
{
	int instruct = m.instructions[step];
	bool flag = false; //denotes if opcode is supported
	unsigned int opcode = instruct >> 26; //opcode is always bits 31-26
	opcode = opcode & (0x3F); //mask to ensure it is only the opcode
	for(unsigned int i = 0; i < (sizeof(m.opcodes)/4); ++i) //loop to check if opcode is in supported instructions
	{
		if(opcode == m.opcodes[i])
		{
			flag = true;
		}
	}
	if(opcode == 0) //loop to check if subopcode is in supported instructions
	{
		unsigned int subopcode = instruct & (0x3F);
		for(unsigned int i = 0; i < (sizeof(m.subopcodes)/4); ++i)
		{
			if(subopcode == m.subopcodes[i])
			{
				flag = true;
			}
		}
	}
	if(flag)
	{
		return opcode;
	}
	else
	{
		return -1; //"null" value for when opcode isn't supported
	}
}

//function to find destination register
int findDestination(Machine m, int step)
{
	int op = opcode(m, step);
	int dest;
	if(op == 8) //finds destination register for addi
	{
		unsigned int rt = m.instructions[step] >> 16;
		dest = rt & (0x1F);
	}
	else //finds destination register for other instructions
	{
		unsigned int rd = m.instructions[step] >> 11;
		dest = rd & (0x1F);
	}
	return dest;
}

//function for processing MIPS instructions
int next(Machine m, int step)
{
	int instruct = m.instructions[step];
	unsigned int op = opcode(m, step);
	if(op == 0) //checks to see if subopcode needs to be used
	{
		unsigned int subop = instruct & (0x3F);
		if(subop == 32) //add
		{
			unsigned int rs = instruct >> 21; //finds left register
			rs = rs & (0x1F); //masks to ensure output
			unsigned int rt = instruct >> 16; //finds right register
			rt = rt & (0x1F); //masks to ensure output
			return m.regs[rs] + m.regs[rt]; //returns sum of values located at registers rs and rt
		}
		else if(subop == 36) //and
		{
			unsigned int rs = instruct >> 21; //finds left register
			rs = rs & (0x1F); 
			unsigned int rt = instruct >> 16; //finds right register
			rt = rt & (0x1F);				    
			return m.regs[rs] & m.regs[rt]; //returns output of bitwise and of values located at registers rs and rt
		}
		else if(subop == 37) //or
		{
			unsigned int rs = instruct >> 21; //finds left register
			rs = rs & (0x1F);
			unsigned int rt = instruct >> 16; //finds right register
			rt = rt & (0x1F);
			return m.regs[rs] | m.regs[rt]; //returns output of bitwise or of values located at registers rs and rt
		}
		else if(subop == 0) //sll
		{
			unsigned int rt = instruct >> 16; //finds source register
			rt = rt & (0x1F);
			unsigned int sa = instruct >> 6; //finds amount to shift
			sa = sa & (0x1F);
			return m.regs[rt] << sa; //returns value at register rt after being shifted sa amount of times
		}
		else if(subop == 42) //slt
		{
			unsigned int rs = instruct >> 21; //finds left register
			rs = rs & (0x1F);
			unsigned int rt = instruct >> 16; //finds right register
			rt = rt & (0x1F);
			if(m.regs[rs] < m.regs[rt]) //checks if value at rs is less than value at rt
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else if(subop == 3) //sra
		{
			int rt = instruct >> 16; //finds source register
			rt = rt & (0x1F);
			unsigned int sa = instruct >> 6; //finds shift amount
			sa = sa & (0x1F);
			if((int) (m.regs[rt]) < 0) //if the value at rt is negative
			{
				m.regs[rt] = m.regs[rt] >> sa; //shifts value at rt right sa amount of times
				return m.regs[rt] | (-1 << (32 - sa)); //returns value at rt after added zeros are replaced with ones
			}
			else
			{
				m.regs[rt] = m.regs[rt] >> sa; //shifts value at rt right sa amount of times
				return m.regs[rt]; //returns value at rt
			}
		}
		else if(subop == 2) //srl
		{
			unsigned int rt = instruct >> 16; //finds source register
			rt = rt & (0x1F);
			unsigned int sa = instruct >> 6; //finds shift amount
			sa = sa & (0x1F);
			return m.regs[rt] >> sa; //returns value at rt after being shifted right sa amount of times
		}
	}
	else if(op == 8) //addi
	{
		unsigned int rs = instruct >> 21; //finds source register
		rs = rs & (0x1F);
		short immediate = instruct & (0xFFFF); //finds immediate
		return m.regs[rs] + immediate; //returns sum of value located at register rs and the immediate
	}
	else if(op == 4) //beq
	{
		unsigned int rs = instruct >> 21; // finds right register
		rs = rs & (0x1F);
		unsigned int rt = instruct >> 16; //finds left register
		rt = rt & (0x1F);
		if(m.regs[rs] == m.regs[rt]) //checks if values at registers rs and rt are equal
		{
			short offset = instruct & (0xFFFF); //finds offset
			offset = offset << 2; //shifts offset left twice
			return offset;
		}
		else
		{
			return 0;
		}
	}
	else if(op == 5) //bne
	{
		unsigned int rs = instruct >> 21; //finds right register
		rs = rs & (0x1F);
		unsigned int rt = instruct >> 16; //finds left register
		rt = rt & (0x1F);
		if(m.regs[rs] != m.regs[rt]) //checks if values located at rs and rt are not equal 
		{
			short offset = instruct & (0xFFFF); //finds offset
			offset = offset << 2; //shifts offset left twice
			return offset;
		}
		else
		{
			return 0;
		}
	}
	return 0;
}

int main()
{
	Machine mach;
	string input, etc = "";
	stringstream ss;
	while(true)
	{
		cout << "> ";
		getline(cin, input);
		ss.str(input);
		ss >> input >> etc;
		if(mach.num_instructions == 0 && (input != "load" && input != "quit") && (input != "l" && input != "q")) //checks for loaded machine
		{
			printf("No machine loaded. \n");
		}
		else if (input == "regs") //code to print all registers
				{
				for(int i = 0; i < 32; ++i)
				{
					if(i % 4 == 0) //creates newline after 4 registers are printed
					{
						printf("\n");
					}
					printf("%-5s: 0x%08x (%5i) ", reg_names[i].c_str(), mach.regs[i], mach.regs[i]);
				}
				printf("\nPC   : %i \n", mach.pc); //prints current program counter
			}
		else if (input == "reg") //code to print one register
		{
				if(etc == "") //notice if reg is used without enough arguments
				{
					printf("Usage: reg <register> \n");
				}
				else
				{
					int num = -1;
					for(int i = 0; i < 32; ++i) //loop to check reg_names for correct register
					{
						if(etc == reg_names[i])
						{
							num = i;
							break;
						}
					}
					if(num == -1)
					{
						num = stoi(etc); //if a number was used in command line, assigns to num
					}
					printf("%-5s: 0x%08x (%5i) \n", reg_names[num].c_str(), mach.regs[num], mach.regs[num]);
				}
		}
		else if(input == "next" || input == "n") //code to process 1 step of .bin file
		{
				int dest = findDestination(mach, (mach.pc/4)); //finds destination register
				if((mach.pc/4) >= mach.num_instructions) //notice if at end of file
				{
					printf("No more instructions left. \n");
				}
				else
				{
					int op = opcode(mach, (mach.pc/4)); //finds opcode
					if(op == -1) //prints if opcode is not supported
					{
						printf("Unrecognized instruction. Skipping... \n");	
					}
					else if(op == 4 || op == 5) //adjusts program counter if instruction is a branch
					{
						mach.pc += (next(mach, (mach.pc/4)));
					}
					else
					{
						if(dest == 0) //if destination register is $zero, ensures it stays zero
						{
							mach.regs[0] = 0;
						}
						else //sets destination register to new value
						{
							mach.regs[dest] = next(mach, (mach.pc/4));
						}
					}	
					mach.pc += 4; //adds to program counter with offset
				}
		}
		else if(input == "run") //code to run all instructions in .bin file
		{
				while(true) //the code found under 'next' except in a while loop
				{
					int dest = findDestination(mach, (mach.pc/4));
					if((mach.pc/4) >= mach.num_instructions) //breaks out of loop when all instructions are processed
					{
						break;
					}
					else
					{
						int op = opcode(mach, (mach.pc/4));
						if(op == 4 || op == 5)
						{
							mach.pc += (next(mach, (mach.pc/4)));
						}
						else
						{
							if(dest == 0)
							{
								mach.regs[0] = 0;
							}
							else
							{
								mach.regs[dest] = next(mach, (mach.pc/4));
							}
						}	
					}
					mach.pc += 4;
				}
		} 
		else if(input == "load" || input == "l") //code to load in new .bin file
		{
				if(etc == "") //error if command line doesn't have enough arguments
				{
					printf("Usage: load <filename> \n");
				}
				else
				{
					const string& tmp = etc;
					const char* file = tmp.c_str(); //converts etc (a string) to const char*, the only type accepted by fopen
					FILE *fl;
					fl = fopen(file, "rb");
					if(nullptr == fl) //error message if file could not be opened
					{
						printf("Could not open file.\n");
					}
					else
					{
						mach.pc = 0; //resets program counter for new file
						for(int i = 0; i < 32; ++i) //resets all registers to zero
						{
							mach.regs[i] = 0;
						}
						delete[] mach.instructions; //removes previous set of instructions
						fseek(fl, 0, SEEK_END); //places location in file to the end
						mach.num_instructions = (ftell(fl) + 1) / 4; //finds the number of instructions; adding 1 to account for 0 and dividing by 4 to account for size of int
						fseek(fl, 0, SEEK_SET); //places location in file to the beginning
						mach.instructions = new unsigned int[mach.num_instructions]; //allocates enough memory for all instructions
						for(unsigned int i = 0; i < mach.num_instructions; ++i)
						{
							fread(&mach.instructions[i], 1, sizeof(unsigned int), fl); //uses a loop to place all instructions into pointer
						}
						fclose(fl); //closes file
					}
				}
		}
		else if(input == "quit" || input == "q")
		{
				delete[] mach.instructions; //deletes any existing instructions
				break;
		}
		else //prints if command is unsupported
		{
				printf("Unrecognized command. \n");
		}
		etc = ""; //reset etc for next loop
		ss.clear(); //clears the stream
	}
	return 0;
}

