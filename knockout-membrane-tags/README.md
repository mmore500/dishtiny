# Source Update

`2**18`

# ID Key

1. standard treatment
2. standard treatment with spiker PutMembraneBringer, PutMembraneBlocker, SetMembraneRegulator knocked out

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
    sed -i -- "${SPIKER_START},${FILE_LENGTH}s/\"id\": 98/\"id\": 27/g" $file
    sed -i -- "${SPIKER_START},${FILE_LENGTH}s/\"id\": 99/\"id\": 27/g" $file
    sed -i -- "${SPIKER_START},${FILE_LENGTH}s/\"id\": 100/\"id\": 27/g" $file
  done
done
```
