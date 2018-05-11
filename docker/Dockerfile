FROM fairroot:17.04

WORKDIR /opt

RUN git clone https://github.com/ExpertRootGroup/er &&\
	cd er &&\
	git checkout dev

WORKDIR /opt/er
RUN	export SIMPATH=/opt/fair_install/FairSoft/installation &&\
	export FAIRROOTPATH=/opt/FairRoot/insttallation &&\
	mkdir build &&\
	cd build &&\
	cmake ../ -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ &&\
	make -j4

USER jovyan