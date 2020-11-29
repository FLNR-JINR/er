#include "ERCalibrationSSD.cxx"

using namespace ERCalibrationSSD;

void ssd_one_spectrum_calibration() {
  // Define input file path with raw data converted by TNEvent go4-based library
  const TString file_calib_path = "./clb_07_tetra_1to10.root";
  // [Prepare information about sensor in the calibration run]
  // Constructor parameters: 
  // * sensor branch name in raw data file
  // * stips amount
  // * bins amount in analysis histograms
  // * raw data file path
  //
  auto ntetra_x = new SensorRunInfo("Ntetra_x", 16, 2500, file_calib_path);
  ntetra_x->SetNoiseThreshold(200);
  // auto ntetra_y = new SensorRunInfo("Ntetra_y", 16, 2500, file_calib_path);
  // ntetra_y->SetNoiseThreshold(200);

  // [Preprocessing]
  // Constructor parameter is a raw data file path.
  // The base of a raw input file name is a run ID.
  // For example, if the input file path is /path/to/file/calib_1mm_90_all.root
  // then run ID is 'calib_1mm_90_all' which will be used as subfolder to
  // store results in a resulting directory
  //
  // For the calibration  only one sensor should be added to 
  // preprocessing by AddSensor() method
  auto prep_ntetra = new Preprocessing(file_calib_path);
  prep_ntetra->AddSensor(ntetra_x);
  // prep_ntetra->AddSensor(ntetra_y);

  // Execution of preprocessing may be performed fully automaticaly by call Exec()
  // method
  // prep_ntetra->Exec();
  // or separately, if user one want to control each preprocessing step results.
  // Important that each the following steps can work only with a data produced by
  // precending method in order `ConvertTree() -> FindThresholds() -> MultiplicitySelection()`.
  // * Convertation of an input tree by leaving only the single branch conrned with
  //   analyzed sensor set by AddSensor() method.
  //   ouput file is stored by path
  //   ./result/[run_id]/input/input_[run_id]_[sensor_name].root
  // prep_ntetra->ConvertTree();

  // * Find noise thresholds in each strip
  //   resulting file is stored by path
  //   ./result/[run_id]/[sensor_name]/txt/threshold_[run_id]_[sensor_name].txt
  // prep_ntetra->FindThresholds();
  // * Leave for further analysis only the data with single multiplity - events there
  //   only one of strips has signal higher than a noise threshold.
  //   Strips spectra histograms can be viewed by path
  //  ./resu lt/[run_id]/[sensor_name]/draw/spectra_[run_id]_[sensor_name].root
  //   Tree with a data is in the
  //  ./result/[run_id]/input/mult_one_[run_id]_[sensor_name].root
  // prep_ntetra->MultiplicitySelection(ntetra_x);
  // prep_ntetra->MultiplicitySelection(ntetra_y);
  // exit(0);

  // [Calibration] (http://er.jinr.ru/si_detector_calibration.html)
  auto calib_ntetra_x = new Calibration(file_calib_path);
  calib_ntetra_x->SetSensor(ntetra_x);

  // [[Define peak search algoritm parameters]] 
  // Default values may be found in PeakSearch class documentation

  // Choose peak search method. Two option are avaliable: "sliding_window" or "gauss"
  calib_ntetra_x->SetPeakSearchMethod("sliding_window");
  
  // [[Set algorithm options]]
  
  // All the algoritms use TSpectrum peak search result as initial guess.
  // It's parameters are:
  // * Minimal peak standard deviation;
  calib_ntetra_x->SetFitMinSigma(6.);
  // * Relative peks height with respect to the highest one.
  calib_ntetra_x->SetFitPeakThreshold(0.7);

  // [[Algorithm specific parameters]]

  // If "sliding_window" set by SetPeakSearchMethod()
  // * Sliding window width [bins] (w_width) 
  calib_ntetra_x->SetSlideWindowWidth(40);
  // * Interval of rearch around TSpectrum found peak (tspec_val) 
  //   is defined by [tspec_val - w_width; tspec_val + w_width]
  calib_ntetra_x->SetSearchRadius(50);

  // If "gauss" set by SetPeakSearchMethod() 
  // the same option works
  // calib_ntetra_x->SetSearchRadius(10);

  // Execute all calibration step:
  // 1) Peaks position determination
  //    results are stored in
  //    ./result/[run_id]/[sensor_name]/txt/peakpos_[run_id]_[sensor_name].txt
  // 2) Dead layer estimation. 
  //    Found values are in
  //    ./result/[run_id]/[sensor_name]/txt/dead_[run_id]_[sensor_name].txt
  // 3) Calibration coefficients calculation. 
  //    Found values are in
  //    ./result/[run_id]/[sensor_name]/txt/coeff_[run_id]_[sensor_name].txt
  // 4) Report file printing containing all the information about calubration run
  //    ./result/[run_id]/[sensor_name]/report_[run_id]_[sensor_name].txt
  calib_ntetra_x->SearchPeaks();
  calib_ntetra_x->DeadLayerEstimation();
  ntetra_x->SetDeadLayersPerStrip(
    {2.6, 2.7, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6}
  );
  calib_ntetra_x->CalcCalibrationCoefficients(true);
}
