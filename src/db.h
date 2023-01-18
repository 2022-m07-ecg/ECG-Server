#ifndef _DB_H
#define _DB_H 1

#include <stdint.h>
#include "parse.h"

int dbInit(void);

int dbStore(struct dataSample_t *sample, uint64_t ECGID, int n);

void dbClose(void);

#endif //_DB_H