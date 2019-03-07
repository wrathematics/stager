#include "common.h"


int main(int argc, char **argv)
{
  int check;
  pinfo_t p;
  p.comm = MPI_COMM_WORLD;
  
  MPI_Init(NULL, NULL);
  pinfo_init(&p);
  
  if (argc != 2)
    MPI_err(p, "usage is ./stager /path/to/input/file.tar.gz");
  
  if (p.localrank == 0)
  {
    sprintf(cmd, "%s %s %s", "cp", argv[1], OUTPATH);
    check = system(cmd);
    if (check == SYSTEM_ERR)
      goto wrapup;
    
    sprintf(cmd, "%s %s%s", "tar -zxf", OUTPATH, argv[1]);
    check = system(cmd);
    if (check == SYSTEM_ERR)
      goto wrapup;
  }
  
wrapup:
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
