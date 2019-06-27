#include "bitmance.h"

int main(){
	void* float_mem = malloc(4); //4 bytes -- size of float
	void* double_mem = malloc(8); //8 bytes -- size of double
	//print_bits_long_type(float_mem);
	//bit_manip_inside_byte(1, 0, float_mem);
	//bit_manip_inside_byte(1, 1, float_mem);
	//bit_manip_long_type(1, 31, float_mem);
	//print_bits_long_type(float_mem);
	//cout<<*((int*)float_mem)<<endl;
	//gen_float_from_man_exp(1, 127, 3, float_mem); //127 bias
	//gen_float_from_man_exp(1, 1023, 3, double_mem); //1023 bias
	//double a = 1.5;
	//print_bits_long_type(&a, true);
	//cout<<*((float*)float_mem)<<endl;
	//cout<<*((double*)double_mem)<<endl;
	//print_bits_long_type(float_mem, true);
	//print_bits_long_type(double_mem, true);
	//for(int i=0; i<100; i++)
		//cout<<get_random_float()<<endl;
	for(int i=0; i<100; i++)
		cout<<get_random_double()<<endl;
}
