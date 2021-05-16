FROM mmore500/conduit@sha256:b1590176a947b3d3887470c3dee6d81243c7f05c9f81388bffd67b903586037f

USER root

COPY . /opt/signalgp-lite

RUN \
  pip3 install -r /opt/signalgp-lite/docs/requirements.txt \
    && \
  echo "installed documentation build requirements"

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
