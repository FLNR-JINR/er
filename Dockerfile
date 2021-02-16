ARG ER=dev
ARG ACCDAQ=master
ARG DEPENDENCIES=ghcr.io/flnr-jinr/fs_oct17p4:latest

FROM ${DEPENDENCIES}

WORKDIR /opt

RUN git clone https://github.com/flnr-jinr/ACCULINNA_go4_user_library accdaq &&\
	cd accdaq &&\
	git checkout ${ACCDAQ} &&\
	mkdir build && cd build &&\
	/bin/bash -c 'source /opt/FairSoft/bin/thisroot.sh && source /opt/go4/go4login && cmake ../ -DCMAKE_INSTALL_PREFIX=/opt/accdaq/install' &&\
	make install -j4 

RUN cd /opt && git clone https://github.com/flnr-jinr/er &&\
	cd er &&\
	git checkout ${ER} &&\
	export SIMPATH=/opt/FairSoft/ &&\
	export FAIRROOTPATH=/opt/FairRoot/ &&\
	mkdir build &&\
	cd build &&\
	cmake ../ -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DACCULINNA_GO4=/opt/accdaq/install &&\
	make -j4


