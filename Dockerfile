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
    imagemagick \
    ioping \
    libcurl4-openssl-dev \
    libdw-dev \
    libomp-dev \
    libsfml-dev \
    locales \
    rdfind \
    rename \
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

# set up locale
# adapted from https://stackoverflow.com/a/55077451
RUN \
  locale-gen en_US.UTF-8 \
    && \
  echo "generated en_US.UTF-8 locale"

# install python3.8
RUN \
  add-apt-repository -y ppa:deadsnakes/ppa \
    && \
  apt-get install -y python3.8 python3.8-distutils \
    && \
  apt-get clean \
    && \
  rm -rf /var/lib/apt/lists/* \
    && \
  echo "installed python3.8"

COPY container/policy.xml /etc/ImageMagick-6/policy.xml

COPY container/ccache.conf /etc/ccache.conf
ENV CCACHE_CONFIGPATH=/etc/ccache.conf

RUN \
  python3 -m pip install --no-cache-dir --timeout 60 --retries 100 --upgrade pip==21.0.1 \
    && \
  python3.8 -m pip install --no-cache-dir --timeout 60 --retries 100 --upgrade pip==21.0.1 \
    && \
  echo "upgraded pip"

# need --ignore-installed for docutils
# see https://stackoverflow.com/a/53807588
RUN \
  python3 -m pip install --no-cache-dir --timeout 60 --retries 100 --ignore-installed -r /opt/dishtiny/third-party/requirements.txt \
    && \
  python3.8 -m pip install --no-cache-dir --timeout 60 --retries 100 --ignore-installed -r /opt/dishtiny/third-party/requirements.txt \
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

# must be installed as user for executable to be available on PATH
RUN \
  pip3 install --timeout 60 --retries 100 editorconfig-checker==2.3.54 \
    && \
  ln -s /home/user/.local/bin/ec /home/user/.local/bin/editorconfig-checker \
    && \
  echo "installed editorconfig-checker"

# adapted from https://askubuntu.com/a/799306
# and https://stackoverflow.com/a/38905161
ENV PATH "/home/user/.local/bin:$PATH"

# python needs this
ENV LC_ALL=C.UTF-8
ENV LANG=C.UTF-8

# default to clang, which gives ~5% better performance than GCC
ENV MPICH_CXX=clang++
ENV OMPI_CXX=clang++

# Define default working directory.
WORKDIR /opt/dishtiny
