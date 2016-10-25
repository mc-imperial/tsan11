FROM phusion/baseimage:0.9.18

RUN /etc/my_init.d/00_regen_ssh_host_keys.sh

RUN echo "deb http://archive.ubuntu.com/ubuntu/ trusty main universe multiverse restricted" > /etc/apt/sources.list
RUN echo "deb http://archive.ubuntu.com/ubuntu/ trusty-updates main universe multiverse restricted" >> /etc/apt/sources.list
RUN echo ttf-mscorefonts-installer msttcorefonts/accepted-mscorefonts-eula select true | debconf-set-selections


RUN \
  apt-get update && \
  apt-get -y install \
    msttcorefonts \
    python autoconf2.13 build-essential libgtk2.0-dev libgtk-3-dev libxt-dev libgconf2-dev libdbus-glib-1-dev yasm \
    libao-dev libpulse-dev \
    ccache python-dev python-pip python-setuptools unzip uuid zip \
    libasound2-dev \
    subversion wget git ipython3 time mercurial xauth x11-apps \
    emacs nano \
    \
    apache2-bin \
    bison \
    cdbs curl \
    devscripts dpkg-dev \
    elfutils \
    fakeroot flex fonts-thai-tlwg \
    g++ git-core git-svn gperf \
    language-pack-da language-pack-fr language-pack-he language-pack-zh-hant \
    lib32gcc1 lib32stdc++6 libapache2-mod-php5 libasound2 libasound2-dev libatk1.0-0 libatk1.0-dbg \
    libav-tools \
    libbluetooth-dev libbrlapi-dev libbrlapi0.6 libbz2-1.0 \
    libbz2-dev \
    libc6 libc6-dbg libc6-i386 libcairo2 libcairo2-dbg libcairo2-dev libcap-dev \
    libcap2 libcups2 libcups2-dev libcurl4-gnutls-dev \
    libdrm-dev \
    libelf-dev libexpat1 \
    libffi-dev libffi6 libffi6-dbg libfontconfig1 libfontconfig1-dbg libfreetype6 \
    libgbm-dev libgconf2-dev libgl1-mesa-dev libgles2-mesa-dev libglib2.0-0 \
    libglib2.0-0-dbg libglib2.0-dev libglu1-mesa-dev libgnome-keyring-dev libgnome-keyring0 \
    libgtk2.0-0 libgtk2.0-0-dbg libgtk2.0-dev \
    libjpeg-dev \
    libkrb5-dev \
    libnspr4 libnspr4-dbg libnspr4-dev libnss3 libnss3-dbg libnss3-dev \
    libpam0g libpam0g-dev libpango1.0-0 libpango1.0-0-dbg libpci-dev libpci3 libpcre3 libpcre3-dbg libpixman-1-0 \
    libpixman-1-0-dbg libpng12-0 libpulse-dev libpulse0 \
    libsctp-dev libspeechd-dev libspeechd2 libsqlite3-0 libsqlite3-0-dbg libsqlite3-dev libssl-dev libstdc++6 \
    libstdc++6-4.8-dbg \
    libudev-dev libudev1 \
    libwww-perl \
    libx11-6 libx11-6-dbg libx11-xcb1 libx11-xcb1-dbg libxau6 libxau6-dbg libxcb1 libxcb1-dbg \
    libxcomposite1 libxcomposite1-dbg libxcursor1 libxcursor1-dbg libxdamage1 libxdamage1-dbg libxdmcp6 libxdmcp6-dbg \
    libxext6 libxext6-dbg libxfixes3 libxfixes3-dbg libxi6 libxi6-dbg libxinerama1 libxinerama1-dbg \
    libxkbcommon-dev libxrandr2 libxrandr2-dbg libxrender1 libxrender1-dbg libxslt1-dev libxss-dev \
    libxt-dev libxtst-dev libxtst6 libxtst6-dbg \
    mesa-common-dev \
    openbox \
    patch perl php5-cgi pkg-config python python-cherrypy3 python-crypto python-dev python-numpy python-opencv \
    python-openssl python-psutil python-yaml \
    realpath rpm ruby \
    subversion \
    ttf-dejavu-core ttf-indic-fonts ttf-kochi-gothic ttf-kochi-mincho \
    wdiff \
    xcompmgr xfonts-mathml \
    zip zlib1g zlib1g-dbg && \
  apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

COPY . /data/tsan11/

RUN \
  adduser --disabled-password --gecos "" paul && \
  chown -R paul:paul /data && \
  /data/tsan11/scripts/enable_insecure_key paul && \
  /usr/sbin/enable_insecure_key && \
  rm -f /etc/service/sshd/down

USER paul

RUN /data/tsan11/docker/build

USER root

CMD ["/sbin/my_init"]
