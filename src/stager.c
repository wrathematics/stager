#include "common.h"
#include <string.h>

#define SHOW_RUNTIME 1

#define MAX(a,b) ((a)<(b)?(b):(a))

#define TGZLEN 256
char tgz[TGZLEN];


int main(int argc, char **argv)
{
  double t_start;
  int check;
  pinfo_t p;
  p.comm = MPI_COMM_WORLD;
  
  MPI_Init(NULL, NULL);
  pinfo_init(&p);
  
  if (argc != 2)
    MPI_err(p, "usage is ./stager /path/to/input/file.tar.gz");
  
  if (p.localrank == 0)
  {
    t_start = MPI_Wtime();
    
    FILE *p;
    sprintf(cmd, "basename %s", argv[1]);
    p = popen(cmd, "r");
    fgets(tgz, TGZLEN, p);
    tgz[MAX(0, strlen(tgz) - 1)] = '\0';
    
    sprintf(cmd, "cp %s %s", argv[1], OUTPATH);
    check = system(cmd);
    if (check == SYSTEM_ERR)
      goto wrapup;
    
    sprintf(cmd, "tar -zxf %s%s -C %s", OUTPATH, tgz, OUTPATH);
    check = system(cmd);
    if (check == SYSTEM_ERR)
      goto wrapup;
  }
  
wrapup:
  MPI_Allreduce(MPI_IN_PLACE, &check, 1, MPI_INT, MPI_SUM, p.comm);
  if (check == SYSTEM_ERR)
  {
    fprintf(stderr, "ERROR: failed to run '%s'\n", cmd);
    exit(SYSTEM_ERR);
  }
  
  if (p.rank == 0 && SHOW_RUNTIME)
  {
    double t_end = MPI_Wtime();
    printf("Staging time: %.3f\n", t_end-t_start);
  }
  
  MPI_Barrier(p.comm);
  
  MPI_Finalize();
  return 0;
}
