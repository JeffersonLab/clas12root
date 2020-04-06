FROM uofscphysics/root:v6_18_02
# Container with root and python/Jupyter notebooks already installed
# Dockerfiles and build at https://github.com/uofscphysics/dockerfiles

LABEL maintainer "Nick Tyler <tylern@jlab.org>"
USER root

# Setup evn for RCDB
ARG RCDB_VERSION=v0.06.00
ENV RCDB_HOME /usr/local/rcdb
ENV LD_LIBRARY_PATH $RCDB_HOME/cpp/lib:$LD_LIBRARY_PATH
ENV PYTHONPATH $RCDB_HOME/python:$PYTHONPATH
ENV PATH $RCDB_HOME:$RCDB_HOME/bin:$RCDB_HOME/cpp/bin:$PATH

# Clone newest version of RCDB
# Could be changed to a specific version 
RUN git clone --recurse-submodules --single-branch --branch ${RCDB_VERSION} \
    https://github.com/JeffersonLab/rcdb.git /usr/local/rcdb \
    && rm -rf /usr/local/rcdb/.git

# Setup env for clas12root
ARG CLAS12ROOT_VERSION=1.4
ENV CLAS12ROOT /usr/local/clas12root
ENV PATH $PATH:$CLAS12ROOT/bin

# Build and install clas12root
RUN git clone --recurse-submodules --single-branch --branch ${CLAS12ROOT_VERSION} \
    https://github.com/jeffersonlab/clas12root.git ${CLAS12ROOT} \
    && mkdir -p ${CLAS12ROOT}/build \
    && cd ${CLAS12ROOT}/build \
    && cmake .. \
    && make -j$(nproc) \
    && cmake .. \
    && make install

USER user

# Set the entrypoint to clas12root so that it can be used as if it is a local executable
# Examples and functions for running in the readme file
ENTRYPOINT ["clas12root"]