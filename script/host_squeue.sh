#!/bin/bash

# load secrets into environment variables, if available
[[ -f ~/.secrets.sh ]] && source ~/.secrets.sh

for retry in {0..20}; do

  if ((${retry}==20)); then echo "host_squeue.sh failed" && exit 1; fi

  sshpass -p "${HOST_PASSWORD}" \
    ssh -q -o "StrictHostKeyChecking no" -o "UserKnownHostsFile /dev/null" \
      "${HOST_USERNAME}@$(hostname)" \
      "squeue $@" \
  && break

done
