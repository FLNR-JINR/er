# ER
Program tools for simulation, reconstruction, data acquisition and analysis of low energy physics experiments.
_er_ integrated with [accdaq](https://github.com/FLNR-JINR/ACCULINNA_go4_user_library) 
go4 library in order to work with experimental data.

## Run using docker

We highly recommend using [docker](https://www.docker.com) to work with er, because it has many dependencies.

Here we demonstrate how to build and run a docker container for dev branch of er.
It contains _accdaq_ on board.


1. Install docker desktop on your system: https://www.docker.com/get-started
2. Clone er repository:

```
git clone https://github.com/ExpertRootGroup/er/ .
git checkout dev
```

3. Build docker image with _er_:

```
docker build --build-arg ER=dev -t er .
```

4. Run your commads with _root_, _go4_, _accdaq_ and _er_ functionality:

```
docker run er root -b -q er_sim.C
docker run er ${GO4SYS}/bin/go4analysis -v -lib libAccDaqUserAnalysis.so -number ${NEVENTS} -asf ${AUTOSAVEFILE} -file ${INPUTFILE} -args ${OUTPUTFILE} ${SETUPFILE}
docker run er run.sh
```


## Step by Step installation

If you need to install all the dependencies on your system, then here is the instruction.

### 1. Install [FairSoft](https://github.com/FairRootGroup/FairSoft/tree/dev)

```
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
```
BRANCH_NAME=oct17p4

### 2. Install [FairRoot] (https://github.com/FairRootGroup/FairRoot)

```
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
```
BRANCH_NAME=v-17.10

### 3. Install [ExpertRoot] (#)

```
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
```

## CMake options

There will be cmake options that affect the build process.

* To use _ERDigibuilder_ to read experimental data from _ACCULINNA \ _go4 \ _user \ _library_ use the _-DACCULINNA \ _GO4 = acculina \ _go4 \ _install \ _path_ flag.
  In which you should indicate in which directory the library was installed. 

# Initialize

Before working with expertroot, you need to declare special environment variables.
To do this, while in the folder where expertroot is built (i.e. build), run the following command: 

```
. ./config.sh
```

The declared variables are active only in the current terminal session. 
