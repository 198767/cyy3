/*
 *	该库里面包含ln的构造及释放的函数，以及转换函数等辅助函数
 */
#include "lfrac.h"
/*
 * 作用:ntor和dtor作为分子和分母创建lfrac
 * 参数:
 * 	ntor:分子
 * 	dtor:分母
 * 	res_type:深复制或者浅复制参数
 * 返回值:
 * 	成功:lfrac
 * 	失败:NULL
 */
lfrac lfrac_creat(ln ntor,ln dtor,res_type restype)
{
	lfrac n;
	//检查参数
	if(ln_checknull(ntor)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	if(ln_checknull(dtor)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	if(restype !=copy && restype !=deepcopy)
	{
		fprintf(stderr,"[%s %d] %s error,reason: invalid restype\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}

	//去除前置0
	ln_stripleadingzero(ntor);
	ln_stripleadingzero(dtor);

	//分配空间
	n=(lfrac)malloc(sizeof(struct _lfrac));
	if(!n)
	{
		fprintf(stderr,"[%s %d] %s malloc error,reason: %s\n",__FILE__,__LINE__,__FUNCTION__,strerror(errno));
		return NULL;			
	}
	if(restype==deepcopy) //深度复制
	{
		n->ntor=ln_copy(n->ntor,ntor);
		if(n->ntor==NULL)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_copy fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;			
		}
		n->dtor=ln_copy(n->dtor,dtor);
		if(n->dtor==NULL)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_copy fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;			
		}
	}
	else
	{	n->ntor=ntor;
		n->dtor=dtor;
	}
	return n;
}

/*
 * 作用:检查lfrac是否合法(不包含NULL)
 * 参数:
 *	n:要检查的ln
 * 返回值:
 * 	成功:返回0
 * 	失败:返回-1
 */
int lfrac_checknull(lfrac n)
{
	if(n==NULL) //空指针
	{
		fprintf(stderr,"[%s %d] %s error,reason:n=NULL\n",__FILE__,__LINE__,__FUNCTION__);
		return -1;	
	}
	//检测分子
	if(ln_checknull(n->ntor) !=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason:ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return -1;	
	}
	//检测分母
	if(ln_checknull(n->dtor) !=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason:ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return -1;	
	}
	return 0;
}
/*
 * 注意:如果是浅复制的,注意释放后不能再引用分子或者分母
 * 作用:释放lfrac
 * 参数:
 *	n:要释放的lfrac的指针
 * 返回值:
 * 	无
 */
void lfrac_free(lfrac* n)
{
	if(n==NULL)
	{
		fprintf(stderr,"[%s %d] %s error,reason: n=NULL\n",__FILE__,__LINE__,__FUNCTION__);
		return;	
	}
	//检测分子
	if(lfrac_checknull(*n)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: lfrac_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return;	
	}
	//释放分子
	ln_free(&((*n)->ntor));
	//释放分母
	ln_free(&((*n)->dtor));
	*n=NULL;
	return;
}

/*
 * 作用:复制b的值给a
 * 参数:
 *	b:原lfrac
 *	a:目标lfrac,如果传入NULL则返回新创建的b的副本
 * 返回值:
 * 	成功:如果a非NULL,返回a,否则返回新创建的b的副本
 * 	失败:返回NULL
 */
lfrac lfrac_copy(lfrac a,lfrac b)
{
	//验证b
	if(lfrac_checknull(b) !=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: lfrac_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;
	}
	if(!a)
	{
		a=lfrac_creat(b->ntor,b->dtor,deepcopy); //深复制创建分数
		if(a==NULL)
		{
			fprintf(stderr,"[%s %d] %s error,reason: lfrac_creat fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;
		}
	}
	else
	{
		if(lfrac_checknull(a) !=0)
		{
			fprintf(stderr,"[%s %d] %s error,reason: lfrac_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;
		}

		if(ln_copy(a->ntor,b->ntor)==NULL)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_copy fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;
		}
		if(ln_copy(a->dtor,b->dtor)==NULL)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_copy fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;
		}

	}
	return a;
}

/*
 * 作用:化简分数lfrac
 * 参数:
 *	n:要化简的分数
 *	restype:结果存放方式
 * 返回值:
 * 	成功:返回最简分数
 * 	失败:NULL
 */
lfrac lfrac_simplify(lfrac n,res_type restype)
{
	lfrac m;
	ln gcd;
	int sign;
	//检测参数
	if(lfrac_checknull(n)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: lfrac_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}

	if(restype==firstln)
		m=n;
	else
	{
		m=lfrac_copy(NULL,n);
		if(m==NULL)
		{
			fprintf(stderr,"[%s %d] %s error,reason: lfrac_copy fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;	
		}
	}

	//调整分数指数
	m->ntor->power-=m->dtor->power;
	if(m->ntor->power>=0)
		m->dtor->power=0;
	else
	{
		m->dtor->power=-m->ntor->power;
		m->ntor->power=0;
	}
	if(m->ntor->sign==m->dtor->sign)
		sign=1;
	else
		sign=-1;
	m->ntor->sign=1;
	m->dtor->sign=1;

	//获取gcd
	gcd=ln_gcd(m->ntor,m->dtor);
	if(gcd==NULL)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_gcd fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	if(ln_divide(m->ntor,gcd,0,round_res,firstln)==NULL)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_divide fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	if(ln_divide(m->dtor,gcd,0,round_res,firstln)==NULL)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_divide fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	m->ntor->sign=sign;
	return m;
}
/*
 * 作用:打印分数lfrac
 * 参数:
 *	n:要输出的分数
 * 返回值:
 * 	无
 */
void lfrac_output(lfrac n)
{
	char *p;
	//检测参数
	if(lfrac_checknull(n)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: lfrac_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return;	
	}
	p=ln2str(n->ntor);
	if(!p)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln2str fail\n",__FILE__,__LINE__,__FUNCTION__);
		return;			
	}
	printf("ntor: %s",p);
	free(p);
	p=ln2str(n->dtor);
	if(!p)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln2str fail\n",__FILE__,__LINE__,__FUNCTION__);
		return;			
	}
	printf("dtor: %s",p);
	free(p);
	return;
}
