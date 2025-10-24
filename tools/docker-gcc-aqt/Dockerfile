FROM ubuntu:22.04
ARG AQT_VERSION="aqtinstall==3.3.0"
ARG QT_PATH="/opt/Qt"
ARG QT_VERSION="6.9.3"
ARG LINUXDEPLOY_VERSION="1-alpha-20250213-2"
ENV DEBIAN_FRONTEND=noninteractive \
    DEBCONF_NONINTERACTIVE_SEEN=true \
    AQT_VERSION=${AQT_VERSION} \
    QT_PATH=${QT_PATH} \
    QT_VERSION=${QT_VERSION} \
    PATH=${QT_PATH}/Tools/CMake/bin:${QT_PATH}/Tools/Ninja:${QT_PATH}/${QT_VERSION}/gcc_64/bin:$PATH

RUN apt update && apt upgrade -y && \
    apt install -y --no-install-recommends sudo locales git openssh-client curl ca-certificates 7zip build-essential python3 patchelf file pandoc libgl1-mesa-dev && \
    echo 'ubuntu ALL=NOPASSWD: ALL' > /etc/sudoers.d/ubuntu && \
    locale-gen en_US.UTF-8 && dpkg-reconfigure locales && \
    groupadd -r ubuntu && useradd --create-home --gid ubuntu ubuntu && \
    echo 'ubuntu ALL=NOPASSWD: ALL' > /etc/sudoers.d/ubuntu && \
    apt -qq clean && rm -rf /var/lib/apt/lists/* /tmp/*

COPY --chmod=755 install.sh /tmp/
RUN /tmp/install.sh && \
    apt -qq clean && rm -rf /var/lib/apt/lists/* /tmp/*

USER ubuntu
WORKDIR /home/ubuntu
ENV HOME=/home/ubuntu
