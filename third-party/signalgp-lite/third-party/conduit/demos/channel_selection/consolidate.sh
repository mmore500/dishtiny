#!/bin/bash

paste -d "\n" *sent*.txt | paste -sd+ | bc
cat *sent*.txt | sort -n | head -n 1
cat *sent*.txt | sort -n | tail -n 1
cat *sent*.txt | tr '\n' ' ' | python3 -c "import numpy as np; import sys; print(np.std(list(map(float,next(sys.stdin).split()))))"
paste -d "\n" *received*.txt | paste -sd+ | bc
cat *received*.txt | sort -n | head -n 1
cat *received*.txt | sort -n | tail -n 1
cat *received*.txt | tr '\n' ' ' | python3 -c "import numpy as np; import sys; print(np.std(list(map(float,next(sys.stdin).split()))))"
paste -d "\n" *conflicts*.txt | paste -sd+ | bc
cat *conflicts*.txt | sort -n | head -n 1
cat *conflicts*.txt | sort -n | tail -n 1
cat *conflicts*.txt | tr '\n' ' ' | python3 -c "import numpy as np; import sys; print(np.std(list(map(float,next(sys.stdin).split()))))"
paste -d "\n" *updates*.txt | paste -sd+ | bc
cat *updates*.txt | sort -n | head -n 1
cat *updates*.txt | sort -n | tail -n 1
cat *updates*.txt | tr '\n' ' ' | python3 -c "import numpy as np; import sys; print(np.std(list(map(float,next(sys.stdin).split()))))"

rm *.txt
