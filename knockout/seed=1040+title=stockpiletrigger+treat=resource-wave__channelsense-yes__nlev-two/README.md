# Source Update

`2**20`

# ID Key
1. wild type
2. knockout
  * IsPoorerThan trigger (idx 3) -> 0

# Knockout Prep

```
for f in *; do cp $f id=1+$f; done
for f in c*; do mv $f id=2+$f; done
sed -i -- 's/DuplicateAdjOwnRegulator/Nop/g' id\=2+component\=program*
sed -i -- 's/AdjOwnRegulator/Nop/g' id\=2+component\=program*
sed -i -- 's/DuplicateSetOwnRegulator/Nop/g' id\=2+component\=program*
sed -i -- 's/SetOwnRegulator/Nop/g' id\=2+component\=program*
sed -i -- 's/DuplicateAdjRegulator/Nop/g' id\=2+component\=program*
sed -i -- 's/AdjRegulator/Nop/g' id\=2+component\=program*
sed -i -- 's/DuplicateSetRegulator/Nop/g' id\=2+component\=program*
sed -i -- 's/SetRegulator/Nop/g' id\=2+component\=program*
```
