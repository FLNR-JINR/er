Getting started
===============

Run using Docker
----------------

Due to high dependencies on the external packages of various versions, we strongly recommend to use
the docker-image to deploy ER installation.

1. Install docker desktop on your system: https://www.docker.com/get-started
2. Clone ER repository:

::

	git clone https://github.com/FLNR-JINR/er/
	cd er
	git checkout dev


3. Build docker image with *ER*:

::

  docker build --build-arg ER=dev -t er .

4. Enjoy  *ROOT*, *Go4*, *AccDAQ* and *ER*:

::

  docker run er root -b -q er_sim.C
  docker run er ${GO4SYS}/bin/go4analysis -v -lib libAccDaqUserAnalysis.so -number ${NEVENTS} -asf ${AUTOSAVEFILE} -file ${INPUTFILE} -args ${OUTPUTFILE} ${SETUPFILE}
  docker run er run.sh


Step by Step installation
-------------------------

In case Docker-image installation is not preferable or impossible in some reasons one can install 
all dependencies using the following instructions.

Install `FairSoft <https://github.com/FairRootGroup/FairSoft/tree/dev>`_
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

::

  mkdir ~/fair_install
  cd ~/fair_install
  git clone https://github.com/FairRootGroup/FairSoft.git
  cd FairSoft
  git checkout BRANCHE_NAME
  ./configure.sh
  # 1) gcc (on Linux) 5) Clang (on OSX)
  # 2) No Debug Info
  # 3) Yes (ROOT6)
  # 4) Yes (Install engines)
  # 5) Internet (install G4 files from internet)
  # 6) No (without python)
  # path: ~/fair_install/FairSoftInst

BRANCH_NAME=oct17p4

Install `FairRoot <https://github.com/FairRootGroup/FairRoot>`_
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

::

  # Set the shell variable SIMPATH to the installation directory
  export SIMPATH=~/fair_install/FairSoftInst

  cd ~/fair_install
  git clone https://github.com/FairRootGroup/FairRoot.git
  cd FairRoot
  git checkout BRANCH_NAME
  mkdir build
  cd build
  cmake -DCMAKE_INSTALL_PREFIX="~/fair_install/FairRootInst" ..
  make
  make install

BRANCH_NAME=v-17.10

Install ExpertRoot
++++++++++++++++++

::

  # Set the shell variable FAIRROOTPATH to the FairRoot installation directory
  export FAIRROOTPATH=~/fair_install/FairRootInst
  cd ~
  mkdir expertroot
  cd expertroot
  git clone https://github.com/ExpertRootGroup/er/ .
  git checkout BRANCH_NAME
  cd ../
  mkdir build
  cd build
  cmake ../ -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
  make

Experimental data preprocessing
-------------------------------

If you need to handle a raw experimental data, please install the *Go4* and *AccDAQ*

Install `Go4 <https://www.gsi.de/en/work/research/experiment_electronics/data_processing/data_analysis/the_go4_home_page.htm>`_
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

::

  wget -O go4.tar.gz http://web-docs.gsi.de/~go4/software/download.php?http://web-docs.gsi.de/~go4/download/go4-6.0.0.tar.gz
  tar xzf go4.tar.gz
  rm go4.tar.gz
  mv go4-6.0.0 go4
  cd go4
  source $SIMPATH/bin/thisroot.sh
  make withqt=no -j4

Install `AccDAQ <https://github.com/FLNR-JINR/ACCULINNA_go4_user_library>`_
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

::

  git clone https://github.com/flnr-jinr/ACCULINNA_go4_user_library accdaq
  cd accdaq
  git checkout dev
  mkdir build && cd build
  source $SIMPATH/bin/thisroot.sh
  source $GO4_PATH/go4login
  cmake ../ -DCMAKE_INSTALL_PREFIX=<installtion directory path>
  make install -j4

ER for working with experimental data
+++++++++++++++++++++++++++++++++++++

To use *ERDigibuilder* to read experimental data from AccDAQ use the cmake flag `-DACCULINNA_GO4 = <acculina_go4_install_path_>` 
in er compilation. In which you should indicate in which directory the library was installed. 

Initialization
--------------

Environment variables must be initialized in **every** terminal session.

::

  source <path_to_build>/config.sh

Otherwise ER won't work.

Local documentation building
----------------------------

The documentation for ER is written for the system
`sphinx-doc <http://www.sphinx-doc.org/en/1.5.1/>`.
You must install this system.
To install under Ubuntu, use the `` python-sphinx`` package.
Then go to the documentation source folder and compile the project:

::

	cd ${EXPERTROOT}/docs/
	make html

After the completion of the build procedure, a number of files will appear, and the html-top-level page will be called ${EXPERTROOT}/docs/_build/index.html .
