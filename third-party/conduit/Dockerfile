# Pull base image.
FROM ubuntu:bionic-20180125@sha256:d6f6cc62b6bed64387d84ca227b76b9cc45049b0d0aefee0deec21ed19a300bf

COPY . /opt/conduit/

SHELL ["/bin/bash", "-c"]

# Define default working directory.
WORKDIR /opt/conduit

# Prevent interactive time zone config.
# adapted from https://askubuntu.com/a/1013396
ENV DEBIAN_FRONTEND=noninteractive

# adapted from https://users.open-mpi.narkive.com/tEPxZF0B/ompi-users-how-to-get-rid-of-openmpi-warning-unable-to-find-any-relevant-network-interfaces
# see also https://github.com/open-mpi/ompi-www/issues/161#issue-390004007
RUN \
  mkdir -p "/root/.openmpi" \
    && \
  mkdir -p "/home/user/.openmpi" \
    && \
  echo "btl_base_warn_component_unused = 0" >> /etc/openmpi-mca-params.conf \
    && \
  echo "btl_base_warn_component_unused = 0" >> /root/.openmpi/mca-params.conf \
    && \
  echo "btl_base_warn_component_unused = 0" >> /home/user/.openmpi/mca-params.conf \
    && \
  echo "configured system-wide openmpi mca parameters"

RUN \
  echo 'Acquire::http::Timeout "60";' >> "/etc/apt/apt.conf.d/99timeout" \
    && \
  echo 'Acquire::ftp::Timeout "60";' >> "/etc/apt/apt.conf.d/99timeout" \
    && \
  echo 'Acquire::Retries "100";' >> "/etc/apt/apt.conf.d/99timeout" \
    && \
  echo "buffed apt-get resiliency"

