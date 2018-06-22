import sys
from pathlib import Path

assert len(sys.argv) == 2, "Argument is run seed."

frames = [f for f in range(12000000) if f%1388==0 or f%1500000<300]

with open("/mnt/home/mmore500/dishtiny/request/run_list__viz-template","r") as fi:
    template=fi.read()
    for f in frames:
        runlist=template.format(**{'frame':f,'run':sys.argv[1]})
        with open("run_list__viz-%d"%f,"w+") as fo:
            fo.write(runlist)
