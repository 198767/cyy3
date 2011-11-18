/*
 *	该文件里面包含ln的基本运算函数
 */
#ifndef LN_NUMBERTHEORY_H
#define LN_NUMBERTHEORY_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "ln.h"
#include "ln_helper.h"
#include "ln_arithmetic.h"

/*
 * 作用:求出a和b的最大公因素
 * 参数:
 * 	a,b:要求出gcd的ln
 * 返回值:
 * 	成功:gcd
 * 	失败:NULL
 */
ln ln_gcd(ln a,ln b);
#endif
