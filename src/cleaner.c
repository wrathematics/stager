#include "common.h"

#define SHOW_RUNTIME 1


int main()
{
  double t_start;
  int check;
  pinfo_t p;
  p.comm = MPI_COMM_WORLD;
  
  MPI_Init(NULL, NULL);
  pinfo_init(&p);
  
  if (p.localrank == 0)
  {
    t_start = MPI_Wtime();
    
    sprintf(cmd, "rm -rf %s/*", OUTPATH);
    check = system(cmd);
  }
  
  MPI_Allreduce(MPI_IN_PLACE, &check, 1, MPI_INT, MPI_SUM, p.comm);
  if (check == SYSTEM_ERR)
  {
    fprintf(stderr, "ERROR: failed to run '%s'\n", cmd);
    exit(SYSTEM_ERR);
  }
  
  if (p.rank == 0 && SHOW_RUNTIME)
  {
    double t_end = MPI_Wtime();
    printf("Cleanup time: %.3f\n", t_end-t_start);
  }
  
  MPI_Barrier(p.comm);
  
  MPI_Finalize();
  return 0;
}
