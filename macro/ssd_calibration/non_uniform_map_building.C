#include "ERCalibrationSSD.cxx" // collection of SSD calibration instruments
using namespace ERCalibrationSSD;

// For thin sensor thickness map building initially two data files are needed:
// * thick sensor calibration run data;
const TString calib_run_path = 
  "/mnt/analysis_nas/exp201904/clb/oldLib/postClb/grouped/calib_1mm_90_since25to33.root";
// * data from the simultaneous radiation exposure of an assembly of a thick sensor and a thin
//   in the front of it
const TString map_run_path = 
  "/mnt/analysis_nas/exp201904/clb/oldLib/postClb/alltel_90.root";

// Define run parameters
auto ssd_1m_1_cal = new SensorRunInfo("SSD_1m_1", 16, 1024, calib_run_path);
auto ssd_1m_1_map = new SensorRunInfo("SSD_1m_1", 16, 1024, map_run_path);
auto ssd_20u_1_map = new SensorRunInfo("SSD_20u_1", 16, 1024, map_run_path);


void thick_sensor_preprocessing() {
  auto preproc = new Preprocessing(calib_run_path);
  preproc->AddSensor(ssd_1m_1_cal);
  preproc->Exec();
  delete preproc;
}

void thick_sensor_calibration() {
  auto calibration = new Calibration(calib_run_path);
  calibration->SetSensor(ssd_1m_1_cal);
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
  preproc->AddSensor(ssd_1m_1_map);
  preproc->AddSensor(ssd_20u_1_map);
  preproc->Exec();
  delete preproc;
}

void build_non_uniform_map() {
  auto thickness_map = new NonUniformityMapBuilder(map_run_path);
  // Set sensors parameters
  thickness_map->SetThickSensor(ssd_1m_1_map);
  thickness_map->SetThinSensor(ssd_20u_1_map);
  thickness_map->SetThickCalibSensor(ssd_1m_1_cal);
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
  thick_sensor_preprocessing();
  map_sensors_preprocessing();
  thick_sensor_calibration();
  build_non_uniform_map();
}
