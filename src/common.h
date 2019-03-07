#ifndef STAGER_COMMON_H
#define STAGER_COMMON_H


#define OUTPATH "/dev/shm/"


#include <mpi.h>
#include <stdarg.h>
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

typedef struct pinfo_t
{ 
  MPI_Comm comm;
  int localrank;
  int rank;
  int size;
} pinfo_t;



static inline void MPI_Comm_localrank(MPI_Comm comm, int *localrank)
{
  MPI_Comm comm_local;
  MPI_Comm_split_type(comm, MPI_COMM_TYPE_SHARED, 0, MPI_INFO_NULL, &comm_local);
  MPI_Comm_rank(comm_local, localrank);
}



static inline void MPI_throw_err(int errno, pinfo_t p, char *fmt, ...)
{
  if (p.rank == 0)
  {
    va_list args;
    
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
  }
  
  exit(errno);
}



static inline void pinfo_init(pinfo_t *p)
{
  MPI_Comm comm = p->comm;
  MPI_Comm_localrank(comm, &p->localrank);
  
  MPI_Comm_localrank(comm, &p->localrank);
  MPI_Comm_rank(comm, &p->rank);
  MPI_Comm_size(comm, &p->size);
}



static inline void hello(pinfo_t p)
{
  int i;
  
  for (i=0; i<p.size; i++)
  {
    if (p.rank == i)
      printf("%d %d %d\n", p.localrank, p.rank, p.size);
    
    MPI_Barrier(p.comm);
  }
}


#endif
