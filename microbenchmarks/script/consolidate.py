import sys
import json
from keyname import keyname as kn
import pandas as pd
from collections import defaultdict
from frozendict import frozendict

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
            'Execution' : kn.unpack(filename)['execution'],
            'Statistic' : (
                benchmark['aggregate_name']
                if 'aggregate_name' in benchmark
                else 'measurement'
            ),
            'Wall Nanoseconds' : benchmark['real_time'],
            'CPU Nanoseconds' : benchmark['cpu_time'],
            'num cells' : benchmark['num cells'],
            'num threads' : benchmark['num threads'],
            'Core-Nanoseconds per Cell Update' : benchmark['Core-Nanoseconds per Cell Update'],
        })

for run_specs, rows in res.items():
    pd.DataFrame(rows).to_csv(
        kn.pack({
            'run_type' : run_specs['run_type'],
            'ext' : '.csv',
        }),
        index=False,
    )
