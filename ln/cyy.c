#include<stdio.h>
#include<limits.h>
#include "ln_helper.h"
#include "ln_arithmetic.h"
#include "lfrac_helper.h"
int main(int argc, char** argv)
{
	ln a=ln_init_str("-2");
	ln b=ln_init_str("-1");
	lfrac c=lfrac_creat(a,b,copy);
	lfrac_output(c);
	lfrac d=lfrac_copy(NULL,c);
	lfrac_output(d);
	return -1;
}
