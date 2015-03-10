# ExpertRoot
Program tools for simulation and reconstruction EXPERT expirement events

## Step by Step installation
### 1. Install [FairSoft](https://github.com/FairRootGroup/FairSoft/tree/dev)

```
mkdir ~/fair_install
cd ~/fair_install
#git clone https://github.com/FairRootGroup/FairSoft.git
git clone -b BRANCH_NAME https://github.com/FairRootGroup/FairSoft.git
cd FairSoft
./configure.sh
# 1) gcc (on Linux) 5) Clang (on OSX)
# 1) No Debug Info
# 2) Internet (install G4 files from internet)
# path: ~/fair_install/FairSoftInst
```
BRANCH_NAME=jul14p3

### 2. Install [FairRoot] (https://github.com/FairRootGroup/FairRoot)

```
# Set the shell variable SIMPATH to the installation directory
export SIMPATH=~/fair_install/FairSoftInst

cd ~/fair_install
git clone -b BRANCH_NAME https://github.com/FairRootGroup/FairRoot.git
cd FairRoot
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX="~/fair_install/FairRootInst" ..
make
make install
```
BRANCH_NAME=nightly_master

### 3. Install [ExpertRoot] (#)

```
# Set the shell variable FAIRROOTPATH to the FairRoot installation directory
export FAIRROOTPATH=~/fair_install/FairRootInst

cd ~/
git clone -b BRANCH_NAME https://github.com/ExpertRootGroup/expertroot.git
cd ~/expertroot
mkdir build
cd build
cmake ../
make
```

For using:

```
. ~/expertroot/build/config.sh
```


