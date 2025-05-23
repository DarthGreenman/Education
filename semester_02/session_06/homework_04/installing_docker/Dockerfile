FROM ubuntu:16.04
COPY . /home/greenman/hd
WORKDIR /home/greenman/hd
RUN apt update
RUN apt install -y g++ nano
RUN g++ -o hello hello_docker.cpp
