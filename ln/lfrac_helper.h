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
#include "ln_numbertheory.h"

struct _lfrac
{
	ln ntor;	//分子
	ln dtor;	//分母
};
typedef struct _lfrac* lfrac;

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
lfrac lfrac_creat(ln ntor,ln dtor,res_type restype);

/*
 * 作用:检查lfrac是否合法(不包含NULL)
 * 参数:
 *	n:要检查的ln
 * 返回值:
 * 	成功:返回0
 * 	失败:返回-1
 */
int lfrac_checknull(lfrac n);
/*
 * 注意:如果是浅复制的,注意释放后不能再引用分子或者分母
 * 作用:释放lfrac
 * 参数:
 *	n:要释放的lfrac的指针
 * 返回值:
 * 	无
 */
void lfrac_free(lfrac* n);
/*
 * 作用:复制b的值给a
 * 参数:
 *	b:原lfrac
 *	a:目标lfrac,如果传入NULL则返回新创建的b的副本
 * 返回值:
 * 	成功:如果a非NULL,返回a,否则返回新创建的b的副本
 * 	失败:返回NULL
 */
lfrac lfrac_copy(lfrac a,lfrac b);
/*
 * 作用:化简分数lfrac
 * 参数:
 *	n:要化简的分数
 *	restype:结果存放方式
 * 返回值:
 * 	成功:返回最简分数
 * 	失败:NULL
 */
lfrac lfrac_simplify(lfrac n,res_type restype);
/*
 * 作用:打印分数lfrac
 * 参数:
 *	n:要输出的分数
 * 返回值:
 * 	无
 */
void lfrac_output(lfrac n)
#endif
