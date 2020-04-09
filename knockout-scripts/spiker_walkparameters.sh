#!/bin/bash

KO_PATH="seedpop/id=1+component=genome+count=1+seed=5066+title=dominant+treat=standard+update=328832+_emp_hash=1874d2a-clean+_source_hash=aa98eba-clean+ext=.json.cereal"

# split into chunks to do knockouts on individual genome components
  # 0th chunk: nada (ok to still sed it though!)
  # odd chunks: pointer
  # even chunks: spiker
csplit --suffix-format="%09d" ${KO_PATH} '/program.*{$/' '{*}'               \
  > /dev/null # ignore byte counts printed to stdout

# knockout pointer components, genome by genome
# NADA

# knockout spiker components, genome by genome
# 27,Nop
# 101,SetConnectionAgingParam
# 102,SetConnectionExploitParam
# 103,SetConnectionDevelopmentParam
# 104,SetConnectionSensingParam

for f in xx*0 xx*2 xx*4 xx*6 xx*8; do
  sed -i -- "s/\"id\": 101\$\|\"id\": 102\$\|\"id\": 103\$\|\"id\": 104\$/\"id\": 27/g" $f &
  while [ $(jobs -r | wc -l) -gt 100 ]; do sleep 1; done
done

# recombine components and delete fragments
wait
cat xx* > ${KO_PATH}
rm xx*
