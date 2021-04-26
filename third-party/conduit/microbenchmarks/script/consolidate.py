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
            'time_type' : (
                'wall_time'
                if (
                    len(benchmark['name'].split('/')) > 3
                    and benchmark['name'].split('/')[3] == 'real_time'
                ) else 'cpu_time'
            )
        })].append({
            'Mesh' : benchmark['name'].split('/')[0],
            'Implementation' : kn.unpack(filename)['impl'],
            'Threads' : benchmark['threads'],
            'Processes' : kn.unpack(filename)['procs'],
            'Statistic' : (
                benchmark['aggregate_name']
                if 'aggregate_name' in benchmark
                else 'measurement'
            ),
            'Wall Nanoseconds' : benchmark['real_time'],
            'CPU Nanoseconds' : benchmark['cpu_time'],
            'Latency' : benchmark['Latency'],
            'Lossiness' : benchmark['Lossiness'],
        })

for run_specs, rows in res.items():
    pd.DataFrame(rows).to_csv(
        kn.pack({
            'run_type' : run_specs['run_type'],
            'time_type' : run_specs['time_type'],
            'ext' : '.csv',
        }),
        index=False,
    )
