#include "add.h"

//add the two numbers
char* add_nums(const char* num1,const char* base1,const char* num2,const char* base2)
{
	const char* base_str[2];
	const char* num_str[2];
	char *p,*q;
	int base_gcd;
	int i,test_res;
	int base[2];
	ln numerator[3],denominator[3];
	vector digits;

	//test bases
	base_str[0]=base1;
	base_str[1]=base2;
	for(i=0;i<2;i++)
	{
		base[i]=test_base(base_str[i]);
		if(base[i]==-1)
		{
			printf("Error: invalid base [%s]!\n",base_str[i]);
			return NULL;
		}
		else if(base[i]==-2)
		{
			printf("Error: base [%s] is out of range "MACRO2STR(MIN_BASE)"-"MACRO2STR(MAX_BASE)"!\n",base_str[i]);
			return NULL;
		}
	}

	//test numbers 
	num_str[0]=num1;
	num_str[1]=num2;
	for(i=0;i<2;i++)
	{
		test_res=test_num(num_str[i],base[i]);
		if(test_res==0)
		{
			printf("Error: invalid number [%s],the number should match the regular expression [0-9]+(.[0-9]+)? !\n",num_str[i]);
			return NULL;
		}
		else if(test_res==2)
		{
			printf("Error: invalid number [%s] in base %d !\n",num_str[i],base[i]);
			return NULL;
		}
	}

	//convert the two numbers into fraction form
	for(i=0;i<2;i++)
	{
		denominator[i]=get_denominator(num_str[i],base[i]);
		numerator[i]=get_numerator(num_str[i],base[i]);
	}

	for(i=0;i<2;i++)
	{
		p=ln2str(numerator[i]);
		q=ln2str(denominator[i]);
		printf("operand in fraction:%s/%s\n",p,q);
		fflush(stdout);
		free(p);
		free(q);
	}
	//compute the fraction
	denominator[2]=ln_multiply(denominator[0],denominator[1],newln);
	numerator[0]=ln_multiply(numerator[0],denominator[1],firstln);
	numerator[1]=ln_multiply(numerator[1],denominator[0],firstln);
	numerator[2]=ln_add(numerator[0],numerator[1],newln);
	//free unused numbers
	for(i=0;i<2;i++)
	{
		ln_free(&(numerator[i]));
		ln_free(&(denominator[i]));
	}


	p=ln2str(numerator[2]);
	q=ln2str(denominator[2]);
	printf("result in fraction:%s/%s\n",p,q);


	//simplifying the result fraction
	base_gcd=gcd(base[0],base[1]);
	if(base_gcd !=1)
	{
		while(ln_divideable_num(numerator[2],base_gcd)==1 && ln_divideable_num(denominator[2],base_gcd)==1)
		{

			ln_divide_int(numerator[2],base_gcd,0,trunc_res,firstln);
			ln_divide_int(denominator[2],base_gcd,0,trunc_res,firstln);
		}
	}

	p=ln2str(numerator[2]);
	q=ln2str(denominator[2]);
	printf("result in fraction:%s/%s\n",p,q);
	fflush(stdout);
	if(strcmp(q,"1")==0)
	{
		free(q);
		q=(char*)malloc(strlen(p)+4);
		if(!q)
		{
			ln_free(&(numerator[2]));
			ln_free(&(denominator[2]));
			printf("Error: coverting fraction to decimal number failed at line %d",__LINE__);
			return NULL;
		}
		sprintf(q,"%s 10",p);
		free(p);
		return q;
	}




	free(p);
	free(q);
	digits=get_decimal_digit(numerator[2],denominator[2]);
	if(!digits)
	{
		ln_free(&(numerator[2]));
		ln_free(&(denominator[2]));
		printf("Error: coverting fraction to decimal number failed at line %d",__LINE__);
		return NULL;
	}

	p=get_decimalstr(digits,denominator[2]);
	if(!p)
	{
		ln_free(&(numerator[2]));
		ln_free(&(denominator[2]));
		free_digit(digits);
		printf("Error: coverting fraction to decimal number failed at line %d",__LINE__);
		return NULL;
	}
	return p;
}

/* The base should be a integral between MIN_BASE~MAX_BASE
   return values: 
	-1: invalid base  
	-2: base is not in MIN_BASE~MAX_BASE
	else: the base coverted to int
*/
int test_base(const char* base)
{
	int i;
	const char* p;
	p=base;
	while(*p)
	{
		if(!isdigit(*p))
			return -1;
		p++;
	}

	//ignore leading zeros
	p=base;
	while(*p=='0' && *p)
		p++;

	// all zeros
	if(!*p)
		return -2;

	// >MAX_BASE
	if(strlen(p) > strlen(MACRO2STR(MAX_BASE)))
		return -2; 
	else if(strlen(p)==strlen(MACRO2STR(MAX_BASE)))
	{
		if(strcmp(p,MACRO2STR(MAX_BASE))>0)
			return -2; 
	}
	i=atoi(p);
	if(i<2)
		return -2;
	return i;
}

/* The number should be of the form [0-9]+(.[0-9]+)?
   return values: 
	0: invalid number
	1: valide number
        2: valide number but in wrong base
 */
int test_num(const char* n,int base)
{
	const char* p=n;
	int flag=0;
	if(!isdigit(*p))
		return 0;
	else if(*p-'0'>=base)
		return 2;
	p++;
	while(*p)
	{
		if(*p=='.')
		{
			if(flag==1) // already encount a pointer
				return 0;
			else
				flag=1;
		}
		else if(!isdigit(*p))
			return 0;
		else
		{
			if(*p-'0'>=base)
				return 2;
		}
		p++;
	}

	if(flag==1 && p[-1]=='.') // end with a pointer
		return 0;
	return 1;
}




