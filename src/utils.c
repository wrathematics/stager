#include "common.h"
#include "utils.h"


void MPI_Comm_localrank(MPI_Comm comm, int *localrank)
{
  MPI_Comm comm_local;
  MPI_Comm_split_type(comm, MPI_COMM_TYPE_SHARED, 0, MPI_INFO_NULL, &comm_local);
  MPI_Comm_rank(comm_local, localrank);
}



void MPI_throw_err(int errno, pinfo_t p, char *fmt, ...)
{
  if (p.rank == 0)
  {
    va_list args;
    
    fprintf(stderr, "ERROR: ");
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
  }
  
  exit(errno);
}



void pinfo_init(pinfo_t *p)
{
  MPI_Comm comm = p->comm;
  MPI_Comm_localrank(comm, &p->localrank);
  
  MPI_Comm_localrank(comm, &p->localrank);
  MPI_Comm_rank(comm, &p->rank);
  MPI_Comm_size(comm, &p->size);
}



void hello(pinfo_t p)
{
  int i;
  
  for (i=0; i<p.size; i++)
  {
    if (p.rank == i)
      printf("%d %d %d\n", p.localrank, p.rank, p.size);
    
    MPI_Barrier(p.comm);
  }
}
