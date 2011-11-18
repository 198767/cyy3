#include<stdio.h>
#include<limits.h>
#include "lfrac_helper.h"
#include "ln_helper.h"
#include "ln_arithmetic.h"
int main(int argc, char** argv)
{

	ln a=ln_init_str("943.75843534");
	ln b=ln_init_str("312348372");
	lfrac c=lfrac_creat(a,b,copy);
	lfrac_output(c);
	c=lfrac_simplify(c,firstln);	
	lfrac_output(c);
	//	ln c=ln_gcd(a,b);
	//	ln c=ln_divide(a,b,0,trunc_res,newln);
	return 0;
}
