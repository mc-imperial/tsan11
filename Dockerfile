FROM phusion/baseimage:0.9.18

RUN \
  apt-get update && \
  apt-get -y install \
    python autoconf2.13 build-essential libgtk2.0-dev libgtk-3-dev libxt-dev libgconf2-dev libdbus-glib-1-dev yasm \
    libao-dev libpulse-dev \
    ccache python-dev python-pip python-setuptools unzip uuid zip \
    libasound2-dev \
    subversion wget git ipython3 time mercurial && \
  apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

COPY . /data/tsan11/

RUN adduser --disabled-password --gecos "" paul

RUN /data/tsan11/scripts/enable_insecure_key paul

RUN /usr/sbin/enable_insecure_key

RUN rm -f /etc/service/sshd/down

RUN /etc/my_init.d/00_regen_ssh_host_keys.sh

CMD ["/sbin/my_init"]
