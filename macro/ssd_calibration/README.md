# Silicon Strip Detector (SSD) calibration utilities

## Introduction

The calibration procedure is described in proper [ExpertRoot documentation section](http://er.jinr.ru/si_detector_calibration.html)

Files in this directory demonstrate examples of work with ERCalibration functions. All the files are ready to use on er.jinr.ru host.

* [ERCalibration.cxx](ERCalibration.cxx) - calibration classes and functions which are imported to user code.
* [ssd_one_spectrum_calibration.C](ssd_one_spectrum_calibration.C) - thick sensor calibration example.
* [non_uniform_map_building.C](non_uniform_map_building.C) - thin sensor thickness map building example.
* [thin_sensor_map_effective_to_real.C](thin_sensor_map_effective_to_real.C) - recalculation of the effective thickness to real one. The non-uniform map building procedure gives effective thickness distribution. Recalculating may be required when the sensor is significantly shifted from a position where direct exposure assumption is applicable.

The folder `auxiliary` contains calculations of reference values including macros for dependencies fitting, comparison of ionization models.
* [fit_eta.C](./auxiliary/fit_eta.C) - quadratic fit of the $$d(\eta)$$ dependence.
* [fit_eloss_highE.C](./auxiliary/fit_eloss_highE.C) - energy loss fit depending on a dead layer thickness for $$\alpha$$ particle with start energy 7.6869 [MeV] used in the .
* [ionization](./auxiliary/ionization) - files useful for comparing different ways of data production for the fitting dependencies.

## Hydra.jinr.ru how-to

### Prerequisites

Prerequisites for using calibration utilities on [HybriLIT](hydra.jinr.ru):

* [Registarion on HybriLIT](http://hlit.jinr.ru/for_users/registration/).
* The calibration run data need to be converted from `*.lmd` to `*.root` with TNeEvent-based library.
Ask FLNR data aquisition specialists how to prepare data.

### Setup environment

1. Connect to `hydra.jinr.ru`

```bash
ssh <username>@hydra.jinr.ru
```

2. Activate environment with ROOT version 18 or elder using [`module`](http://hlit.jinr.ru/for_users/user_guide/) package.

```bash
module add FairRoot/v18.0.4
```
3. Enjoy calibraions!

### Hints and tricks

#### Merge several root files
If you need to merge several root files into single file use `hadd` tool provided by ROOT.

```bash
cd <folder_with_root_files>
hadd <target_name>.root <list_of_files_to_merge> 
```
For additional information read `hadd` manual.

```bash
man hadd
```

* Example 1

Merge three `*.root` files into single `output.root`

```bash
hadd output.root file1.root file2.root file3.root
```

* Example 2

The `<folder_with_root_files>` contains files named `clb_0001.root`, `clb_0002.root`, ..., `clb_0055.root`. One wants to merge files with indicies from `_0010` to `_0029`. It can be done quite elegant with regular expression:

```
hadd output.root $(find . -type f -regex "./clb_.*00[12].*")
```
