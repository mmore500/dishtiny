FROM mmore500/conduit

USER root

COPY . /opt/signalgp-lite

RUN \
  apt-get install -qq \
    python-virtualenv \
    python3-virtualenv \
    python-pip \
    python3-pip \
    doxygen \
    && \
  echo "installed docs dependencies"

RUN \
  pip3 install -r /opt/signalgp-lite/docs/requirements.txt \
    && \
  echo "installed documentation build requirements"

RUN \
  add-apt-repository -y ppa:ubuntu-toolchain-r/test \
    && \
  wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - \
    && \
  apt-add-repository "deb https://apt.llvm.org/xenial/ llvm-toolchain-xenial-7 main" \
    && \
  apt-get update -qq \
    && \
  apt-get install -y cmake build-essential python-virtualenv python-pip nodejs tar gzip libclang-7-dev llvm-7 clang-7 libstdc++-7-dev gdb \
    && \
  update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-7 90 \
    && \
  update-alternatives --install /usr/bin/llvm-config llvm-config /usr/bin/llvm-config-7 90 \
    && \
  update-alternatives --install /usr/bin/llvm-profdata llvm-profdata /usr/bin/llvm-profdata-7 90 \
    && \
  update-alternatives --install /usr/bin/llvm-cov llvm-cov /usr/bin/llvm-cov-7 90 \
    && \
  echo "installed force cover build dependencies"

# make sure unprivileged user has access to new files in opt
# adapted from https://stackoverflow.com/a/27703359
# and https://superuser.com/a/235398
RUN \
  chgrp --recursive user /opt/signalgp-lite \
    && \
  chmod --recursive g+rwx /opt/signalgp-lite \
    && \
  echo "user granted permissions to /opt/signalgp-lite"

USER user

RUN \
  cd /opt/signalgp-lite \
    && \
  make install-test-dependencies \
    && \
  echo "installed test dependencies"

# Define default working directory.
WORKDIR /opt/signalgp-lite
