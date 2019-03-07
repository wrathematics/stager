#ifndef STAGER_COMMON_H
#define STAGER_COMMON_H


#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#if MPI_VERSION < 3
#error "MPI_VERSION >= 3 required"
#endif

#define ERR_SYSTEM  -1
#define ERR_FGETS   -2
#define ERR_ARGS    -10

#define CMDLEN 512
char cmd[CMDLEN];

#define MAX(a,b) ((a)<(b)?(b):(a))
#define UNUSED(x) (void)(x)

typedef struct pinfo_t
{ 
  MPI_Comm comm;
  int localrank;
  int rank;
  int size;
} pinfo_t;


#endif
