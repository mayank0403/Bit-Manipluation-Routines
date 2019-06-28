#include <iostream>
#include <stdlib.h>
#include <cstring>

using namespace std;

// Use double precisions for floats
#define DOUBLE

// Print mantissa, sign and exponent as integers
#define PARSE_PRINT

void bit_manip_inside_byte(int bit /*0/1*/, int location, void* number){
	if(location >= 8){
		cout<<"Cannot set to farther than 8th bit"<<endl;
		return;
	}
	if(bit<0 || bit>1){
		cout<<"Bit cannot be anything other than 0/1"<<endl;
		return;
	}
	if(bit == 1){
		// setting the bit
		*((unsigned long*)number) |= 1UL << location;
	}
	else if(bit == 0){
		// clearing the bit
		*((unsigned long*)number) &= ~(1UL << location); //~ is bitwise NOT.
	}
}

//Print from LSB to MSB
void print_bits_inside_byte(void* number, bool new_line = false){
	for(int i=0; i<8; i++){
		int bit = ((*((unsigned long*)number)) >> i) & 1UL;
		cout<<bit<<" ";
	}
	if(new_line)
		cout<<" | ";
	else
		cout<<endl;
}

void bit_manip_long_type(int bit, int location, void* number);

//This will print bits of all 4 bytes at a mem location
void print_bits_long_type(void* number, bool is_float = false){
	if(is_float){
		if(__BYTE_ORDER == __LITTLE_ENDIAN){
			cout<<"Priting floats in little endian format"<<endl;
		}
		else{
			cout<<"TODO: Printing floats in big endian format"<<endl;
			return;
		}
		cout<<" | man (reverse of expected) => ";
		int i = 0;
		long long int mantissa, exponent;
		mantissa = 0ULL;
		exponent = 0ULL;
		int sign;
#ifdef DOUBLE
		for(i=0; i<52; i++){
			int bit = ((*((long long int*)number)) >> i) & 1UL;
			cout<<bit<<" ";
			bit_manip_long_type(bit, 51-i, &mantissa);
		}

#else
		for(i=0; i<23; i++){
			int bit = ((*((unsigned long*)number)) >> i) & 1UL;
			cout<<bit<<" ";
			bit_manip_long_type(bit, 22-i, &mantissa);
		}
#endif
		cout<<". 1";
		cout<<" | exp (parsed normally, no rev)=> ";
#ifdef DOUBLE
		for(; i<63; i++){
			int bit = ((*((long long int*)number)) >> i) & 1UL;
			cout<<bit<<" ";
			bit_manip_long_type(bit, i-52, &exponent);
		}

#else
		for(; i<31; i++){
			int bit = ((*((unsigned long*)number)) >> i) & 1UL;
			cout<<bit<<" ";
			bit_manip_long_type(bit, i-23, &exponent);
		}
#endif
		cout<<" | sign => ";
#ifdef DOUBLE
		for(; i<64; i++){
			int bit = ((*((long long int*)number)) >> i) & 1UL;
			cout<<bit<<" ";
			sign = -1*bit - 1*(bit-1);
		}

#else
		for(; i<32; i++){
			int bit = ((*((unsigned long*)number)) >> i) & 1UL;
			cout<<bit<<" ";
			sign = -1*bit - 1*(bit-1);

		}
#endif
		cout<<" |"<<endl;
#ifdef PARSE_PRINT
#ifdef DOUBLE
		cout<<"Sign = "<<sign<<" | Mantissa = "<<mantissa<<" | Exponent(-bias) = "<<exponent-1023<<endl;
#else
		cout<<"Sign = "<<sign<<" | Mantissa = "<<mantissa<<" | Exponent(-bias) = "<<exponent-127<<endl;
#endif
#endif
		return;
	}
	for(int i=0; i<4; i++){
		print_bits_inside_byte(number+i, true);
	}
	cout<<endl;
}

//Set/Unset nth bit (of 32/64)
void bit_manip_long_type(int bit, int location, void* number){
	int chunk_num = location/8;
	int relative_location = location%8;
	bit_manip_inside_byte(bit, relative_location, number+chunk_num);
}

//Returns int 0/1 as the bit.
int extract_bit_from_byte(int position, void* number){
	return ((*((unsigned long*)number)) >> position) & 1UL;
}

//For both double and float
int extract_bit_from_long_type(int location, void* number){
	int chunk_num = location/8;
	int relative_location = location%8;
	return extract_bit_from_byte(relative_location, number+chunk_num);
}

//Starting from LSB, copy x no. of bits from <from> to <to>.
void set_x_bits_to(int x, int start_bit_pos /*bit offset to start from in <to>*/, void* from, void* to){
	for(int i=start_bit_pos; i<(x+start_bit_pos); i++){
		bit_manip_long_type(extract_bit_from_long_type(i-start_bit_pos, from), i, to);
	}
}

void set_x_bits_to_little_endian_float(int x, void* from, void* to){
	for(int i=x-1; i>=0; i--){
		bit_manip_long_type(extract_bit_from_long_type(x-1-i, from), i, to);
	}
}

