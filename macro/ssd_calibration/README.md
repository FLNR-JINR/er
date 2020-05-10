# Silicon Strip Detector (SSD) calibration utilities

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
