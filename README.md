# ExpertRoot
Program tools for simulation, reconstruction, data acquisition and analysis of low energy physics experiments

## Step by Step installation
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
BRANCH_NAME=may16p1

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
BRANCH_NAME=v-16.06

### 3. Install [ExpertRoot] (#)

```
# Set the shell variable FAIRROOTPATH to the FairRoot installation directory
export FAIRROOTPATH=~/fair_install/
FairRootInst
```

Далее предполагается, что expertroot будет находиться в папке expertroot в домашней папке пользователя, т.е. в '~/expertroot'.

```
cd ~
mkdir expertroot
cd expertroot
```

Создайте локальный репозиторий путём клонирования 'центрального' репозитория с github и ОБЯЗАТЕЛЬНО перейдите на требуемую ветку.

```
git clone https://github.com/ExpertRootGroup/er/ .
git checkout BRANCH_NAME
```

Альтернативно, можно воспользоваться командой:

```
git checkout -b <local_branch_name> <remote_branch_name>
```

BRANCH_NAME=master - для установки текущей стабильной версии

BRANCH_NAME=dev - для установки версии разработчиков

Для того чтобы получить полный список веток, воспользуйтесь командой:

```
git branch --all
```

Директория, в которой будет собран expertroot может находиться как внутри репозитория, так и за его пределами.
Настоятельно рекомендуется использовать второй подход.
Для этого создайте папку build на одном уровне с папкой expertroot (либо в любом другом месте):

```
cd ../
mkdir build
cd build
```

Чтобы собрать expertroot необходимо сначала сгенерировать исходники с помощью cmake, а затем выполнить компиляцию с помощью make.

```
cmake ../
make
```

При выполнении cmake возможно появление ошибки, в которой говорится о несоответствии компиляторов.
В таком случае рекомендуется явно указывать компилятор, а не использовать флаг -DUSE_DIFFERENT_COMPILER.

```
cmake ../ -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
```

## CMake options

Тут будут перечислены опции cmake влиящие на процесс сборки.

Для использования _ERDigibuilder_ для чтения экспериментальных данных, полученных с помощью _ACCULINNA\_go4\_user\_library_ используйте флаг _-DACCULINNA\_GO4=acculina\_go4\_install\_path_. В котором следует указать в какую директорию была установлена библиотека.

# Initialize

Перед работой с expertroot необходимо объявить специальные переменные окружения. Для этого, находясь в папке, где собран expertroot (т.е. build), выполните следующую команду:

```
. ./config.sh
```

Объявленные переммые активны только в текущей сессии терминала.
