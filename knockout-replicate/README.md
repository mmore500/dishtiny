# Source Update

`2**18`

# ID Key

1. standard treatment
2. standard treatment with regular TryReproduce-Lev0, TryReproduce-Lev1, TryReproduce-Lev2 knocked out

# Knockout Prep

```
for t in treat=*; do
  for f in $(ls $t); do cp $t/$f $t/id=1+$f; done
  for f in $(ls $t | grep -v id=1); do mv $t/$f $t/id=2+$f; done
  for file in $t/id=2*.json; do
    SPIKER_START=$(grep -n program_spiker $file | cut -d : -f 1 | head -n 1)
    FILE_LENGTH=$(cat $file | wc -l)
    echo $SPIKER_START
    echo $FILE_LENGTH
    sed -i -- "1,${SPIKER_START}s/\"id\": 116/\"id\": 27/g" $file
    sed -i -- "1,${SPIKER_START}s/\"id\": 117/\"id\": 27/g" $file
    sed -i -- "1,${SPIKER_START}s/\"id\": 118/\"id\": 27/g" $file
  done
done
```
