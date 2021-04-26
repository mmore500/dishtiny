#!/bin/sh

################################################################################
echo
echo "running setup_cloudshell.sh"
echo "---------------------------"
################################################################################

export DEBIAN_FRONTEND="noninteractive"

mkdir -p ~/.cloudshell/ && touch ~/.cloudshell/no-apt-get-warning
sudo wget -O- http://neuro.debian.net/lists/xenial.us-ca.full | sudo tee /etc/apt/sources.list.d/neurodebian.sources.list
sudo apt-key adv --recv-keys --keyserver hkp://pool.sks-keyservers.net:80 0xA5D32F012649A5A9
sudo apt-get update
sudo apt-get install -y docker singularity-container xvfb

export DISPLAY=":$$"
rm -f "/tmp/.X$$-lock"
Xvfb "${DISPLAY}" -auth /dev/null/ &
export XVFB_PID=$!

yes | docker system prune --all
rm -rf ~/.singularity/docker/
