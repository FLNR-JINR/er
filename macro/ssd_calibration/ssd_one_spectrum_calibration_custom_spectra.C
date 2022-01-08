/*
Macro provides example of calibration workflow usage to evaluate calibration 
coefficients using custom spectra. The preprocessing is not needed is this case as far as
all the data is ready to run.
*/

#include "ERCalibrationSSD.cxx"

using namespace ERCalibrationSSD;

void ssd_one_spectrum_calibration_custom_spectra() {
  // const TString runID = "source0degSh500nsG8V250-H2det"; // name of the resulting folder in `results`
  // const TString custom_hists_path = "./source0degSh500nsG8V250-H2det.root";
  const TString runID = "source0degSh500nsG8V250-H1det_inv"; // name of the resulting folder in `results`
  const TString custom_hists_path = "./source0degSh500nsG8V250-H1det_inv.root";
  // const TString runID = "source70degSh500nsG8V5_3-H1det20um"; // name of the resulting folder in `results`
  // const TString custom_hists_path = "./source70degSh500nsG8V5_3-H1det20um.root";
  auto x = new SensorRunInfo("x", 16, 4096, runID);
  x->SetNoiseThreshold(500);
  // [Calibration] (http://er.jinr.ru/si_detector_calibration.html)
  auto calib_x = new Calibration(runID);
  calib_x->SetSensor(x);
  /* Set path containing user's custom spectra. If set, prepocessing results are ignored,
  only spectra from the defined folder are used.
  */
  calib_x->SetPathToCustomSpectra(custom_hists_path);

  calib_x->SetPeakSearchMethod("sliding_window");
  calib_x->SetFitMinSigma(6.);
  calib_x->SetFitPeakThreshold(0.3);
  calib_x->SetSlideWindowWidth(50);
  calib_x->SetSearchRadius(50);

  calib_x->SearchPeaks();
  calib_x->DeadLayerEstimation();
  // ntetra_x->SetDeadLayersPerStrip(
  //   {2.6, 2.7, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6}
  // );
  const bool fitOnlyLastTwoPointsNvsE = false;
  calib_x->CalcCalibrationCoefficients(fitOnlyLastTwoPointsNvsE);

  // Save calibrated hists in ./result/[run_id]/[sensor_name]/draw/calibrated_[run_id]_[sensor_name].root
  calib_x->DrawCalibratedHists();
}
