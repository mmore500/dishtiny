# Source Update

`2**20`

# ID Key

1. knockout
  * `BcstMsgExternal`
  * `SendMsgExternal`
2. knockout
  * `BcstMsgExternal`
  * `SendMsgExternal`
  * `SendBigFracResource`
  * `SendSmallFracResource`

# Knockout Prep

```
for f in *; do cp $f id=1+$f; done
for f in c*; do mv $f id=2+$f; done
sed -i -- 's/BcstMsgExternal/Nop/g' id\=1+component\=program*
sed -i -- 's/SendMsgExternal/Nop/g' id\=1+component\=program*
sed -i -- 's/BcstMsgExternal/Nop/g' id\=2+component\=program*
sed -i -- 's/SendMsgExternal/Nop/g' id\=2+component\=program*
sed -i -- 's/SendBigFracResource/Nop/g' id\=2+component\=program*
sed -i -- 's/SendSmallFracResource/Nop/g' id\=2+component\=program*
```
