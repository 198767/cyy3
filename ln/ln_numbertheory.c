/*
 *	该库里面包含数论相关函数
 */
#include "ln_numbertheory.h"
/*
 * 作用:求出a和b的最大公因素
 * 参数:
 * 	a,b:要求出gcd的ln
 * 返回值:
 * 	成功:gcd
 * 	失败:NULL
 */
ln ln_gcd(ln a,ln b)
{
	ln m,n,q,r;
	//检查参数
	if(ln_checknull(a)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	if(ln_checknull(b)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	//a,b必须是正整数
	if(a->sign==-1 ||  (a->power<0 && ln_endingzeronum(a)+a->power<0) )
	{
		fprintf(stderr,"[%s %d] %s error,reason: a isn't positive integer\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	if(b->sign==-1 ||  (b->power<0 && ln_endingzeronum(b)+b->power<0) )
	{
		fprintf(stderr,"[%s %d] %s error,reason: b isn't positive integer\b",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}

	//复制a,b
	m=ln_copy(NULL,a);
	if(m==NULL)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_copy fail\b",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	n=ln_copy(NULL,b);
	if(n==NULL)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_copy fail\b",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	//m始终指向最大的整数
	if(ln_cmp(m,n)<0)
	{
		q=m;
		m=n;
		n=q;
	}
	q=NULL;
	r=NULL;
	//欧几里得算法
	while(ln_cmp_int(n,1)!=0)
	{
		q=ln_divide(m,n,0,round_res,newln);
		if(q==NULL)
		{
			ln_free(&m);
			ln_free(&n);
			fprintf(stderr,"[%s %d] %s error,reason: ln_divide fail\b",__FILE__,__LINE__,__FUNCTION__);
			return NULL;	
		}
		if(ln_multiply(q,n,firstln)==NULL) //q=q*n
		{
			ln_free(&m);
			ln_free(&n);
			ln_free(&q);
			fprintf(stderr,"[%s %d] %s error,reason: ln_divide fail\b",__FILE__,__LINE__,__FUNCTION__);
			return NULL;	
		}
		q->sign=-1;			//q=q*-1
		if(ln_sub(m,q,firstln)==NULL) 	//m=m-n*q
		{
			ln_free(&m);
			ln_free(&n);
			ln_free(&q);
			fprintf(stderr,"[%s %d] %s error,reason: ln_divide fail\b",__FILE__,__LINE__,__FUNCTION__);
			return NULL;	

		}
		//现在 n>m 交换
		r=m;
		m=n;
		n=r;
		ln_free(&q);
	}
	ln_free(&n);
	if(q!=NULL)
		ln_free(&q);
	return m;
}
