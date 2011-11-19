/*
 *	该库里面包含lfrac的构造及释放的函数，以及转换函数等辅助函数
 */
#include "lfrac_helper.h"
/*
 * 作用:ntor和dtor作为分子和分母创建lfrac
 * 注意:如果是浅复制,则在此之后只应该在lfrac引用那些ln,否则会出错
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
	//分母不能为0
	if(ln_cmp_int(dtor,0)==0)
	{
		fprintf(stderr,"[%s %d] %s error,reason:dtor is 0\n",__FILE__,__LINE__,__FUNCTION__);
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
		n->ntor=ln_copy(NULL,ntor);
		if(n->ntor==NULL)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_copy fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;			
		}
		n->dtor=ln_copy(NULL,dtor);
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
	if(n->ntor->sign == n->dtor->sign)
		n->sign=1;
	else
		n->sign=-1;
	n->ntor->sign=1;
	n->dtor->sign=1;
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
	//分子分母必须是正数,分数的符号在sign单独设置
	if(n->ntor->sign!=1)
	{
		fprintf(stderr,"[%s %d] %s error,reason:ntor <=0\n",__FILE__,__LINE__,__FUNCTION__);
		return -1;	
	}
	if(n->dtor->sign!=1)
	{
		fprintf(stderr,"[%s %d] %s error,reason:dtor <=0\n",__FILE__,__LINE__,__FUNCTION__);
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
	a->sign=b->sign;
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
 * 作用:把lfrac转换为字符串,形式为[分子/分母],但如果分子是0,直接返回0
 * 参数:
 *	n:要处理的lfrac
 * 返回值:
 * 	成功:返回lfrac的字符串表示(需要用free释放)
 * 	失败:NULL
 */
char* lfrac2str(lfrac n)
{
	char *p,*q,*str;
	//检测参数
	if(lfrac_checknull(n)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: lfrac_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	p=ln2str(n->ntor);
	if(!p)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln2str fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;			
	}
	if(p[0]=='0' && p[1]=='\0') //如果分子是0,直接返回0
		return p;

	q=ln2str(n->dtor);
	if(!q)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln2str fail\n",__FILE__,__LINE__,__FUNCTION__);
		free(p);
		return NULL;			
	}
	str=malloc(strlen(p)+strlen(q)+5);
	if(!str)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln2str fail\n",__FILE__,__LINE__,__FUNCTION__);
		free(p);
		free(q);
		return NULL;			
	}
	sprintf(str,"%s%s/%s",n->sign==1?"":"-",p,q);
	free(p);
	free(q);
	return str;
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
	p=lfrac2str(n);
	if(!p)
	{
		fprintf(stderr,"[%s %d] %s error,reason: lfrac2str fail\n",__FILE__,__LINE__,__FUNCTION__);
		return;			
	}
	puts(p);
	free(p);
	return;			
}
