FROM ubuntu:plucky

RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y git wget flex bison gperf python3 python3-pip python3-venv\
 cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0

RUN mkdir /esp
WORKDIR /esp
RUN git clone -b v5.5.2 --recursive https://github.com/espressif/esp-idf.git
WORKDIR /esp/esp-idf/
RUN ./install.sh

RUN mkdir -p /esp

RUN apt-get install -y usbutils minicom nano bear clangd

WORKDIR /src
ENTRYPOINT [ "/src/entrypoint.sh" ]