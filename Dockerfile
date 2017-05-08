# Base Image
FROM alpine:latest

# Add Required Packages [Layer 1]
RUN apk update && \
    apk add ca-certificates cmake g++ git make

# Download Sources [Layer 2]
RUN cd ~ && \
    git clone https://github.com/remote-wiring/remote-wiring.git --recursive && \
    cd remote-wiring/ && \
    mkdir build

# Verify Build and Install
CMD cd ~/remote-wiring/build/ && \
    cmake .. && \
    make
