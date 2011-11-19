/*
 *	该库里面包含lcomplex的构造及释放的函数，以及转换函数等辅助函数
 */
#include "lcomplex_helper.h"
/*
 * 作用:real和imag作为实部和虚部创建lcomplex
 * 参数:
 * 	real:实部
 * 	imag:虚部
 * 	res_type:深复制或者浅复制参数
 * 返回值:
 * 	成功:lcomplex
 * 	失败:NULL
 */
lcomplex lcomplex_creat(ln real,ln imag,res_type restype)
{
	lcomplex n;
	//检查参数
	if(ln_checknull(real)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	if(ln_checknull(imag)!=0)
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
	ln_stripleadingzero(real);
	ln_stripleadingzero(imag);

	//分配空间
	n=(lcomplex)malloc(sizeof(struct _lcomplex));
	if(!n)
	{
		fprintf(stderr,"[%s %d] %s malloc error,reason: %s\n",__FILE__,__LINE__,__FUNCTION__,strerror(errno));
		return NULL;			
	}
	if(restype==deepcopy) //深度复制
	{
		n->real=ln_copy(n->real,real);
		if(n->real==NULL)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_copy fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;			
		}
		n->imag=ln_copy(n->imag,imag);
		if(n->imag==NULL)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_copy fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;			
		}
	}
	else
	{	n->real=real;
		n->imag=imag;
	}
	return n;
}

/*
 * 作用:检查lcomplex是否合法(不包含NULL)
 * 参数:
 *	n:要检查的ln
 * 返回值:
 * 	成功:返回0
 * 	失败:返回-1
 */
int lcomplex_checknull(lcomplex n)
{
	if(n==NULL) //空指针
	{
		fprintf(stderr,"[%s %d] %s error,reason:n=NULL\n",__FILE__,__LINE__,__FUNCTION__);
		return -1;	
	}
	//检测实部
	if(ln_checknull(n->real) !=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason:ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return -1;	
	}
	//检测虚部
	if(ln_checknull(n->imag) !=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason:ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return -1;	
	}
	return 0;
}
/*
 * 注意:如果是浅复制的,注意释放后不能再引用实部或者虚部
 * 作用:释放lcomplex
 * 参数:
 *	n:要释放的lcomplex的指针
 * 返回值:
 * 	无
 */
void lcomplex_free(lcomplex* n)
{
	if(n==NULL)
	{
		fprintf(stderr,"[%s %d] %s error,reason: n=NULL\n",__FILE__,__LINE__,__FUNCTION__);
		return;	
	}
	//检测实部
	if(lcomplex_checknull(*n)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: lcomplex_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return;	
	}
	//释放实部
	ln_free(&((*n)->real));
	//释放虚部
	ln_free(&((*n)->imag));
	*n=NULL;
	return;
}

/*
 * 作用:复制b的值给a
 * 参数:
 *	b:原lcomplex
 *	a:目标lcomplex,如果传入NULL则返回新创建的b的副本
 * 返回值:
 * 	成功:如果a非NULL,返回a,否则返回新创建的b的副本
 * 	失败:返回NULL
 */
lcomplex lcomplex_copy(lcomplex a,lcomplex b)
{
	//验证b
	if(lcomplex_checknull(b) !=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: lcomplex_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;
	}
	if(!a)
	{
		a=lcomplex_creat(b->real,b->imag,deepcopy); //深复制创建分数
		if(a==NULL)
		{
			fprintf(stderr,"[%s %d] %s error,reason: lcomplex_creat fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;
		}
	}
	else
	{
		if(lcomplex_checknull(a) !=0)
		{
			fprintf(stderr,"[%s %d] %s error,reason: lcomplex_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;
		}

		if(ln_copy(a->real,b->real)==NULL)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_copy fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;
		}
		if(ln_copy(a->imag,b->imag)==NULL)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_copy fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;
		}

	}
	return a;
}

/*
 * 作用:化简分数lcomplex
 * 参数:
 *	n:要化简的分数
 *	restype:结果存放方式
 * 返回值:
 * 	成功:返回最简分数
 * 	失败:NULL
 */
lcomplex lcomplex_simplify(lcomplex n,res_type restype)
{
	lcomplex m;
	ln gcd;
	int sign;
	//检测参数
	if(lcomplex_checknull(n)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: lcomplex_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}

	if(restype==firstln)
		m=n;
	else
	{
		m=lcomplex_copy(NULL,n);
		if(m==NULL)
		{
			fprintf(stderr,"[%s %d] %s error,reason: lcomplex_copy fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;	
		}
	}

	//调整分数指数
	m->real->power-=m->imag->power;
	if(m->real->power>=0)
		m->imag->power=0;
	else
	{
		m->imag->power=-m->real->power;
		m->real->power=0;
	}
	if(m->real->sign==m->imag->sign)
		sign=1;
	else
		sign=-1;
	m->real->sign=1;
	m->imag->sign=1;

	//获取gcd
	gcd=ln_gcd(m->real,m->imag);
	if(gcd==NULL)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_gcd fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	if(ln_divide(m->real,gcd,0,round_res,firstln)==NULL)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_divide fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	if(ln_divide(m->imag,gcd,0,round_res,firstln)==NULL)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_divide fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	m->real->sign=sign;
	return m;
}
/*
 * 作用:打印分数lcomplex
 * 参数:
 *	n:要输出的分数
 * 返回值:
 * 	无
 */
void lcomplex_output(lcomplex n)
{
	char *p;
	//检测参数
	if(lcomplex_checknull(n)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: lcomplex_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return;	
	}
	p=ln2str(n->real);
	if(!p)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln2str fail\n",__FILE__,__LINE__,__FUNCTION__);
		return;			
	}
	printf("real: %s\n",p);
	free(p);
	p=ln2str(n->imag);
	if(!p)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln2str fail\n",__FILE__,__LINE__,__FUNCTION__);
		return;			
	}
	printf("imag: %s\n",p);
	free(p);
	return;
}
