#include "common.h"


int main()
{
  int check;
  pinfo_t p;
  p.comm = MPI_COMM_WORLD;
  
  MPI_Init(NULL, NULL);
  pinfo_init(&p);
  
  if (p.localrank == 0)
  {
    sprintf(cmd, "%s %s*", "rm -rf", OUTPATH);
    check = system(cmd);
  }
  
  MPI_Barrier(p.comm);
  
  MPI_Allreduce(MPI_IN_PLACE, &check, 1, MPI_INT, MPI_SUM, p.comm);
  if (check == SYSTEM_ERR)
  {
    fprintf(stderr, "ERROR: failed to run '%s'\n", cmd);
    exit(SYSTEM_ERR);
  }
  
  MPI_Finalize();
  return 0;
}
