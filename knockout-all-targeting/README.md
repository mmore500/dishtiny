# Source Update

`2**18`

# ID Key

1. standard treatment
2. standard treatment with knockked out:
  * RemoveOutgoingConnection
  * RemoveIncomingConnection
  * AddDevoUpQuery
  * AddDevoDownQuery
  * SetConnectionAgingParam
  * SetConnectionExploitParam
  * SetConnectionDevelopmentParam
  * SetConnectionSensingParam
  * PutMembraneBringer
  * PutMembraneBlocker
  * SetMembraneRegulator

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
    sed -i -- "${SPIKER_START},${FILE_LENGTH}s/\"id\": 86/\"id\": 27/g" $file
    sed -i -- "${SPIKER_START},${FILE_LENGTH}s/\"id\": 87/\"id\": 27/g" $file
    sed -i -- "${SPIKER_START},${FILE_LENGTH}s/\"id\": 88/\"id\": 27/g" $file
    sed -i -- "${SPIKER_START},${FILE_LENGTH}s/\"id\": 89/\"id\": 27/g" $file
    sed -i -- "${SPIKER_START},${FILE_LENGTH}s/\"id\": 91/\"id\": 27/g" $file
    sed -i -- "${SPIKER_START},${FILE_LENGTH}s/\"id\": 92/\"id\": 27/g" $file
    sed -i -- "${SPIKER_START},${FILE_LENGTH}s/\"id\": 93/\"id\": 27/g" $file
    sed -i -- "${SPIKER_START},${FILE_LENGTH}s/\"id\": 94/\"id\": 27/g" $file
    sed -i -- "${SPIKER_START},${FILE_LENGTH}s/\"id\": 98/\"id\": 27/g" $file
    sed -i -- "${SPIKER_START},${FILE_LENGTH}s/\"id\": 99/\"id\": 27/g" $file
    sed -i -- "${SPIKER_START},${FILE_LENGTH}s/\"id\": 100/\"id\": 27/g" $file
  done
done
```
