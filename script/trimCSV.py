import pandas as pd
import sys

assert len(sys.argv) >= 3, \
    "First argument is maximum update (inclusive), last arguments are filenames."

for f in sys.argv[2:]:

    print(f)

    print("read")

    chunks = []
    iter = pd.read_csv(f, chunksize=14400)
    for chunk in iter:
        chunks.append(chunk)
        if all(chunk['update'] > int(sys.argv[1])):
            break

    print("process")

    df = pd.concat(chunks)

    df = df[df['update'] <= int(sys.argv[1])]

    print("write")

    df.to_csv(f +"_tr")
