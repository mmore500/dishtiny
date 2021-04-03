FROM mmore500/conduit:sha-785924c@sha256:b849a941afe8fff6135b2b8d22e7f5d206ffe94b9aa1477922df362ae9a1d013

USER root

COPY . /opt/dishtiny

# checking out fails unless we deinit and then init
RUN \
  git -C /opt/dishtiny remote --verbose \
    && \
  git -C /opt/dishtiny submodule deinit . \
    && \
  git -C /opt/dishtiny submodule update --init --recursive \
    && \
  git -C /opt/dishtiny fetch --recurse-submodules --jobs 16 \
    && \
  echo "unshallowed source for use as cache"

RUN \
  echo "deb http://archive.ubuntu.com/ubuntu focal main restricted universe multiverse" >> "/etc/apt/sources.list" \
    && \
  echo "Package: libhdf5-dev" >> "/etc/apt/preferences" \
    && \
  echo "Pin: release n=bionic" >> "/etc/apt/preferences" \
    && \
  echo "Pin-priority: -10" >> "/etc/apt/preferences" \
    && \
  echo >> "/etc/apt/preferences" \
    && \
  echo "Package: libhdf5-dev" >> "/etc/apt/preferences" \
    && \
  echo "Pin: release n=focal" >> "/etc/apt/preferences" \
    && \
  echo "Pin-priority: 900" >> "/etc/apt/preferences" \
    && \
  apt-get update \
    && \
  apt-get upgrade -y \
    && \
  apt-get install -y aptitude \
    && \
  apt-get purge -y libhdf5-dev \
    && \
  aptitude install -y libhdf5-dev \
    && \
  echo "upgrade success"

RUN \
  apt-get update -qq \
    && \
  apt-get install -y --allow-downgrades --no-install-recommends \
    ccache \
    rdfind \
    imagemagick \
    ioping \
    libcurl4-openssl-dev \
    libsfml-dev \
    libdw-dev \
    libomp-dev \
    sharutils \
    sshpass \
    tree \
    xauth \
    && \
  apt-get clean \
    && \
  rm -rf /var/lib/apt/lists/* \
    && \
  echo "installed apt packages"

COPY container/policy.xml /etc/ImageMagick-6/policy.xml

COPY container/ccache.conf /etc/ccache.conf
ENV CCACHE_CONFIGPATH=/etc/ccache.conf

RUN \
  pip3 install --upgrade pip==21.0.1 \
    && \
  pip3 install --timeout 60 --retries 100 -r /opt/dishtiny/third-party/requirements.txt \
    && \
  echo "installed python requirements"

RUN \
  cd /opt/dishtiny/third-party \
    && \
  ./install_dependencies.sh \
    && \
  echo "installed third party dependencies"

# make sure unprivileged user has access to new files in opt
# adapted from https://stackoverflow.com/a/27703359
# and https://superuser.com/a/235398
RUN \
  chgrp --recursive user /opt/dishtiny \
    && \
  chmod --recursive g+rwx /opt/dishtiny \
    && \
  echo "user granted permissions to /opt/dishtiny"

USER user

# python needs this
ENV LC_ALL=C.UTF-8
ENV LANG=C.UTF-8

# default to clang, which gives ~5% better performance than GCC
ENV MPICH_CXX=clang++
ENV OMPI_CXX=clang++

# Define default working directory.
WORKDIR /opt/dishtiny
