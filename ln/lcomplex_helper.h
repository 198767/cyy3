/*
 *	该文件里面包含ln构成的分数的基本操作函数
 */
#ifndef LN_FRAC_H
#define LN_FRAC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "ln.h"
#include "ln_helper.h"

struct _lcomplex
{
	ln real;	//实部
	ln imag;	//虚部
};
typedef struct _lcomplex* lcomplex;

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
lcomplex lcomplex_creat(ln real,ln imag,res_type restype);

/*
 * 作用:检查lcomplex是否合法(不包含NULL)
 * 参数:
 *	n:要检查的ln
 * 返回值:
 * 	成功:返回0
 * 	失败:返回-1
 */
int lcomplex_checknull(lcomplex n);
/*
 * 注意:如果是浅复制的,注意释放后不能再引用实部或者虚部
 * 作用:释放lcomplex
 * 参数:
 *	n:要释放的lcomplex的指针
 * 返回值:
 * 	无
 */
void lcomplex_free(lcomplex* n);
/*
 * 作用:复制b的值给a
 * 参数:
 *	b:原lcomplex
 *	a:目标lcomplex,如果传入NULL则返回新创建的b的副本
 * 返回值:
 * 	成功:如果a非NULL,返回a,否则返回新创建的b的副本
 * 	失败:返回NULL
 */
lcomplex lcomplex_copy(lcomplex a,lcomplex b);
/*
 * 作用:打印lcomplex
 * 参数:
 *	n:要输出的复数
 * 返回值:
 * 	无
 */
void lcomplex_output(lcomplex n);
#endif
