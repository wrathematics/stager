#include "common.h"
#include "options.h"
#include "utils.h"

#include <string.h>


#define TGZLEN 256
char tgz[TGZLEN];


int main(int argc, char **argv)
{
  double t_start = 0;
  int check;
  pinfo_t p;
  p.comm = MPI_COMM_WORLD;
  
  MPI_Init(NULL, NULL);
  pinfo_init(&p);
  
  if (argc != 2)
    MPI_throw_err(ERR_ARGS, p, "usage is mpirun [...] ./stager /path/to/input/file.tar.gz");
  
  if (p.localrank == 0)
  {
    t_start = MPI_Wtime();
    
    // get .tar.gz basename
    FILE *p;
    sprintf(cmd, "basename %s", argv[1]);
    p = popen(cmd, "r");
    char *check_tgz = fgets(tgz, TGZLEN, p);
    if (check_tgz == NULL)
    {
      check = ERR_FGETS;
      goto wrapup;
    }
    tgz[MAX(0, strlen(tgz) - 1)] = '\0';
    
    // move to /dev/shm
    sprintf(cmd, "cp %s %s", argv[1], OUTPATH);
    check = system(cmd);
    if (check == ERR_SYSTEM)
      goto wrapup;
    
    // untar
    sprintf(cmd, "tar -zxf %s%s -C %s", OUTPATH, tgz, OUTPATH);
    check = system(cmd);
    if (check == ERR_SYSTEM)
      goto wrapup;
  }
  
wrapup:
  MPI_Allreduce(MPI_IN_PLACE, &check, 1, MPI_INT, MPI_SUM, p.comm);
  if (check == ERR_FGETS)
    MPI_throw_err(check, p, "ERROR: failed to detect .tar.gz basename");
  else if (check == ERR_SYSTEM)
    MPI_throw_err(check, p, "ERROR: failed to run '%s'\n", cmd);
  
  if (p.rank == 0 && SHOW_RUNTIME)
  {
    double t_end = MPI_Wtime();
    printf("Staging time: %.3f\n", t_end-t_start);
  }
  
  MPI_Barrier(p.comm);
  
  MPI_Finalize();
  return 0;
}
