#include "bitmance.h"

int main(){
	void* float_mem = malloc(4); //4 bytes -- size of float
	//print_bits_long_type(float_mem);
	//bit_manip_inside_byte(1, 0, float_mem);
	//bit_manip_inside_byte(1, 1, float_mem);
	//bit_manip_long_type(1, 31, float_mem);
	//print_bits_long_type(float_mem);
	//cout<<*((int*)float_mem)<<endl;
	gen_float_from_man_exp(0, 127, 3, float_mem);
	cout<<*((float*)float_mem)<<endl;
	print_bits_long_type(float_mem, true);
}
