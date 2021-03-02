#include "ERCalibrationSSD.cxx" // collection of SSD calibration instruments
using namespace ERCalibrationSSD;

// For thin sensor thickness map building initially two data files are needed:
// * thick sensor calibration run data;
const TString calib_run_path = 
  "./input/clb_09_tetra_00to19.root";
// * data from the simultaneous radiation exposure of an assembly of a thick sensor and a thin
//   in the front of it
const TString map_run_path = 
  "./input/clb_05_tetra_all.root";

// Define run parameters
auto ntetra_y_cal = new SensorRunInfo("Ntetra_y", 16, 1024, calib_run_path);
auto ntetra_y_map = new SensorRunInfo("Ntetra_y", 16, 1024, map_run_path);
auto ntetra_x_map = new SensorRunInfo("Ntetra_x", 16, 1024, map_run_path);


void thick_sensor_preprocessing() {
  auto preproc = new Preprocessing(calib_run_path);
  preproc->AddSensor(ntetra_y_cal);
  preproc->Exec();
  delete preproc;
}

void thick_sensor_calibration() {
  auto calibration = new Calibration(calib_run_path);
  calibration->SetSensor(ntetra_y_cal);
  calibration->SetPeakSearchMethod("sliding_window");
  calibration->SetFitMinSigma(6.);
  calibration->SetFitPeakThreshold(0.7);
  calibration->SetSlideWindowWidth(10);
  calibration->SetSearchRadius(15);
  calibration->Exec();
  delete calibration;
}

void map_sensors_preprocessing() {
  auto preproc = new Preprocessing(map_run_path);
  ntetra_y_map->SetNoiseThreshold(120);
  ntetra_x_map->SetNoiseThreshold(200);
  preproc->AddSensor(ntetra_y_map);
  preproc->AddSensor(ntetra_x_map);
  preproc->Exec();
  delete preproc;
}

void build_non_uniform_map() {
  auto thickness_map = new NonUniformityMapBuilder(map_run_path);
  // Set sensors parameters
  thickness_map->SetThickSensor(ntetra_y_map);
  thickness_map->SetThinSensor(ntetra_x_map);
  thickness_map->SetThickCalibSensor(ntetra_y_cal);
  // Set peak search algorithm parameters
  thickness_map->SetPeakSearchMethod("sliding_window");
  thickness_map->SetFitMinSigma(6.);
  thickness_map->SetFitPeakThreshold(0.7);
  thickness_map->SetSlideWindowWidth(10);
  thickness_map->SetSearchRadius(15);
  // Run map builder
  thickness_map->Exec();
  delete thickness_map;
}

void non_uniform_map_building() {
  // thick_sensor_preprocessing();
  // thick_sensor_calibration();
  map_sensors_preprocessing();
  build_non_uniform_map();
}
