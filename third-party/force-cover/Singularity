################################################################################
# Basic bootstrap definition to build Ubuntu container from Docker container
################################################################################

Bootstrap:docker
From:ubuntu:18.04

%labels
Maintainer Emily Dolson
Version 0.0.0

################################################################################
# Copy any necessary files into the container
################################################################################
%files
. /opt/force-cover

%post
################################################################################
# Install additional packages
################################################################################
apt-get update
apt-get install -y software-properties-common

apt-get install -y wget

add-apt-repository -y ppa:ubuntu-toolchain-r/test
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -
apt-add-repository "deb https://apt.llvm.org/xenial/ llvm-toolchain-xenial-7 main"

apt-get update -qq

apt-get install -y -qq g++-8
update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-8 90
apt-get install -y cmake build-essential python-virtualenv python-pip nodejs tar gzip libclang-7-dev llvm-7 clang-7 libstdc++-7-dev gdb

update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-7 1000
update-alternatives --install /usr/bin/llvm-config llvm-config /usr/bin/llvm-config-7 1000

################################################################################
# Run the user's login shell, or a user specified command
################################################################################
%runscript
echo "Nothing here but us birds!"
