FROM ubuntu:22.04
ARG ADDITIONAL_PACKAGES="sudo git openssh-client ca-certificates build-essential curl python3 locales patchelf file pandoc libgl1-mesa-dev"
ARG AQT_VERSION="aqtinstall==3.3.0"
ARG QT_PATH="/opt/Qt"
ARG QT_VERSION="6.9.0"
ARG LINUXDEPLOY_VERSION="1-alpha-20250213-2"
ENV DEBIAN_FRONTEND=noninteractive \
    DEBCONF_NONINTERACTIVE_SEEN=true \
    AQT_VERSION=${AQT_VERSION} \
    QT_PATH=${QT_PATH} \
    QT_VERSION=${QT_VERSION} \
    PATH=${QT_PATH}/Tools/CMake/bin:${QT_PATH}/Tools/Ninja:${QT_PATH}/${QT_VERSION}/gcc_64/bin:$PATH
COPY install.sh /tmp/

RUN /tmp/install.sh

USER user
WORKDIR /home/user
ENV HOME=/home/user