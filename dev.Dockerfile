FROM uofscphysics/root:v6_18_02
# Container with root and python/Jupyter notebooks already installed
# Dockerfiles and build at https://github.com/uofscphysics/dockerfiles

LABEL maintainer "Nick Tyler <tylern@jlab.org>"
USER root

# Setup env for RCDB
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

# Setup env for CCDB
ARG CCDB_VERSION=scon-python-fix
ENV CCDB_HOME /usr/local/ccdb
ENV LD_LIBRARY_PATH $CCDB_HOME/lib:$LD_LIBRARY_PATH
ENV PYTHONPATH $CCDB_HOME/python:$PYTHONPATH
ENV PATH $CCDB_HOME:$CCDB_HOME/bin:$PATH

# Install scons for building CCDB
RUN git clone https://github.com/SCons/scons.git /usr/local/scons
RUN cd /usr/local/scons \
    && python setup.py install

# Clone CCDB (version defined above)
RUN git clone --depth 1 --branch ${CCDB_VERSION} \ 
    https://github.com/ademus4/ccdb.git /usr/local/ccdb 

RUN cd $CCDB_HOME \
    && scons with-mysql=false

# Setup env for clas12root
ARG CLAS12ROOT_VERSION=development
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
    && cmake .. \
    && make install

USER user

# Set the entrypoint to clas12root so that it can be used as if it is a local executable
# Examples and functions for running in the readme file
ENTRYPOINT ["zsh"]