//Generate a float value from integer representations of mantissa and exponent.
void gen_float_from_man_exp(int sign /*1 -ve*/, long long int exponent /*<2pow8-- actual exponent = exponent-127*/, long long int mantissa /*<2pow23*/, void* number){
	if(__BYTE_ORDER == __BIG_ENDIAN){
		//cout<<"BIG ENDIAN"<<endl;
#ifdef DOUBLE
		bit_manip_long_type(sign, 0, number); //Set sign.
		set_x_bits_to(11, 1, &exponent, number);
		set_x_bits_to(52, 12, &mantissa, number);

#else
		bit_manip_long_type(sign, 0, number); //Set sign.
		set_x_bits_to(8, 1, &exponent, number);
		set_x_bits_to(23, 9, &mantissa, number);
#endif
	}
	else if(__BYTE_ORDER == LITTLE_ENDIAN){
		//cout<<"LITTLE ENDIAN"<<endl;
#ifdef DOUBLE
		set_x_bits_to_little_endian_float(52, &mantissa, number); //parsed in little endian form.
		set_x_bits_to(11, 52, &exponent, number);
		bit_manip_long_type(sign, 63, number); //Set sign.

#else
		set_x_bits_to_little_endian_float(23, &mantissa, number); //parsed in little endian form.
		set_x_bits_to(8, 23, &exponent, number);
		bit_manip_long_type(sign, 31, number); //Set sign.
#endif
	}
}

//Works for both float and double
void* gen_random_valid_float(){
	long long int random_sign, random_exponent, random_mantissa;
	random_sign = rand()%2; //1 bit
#ifdef DOUBLE
	random_exponent = rand()%(2*1024); //11 bits
	while(random_exponent == 0 || random_exponent == (2*1024 - 1)){
		//if 0, then either 0 or denormal/subnormal number
		//if 2*1024-1, then chances are that it is NaN or inf
		random_exponent = rand()%(2*1024); //8 bits
	}
	long long int modulus = (1024*1024*1024);
	modulus *= 1024;
	modulus *= 1024*4;
	random_mantissa = rand()%(modulus); //52 bits
	void* rand_double = malloc(8);
	gen_float_from_man_exp(random_sign, random_exponent, random_mantissa, rand_double);
	return rand_double;

#else
	random_exponent = rand()%(256); //8 bits
	while(random_exponent == 0 || random_exponent == 255){
		//if 0, then either 0 or denormal/subnormal number
		//if 255, then chances are that it is NaN or inf
		random_exponent = rand()%(256); //8 bits
	}
	random_mantissa = rand()%(1024*1024*8); //23 bits
	void* rand_float = malloc(4);
	gen_float_from_man_exp(random_sign, random_exponent, random_mantissa, rand_float);
	return rand_float;
#endif
}

float get_random_float(){
	float value = 0.0;
	void* rand_val = gen_random_valid_float();
	memcpy(&value, rand_val, 4);
	return value;
}

double get_random_double(){
	double value = 0.0;
	void* rand_val = gen_random_valid_float();
	memcpy(&value, rand_val, 8);
	return value;
}

//This will compare floats/doubles bit by bit
//Return 0 if values match, 1 if one is denormal
//2 if one is INF, 3 if one is NaN and -1 if values don't match.
int compare_floats(void* a, void* b){
	int i = 0;
	long long int mantissa1, exponent1, mantissa2, exponent2;
	mantissa1 = 0ULL;
	exponent1 = 0ULL;
	mantissa2 = 0ULL;
	exponent2 = 0ULL;
	int sign1, sign2;
#ifdef DOUBLE
	for(i=0; i<52; i++){
		int bit1 = ((*((long long int*)a)) >> i) & 1UL;
		bit_manip_long_type(bit1, 51-i, &mantissa1);
		int bit2 = ((*((long long int*)b)) >> i) & 1UL;
		bit_manip_long_type(bit2, 51-i, &mantissa2);
	}

#else
	for(i=0; i<23; i++){
		int bit1 = ((*((unsigned long*)a)) >> i) & 1UL;
		bit_manip_long_type(bit1, 22-i, &mantissa1);
		int bit2 = ((*((unsigned long*)b)) >> i) & 1UL;
		bit_manip_long_type(bit2, 22-i, &mantissa2);
	}
#endif
#ifdef DOUBLE
	for(; i<63; i++){
		int bit1 = ((*((long long int*)a)) >> i) & 1UL;
		bit_manip_long_type(bit1, i-52, &exponent1);
		int bit2 = ((*((long long int*)b)) >> i) & 1UL;
		bit_manip_long_type(bit2, i-52, &exponent2);
	}

#else
	for(; i<31; i++){
		int bit1 = ((*((unsigned long*)a)) >> i) & 1UL;
		bit_manip_long_type(bit1, i-23, &exponent1);
		int bit2 = ((*((unsigned long*)b)) >> i) & 1UL;
		bit_manip_long_type(bit2, i-23, &exponent2);
	}
#endif
#ifdef DOUBLE
	for(; i<64; i++){
		int bit1 = ((*((long long int*)a)) >> i) & 1UL;
		sign1 = -1*bit1 - 1*(bit1-1);
		int bit2 = ((*((long long int*)b)) >> i) & 1UL;
		sign2 = -1*bit2 - 1*(bit2-1);
	}

#else
	for(; i<32; i++){
		int bit1 = ((*((unsigned long*)a)) >> i) & 1UL;
		sign1 = -1*bit1 - 1*(bit1-1);
		int bit2 = ((*((unsigned long*)b)) >> i) & 1UL;
		sign2 = -1*bit2 - 1*(bit2-1);
	}
#endif
	if(sign1==sign2 && mantissa1==mantissa2 && exponent1==exponent2){
		return 0;
	}
	else if(exponent1==0 && exponent2==0){
		return 1; //One of them is a subnormal number.
	}
#ifdef DOUBLE
	else if(exponent1==2047 && exponent2==2047){
		if(mantissa1==0 || mantissa2==0){
			return 2; //One of them in INF.
		}
		else{
			return 3; //One of them is a NaN.
		}
	}
#else
	else if(exponent1==255 && exponent2==255){
		if(mantissa1==0 || mantissa2==0){
			return 2; //One of them in INF.
		}
		else{
			return 3; //One of them is a NaN.
		}
	}
#endif
	//Else the numbers are not equal and valid.
	return -1;
}


