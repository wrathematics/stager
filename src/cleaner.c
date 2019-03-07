#include "common.h"

#define SHOW_RUNTIME 1

#define UNUSED(x) (void)(x)


int main(int argc, char **argv)
{
  double t_start = 0;
  int check;
  pinfo_t p;
  p.comm = MPI_COMM_WORLD;
  
  UNUSED(argv);
  
  MPI_Init(NULL, NULL);
  pinfo_init(&p);
  
  if (argc != 1)
    MPI_throw_err(ERR_ARGS, p, "usage is mpirun [...] ./cleaner");
  
  if (p.localrank == 0)
  {
    t_start = MPI_Wtime();
    
    sprintf(cmd, "rm -rf %s/*", OUTPATH);
    check = system(cmd);
  }
  
  MPI_Allreduce(MPI_IN_PLACE, &check, 1, MPI_INT, MPI_SUM, p.comm);
  if (check == ERR_SYSTEM)
    MPI_throw_err(check, p, "ERROR: failed to run '%s'\n", cmd);
  
  if (p.rank == 0 && SHOW_RUNTIME)
  {
    double t_end = MPI_Wtime();
    printf("Cleanup time: %.3f\n", t_end-t_start);
  }
  
  MPI_Barrier(p.comm);
  
  MPI_Finalize();
  return 0;
}
