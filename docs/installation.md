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

## Containerization

This project has a [containerized build enviroment](https://docs.docker.com/engine/reference/commandline/build/) available with all its dependencies installed. Either build a copy of the container locally from the project's `Dockerfile`,
or get a copy of the container from [DockerHub](https://hub.docker.com/r/mmore500/dishtiny).

If you have [Docker](https://docs.docker.com/) installed, you can launch an interactive shell inside the dishtiny container like so,
```bash
sudo docker run -it mmore500/dishtiny
```

Likewise, if you have [Singularity](https://sylabs.io/docs/) installed,
```bash
singularity shell docker://mmore500/dishtiny
```

## Hassle-free Cloud-based Development Environment with Web GUI |:sunglasses:|

Go to <https://shell.cloud.google.com>.
As of Febuary 2021, Google provides this service free of charge!

```bash
sudo wget -O- http://neuro.debian.net/lists/xenial.us-ca.full | sudo tee /etc/apt/sources.list.d/neurodebian.sources.list && \
    sudo apt-key adv --recv-keys --keyserver hkp://pool.sks-keyservers.net:80 0xA5D32F012649A5A9 && \
    sudo apt-get update
yes | sudo apt-get install -y singularity-container xvfb
export DISPLAY=":$$"
rm -f "/tmp/.X$$-lock"
Xvfb "${DISPLAY}" -auth /dev/null/ &
export XVFB_PID=$!
singularity shell docker://mmore500/dishtiny
```

This'll take a few minutes, so go get yourself a cup of coffee |:coffee:|.
When you get back, a shell prompt from the container should be ready to rock and roll.
At this point, you can check out the source code, make, and run!
```bash
git clone https://github.com/mmore500/dishtiny.git --recursive --shallow-submodules
cd dishtiny
make
mkdir temp && cd temp
cp ../rundishtiny .
./rundishtiny --RUN_SECONDS 30 --DATA_DUMP 1
```

You might see the message `(EE) Failed to open authorization file "/dev/null/": Not a directory`.
That's okay!

Compiling takes a minute or so and dumping data (in particular, PNG grid visualizations) takes a few minutes.
That's also okay!

Singularity mounts most of the host filesystem, meaning that any changes you make using the web editor will seamlessly apply inside the container. Nice!
