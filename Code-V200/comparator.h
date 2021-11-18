#ifndef __COMPARATOR_H_
#define __COMPARATOR_H_

#include "STC15Wxx.h"
//#include "main.h"

void init_cmpt(void);
#define get_CMP() (CMPCR1 & CMPRES)


#endif

