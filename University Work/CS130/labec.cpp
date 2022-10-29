//float_lab.cpp
//COSC130
//Alex Yu
//2/12/2021
//https://www.youtube.com/watch?v=YcAHHKY3Y0I

#include <cstdio>
struct Real
{
//sign bit
	int sign;
//UNBIASED exponent
	long exponent;
//Fraction including implied 1. at bit index 23
	unsigned long fraction;
};

Real Decode(int float_value);
int Encode(Real real_value);
Real Normalize(Real value);
Real Multiply(Real left, Real right);
Real Add(Real left, Real right);

int main(int argc, char *argv[])
{
	int left, right;
	char op;
	int value; Real rLeft, rRight, result; 
	if (argc < 4) 
	{
		printf("Usage: %s <left> <op> <right>\n", argv[0]);
		return -1;
	} 
	sscanf(argv[1], "%f", (float *)&left);
	sscanf(argv[2], "%c", &op);
	sscanf(argv[3], "%f", (float *)&right);
	rLeft = Decode(left);
	rRight = Decode(right);
	if (op == 'x')
	{
		result = Multiply(rLeft, rRight);
	}
	else if (op == '+') 
	{
		result = Add(rLeft, rRight);
	}
	else 
	{
		printf("Unknown operator '%c'\n", op);
		return -2;
	}
	value = Encode(result);
	printf("%.3f %c %.3f = %.3f (0x%08x)\n", *((float*)&left), op, *((float*)&right), *((float*)&value), value);
 	return 0;
}
//Implement the following functions

//function to decode
Real Decode(int float_value)
{
	Real decoded;
	decoded.sign = (float_value >> 31); //moves sign bit to least signifcant position and assigns it to decoded
	decoded.exponent = ((float_value >> 23) & 0xFF) - 127; //acquires exponent  using a mask and BIAS
	decoded.fraction = (float_value & 0x007FFFFF); //uses a mask to acquire the fraction
	decoded.fraction |= (1 << 23); //sets the 23rd bit in fraction (implied 1)
	return decoded;
}
//function to encode
int Encode(Real real_value)
{
	int encoded = 0;	
	int sign = real_value.sign << 31; //creates a variable to position sign bit
	int exponent = (real_value.exponent + 127) << 23; //creates a variable to position exponent bits
	if(exponent < 0) //sets exponent to zero if it is less than zero
	{
		exponent = 0;
	}
	int fraction = real_value.fraction; //creates a variable to position fraction bits
	fraction &= ~(1 << 23); //clears the 23rd bit (implied 1)
	encoded |= sign; //uses logical OR to put sign into encoded
	encoded |= exponent; //uses logical OR to put exponent into encoded
	encoded |= fraction; //uses logical OR to put fraction into encoded
	return encoded;
}
//function to normalize
Real Normalize(Real value)
{
	long normal = (value.fraction >> 24) & 0xFF; //assigns a variable with the first 8 bits after the 23rd bit
	bool twentythree = (value.fraction & (1 << 23)); //tests the 23rd bit
	Real normalized;
	normalized.sign = value.sign; //sets normalized parameters to value parameters
	normalized.exponent = value.exponent; 
	normalized.fraction = value.fraction;
	if(normal == 0) //if normal is zero, returns original fraction given in function parameter
	{
		normalized.fraction = value.fraction;
		return normalized;
	}
	if(normal != 0) //if normal isn't zero, performs loop until normal is zero
	{
		while(normal != 0)
		{
			normalized.fraction = normalized.fraction >> 1; //moves bits of normalized fraction to the right once
			normal = normal >> 1; //moves bit of normal to the right once
			++normalized.exponent; //increases exponent by one
		}
		twentythree = (normalized.fraction & (1 << 23)); //tests 23rd bit to make sure it is 1
	}
	if(!twentythree && normal == 0) //if normal is zero but 23rd bit isn't 1, performs loop until 23rd bit is 1
	{
		while(!twentythree)
		{
			normalized.fraction = normalized.fraction << 1; //moves bits of normalized left once
			twentythree = (normalized.fraction & (1 << 23)); //tests 23rd bit
			--normalized.exponent; //removes one from exponent
		}
	}
	return normalized;
}
Real Multiply(Real left, Real right)
{
	Real product;
	if(left.sign == -1 && right.sign == -1) //checks for sign
	{
		product.sign = 0;
	}
	else if(left.sign == -1 || right.sign == -1) //checks for sign
	{
		product.sign = -1;
	}
	else //checks for sign
	{
		product.sign = 0;
	}
	product.exponent = left.exponent + right.exponent; //adds exponents of parameters
	product.fraction = left.fraction * right.fraction; //multiplies fractions of parameters
	product.fraction = product.fraction >> 23; //moves product fraction left 23 times, since bits from both parameters are combined
	product = Normalize(product); //normalizes product
	return product;
}
Real Add(Real left, Real right)
{
	Real sum;
	if(left.exponent < right.exponent) //checks if exponents are different, left version
	{
		int difference = right.exponent - left.exponent; //finds difference between exponents
		left.exponent += difference; //adds difference to lower exponent
		left.fraction = left.fraction >> (difference); //shifts fraction right a number of times equal to difference
	}
	if(left.exponent > right.exponent) //checks if exponents are different, right edition
	{
		int difference = left.exponent - right.exponent; //finds difference between exponents
		right.exponent += difference; //adds difference to lower exponent
		right.fraction = right.fraction >> (difference); //shifts fraction right a number of times equal to difference
	}
	if(left.sign == -1 && right.sign == -1) //checks signs of parameters; if both negative, sum is negative
	{
		sum.sign = -1;
	}
	else //other situations regarding sign
	{
		if(left.fraction > right.fraction) //if abs(left) is larger than abs(right), sign of sum is sign of left
		{
			sum.sign = left.sign;
		}
		if(right.fraction > left.fraction) //if abs(right) is larger than abs(left), sign of sum is sign of right
		{
			sum.sign = right.sign;
		}	
		if(left.sign == -1) //if left is negative, performs two's complement
		{
			left.fraction = ((~left.fraction) + 1);
		}
		if(right.sign == -1) //if right is negative, performs two's complement
		{
			right.fraction = ((~right.fraction) + 1);
		}
	}
	sum.fraction = left.fraction + right.fraction; //adds parameter fractions
	sum.exponent = left.exponent; //sets exponent of sum to exponent of left(which at this point should equal exponent of right) as a precaution
	sum = Normalize(sum); //normalizes sum
	return sum;
}
