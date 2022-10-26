/* Alex Yu */
/* Binary Arithmetic Lab */
/* TAs: Chris Muncey and Zach McMichael */
/* Completed 2/10/2021 */
//https://www.youtube.com/watch?v=Vn_GO50AAjE
#include <cstdio>

int Multiply(int leftop, int rightop);
int Add(int leftop, int rightop);
int Sub(int leftop, int rightop);
int Twos(int op);
int Div(int leftop, int rightop);
int Mod(int leftop, int rightop);

int main(int argc, char *argv[])
{
	int left, right, result;
	char op;

	if (4 != argc)
	{
		printf("Usage: %s <left> <op> <right>\n", argv[0]);
		return -1;
	}
	sscanf(argv[1], "%d", &left);
	sscanf(argv[2], "%c", &op);
	sscanf(argv[3], "%d", &right);

	switch (op)
	{
		case 'm':
		case 'x':
		case '*':
			result = Multiply(left, right);
			break;
		case 'a':
		case '+':
			result = Add(left, right);
			break;
		case 's':
		case '-':
			result = Sub(left, right);
			break;
		case 'd':
		case '/':
			result = Div(left, right);
			break;
		case '%':
			result = Mod(left, right);
			break;
		default:
			result = -11111111;
			break;
	}
	printf("%d\n", result);
	return 0;
}

//function used to add
int Add(int leftop, int rightop)
{
	int sum = 0;
	int carry = 0;
	for(int i = 0; i < 32; ++i)
	{
		int a = (leftop >> i) & 1; //test bit for leftop at i 
		int b = (rightop >> i) & 1; //test bit for rightop at i
		int c = (carry >> i) & 1; //test bit for carry at i
		sum |= ((a^b^c) << i); //sets bit for sum at i
		carry |= ((((a & b) | (a & c)) | (b & c))<< (i + 1)); //sets bit for carry at i + 1
	}
	return sum;
}

//function for twos complement
int Twos(int op)
{
	return Add(~op, 1); //uses add function using inverted op and 1
}

//function to subtract
int Sub(int leftop, int rightop)
{
	return Add(leftop, Twos(rightop)); //uses add function and twos function
}

//function to multiply
int Multiply(int leftop, int rightop)
{
	int product = 0;
	int counter = 0; //counter to check for negatives
	if(leftop < 0) //if number is negative, uses twos then adds one to counter
	{
		leftop = Twos(leftop);
		counter = Add(counter, 1);
	}
	if(rightop < 0) //if number is negative, uses twos then adds one to counter
	{
		rightop = Twos(rightop);
		counter = Add(counter, 1);
	}
	while(rightop != 0) //iterates until rightop is zero
	{
		if((rightop & 1) == 1) //tests bit for rightop at i
		{
			product = Add(product, leftop); //uses add function to add leftop into product
		}
		leftop = (leftop << 1); //every iteration shifts leftop left once
		rightop = (rightop >> 1); //every iteration shifts rightop right once
	}
	if(counter == 1) //if only one of the two numbers is negative, uses twos function to switch product to negative
	{
		product = Twos(product);
	}
	return product;
}

//function to divide
int Div(int leftop, int rightop)
{
	int quotient = 0;
	int counter = 0; //counter to check for negatives
	long remainder; //long variables needed for restoring division
	long divisor;
	if(leftop < 0) //if number is negative, uses twos, sets remainder then adds one to counter
	{
		remainder = Twos(leftop);
		counter++;
	}
	else //sets remainder to leftop
	{
		remainder = leftop;
	}
	if(rightop < 0) //if number is negative, uses twos, sets divisor then adds one to counter 
	{
		divisor = Twos(rightop);
		divisor = (divisor << 32); //need to shift divisor left 32 times for restoring division to work
		counter++;
	}
	else //sets divisor to rightop
	{
		divisor = rightop;
		divisor = (divisor << 32);
	}
	for(int i = 31; i > -1; --i) //restoring division formula
	{
		remainder = (remainder << 1) - divisor;
		if(remainder >= 0)
		{
			quotient |= (1 << i);
		}
		else
		{
			remainder = remainder + divisor;
		}
	}
	if(counter == 1) //if one of the two numbers was negative, switches sign of quotient using twos
	{
		quotient = Twos(quotient);
	}
	return quotient;
}

//function for modulus
int Mod(int leftop, int rightop)
{
	int modulus;
	int quotient = Div(leftop, rightop); //divides numbers first
	int product = Multiply(rightop, quotient); //mulitplies quotient by the denominator
	modulus = Sub(leftop, product); //subtracts product from numerator
	return modulus;
}
