FROM ubuntu:14.04

RUN \
  apt-get update && \
  apt-get -y install \
    python autoconf2.13 build-essential libgtk2.0-dev libgtk-3-dev libxt-dev libgconf2-dev libdbus-glib-1-dev yasm \
    libao-dev libpulse-dev \
    ccache python-dev python-pip python-setuptools unzip uuid zip \
    libasound2-dev

RUN apt-get -y install subversion wget git ipython3 time

RUN apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

COPY . /data/tsan11/

CMD ["/data/tsan11/build_env.sh.template"]

