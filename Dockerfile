FROM bitnami/minideb:jessie

RUN apt-get update && apt-get install build-essential nasm

WORKDIR /dax86/

ADD . /dax86/

USER root

CMD ["/bin/sh"]