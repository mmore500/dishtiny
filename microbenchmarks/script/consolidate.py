from collections import defaultdict
from frozendict import frozendict
import json
from keyname import keyname as kn
import pandas as pd
import sys

def load_json(filename):
    with open(filename) as json_file:
        data = json.load(json_file)
    return data

res = defaultdict(list)

for filename, entry in [
        (filename, load_json(filename))
        for filename in sys.argv[1:]
    ]:
    for benchmark in entry['benchmarks']:
        res[frozendict({
            'run_type' : benchmark['run_type'],
        })].append({
            'Task' : kn.unpack(filename)['task'],
            'Statistic' : (
                benchmark['aggregate_name']
                if 'aggregate_name' in benchmark
                else 'measurement'
            ),
            'Wall Nanoseconds' : benchmark['real_time'],
            'CPU Nanoseconds' : benchmark['cpu_time'],
            'Num Cells' : int(benchmark['Num Cells']),
            'Num Threads' : int(benchmark['Num Threads']),
            'Cell-Updates Executed' : int(benchmark['Cell-Updates Executed']),
            'CPU Core-Nanoseconds per Cell Update' : (
                benchmark['cpu_time'] * benchmark['iterations']
                / ( benchmark['Cell-Updates Executed'] or 1 )
            ) ,
            'Wall Core-Nanoseconds per Cell Update' : (
                benchmark['Num Threads']
                * benchmark['real_time'] * benchmark['iterations']
                / ( benchmark['Cell-Updates Executed'] or 1 )
            ) ,
        })

for run_specs, rows in res.items():
    pd.DataFrame(rows).to_csv(
        kn.pack({
            'run_type' : run_specs['run_type'],
            'ext' : '.csv',
        }),
        index=False,
    )
