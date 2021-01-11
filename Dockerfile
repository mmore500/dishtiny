FROM mmore500/conduit

USER root

COPY . /opt/dishtiny

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
    rdfind \
    imagemagick \
    libcurl4-openssl-dev \
    libsfml-dev \
    libomp-dev \
    && \
  apt-get clean \
    && \
  rm -rf /var/lib/apt/lists/* \
    && \
  echo "installed apt packages"

RUN \
  pip3 install -r /opt/dishtiny/docs/requirements.txt \
    && \
  pip3 install -r /opt/dishtiny/third-party/requirements.txt \
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

# Define default working directory.
WORKDIR /opt/dishtiny
