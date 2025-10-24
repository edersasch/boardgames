FROM ubuntu:24.04
ARG AQT_VERSION="3.3.0"
ARG QT_PATH=C:\\Qt
ARG QT_VERSION="6.9.3"
ENV DEBIAN_FRONTEND=noninteractive \
    DEBCONF_NONINTERACTIVE_SEEN=true \
    AQT_VERSION=${AQT_VERSION} \
    QT_PATH=${QT_PATH} \
    QT_VERSION=${QT_VERSION} \
    WINEDEBUG=-all

RUN apt update && apt upgrade -y && \
    apt install -y --no-install-recommends sudo locales git openssh-client curl ca-certificates 7zip wine && \
    echo 'ubuntu ALL=NOPASSWD: ALL' > /etc/sudoers.d/ubuntu && \
    locale-gen en_US.UTF-8 && dpkg-reconfigure locales && \
    apt -qq clean && rm -rf /var/lib/apt/lists/* /tmp/*

USER ubuntu
WORKDIR /home/ubuntu
ENV HOME=/home/ubuntu

COPY --chown=ubuntu:ubuntu --chmod=755 install.sh /tmp/
RUN /tmp/install.sh && \
    sudo -E apt -qq clean && sudo -E rm -rf /var/lib/apt/lists/* /tmp/*
