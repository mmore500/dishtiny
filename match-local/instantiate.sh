#!/bin/bash

for treat in $(ls match-local/*.tar.gz | grep -o 'batch.*+' | sed 's/.$//'); do echo $treat; export treat=$treat; j2 "match-local
/run_list.slurm.sh.jinja" -o "match-local/instantiations/treat=${treat}+ext=.slurm.sh"; done
