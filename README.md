# stager

Pre-stage your compute nodes with a .tar.gz file you want unpacked in /dev/shm. You should not use this if you can run containers (singularity or shifter). This approach is also not meant for small jobs where it doesn't really matter what you do.

This is extremely important if you are running a large number (1000+) of dynamically built processes (e.g. R, python). Running the application directly from the parallel file system will have very poor performance and scale terribly.

Other approaches:

* Aron Ahmadia's [collfs](https://github.com/wscullin/collfs)
* LLNL's [Spindle](https://computation.llnl.gov/projects/spindle)

References:

* [Massively Parallel Loading](https://computation.llnl.gov/projects/spindle/spindle-paper.pdf)
* [Shifter: Containers for HPC](https://cug.org/proceedings/cug2016_proceedings/includes/files/pap103s2-file1.pdf) (see Section IV A) 
* [CORAL 2 RFP](https://procurement.ornl.gov/rfp/CORAL2/03_CORAL-2-SOW-DRAFT-v23.pdf) (see section 5.2.9)

## Compiling

Set `MPICC` and `CFLAGS` appropriately in the Makefile and type `make`.

## Running

Here's an example:

```bash
#!/bin/sh

# stage
mpirun -np 2 ./stager mydynamicapp.tar.gz

# run
mpirun -np 2 /dev/shm/mydynamicapp

# clean
mpirun -np 2 ./cleaner
```
