Установка
=========

Установка FairSoft
-------------------

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

BRANCH_NAME=may16p1

В случае, если установка прекращена из-за отсутствия пакетов, проверьте "~/fair_install/FairSoft/DEPENDENCIES" и следуйте инструкциям из него.

Установка FairRoot
------------------

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

BRANCH_NAME=v-16.06

Установка ExpertRoot
--------------------

::

	# Set the shell variable FAIRROOTPATH to the FairRoot installation directory
	export FAIRROOTPATH=~/fair_install/FairRootInst

	mkdir ~/expertroot
	cd ~/expertroot
	git clone http://er.jinr.ru/er.git .
	git checkout BRANCH_NAME
	mkdir build
	cd build
	cmake ../ -DUSE_DIFFERENT_COMPILER=TRUE
	make

* BRANCH_NAME=v-0.3 - для установки последнего релиза
* BRANCH_NAME=dev - для установки версии разработчиков

Инициализация ExpertRoot
------------------------

::

	. ~/expertroot/build/config.sh