RUN \
  find /etc/apt -type f -name '*.list' -exec sed -i 's/\(^deb.*-backports.*\)/#\1/; s/\(^deb.*-updates.*\)/#\1/; s/\(^deb.*-proposed.*\)/#\1/; s/\(^deb.*-security.*\)/#\1/' {} + \
    && \
  rm -rf /var/lib/apt/lists/* \
    && \
  echo "removed -backports, -updates, -proposed, -security repositories"

RUN \
  apt-get update -qq \
    && \
  apt-get install -y --allow-downgrades --no-install-recommends \
    build-essential \
    ca-certificates \
    cmake \
    curl \
    doxygen \
    emacs \
    fonts-liberation \
    gconf-service \
    gdb \
    gpg-agent \
    gzip \
    hdf5-helpers \
    hdf5-tools \
    htop \
    libappindicator1 \
    libasound2 \
    libatk1.0-0 \
    libc6 \
    libcairo2 \
    libcups2 \
    libcurl4 \
    libcurl4-openssl-dev \
    libdbus-1-3 \
    libexpat1 \
    libfontconfig1 \
    libgconf-2-4 \
    libgdk-pixbuf2.0-0 \
    libglib2.0-0 \
    libgtk-3-0 \
    libhdf5-100 \
    libhdf5-cpp-100 \
    libhdf5-dev \
    libhdf5-doc \
    libhdf5-mpi-dev \
    libhdf5-mpich-100 \
    libhdf5-mpich-dev \
    libhdf5-openmpi-100 \
    libhdf5-openmpi-dev \
    libhdf5-serial-dev \
    libmpich-dev \
    libmpich12 \
    libnspr4 \
    libnss3 \
    libopenmpi-dev \
    libopenmpi2 \
    libpango-1.0-0 \
    libpangocairo-1.0-0 \
    libpthread-stubs0-dev \
    libstdc++-7-dev \
    libstdc++6=8-20180414-1ubuntu2 \
    libx11-6 \
    libx11-xcb1 \
    libxcb1 \
    libxcomposite1 \
    libxcursor1 \
    libxdamage1 \
    libxext6 \
    libxfixes3 \
    libxi6 \
    libxrandr2 \
    libxrender1 \
    libxss1 \
    libxtst6 \
    lsb-release \
    man \
    mpich \
    multitail \
    nano \
    ninja-build \
    nodejs \
    npm \
    openmpi-bin \
    openmpi-common \
    openmpi-doc \
    openssh-server \
    python-dev \
    python-h5py \
    python-pip \
    python-setuptools \
    python-sphinx \
    python-virtualenv \
    python-wheel \
    python3-dev \
    python3-h5py \
    python3-pip \
    python3-setuptools \
    python3-sphinx \
    python3-virtualenv \
    python3-wheel \
    rsync \
    slurm-client \
    software-properties-common \
    tar \
    unzip \
    vim \
    wget \
    xdg-utils \
    && \
  add-apt-repository ppa:git-core/ppa -y \
    && \
  apt-get update -qq \
    && \
  apt-get install -y --no-install-recommends git \
    && \
  apt-get clean \
    && \
  rm -rf /var/lib/apt/lists/* \
    && \
  echo "installed fundamentals"

# adapted in part from https://askubuntu.com/a/916451
RUN \
  apt-get update -qq \
    && \
  rm /etc/apt/apt.conf.d/docker-gzip-indexes \
    && \
  apt-get purge apt-show-versions \
    && \
  rm /var/lib/apt/lists/*lz4 \
    && \
  apt-get -o Acquire::GzipIndexes=false update \
    && \
  add-apt-repository -y ppa:ubuntu-toolchain-r/test \
    && \
  wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - \
    && \
  apt-add-repository "deb https://apt.llvm.org/xenial/ llvm-toolchain-xenial-7 main" \
    && \
  add-apt-repository -y ppa:ubuntu-toolchain-r/test \
    && \
  apt-get update -qq \
    && \
  apt-get clean \
    && \
  rm -rf /var/lib/apt/lists/* \
    && \
  echo "configured packaging system"

RUN \
  apt-get update -qq \
    && \
  apt-get install -qq \
    libclang-7-dev=1:7.1.0~svn353565-1~exp1~20190408084827.60 \
    llvm-7=1:7.1.0~svn353565-1~exp1~20190408084827.60 \
    llvm-7-dev=1:7.1.0~svn353565-1~exp1~20190408084827.60 \
    clang-7=1:7.1.0~svn353565-1~exp1~20190408084827.60 \
    g++-9=9.3.0-11ubuntu0~18.04.1 \
    && \
  apt-get clean \
    && \
  rm -rf /var/lib/apt/lists/* \
    && \
  echo "installed llvm-7 dependencies"

# magic from https://github.com/puppeteer/puppeteer/issues/3451#issuecomment-523961368
RUN echo 'kernel.unprivileged_userns_clone=1' > /etc/sysctl.d/userns.conf

RUN \
  pip3 install  --timeout 60 --retries 100 -r /opt/conduit/third-party/requirements.txt \
    && \
  pip3 install  --timeout 60 --retries 100 -r /opt/conduit/docs/requirements.txt \
    && \
  echo "installed Python packages"

RUN \
  update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 90 \
    && \
  update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-7 90 \
    && \
  update-alternatives --install /usr/bin/llvm-config llvm-config /usr/bin/llvm-config-7 90 \
    && \
  update-alternatives --install /usr/bin/llvm-profdata llvm-profdata /usr/bin/llvm-profdata-7 90 \
    && \
  update-alternatives --install /usr/bin/llvm-cov llvm-cov /usr/bin/llvm-cov-7 90 \
    && \
  npm install -g n@6.7.0 \
    && \
  n 12.18.2 \
    && \
  export python="/usr/bin/python" \
    && \
  npm install source-map@0.7.3 \
    && \
  echo "finalized dependency versions"

RUN \
  npm install -g jsonlint@1.6.3 \
    && \
  echo "installed npm dependencies"

RUN \
  cd third-party \
    && \
  ./install_dependencies.sh \
    && \
  echo "installed third party dependencies"

# Set enviroment variables
# Use mimalloc override within the container.
ENV LD_PRELOAD=/usr/local/lib/mimalloc-1.6/libmimalloc.so
ENV LC_ALL=C.UTF-8
ENV LANG=C.UTF-8

RUN \
  git remote set-url origin https://github.com/mmore500/conduit.git \
    && \
  echo "switched to https origin remote url"

# Perform any further action as an unprivileged user.
# adapted from https://stackoverflow.com/a/27703359
# and https://superuser.com/a/235398
RUN \
  useradd --create-home --shell /bin/bash user \
    && \
  groupadd group \
    && \
  gpasswd -a user group \
    && \
  chgrp --recursive user /opt \
    && \
  chmod --recursive g+rwx /opt \
    && \
  chmod --recursive g+rwx /home/user \
    && \
  chown -R user /home/user/ \
    && \
  echo "user added and granted permissions to /opt and /home/user"

RUN \
  mkdir /context/ \
    && \
  chown user:user /context/ \
    && \
  mkdir /__w/ \
    && \
  chown user:user /__w/ \
    && \
  echo "/context/ /__w/ directories set up, user granted permissions"

USER user

CMD ["bash"]
