#include<stdio.h>
#include<limits.h>
#include "lfrac_helper.h"
#include "ln_helper.h"
#include "ln_arithmetic.h"
int main(int argc, char** argv)
{

	ln a=ln_init_str("432232554655");
	ln b=ln_init_str("55");
	lfrac c=lfrac_creat(a,b,copy);
	lfrac_output(c);
	c=lfrac_simplify(c,firstln);	
	lfrac_output(c);
	return 0;
}
