#include<stdio.h>
#include<limits.h>
#include "lfrac.h"
#include "ln_helper.h"
#include "ln_arithmetic.h"
int main(int argc, char** argv)
{

	ln a=ln_init(INT_MIN);
	ln b=ln_init(INT_MAX);
	lfrac c=lfrac_creat(a,b,copy);
	lfrac_output(c);
	return 0;
}
