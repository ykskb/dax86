FROM bitnami/minideb:jessie

RUN apt-get -y update && apt-get -y install build-essential nasm

WORKDIR /dax86/

ADD . /dax86/

USER root

CMD ["/bin/bash"]