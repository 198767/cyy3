#include<stdio.h>
#include<limits.h>
#include "lfrac_helper.h"
#include "ln_helper.h"
#include "ln_arithmetic.h"
int main(int argc, char** argv)
{
	ln a=ln_init_str("3");
	ln b=ln_init_str("2");
	ln c;
	int i;
	for(i=0;i<1000;i++)
	{
		ln_add(b,a,firstln);
		c=b;
		b=a;
		a=c;
	}
	
	c=ln_divide(a,b,100,trunc_res,newln);
	ln_output(c);
	//	ln c=ln_gcd(a,b);
	//	ln c=ln_divide(a,b,0,trunc_res,newln);
	return 0;
}
