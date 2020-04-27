#!/bin/bash
# sed magic adapted from https://unix.stackexchange.com/a/200576

KO_PATH=$1

# split into chunks to do knockouts on individual genome components
  # 0th chunk: nada (ok to still sed it though!)
  # odd chunks: pointer
  # even chunks: spiker
csplit --suffix-format="%09d" ${KO_PATH} '/program.*{$/' '{*}'               \
  > /dev/null # ignore byte counts printed to stdout

# knockout pointer components, genome by genome
# 121, TryAddFledglingConnection
# 27,Nop

for f in xx*1 xx*3 xx*5 xx*7 xx*9; do
  sed -i -- "/\"id\": 27\$/{
    h
    s//\"id\": 121/g
    H
    x
    s/\n/ >>> /
    w changelog${f}
    x
  }" $f &
  while [ $(jobs -r | wc -l) -gt 100 ]; do sleep 1; done
done

# knockout spiker components, genome by genome
# 98, TryAddFledglingConnection
# 27,Nop

for f in xx*0 xx*2 xx*4 xx*6 xx*8; do
  sed -i -- "/\"id\": 27\$/{
    h
    s//\"id\": 98/g
    H
    x
    s/\n/ >>> /
    w changelog${f}
    x
  }" $f &
  while [ $(jobs -r | wc -l) -gt 100 ]; do sleep 1; done
done

# recombine components and delete fragments
wait
cat xx* > ${KO_PATH}
rm xx*

cat changelogxx* > "${KO_PATH}___knockout_changelog+ext=.txt"
rm changelogxx*
