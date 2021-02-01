# Installation

The sources for dishtiny can be downloaded from the [Github repo](https://github.com/mmore500/dishtiny).

You can either clone the public repository:

```bash

    $ git clone git://github.com/mmore500/dishtiny
```
Or download the [tarball](https://github.com/mmore500/dishtiny/tarball/master):

```bash
    $ curl  -OL https://github.com/mmore500/dishtiny/tarball/master
```

## Docker Container

This project has a [containerized build enviroment](https://docs.docker.com/engine/reference/commandline/build/) available with all its dependencies installed. Either build a copy of the container locally from the project's `Dockerfile`,
or get a copy of the container from [DockerHub](https://hub.docker.com/r/mmore500/dishtiny).

## Web-based Cloud Solution with GUI!

Go to <https://shell.cloud.google.com>.

```
sudo wget -O- http://neuro.debian.net/lists/xenial.us-ca.full | sudo tee /etc/apt/sources.list.d/neurodebian.sources.list && \
    sudo apt-key adv --recv-keys --keyserver hkp://pool.sks-keyservers.net:80 0xA5D32F012649A5A9 && \
    sudo apt-get update
sudo apt-get install -y singularity-container
singularity shell docker://mmore500/dishtiny
```

(At this point, it'll take a few minutes so go get yourself a cup of coffee :coffee:)

Then check out the code, make, and run.
```
git clone https://github.com/mmore500/dishtiny.git --recursive --shallow-submodules
cd dishtiny
make
mkdir temp && cd temp
cp ../rundishtiny .
./rundishtiny --RUN_SECONDS 30
```

As of Febuary 2021, Google provides this service free of charge!
