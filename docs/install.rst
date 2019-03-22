Installation
============

FairSoft installation
---------------------

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

In case the installation is stopped due to missing packages, check "~ / fair_install / FairSoft / DEPENDENCIES" and follow the instructions from it.

FairRoot installation
---------------------

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

ExpertRoot installation
-----------------------

::

	# Set the shell variable FAIRROOTPATH to the FairRoot installation directory
	export FAIRROOTPATH=~/fair_install/FairRootInst

	mkdir ~/expertroot
	cd ~/expertroot
	git clone https://github.com/ExpertRootGroup/er/ .
	git checkout BRANCH_NAME
	mkdir build
	cd build
	cmake ../ -DUSE_DIFFERENT_COMPILER=TRUE
	make

* BRANCH_NAME=dev


CMake options
~~~~~~~~~~~~~

This will list the cmake options that affect the build process.

To use _ERDigibuilder_ to read experimental data obtained with _ACCULINNA\_go4\_user\_library_, use the _-DACCULINNA\_GO4 = acculina\_go4\_install\_path_ flag, which should indicate directory the library was installed.

ExpertRoot initialization
-------------------------

To initialize package environment variables, you must call:

::

	. ~/expertroot/build/config.sh

To use the package, it is desirable to create a separate run directory. After that, you can copy the standard macro and macro folders, modify it and run it.

::

	cd ~/expertroot/
	mkdir run
	cd run
	cp ../macro/NeuRad/NeuRad_sim.C
	root -l NeuRad_sim.C

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
