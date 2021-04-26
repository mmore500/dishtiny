#!/bin/bash

paste -d "\n" outbenchmarks/*elapsed_updates*.txt | paste -sd+ | bc
cat outbenchmarks/*elapsed_updates*.txt | sort -n | head -n 1
cat outbenchmarks/*elapsed_updates*.txt | sort -n | tail -n 1
cat outbenchmarks/*elapsed_updates*.txt | tr '\n' ' ' | python3 -c "import numpy as np; import sys; print(np.std(list(map(float,next(sys.stdin).split()))))"

rm outbenchmarks/*elapsed_updates*.txt
