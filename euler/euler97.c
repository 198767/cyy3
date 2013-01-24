#include <stdio.h>
#include "ln.h"

int main()
{
	int i,b;
	ln sum=ln_init(1);
	ln k=NULL;
	ln temp=NULL;
	ln j=ln_init(1);
	i=2;
	b=7830457;
	j->lowestdigit->digit=i;
	j->highestdigit=j->lowestdigit;
	k=ln_init(i);
	b--;	
	while(1)
	{
		temp=copy_largenums(temp,j);
		multiply_largenums(j,temp,firstnumber);
		if(get_digitnum(j)>10)
			ln_truncate(j,3);
		b/=2;
		if(b==1)
			break;
		if(b%2!=0)
		{
			multiply_largenums(k,j,firstnumber);
			b--;
		}
	}
	free_largenum(temp);
	multiply_largenums(k,j,firstnumber);
	j=largenum_setval(j,28433);
	multiply_largenums(k,j,firstnumber);


	add_largenums(sum,k,firstnumber);

	show_largenum(sum);
	return 0;
}

