# ER
ExpertRoot(ER) is a [FairRoot](https://github.com/FairRootGroup/FairRoot)-based framework dedicated
to the simulation, reconstruction, data acquisition, and analysis of the nuclear physics experiments.
Primarily developed for the needs of experiments on fragment separator 
[ACCULLINA-2](http://aculina.jinr.ru/a-2.html) at Fleurov Nuclear Research in JINR and experiment
[EXPERT](http://aculina.jinr.ru/expert.html) on fragment separator SUPER-FRS at FAIR in GSI.

## Library Stack

Simulation, reconstruction and analysis in the ER is based on
* [FairSoft](https://github.com/FairRootGroup/FairSoft/tree/dev) - includes widespread Geant4, ROOT,
   VMC and others utilities.
* [FairRoot](https://github.com/FairRootGroup/FairRoot) - framework which provides the base classes
  for IO-management, base classes event-based _tasks_ for the simulation and reconstruction.

Experimental data processing
* [Go4](https://www.gsi.de/en/work/research/experiment_electronics/data_processing/data_analysis/the_go4_home_page.htm) -
  framework for on-line and off-line processing of experimental data.
* [AccDAQ](https://github.com/FLNR-JINR/ACCULINNA_go4_user_library) - library for unpacking and 
  transformation of the raw experimental data in _lmd_ file format produced by Go4 to _root_ format.
  Output files are applicable for the analysis in the ExpertRoot. 

ER extends FairRoot with experiment-specific tasks and adds several features which enables to use it
for different studies.

## Key features:

* User-defined interactions: reactions and decays.
* It is possible to take into account energy losses of projectiles in the target and the beam 
  detector components such as ToF scintillators and MWPC stations.
* Simulation and analysis of cocktail and defocusing beam.
* Accounting for energy losses in the dead layers of the detector during reconstruction of ejectiles
  energies by experimental data.

## Run using Docker

Due to high dependencies on the external packages of various versions, we strongly recommend to use
the docker-image [docker](https://www.docker.com) to deploy ER installation.

1. Install docker desktop on your system: https://www.docker.com/get-started
2. Clone ER repository:

```
git clone https://github.com/FLNR-JINR/er/ .
git checkout dev
```

3. Build docker image with _ER_:

```
docker build --build-arg ER=dev -t er .
```

4. Enjoy  _ROOT_, _Go4_, _AccDAQ_ and _ER_:

```
docker run er root -b -q er_sim.C
docker run er ${GO4SYS}/bin/go4analysis -v -lib libAccDaqUserAnalysis.so -number ${NEVENTS} -asf ${AUTOSAVEFILE} -file ${INPUTFILE} -args ${OUTPUTFILE} ${SETUPFILE}
docker run er run.sh
```


## Step by Step installation

In case Docker-image installation is not preferable or impossible in some reasons one can install 
all dependencies using the following instructions.

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

### 2. Install [FairRoot](https://github.com/FairRootGroup/FairRoot)

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

### 3. Install [ExpertRoot](#)

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

### Experimental data preprocessing

If you need to handle a raw experimental data, please install the _Go4_ and _AccDAQ_

### Install [Go4](https://www.gsi.de/en/work/research/experiment_electronics/data_processing/data_analysis/the_go4_home_page.htm)

```
wget -O go4.tar.gz http://web-docs.gsi.de/~go4/software/download.php?http://web-docs.gsi.de/~go4/download/go4-6.0.0.tar.gz
tar xzf go4.tar.gz
rm go4.tar.gz
mv go4-6.0.0 go4
cd go4
source $SIMPATH/bin/thisroot.sh
make withqt=no -j4
```

### Install [AccDAQ](https://github.com/FLNR-JINR/ACCULINNA_go4_user_library)

```
git clone https://github.com/flnr-jinr/ACCULINNA_go4_user_library accdaq &&\
cd accdaq
git checkout master
mkdir build && cd build
source $SIMPATH/bin/thisroot.sh
source $GO4_PATH/go4login
cmake ../ -DCMAKE_INSTALL_PREFIX=<installtion directory path>
make install -j4
```


## CMake options

There will be cmake options that affect the build process.

* ```-DACCULINNA_GO4 = <acculina_go4_install_path_> ```

To use _ERDigibuilder_ to read experimental data from _AccDAQ_.
You should indicate in which directory the library was installed. 

# Initialize

Environment variables must be initialized in **every** terminal session.

```
source <path_to_build>/config.sh
```

Otherwise ER won't work.

# Run tests

To run the entire test suite:

```
cd build && cmake build . && ctest .
```

To run a specific test:

```
cd build && cmake build . && ctest -R exp1904_h7_sim
```
