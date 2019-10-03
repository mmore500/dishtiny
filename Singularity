################################################################################
# Basic bootstrap definition to build Ubuntu container from Docker container
################################################################################

Bootstrap:docker
From:ubuntu:latest

%labels
Maintainer Matthew Andres Moreno
Version 0.0.0

################################################################################
# Copy any necessary files into the container
################################################################################
%files
. /opt/dishtiny

%post
################################################################################
# Install additional packages
################################################################################
apt-get update

apt-get install -y software-properties-common
add-apt-repository ppa:ubuntu-toolchain-r/test
apt-get update
apt-get install -y build-essential
apt-get install -y gcc-8 g++-8

apt-get install -y git
apt-get install -y make
apt-get install -y python3
apt-get install -y python3-pip
apt-get install -y ffmpeg
apt-get install -y imagemagick
apt-get install -y mpich
apt-get install -y hdf5-helpers hdf5-tools

update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-8 10
update-alternatives --set g++ "/usr/bin/g++-8"

pip3 install numpy
pip3 install matplotlib
pip3 install pandas
pip3 install seaborn
pip3 install networkx
pip3 install keyname
pip3 install fileshash
pip3 install h5py
pip3 install tqdm
pip3 install joblib
pip3 install sklearn

# rm -rf /opt/Empirical
git clone https://github.com/devosoft/Empirical /opt/Empirical -b match-bin

chmod 777 -R /opt

cd /opt/dishtiny && make static && cd ..

# rm -rf /usr/bin/dishtiny
ln -s /opt/dishtiny/dishtiny /usr/bin/

################################################################################
# Run the user's login shell, or a user specified command
################################################################################
%runscript
echo "Nothing here but us birds!"
