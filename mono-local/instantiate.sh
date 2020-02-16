#!/bin/bash

for treat in $(ls mono-local/*.tar.gz | grep -o 'batch.*+' | sed 's/.$//'); do echo $treat; export treat=$treat; j2 "mono-local/run_list.slurm.sh.jinja" -o "mono-local/instantiations/treat=${treat}+ext=.slurm.sh"; done
