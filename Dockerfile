FROM uofscphysics/root:v6_18_02
# Container with root and python/Jupyter notebooks already installed
# Dockerfiles and build at https://github.com/uofscphysics/dockerfiles

LABEL maintainer "Nick Tyler <tylern@jlab.org>"
USER root

# Setup evn for RCDB
ENV RCDB_HOME /usr/local/rcdb
ENV LD_LIBRARY_PATH $RCDB_HOME/cpp/lib:$LD_LIBRARY_PATH
ENV PYTHONPATH $RCDB_HOME/python:$PYTHONPATH
ENV PATH $RCDB_HOME:$RCDB_HOME/bin:$RCDB_HOME/cpp/bin:$PATH

# Clone newest version of RCDB
# Could be changed to a specific version 
RUN git clone --recurse-submodules https://github.com/JeffersonLab/rcdb.git /usr/local/rcdb

# Setup env for clas12root
ENV CLAS12ROOT /usr/local/clas12root
ENV PATH $PATH:$CLAS12ROOT/bin

# Copy in the local directory contents into the contsiner
# Allows for builds on local changes for easier development and ci
COPY . /usr/local/clas12root

# Build and install clas12root
RUN mkdir -p /usr/local/clas12root/build \
    && cd /usr/local/clas12root/build \
    && cmake .. \
    && make -j$(nproc) \
    && make install

# Copy pcm to the library directory to fix library not found errors
# TODO: Find a better fix for this, probably by modifying CMake files
RUN cp /usr/local/clas12root/build/Clas12Banks/libClas12Banks_rdict.pcm /usr/local/clas12root/lib
RUN cp /usr/local/clas12root/build/Clas12Root/libClas12Root_rdict.pcm /usr/local/clas12root/lib
RUN cp /usr/local/clas12root/build/hipo4/libHipo4_rdict.pcm /usr/local/clas12root/lib

USER user

# Set the entrypoint to clas12root so that it can be used as if it is a local executable
# Examples and functions for running in the readme file
ENTRYPOINT ["clas12root"]