//get the denominator part of the num in base b and converted to base 10
ln get_denominator(const char* num,int b)
{
	const char*p;
	ln denominator;
	denominator=ln_init(1);
	p=strstr(num,".");
	if(p) //has fractional part
	{
		p++;
		while(*p)
		{
			ln_multiply_int(denominator,b,firstln);
			p++;
		}
	}
	return denominator;
}

//get the numerator part of the num in base b and converted to base 10
ln get_numerator(const char* num,int b)
{
	const char*p;
	ln numerator;
	ln base;
	ln add_digit;
	numerator=ln_init(0);
	base=ln_init(1);
	p=num+strlen(num)-1;
	while(p>=num)
	{
		if(*p !='.')
		{
			add_digit=ln_multiply_int(base,*p-'0',newln);
			ln_add(numerator,add_digit,firstln);
			ln_free(&add_digit);
			ln_multiply_int(base,b,firstln);
		}
		p--;
	}
	return numerator;
}

/*
 * get the decimal digits
 */
vector get_decimal_digit(const ln numerator,const ln denominator)
{
	ln a,b,c;
	ln i,j,k,l,temp;
	int power;
	vector digits;
	digits=vect_create_pointer();
	if(!digits)
		return NULL;

	a=ln_copy(NULL,numerator);
	b=ln_copy(NULL,denominator);

	//get the digits
	if(ln_cmp(a,b)<0)
	{
		vect_pushelm(digits,a);
		ln_free(&b);
		return digits;
	}
	

	//find out the maxpower
	power=1;
	c=ln_exp_int(b,power,newln);
	while(ln_cmp(c,a)<=0)
	{
		ln_free(c);
		power++;
		c=ln_exp_int(b,power,newln);
	}
	ln_free(c);
	power--;
	while(ln_cmp(a,b)>=0)
	{
		c=ln_exp_int(b,power,newln);
		//binary search
		i=ln_setval(NULL,0); //i=0
		j=ln_add_num(b,-1,newln); //j=numerator-1

		while(1)
		{
			k=ln_add(i,j,newln); //k=(i+j)/2
			k=ln_divide_num(k,2,0,trunc,firstln);
			temp=ln_multiply(k,c,newln);

			if(ln_cmp(temp,a)>0)  //k*c>a
			{
				ln_free(j);
				j=ln_add_num(k,-1,newln); //j=k-1
				ln_free(k);
				ln_free(temp);
			}
			else if(ln_cmp(temp,a)==0) //k*c=a
			{
				ln_free(i);
				ln_free(j);
				ln_free(c);
				break;
			}
			else //k*c<a
			{
				l=ln_add_num(k,1,newln);
				ln_free(temp);
				temp=ln_multiply(l,c,newln);
				if(ln_cmp(temp,a)==0) //(k+1)*c=a
				{
					ln_copy(&k,l);
					ln_free(i);
					ln_free(j);
					ln_free(c);
					ln_free(l);
					break;
				}
				else if(ln_cmp(temp,a)>0) //(k+1)*c>a
				{
					ln_free(temp);
					temp=ln_multiply(k,c,newln);
					ln_free(i);
					ln_free(j);
					ln_free(l);
					ln_free(c);
					break;
				}
				else //(k+1) <a
				{
					ln_free(i);
					i=ln_add_num(k,2,newln); //i=k+2
					ln_free(k);
					ln_free(temp);
				}
			}
		}
		vect_pushelm(digits,k);
		minus_lns(a,temp,firstln);
		ln_free(temp);
		power--;
	}
	vect_pushelm(digits,a);
	ln_free(b);
	return digits;
}

/*
 * get the addtion result with the <decimal base> form 
 * but if the base is too large (>16) and hard to find  letters to present the digits(maybe >16), then use the <digit1*base1^power1+digit2*base2^power2.....> form
 */
char* get_decimalstr(const vector digits,const ln base)
{
	int i;
	char *s1,*s2;
	ln digit;
	vector decimal;
	//produce the decimal string
	decimal=vect_create_str("");
	if(!decimal)
		return NULL;

	s2=ln2str(base);
	for(i=0;i<vect_getlength(digits);i++)
	{
		digit=*((ln *)vect_getelmat(digits,i));
		if(ln_cmp_int(base,16)>0)
		{
			if(ln_cmp_int(digit,0)>0)
			{
				s1=ln2str(digit);
				if(i>0)
					vect_strcat(decimal,"+");
				vect_strcat(decimal,"%s*%s^(%d)",s1,s2,vect_getlength(digits)-i-2);
				free(s1);
			}
		}
		else
		{
			s1=ln2str(digit);
			if(i==vect_getlength(digits)-1) //add the point
			{
				if(strcmp(s1,"0")==0)
				{
					free(s1);
					break;
				}
				if(vect_getlength(decimal)==0)
					vect_strcat(decimal,"0");
				vect_strcat(decimal,".");
			}
			if(strlen(s1)==1) //0-9
				vect_strcat(decimal,"%s",s1);
			else
				vect_strcat(decimal,"%c",'a'+s1[1]-'0'); //10-15 converted to a-f
			free(s1);
		}
	}
	
	//append the base
	if(ln_cmp_int(base,16)<=0)
		vect_strcat(decimal," %s",s2);
	free(s2);

	s1=vect2str(decimal);
	vect_free(decimal);
	return s1;
}

void free_digit(vector digits)
{
	int i;
	ln p;
	for(i=0;i<vect_getlength(digits);i++)
	{
		p=*((ln *)vect_getelmat(digits,i));
		ln_free(*p);
	}
	vect_free(digits);
	return;
}

int gcd(int a,int b)
{
	if(b == 0)
		return a;
	return gcd(b,a%b);
}
