#ifndef STAGER_UTILS_H
#define STAGER_UTILS_H


#include "common.h"
#include <stdarg.h>


void MPI_Comm_localrank(MPI_Comm comm, int *localrank);
void MPI_throw_err(int errno, pinfo_t p, char *fmt, ...);
void pinfo_init(pinfo_t *p);
void hello(pinfo_t p);


#endif
