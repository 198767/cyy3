#include <stdio.h>
#include <limits.h>
#include "ln_helper.h"
int main()
{
	int i,sum;
	ln num=ln_init(1);
	for(i=2;i<=100;i++)
		ln_multiply_int(num,i,firstln);
	ln_output(num);
	sum=get_digitsum(num);
	printf("%d",sum);
	free_largenum(num);
	return 0;
}
