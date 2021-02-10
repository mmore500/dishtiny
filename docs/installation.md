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

## Hassle-free Cloud-based Development Environment with Web GUI ::sunglasses::

Go to <https://shell.cloud.google.com>.
As of Febuary 2021, Google provides this service free of charge!

```bash
curl -s https://raw.githubusercontent.com/mmore500/dishtiny/master/script/setup_cloudshell.sh | sudo bash

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

Compiling takes a minute or so and dumping data (in particular, png grid visualizations) takes a few minutes.
That's also okay!

If you want to build and run the web app from Cloud Shell, run these commands from inside the container in the `dishtiny` source directory,
```bash
cd third-party/ && ./install_emsdk.sh; cd -
make web
cd web && python3 -m SimpleHTTPServer 8080
```

Then, use Cloud Shell's [Web Preview](https://cloud.google.com/shell/docs/using-web-preview) feature to view the web app in your own browser.
As of February 2021, there's a button in the top right hand corner of the user interface that lets you launch the web preview.
(Note that, for now, the web app will only work properly in Google Chrome.)

Singularity mounts most of the host filesystem, meaning that any changes you make using the web editor will seamlessly apply inside the container.
Nice!

Your source code files should persist across Cloud Shell sessions, but your development environment only persists for the duration of your session.
This means that you'll need to re-run the environment setup steps
Kind of annoying, but on the plus side if you accidentally bork your development environment you can open up a new session for a fresh start!
::man_shrugging:: ::rocket::
