namespace ERCalibrationSSD {
/*
  Namespace includes code for a Silicon Strip Detector (SSD) calibration.
  The common scheme of handling process

  Prerequisites for program module usage:
   * Files with calibration run data in *.root format converted by FLNR TNeEvent classes

  Terminology.
  Sensor - an array of a readout channel concerned with a certain ROOT-tree leaf.

  Platforms.
  The code was tested only in the Linux OS, but the ROOT-framework methods used work in
  Windows also. Methods which are used but work on Linux only are: 
  * TString GetFileNameBaseFromPath (TString const &path)
  * void IOManager::MakeDir(TString &dirname)
  For them, path name separators should be extended to Windows ones.

  ROOT versions - ???
*/

/* @TODO
  * Sorting stations by name for reproducible results (path names)
  * Replace SetRunInfo class by RunInfo which will contain several sensors info with their common 
   run id.
  * More log messages for each significant step
*/


TString GetFileNameBaseFromPath(const TString& path);

class SensorRunInfo {
  /** @class SensorRunInfo
   ** @brief Info.
   **
  **/
public:
  SensorRunInfo() = default;
  SensorRunInfo(const TString& name, const Int_t stripAmount, 
                const Int_t binAmount, const TString& runId = "");
  ~SensorRunInfo() = default;

  /** @brief Sets threshold file path.
   ** Threshold integer values for each strip are listed in a file in ascending 
   ** strip numbers order.
   ** Example:
   ** 33 43 48 23 23 55
   ** Threshods for strips 0th - 5th.
   ** 
   ** @param path threshold file path
  **/
  void SetThresholdFile(const TString &path) {fThresholdFilePath = path;}

  void ReadThresholds();
public:
  TString fName; // name of a propper leaf in a tree
  TString fThresholdFilePath; //
  TString fCalibFilePath;
  Int_t fStripAmount = 16;
  Int_t fBinAmount = 1024;
  TString fRunId;
};

SensorRunInfo::SensorRunInfo(const TString& name, const Int_t stripAmount, 
                             const Int_t binAmount, const TString& runId = ""): 
  fName(name), 
  fStripAmount(stripAmount), 
  fBinAmount(binAmount), 
  fRunId(GetFileNameBaseFromPath(runId)) {
}


/**
** @brief Draws strips spectra.
** Writes a histigram for every strip and a canvas containing all the histograms to a
** current ROOT directory.
**
** @param tree tree containing histograms data
** @param sensor sensor to draw
** @param thresolds list of a sensor thresolds
**/
void DrawSensorSpectraByStrip(TTree* tree,  
                              const SensorRunInfo* sensor,
                              const std::vector<UShort_t>& thresholds)  {
  const auto canvas = new TCanvas(Form("canvas_%s", sensor->fName.Data()));
  // prepare approximately square placing of pads on canvas
  Int_t canvFirtsDim = std::round(std::sqrt(sensor->fStripAmount));
  Int_t canvSecondDim = 1;
  for (; canvSecondDim*canvFirtsDim < sensor->fStripAmount; canvSecondDim++) {}
  canvas->Divide(canvFirtsDim, canvSecondDim);

  for (Int_t iStrip = 0; iStrip < sensor->fStripAmount; iStrip++) {
    const TString histParams = Form("(%d,%d,%d)", sensor->fBinAmount, 0, sensor->fBinAmount);
    const TString histName = Form("strip_%d", iStrip);
    const TString drawExpression = Form(
      "%s[%d]>>%s%s", sensor->fName.Data(), iStrip, histName.Data(), histParams.Data()
    ); 
    const TString cutExpression = Form(
      "%s[%d]>%d", sensor->fName.Data(), iStrip, thresholds[iStrip]
    ); 
    canvas->cd(iStrip + 1);
    tree->Draw(drawExpression, cutExpression, "");
    const auto hist = static_cast<TH1D*>(gDirectory->Get(histName));
    hist->Write();
  }
  canvas->Write();
}

/**
** Pixel (stripDraw, stripSelect) is a portion of strip (stripDraw) sensor data 
** which is also registered in a strip of the other station (stripSelect). In both 
** sensors registered signal is higher than the noise threshold
** Writes a histogram for every pixel and canvases containing all the histograms by
** strips to a current ROOT directory.
**
** @param tree tree containing histograms data
** @param sensorDraw sensor to draw
** @param sensorSelect cross oriented sensor with respect to 'sensorDraw'
** @param thresoldsDraw list of the 'Draw' sensor thresolds
** @param thresoldsSelect list of the 'Select' sensor thresolds
**/
void DrawSensorSpectraByPixel(TTree* tree,  
                              const SensorRunInfo* sensorDraw,
                              const SensorRunInfo* sensorSelect,
                              const std::vector<UShort_t>& thresholdsDraw,
                              const std::vector<UShort_t>& thresholdsSelect)  {
  // prepare approximately square placing of pads on canvas
  Int_t canvFirtsDim = std::round(std::sqrt(sensorDraw->fStripAmount));
  Int_t canvSecondDim = 1;
  for (; canvSecondDim*canvFirtsDim < sensorDraw->fStripAmount; canvSecondDim++) {}

  for (Int_t iStripDraw = 0; iStripDraw < sensorDraw->fStripAmount; iStripDraw++) {
    const auto canvasName = Form("canvas_%s[%d]", sensorDraw->fName.Data(), iStripDraw);
    const auto canvas = new TCanvas(canvasName);
    canvas->Divide(canvFirtsDim, canvSecondDim);
    for (Int_t iStripSelect = 0; iStripSelect < sensorSelect->fStripAmount; iStripSelect++) {
      const auto histParams = Form("(%d,%d,%d,%d,%d,%d)", 
                                   sensorDraw->fBinAmount, 0, sensorDraw->fBinAmount,
                                   sensorSelect->fBinAmount, 0, sensorSelect->fBinAmount);
      const auto histName = Form("canvas_%s[%d]_%s[%d]", sensorDraw->fName.Data(), iStripDraw, 
                                                         sensorSelect->fName.Data(), iStripSelect);
      const auto drawExpression = Form("%s[%d]>>%s%s", sensorDraw->fName.Data(), iStripDraw, 
                                                       histName, histParams); 
      const auto cutExpression = Form("%s[%d]>%d&&%s[%d]>%d", 
                                      sensorDraw->fName.Data(), iStripDraw, 
                                      thresholdsDraw[iStripDraw],
                                      sensorSelect->fName.Data(), iStripSelect, 
                                      thresholdsSelect[iStripSelect]);
      canvas->cd(iStripDraw + 1);
      tree->Draw(drawExpression, cutExpression, "");
      const auto hist = static_cast<TH1D*>(gDirectory->Get(histName));
      hist->Write();
    }
    canvas->Write();
  }
}

// External fuctions begin
/**
** @brief Checks multiplicity in a data vector according to thresholds.
**  Returns value in the range [0, 2].
** @param data list of data arrays corresponding to a station info objects
** @param thresholds list of thresolds
**/
int CheckDataMultiplicity(const std::vector<UShort_t>& data,
                          const std::vector<UShort_t>& thresholds) {
  int multiplicity = 0;
  for (Int_t i = 0; i < data.size(); i++) {
    if (data[i] > thresholds[i]) {
      multiplicity++;
      if (multiplicity > 1) {
        break;
      }
    }
  }
  return multiplicity;
}

/**
** @brief Check if data has a single multiplicity in each sensor.
** @param sensors list of sensors information structures
** @param data list of data arrays corresponding to a 
**/
Bool_t CheckIfSingleMultiplicity(const std::vector<SensorRunInfo*>* sensors,
                                 const std::vector<std::vector<UShort_t>>* data,
                                 const std::vector<std::vector<UShort_t>>* thresholds) {
  Bool_t ifSingleMult = true;
  for (Int_t i = 0; i < sensors->size(); i++) {
    const int multiplicity = CheckDataMultiplicity(data->at(i), thresholds->at(i));
    if (multiplicity != 1) {
      ifSingleMult = false;
      break;
    }
  }
  return ifSingleMult;
}

/**
** @brief Deserializes a reaquired TKey object by name from a ROOT-file.
**  If a name is not set the first key is returned.
** ATTENTION! Opened ROOT-file is not closed.
** @param filePath ROOT-file data path.
** @param objName deserealized object name.
**/
TObject* GetObjectFromRootFile(const TString& filePath, const TString& objName = "") {
  auto file = TFile::Open(filePath.Data()); // ATTENTION! file is not closed
  auto obj = (objName.Length())
           ? file->Get(objName)
           : file->Get(file->GetListOfKeys()->At(0)->GetName());
  return obj;
}

/**
** @brief Deserializes a reaquired TKey object by name from a ROOT-file object.
**  If a name is not set the first key is returned.
** @param file ROOT-file pointer.
** @param objName deserealized object name.
**/
TObject* GetObjectFromRootFile(TFile* file, const TString& objName = "") {
  // save current directory
  auto obj = (objName.Length())
           ? file->Get(objName)
           : file->Get(file->GetListOfKeys()->At(0)->GetName());
  return obj;
}


/**  
** @brief Returns file name base withou extension (part of a path after the last dot).
** @param path - path to file
**/
TString GetFileNameBaseFromPath(const TString& path) {
  TString fileName = gSystem->BaseName(path);
  // remove file extension. Extension is considered as part of the name after last "." symbol
  Int_t lastDotPos = fileName.Last('.');
  if (lastDotPos > 0) {
    fileName.Remove(lastDotPos, fileName.Length());    
  }
  return fileName;
}


/**  
** @brief Dumps vector data to a file stream with the set delimiter.
** @param vec - vector of data
** @param file - output file stream
** @param delimiter
**/
template<typename T>
void DumpVector(const std::vector<T>& vec, ofstream& file, const TString& delimiter = "\n") {
  for (const auto &itVec : vec) {
    file << itVec << delimiter;
  }
}

/**  
** @brief Dumps vector data to a file stream with the set delimiter.
** @param vec - vector of data
** @param file - output file stream
** @param delimiter
**/
template<typename T>
void Dump2DVector(const T& vec2d, ofstream& file, 
                  const TString& delimiter = " ") {
  for (const auto &vec: vec2d) {
    for (const auto &elem: vec) {
      file << elem << delimiter;
    }
    file << endl;
  }
}


/**  
** @brief Reads vector data from a file stream.
** @param file - input file stream
** @param size - vector size. If not set - all avaliable from stip sequently is read.
**  Note: The size value higher then std::numeric_limits<UInt_t>::max() will drop
**  the method.
**/
template<typename T>
std::vector<T> ReadVectorFromFile(const TString& filePath) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    Error("Read2DVectorFromFile", "Failed to open file: %s", filePath.Data());
  }
  std::vector<T> vec;
  T value;
  while (file >> value) {
    vec.push_back(value);
  }
  return vec;
}

/**  
** @brief Reads vector data from a file stream.
** @param file - input file stream
** @param size - vector size. If not set - all avaliable from stip sequently is read.
**  Note: The size value higher then std::numeric_limits<UInt_t>::max() will drop
**  the method.
**/
template<typename T>
std::vector<std::vector<T>> Read2DVectorFromFile(const TString& filePath) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    Error("Read2DVectorFromFile", "Failed to open file: %s", filePath.Data());
  }
  std::vector<std::vector<T>> vec2d;
  T value;
  std::string line;
  while (std::getline(file, line)) {
    std::vector<T> vec;
    std::istringstream iss(line);
    T value;
    while (iss >> value) {
      vec.push_back(value);
    }
    vec2d.push_back(vec);
  }
  file.close();
  return vec2d;
}

// External functions end

enum ApproxOrder {
  LINEAR,
  QUAD,
  TRI
};

class IOManager {
public:
  IOManager() = default;
  IOManager(const TString& workdir) = delete;
  ~IOManager() = default; // ought to be virtual

  /**
  ** @brief Creates output ROOT-file
  ** Creates a file and all the needed subdirectories.
  ** @param filePath - path to a file.
  ** Return pointer to a new Root file.
  **/
  TFile* CreateRootFile(const TString& filePath);

  /**
  ** @brief Opens ROOT file
  ** @param filePath path to a file
  **/
  TFile* OpenRootFile(const TString& filePath);

  /**
  **  @brief Creates output text file
  ** Creates a file and all the needed subdirectories.
  ** @param filePath path to a file
  **/
  std::ofstream CreateTextFile(const TString& filePath);

  /**
  ** @brief Opens output text file
  ** @param filePath path to a file
  **/
  std::ifstream OpenTextFile(const TString& filePath);

  
  /**
  ** @brief Serilizes list of objects to a current opened ROOT file directory
  ** @param arr list of objects
  **/
  void Write(const TObjArray* arr);

  /**
  ** @brief Serilizes an object to a current opened ROOT file directory
  ** @param obj object to serialize
  **/
  void Write(TObject* obj);

  /**
  ** @brief Creates directory in a current working directory
  ** @param dirname subdirectory to create name
  **/
  void MakeDir(const TString& dirname);

  /**
  ** @brief Changes working directory
  ** @param dirname subdirectory name
  **/
  void ChangeDir(const TString& dirname);
protected:
  TString fWorkDir = ".";
  Bool_t fRecreate = true; // 'true' if all the produced files are recreated, 'false' to use existing ones
};

TFile* IOManager::CreateRootFile(const TString& filePath) {
  const TString dirPath = gSystem->DirName(filePath);
  this->MakeDir(dirPath);
  auto file = TFile::Open(filePath, "RECREATE");
  if (file) {
    Info("IOManager::CreateRootFile", "Created ROOT file: %s", filePath.Data());
  } else {
    Error("IOManager::CreateRootFile", "Failed to create ROOT file: %s", filePath.Data());
  }
  return file;
}

TFile* IOManager::OpenRootFile(const TString& filePath) {
  auto file = TFile::Open(filePath);
  if (!file->IsOpen()) {
    Error("IOManager::OpenRootFile", "Failed open ROOT file: %s", filePath.Data());
  }
  return file;
}

std::ofstream IOManager::CreateTextFile(const TString& filePath) {
  const TString dirPath = gSystem->DirName(filePath);
  this->MakeDir(dirPath);
  std::ofstream file(filePath, ios::trunc);
  if (file.is_open()) {
    Info("IOManager::CreateTextFile", "Created ASCII file: %s", filePath.Data());
  } else {
    Error("IOManager::CreateTextFile", "Failed to create ASCII file: %s", filePath.Data());
  }
  return file;
}

std::ifstream IOManager::OpenTextFile(const TString& filePath) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    Error("IOManager::OpenTextFile", "Failed open ASCII file: %s", filePath.Data());
  }
  return file;
}


void IOManager::Write(const TObjArray* arr) {
  Info(
    "IOManager::Write", "save to a directory %s the following objects:", 
    gDirectory->GetPath()
  );
  for (const auto *obj: *arr) {
    Info("", "%s", obj->GetName());
    obj->Write();
  }
}

void IOManager::Write(TObject* obj) {
  auto arr = new TObjArray();
  arr->Add(obj);
  Write(arr);
}


void IOManager::MakeDir(const TString& dirname) {
  std::vector<TString> endSym= {".", "/", "~"}; // unix only
  TString dirPath = dirname;
  std::vector<TString> subdirs;
  // find all subdirs
  do {
    subdirs.push_back(gSystem->BaseName(dirPath));
    dirPath = gSystem->DirName(dirPath);
  } while (dirPath != "." && dirPath != "/" && dirPath != "~"); // only unix now

  // create sequently all the subdirs
  for (auto subdirIter = subdirs.rbegin(); subdirIter != subdirs.rend(); subdirIter++) {
    TString subdirName = *subdirIter;
    dirPath = gSystem->PrependPathName(dirPath, subdirName);
    gSystem->MakeDirectory(dirPath);
  }
}

void IOManager::ChangeDir(const TString& dirname) {
  TString dirPath = dirname;
  fWorkDir = gSystem->PrependPathName(fWorkDir, dirPath);
}

enum FileType {
  ROOT_INPUT_REDUCED_TREE_PATH, // *.root file with only tree leaves treated in analysis
  ROOT_MULT_SELECTED_PATH, // *.root ROOT_INPUT_REDUCED_TREE_PATH after excluding non-single multiplycity stations
  ROOT_HIST_SPECTRA_PATH, // *.root strip histograms after multiplycity selection and cut on thresholds
  ROOT_HIST_PEAKS_PATH, // *.root strip histograms with marked peaks found by TSpectrum and chosen algorithm
  ROOT_HIST_PIXEL_PATH, // *.root pixel spectra histograms
  TXT_PEAK_DATA_PATH, // *.root contains three 1D histograms for low middle and high energy peaks
  TXT_THRESHOLD_PATH, // *.txt noise thresholds for sensor
  TXT_DEAD_LAYER_PATH, // *.txt noise thresholds for sensor
  TXT_CALIB_COEFF_PATH, // calibration coefficients
  TXT_REPORT_PATH, // calibration results report
  TXT_HIGH_E_PEAK_PATH // *.root table of high energy peak values in thick sensor
};

class CalibIOManager: public IOManager {
public:

  CalibIOManager() = default;
  CalibIOManager(const TString &workdir) = delete;
  ~CalibIOManager() = default; // ought to be virtual
  /**
  ** @brief Returns file path according to sensor names, prefix and extension by scheme
  ** [WORK_DIR]/(sensor_name_1)_..._[sensor_name_N]/(subdirs)/(prefix)_(name_root)_(sensor_name_1)_..._[sensor_name_N].(extension)
  **/
  TString ConstructSensorFilePath(const std::set<TString>& subdirs, 
                                  const TString& prefix,
                                  const TString& nameRoot,
                                  const std::vector<SensorRunInfo*>* sensors,
                                  const TString& extension = "root");

  /**
  ** @brief Returns file path according to subdirs list, prefix and extension by scheme
  ** [WORK_DIR]/(subdirs)/(prefix)_(name_root).(extension)
  **/
  TString ConstructFilePath(const std::set<TString>& subdirs, 
                            const TString& prefix,
                            const TString& nameRoot,
                            const std::vector<SensorRunInfo*>* sensors,
                            const TString& extension = "root");

  /**
  ** @brief Returns a path to a desired file
  ** File storing structure is fixed for the calibrations procedure.
  ** @param fileType classified by the enum FileType
  ** @param sensors vector of stations
  **/
  TString GetPath(const Int_t fileType, const TString& nameRoot,
                  const std::vector<SensorRunInfo*>* sensors = nullptr);

  /**
  ** @brief Returns a path to a desired file
  ** File storing structure is fixed for the calibrations procedure.
  ** 
  **/
  TString GetPath(const Int_t fileType, const TString& nameRoot, 
                  const SensorRunInfo* sensor = nullptr);

  /**
  ** @brief Returns a std::vector of sensor noise threshold.
  ** If user defined file was set for sensor, values are read from it.
  ** Otherwise, standard file path is used.
  ** 
  **/
  template<typename T>
  std::vector<T> GetSensorThresholds(const SensorRunInfo* sensor,
                                     const TString& runId);

  template<typename T>
  std::vector<std::vector<T>> GetCalibCoefficients(const SensorRunInfo* sensor,
                                                   const TString& runId);
};

// CalibIOManager::CalibIOManager(const TString &workdir)
//  : IOManager(workdir) {
// }


TString CalibIOManager::ConstructFilePath(const std::set<TString>& subdirs, 
                                          const TString& prefix,
                                          const TString& nameRoot,
                                          const std::vector<SensorRunInfo*>* sensors,
                                          const TString& extension = "root") {
  TString fileName = prefix + "_" + nameRoot;
  for (const auto *sensor: *sensors) {
    fileName += Form("_%s", sensor->fName.Data());
  }
  fileName += Form(".%s", extension.Data());
  TString dirPath = fWorkDir;
  for (auto subdir: subdirs) {
    dirPath = gSystem->PrependPathName(dirPath, subdir);
  }
  const TString filePath = gSystem->PrependPathName(dirPath, fileName);
  return filePath;
}

TString CalibIOManager::ConstructSensorFilePath(const std::set<TString>& subdirs, 
                                                const TString& prefix,
                                                const TString& nameRoot,
                                                const std::vector<SensorRunInfo*>* sensors,
                                                const TString& extension = "root") {
  TString fileName = prefix + "_" + nameRoot;
  TString sensorNames = "";
  for (const auto *sensor: *sensors) {
    sensorNames += Form("_%s", sensor->fName.Data());
  }
  TString sensorSubdir = sensorNames(1, sensorNames.Length());
  fileName += Form("%s.%s", sensorNames.Data(), extension.Data());
  TString dirPath = fWorkDir;
  dirPath = gSystem->PrependPathName(dirPath, sensorSubdir);
  for (auto subdir: subdirs) {
    dirPath = gSystem->PrependPathName(dirPath, subdir);
  }
  const TString filePath = gSystem->PrependPathName(dirPath, fileName);
  return filePath;
}

TString CalibIOManager::GetPath(const Int_t fileType,
                                const TString& nameRoot,
                                const std::vector<SensorRunInfo*>* sensors = nullptr) {
  TString path;
  switch (fileType) {
    case ROOT_INPUT_REDUCED_TREE_PATH:
      path = this->ConstructFilePath({"input"}, "input", nameRoot, sensors);
    break;
    case ROOT_MULT_SELECTED_PATH:
      path = this->ConstructFilePath({"input"}, "mult_one", nameRoot, sensors);
    break;
    case ROOT_HIST_SPECTRA_PATH:
      path = this->ConstructSensorFilePath(
        {"draw"}, "spectra", nameRoot, sensors, "root"
      );
    break;
    case ROOT_HIST_PEAKS_PATH:
      path = this->ConstructSensorFilePath(
        {"draw"}, "peaks", nameRoot, sensors, "root"
      );
    break;
    case ROOT_HIST_PIXEL_PATH:
      path = this->ConstructSensorFilePath(
        {"draw"}, "pixels", nameRoot, sensors, "root"
      );
    break;
    case TXT_PEAK_DATA_PATH:
      path = this->ConstructSensorFilePath(
        {"txt"}, "peakpos", nameRoot, sensors, "txt"
      );
    break;
    case TXT_THRESHOLD_PATH: 
      path = this->ConstructSensorFilePath(
        {"input"}, "threshold", nameRoot, sensors, "txt"
      );
    break;
    case TXT_DEAD_LAYER_PATH:
      path = this->ConstructSensorFilePath(
        {"txt"}, "dead", nameRoot, sensors, "txt"
      );
    break;
    case TXT_CALIB_COEFF_PATH:
      path = this->ConstructSensorFilePath(
        {"txt"}, "coeff", nameRoot, sensors, "txt"
      );
    break;
    case TXT_REPORT_PATH:
      path = this->ConstructSensorFilePath(
        {""}, "report", nameRoot, sensors, "txt"
      );
    break;
    case TXT_HIGH_E_PEAK_PATH:
      path = this->ConstructSensorFilePath(
        {"txt"}, "high_map", nameRoot, sensors, "txt"
      );
    break;
  }
  return path;
}

TString CalibIOManager::GetPath(const Int_t fileType,
                                const TString& nameRoot,
                                const SensorRunInfo* sensor = nullptr) {
  auto sensors = new std::vector<SensorRunInfo*>(1, const_cast<SensorRunInfo*>(sensor));
  TString path = GetPath(fileType, nameRoot, sensors);
  delete sensors;
  return path;
}

template<typename T>
std::vector<T> CalibIOManager::GetSensorThresholds(const SensorRunInfo* sensor,
                                                   const TString& runId) {
  const TString filePath  = (sensor->fThresholdFilePath == "") 
                          ? this->GetPath(TXT_THRESHOLD_PATH, runId, sensor)
                          : sensor->fThresholdFilePath;
  auto thresholds = ReadVectorFromFile<T>(filePath);
  return thresholds;
}

template<typename T>
std::vector<std::vector<T>>
CalibIOManager::GetCalibCoefficients(const SensorRunInfo* sensor,
                                     const TString& runId) 
{
  const TString filePath  = (sensor->fCalibFilePath == "") 
                          ? this->GetPath(TXT_CALIB_COEFF_PATH, runId, sensor)
                          : sensor->fCalibFilePath;
  auto coeffs = Read2DVectorFromFile<T>(filePath);
  return coeffs;
}

class TaskManager {
  /** @class TaskManager
   ** @brief Class unifies access to the calibration data stored on the disk.
   **
   ** * Noise thresholds finding for a multiplicity selections
   ** * Multiplicity event selection.
  **/ 

public:
  TaskManager() = default;
  TaskManager(const TString& rawDataPath);
  ~TaskManager() = default;


protected:
  CalibIOManager *fIOManager = nullptr;
  TString fWorkDir = "result";
  TString fRunId = "";
  TString fRawDataPath = "";
};

TaskManager::TaskManager(const TString& rawDataPath) 
  : fIOManager(new CalibIOManager()), fRawDataPath(rawDataPath) {
  TString rawFileNameBase = GetFileNameBaseFromPath(fRawDataPath);
  const TString resDir = gSystem->PrependPathName(fWorkDir, rawFileNameBase);
  fRunId = GetFileNameBaseFromPath(fRawDataPath);
  fIOManager->ChangeDir(resDir);
}


class Preprocessing: public TaskManager {
/** @class Preprocessing
 ** @brief Class makes preliminary handling of a raw calibration data, including:
 ** * Transformation of an input data to a format with a single branch needed for analysis.
 ** An input file can be raw ROOT data file after:
 ** 1) Using of a TNeEvent based library for *.lmd file convertions.
 ** 2) AqqDAQ repacking (https://github.com/evovch/ACCULINNA_go4_user_library)
 **
 ** * Noise thresholds finding for a multiplicity selections
 ** * Multiplicity event selection.
**/
public:
  Preprocessing() = default;
  Preprocessing(const TString& rawDataPath);
  ~Preprocessing() = default;

  // void DrawRawData();
  // void DrawMultiplicitySelected();

  /**
  ** @brief Adds sensor to a current preprocessing.
  ** Sensors are form common file.
  **/
  void AddSensor(SensorRunInfo* sensor) {fSensors->push_back(sensor);}

  /**
  ** @brief Prepares raw input tree by leaving only leaves needed for the further analysis.
  ** As a result file [WORKING_DIR]/[SENSOR_NAME]/input/input*.root is created.
  ** Obligatory it contains aliases "[SENSOR_NAME]" for analyzed branches.
  **
  ** @param option select input file source. Only one option is implemented for now "neevent", 
  ** which means input file were converted by FLNR TNeEvent based library. 
  ** In further "aqqdaq" option will avaliable for files repached by AqqDAQ library 
  ** (https://github.com/evovch/ACCULINNA_go4_user_library)
  **/
  void ConvertTree(const TString& option = "neevent");

  /** @brief Finds noise thresholds, saves results to a file.
  ** Algorithm:
  ** 1) Find bin number with maximal amplitude MAX_BIN_NB
  ** 2) Move to the right since MAX_BIN_NB while bin amplitude value decreases.
  ** The first bin with ascending amplitude value is a threshold boundary.
  ** Resulting values are stored in the [WORKING_DIR]/[SENSOR_NAME]/txt/thresolds_*.root file.
  **
  ** @param opt draw option. 'draw_off' -- draws resulting threshold positions in a file
  ** [WORKING_DIR]/[SENSOR_NAME]/draw/thresholds_*.root.
  **/
  void FindThresholds(const TString& opt = "draw_off");
  
  /** @brief Performs multiplicity selection of a data.
  ** Method creates a tree where in each event analyzed sensors have only one channel
  ** with amplitude value higher then noise thresholds.
  **
  ** @param opt draw option. 'draw_off' -- draws resulting threshold positions in a file
  ** [WORKING_DIR]/[SENSOR_NAME]/draw/multiplicity_*.root.
  **/
  void MultiplicitySelection(const TString& opt = "draw_off");

  /** @brief Creates strips spectra for the further analysis analysis.
  ** Saves resulting hists for each sensor by path
  **   [WORKING_DIR]/[SENSOR_NAME]/draw/spectra_*.root.
  **/  
  void CreateSpectraHists();

  void Exec();

private:
  std::vector<SensorRunInfo*> *fSensors = nullptr; // all the sensors are from one data file
};

Preprocessing::Preprocessing(const TString& rawDataPath) : TaskManager(rawDataPath) {
  fSensors = new std::vector<SensorRunInfo*>();
}

void Preprocessing::ConvertTree(const TString& option = "neevent") {
  if (option == "neevent") {
    auto inFile = fIOManager->OpenRootFile(fRawDataPath);
    const auto tree = static_cast<TTree*>(GetObjectFromRootFile(inFile));
    Info(
      "Preprocessing::ConvertTree", "Converting a tree '%s' from the file '%s'", 
      tree->GetName(), fRawDataPath.Data()
    );
    // leave in a tree sensors for analysis
    tree->SetBranchStatus("*", 0);
    for (const auto *sensor: *fSensors) {
      const TString brName = Form("NeEvent.%s[%d]", sensor->fName.Data(), sensor->fStripAmount);
      tree->SetBranchStatus(brName, 1);
      tree->SetAlias(sensor->fName, brName);
    }
    const TString outFilePath = fIOManager->GetPath(ROOT_INPUT_REDUCED_TREE_PATH, fRunId, fSensors);
    const auto outFile = fIOManager->CreateRootFile(outFilePath);
    const auto *newtree = tree->CloneTree();
    tree->Write();
    outFile->Write();
    outFile->Close();
    inFile->Close();
  }
  if (option == "aqqdaq") {
    /*aqqdaq convertion code will be here*/
  }
}

void Preprocessing::FindThresholds(const TString& opt = "draw_off") {
  for (const auto *sensor: *fSensors) {
    // read tree from a generated input file name
    const TString filePath = fIOManager->GetPath(ROOT_INPUT_REDUCED_TREE_PATH, fRunId, fSensors);
    auto inFile = fIOManager->OpenRootFile(filePath);
    const auto tree = static_cast<TTree*>(GetObjectFromRootFile(inFile));
    Info("Preprocessing::FindThresholds", "Tree entries: %lld", tree->GetEntries());
    std::vector<Double_t> thersholdArray; 
    thersholdArray.resize(sensor->fStripAmount);
    // parameters exclude zero bin from a histograms because in the zero bin 
    // a high amplitude value is occured sometimes
    const Int_t excludeBins = 1;
    const TString histParams = Form(
      "(%d,%d,%d)", sensor->fBinAmount - excludeBins, excludeBins, sensor->fBinAmount
      );
    for (Int_t iStrip = 0; iStrip < sensor->fStripAmount; iStrip++) {
      const TString histName = Form("threshold_strip_%d", iStrip);
      const TString drawExpression = Form(
        "%s[%d]>>%s%s", sensor->fName.Data(), iStrip, histName.Data(), histParams.Data()
      ); 
      tree->Draw(drawExpression,"","");
      const TH1F *histThreshold = static_cast<TH1F*>(gDirectory->Get(histName));
      const Int_t binsAmount = histThreshold->GetXaxis()->GetNbins();
      // Get a number of a bin with the maximal amplitude
      const Int_t maxBinNb = histThreshold->GetMaximumBin();
      Bool_t minBinObtained = false;
      Int_t  thresholdBinNb = 0;
      Int_t  prevBinContent = histThreshold->GetBinContent(maxBinNb);
      // Starting from the maximum value bin, find the first bin with not decreasing value
      for (Int_t binNb = maxBinNb + 1; binNb < binsAmount && !minBinObtained; binNb++) {
        const Int_t binContent = histThreshold->GetBinContent(binNb);
        if (prevBinContent <= binContent) {
          minBinObtained = true;
          thresholdBinNb = binNb;
        }
        prevBinContent = binContent;
      }
      thersholdArray[iStrip] = --thresholdBinNb;
    }
    const TString thresholdsPath = fIOManager->GetPath(TXT_THRESHOLD_PATH, fRunId, sensor);
    auto file = fIOManager->CreateTextFile(thresholdsPath);
    DumpVector(thersholdArray, file);
    file.close();
    inFile->Close();
  }  
}

void Preprocessing::MultiplicitySelection(const TString& opt = "draw_off") {
  // Read input file
  const TString inFilePath = fIOManager->GetPath(ROOT_INPUT_REDUCED_TREE_PATH, fRunId, fSensors);
  auto inFile = fIOManager->OpenRootFile(inFilePath);
  const auto inTree = static_cast<TTree*>(GetObjectFromRootFile(inFile));
  // Create output file and tree
  const TString outFilePath = fIOManager->GetPath(ROOT_MULT_SELECTED_PATH, fRunId, fSensors);
  auto outFile = fIOManager->CreateRootFile(outFilePath);
  TTree *outTree = new TTree(inTree->GetName(), "Tree with a single multiplicity");
  // Create internal tree objects structure
  inTree->SetMakeClass(1);
  auto *sensorsData = new std::vector<std::vector<UShort_t>>();
  auto *thresholds = new std::vector<std::vector<UShort_t>>();
  for (const auto *sensor: *fSensors) {
    sensorsData->push_back(std::vector<UShort_t>(sensor->fStripAmount));
    TString branchName = inTree->GetAlias(sensor->fName);
    inTree->SetBranchAddress(branchName, &(sensorsData->back()[0]));
    // Connect data variables with tree branches
    outTree->Branch(sensor->fName, &(sensorsData->back()[0]), branchName + "/s");
    // Read sensor's thresholds
    auto sensorThresholds = fIOManager->GetSensorThresholds<UShort_t>(sensor, fRunId);
    thresholds->push_back(sensorThresholds);
  }
  Info("Preprocessing::MultiplicitySelection", "Begin multiplicity selection");
  Info("Preprocessing::MultiplicitySelection", "Input tree entries: %lld", inTree->GetEntries());
  for (Long64_t eventNb = 0; eventNb < inTree->GetEntries(); eventNb++) {
    inTree->GetEntry(eventNb);
    if (!CheckIfSingleMultiplicity(fSensors, sensorsData, thresholds)) {
      continue;
    } else {
      outTree->Fill();
    }
  }
  Info("Preprocessing::MultiplicitySelection", "Output tree entries: %lld", outTree->GetEntries());
  outTree->Write();
  outFile->Write();
  outFile->Close();
  inFile->Close();
  CreateSpectraHists();
}

void Preprocessing::CreateSpectraHists() {
  const TString multSelectPath = fIOManager->GetPath(ROOT_MULT_SELECTED_PATH, fRunId, fSensors);
  for (const auto *sensor: *fSensors) {
    auto inFile = fIOManager->OpenRootFile(multSelectPath);
    const auto tree = static_cast<TTree*>(GetObjectFromRootFile(inFile));    
    auto thresholds = fIOManager->GetSensorThresholds<UShort_t>(sensor, fRunId);

    const TString histSpectraPath = fIOManager->GetPath(ROOT_HIST_SPECTRA_PATH, fRunId, sensor);
    auto histSpectraFile = fIOManager->CreateRootFile(histSpectraPath);
    DrawSensorSpectraByStrip(tree, sensor, thresholds);
    histSpectraFile->Close();
    inFile->Close();
  }
}

void Preprocessing::Exec() {
  ConvertTree();
  FindThresholds();
  MultiplicitySelection();
}

class PeakSearch {
public:  
  enum PeakSearchAlgorithm {
    SLIDING_WINDOW,
    GAUSS
  };

  PeakSearch() = default;
  ~PeakSearch() = default;

  void SetPeakSearchMethod(const TString& peakSearchAlgorithm);
  void SetFitMinSigma(const Double_t value) {fFitMinSigma = value;}
  void SetFitPeakThreshold(const Double_t value) {fFitPeakThreshold = value;}
  void SetSearchRadius(const Int_t value) {fSearchRadius = value;}
  void SetSlideWindowWidth(const Int_t value) {fSlideWindowWidth = value;}
  void SetPeakWidth(const Int_t value) {fPeakWidth = value;}
  /**
  ** Returns sorted list of peaks found by TSpectrum algorithm.
  ** @param hist input histogram to search peaks
  **/
  std::list<Double_t> GetPeaksTSpectrum(TH1* hist, 
                                        const Double_t fitMinSigma, 
                                        const Double_t fitPeakThreshold);

   
  std::list<Double_t> SlidingWindowPeakSearch(TH1* hist, const std::list<Double_t>& initGuess,
                                              const Int_t windowWidth,
                                              const Int_t searchRadius);

  std::list<Double_t> GaussPeakSearch(TH1* hist, const std::list<Double_t>& initGuess,
                                      const Int_t peakWidth);

  /**
  ** @brief Returns peaks according to set options.
  ** @param hist histogram to search peak
  ** @param initGuess initial guess for searching methods
  **/
  std::list<Double_t> GetPeaks(TH1* hist, const std::list<Double_t>& initGuess);
protected:
  Int_t fPeakSearchMethod = SLIDING_WINDOW;
  // Peak search algoritm common parameters
  Double_t fFitMinSigma = 6.;
  Double_t fFitPeakThreshold = 0.7;  
  // Sliding window algorithm parameters
  Int_t fSearchRadius = 10; // interval length for window sliding search.
                             // The middle of the interval is peak position found by TSpectrum
  Int_t fSlideWindowWidth = 10;
  // Gauss + pol algorithm parameters
  Int_t fPeakWidth = 15;
};

void PeakSearch::SetPeakSearchMethod(const TString& peakSearchAlgorithm) {
  if (peakSearchAlgorithm == "sliding_window") {
    fPeakSearchMethod = SLIDING_WINDOW;
  }
  if (peakSearchAlgorithm == "gauss") {
    fPeakSearchMethod = GAUSS;
  }
}


std::list<Double_t> PeakSearch::GetPeaksTSpectrum(TH1* hist, 
                                                  const Double_t fitMinSigma, 
                                                  const Double_t fitPeakThreshold)
{
  TSpectrum sc;
  sc.Search(hist, fitMinSigma, "", fitPeakThreshold);
  const Int_t peaksAmount = sc.GetNPeaks();
  Info("PeakSearch::GetPeaksTSpectrum", "Occured peaks amount is %d", peaksAmount);
  Double_t* peaksPos = sc.GetPositionX();
  std::list<Double_t> peaks(peaksPos, peaksPos + peaksAmount);
  peaks.sort();
  // remove markers from the histogram
  auto functions = hist->GetListOfFunctions();
  auto pm = static_cast<TPolyMarker*>(functions->FindObject("TPolyMarker"));
  functions->Remove(pm); 
  return peaks;
}


std::list<Double_t> 
PeakSearch::SlidingWindowPeakSearch(TH1* hist, const std::list<Double_t>& initGuess, 
                                    const Int_t windowWidth,
                                    const Int_t searchRadius) 
{
  std::list<Double_t> peaks;
  for (const auto& guessPos: initGuess) {
    gStyle->SetStatFormat("6.8g");
    const Int_t peakBinNb = hist->GetXaxis()->FindBin(guessPos);
    Int_t maxIntegral = numeric_limits<Int_t>::min();
    Double_t peakMean;
    // Double_t peakRMS;
    for (Int_t i = peakBinNb - searchRadius; i < peakBinNb + searchRadius - windowWidth; i++) {
      hist->GetXaxis()->SetRange(i, i + windowWidth - 1 /*to not include the last bin*/);
      const Int_t integral = hist->Integral();
      if (maxIntegral < integral) {
        maxIntegral = integral;
        peakMean = hist->GetMean();
        // peakRMS = hist->GetStdDev();
      }
    }
    cout << "peakMean " << peakMean << endl;
    peaks.push_back(peakMean);
  }
  return peaks;
}

std::list<Double_t> 
PeakSearch::GaussPeakSearch(TH1* hist, const std::list<Double_t>& initGuess,
                            const Int_t peakWidth) 
{
  std::list<Double_t> peaks;
  peaks = initGuess;
  return peaks;
}

std::list<Double_t> PeakSearch::GetPeaks(TH1* hist, const std::list<Double_t>& initGuess) {
  std::list<Double_t> peaks;
  switch (fPeakSearchMethod) {
    case SLIDING_WINDOW: 
      peaks = SlidingWindowPeakSearch(hist, initGuess, fSlideWindowWidth, fSearchRadius);
      break;
    case GAUSS: 
      peaks = GaussPeakSearch(hist, initGuess, fPeakWidth);
      break;
    default: 
      Error("PeakSearch::SearchPeaks", "Unknown peak search method is set");
  }
  return peaks;
}

/**
** Radioactive source alpha lines
**/
const static std::vector<Double_t> fAlphaE = {4.7844, 6.0024, 7.6869}; 
/** 
** dE(E, d) [MeV] qudratic approximation coefficients for each alpha source energy
** in format {p0, p1, p2} where p0, p1, p2 --- coefficients: dE = p0 + p1*d + p2*d^2  
**/
const static std::vector<std::vector<Double_t>> fElossApprox = {{0.0010319, 0.146954, 0.00181655},
                                                                {0.0004273, 0.127711, 0.00106127},
                                                                {0.0001624, 0.108467, 0.000589357}};
class Calibration: public TaskManager, public PeakSearch {
/* @class Calibration
  @brief Class implements SSD calibration procedure described 
  in http://er.jinr.ru/si_detector_calibration.html.
*/
public:
  Calibration() = default;
  Calibration(const TString& rawDataPath);
  ~Calibration() = default;

  void SetSensor(SensorRunInfo* sensor) {fSensor = sensor;}
  /** @brief Executes all calibration steps
  ** 1) Peaks position determination. 
  ** 2) Dead layer estimation. 
  ** 3) Calibration coefficients calculation. 
  ** 4) Report file printing. 
  **/  
  void Exec();
private:
  /**
  ** @brief Finds peaks with a set algorithm method.
  ** Stores statistic files in the directory [WORK_DIR]/run_id/sensor_name/statistics/ :
  ** * peaks positions found by a chosen algorithm - mean_(run_id)_(sensor_name).txt
  ** * standard deviation (RMS) - rms_(run_id)_(sensor_name).txt
  ** * TSpectrum peaks positions tspectrum_(run_id)_(sensor_name).txt
  **
  ** Files structure by lines:
  ** value_low_energy_strip_0, value_middle_energy_strip_0, value_high_energy_strip_0
  ** value_low_energy_strip_1, value_middle_energy_strip_1, value_high_energy_strip_1
  ** ...
  ** value_low_energy_strip_N, value_middle_energy_strip_N, value_high_energy_strip_N
  **/
  void SearchPeaks();

  void DeadLayerEstimation();
  void CalcCalibrationCoefficients();
  void PrintReport(const std::vector<std::vector<Double_t>>& peaks,
                   const std::vector<Double_t>& deadVec,
                   const Double_t avgDead,
                   const std::vector<std::vector<Double_t>>& coeffsAB);
  void SearchPeaksTSpectrum();

  std::vector<Double_t> GetAlphaEnergiesAfterDeadLayer (const Double_t dead);


  /** 
  ** @brief Returns dead layer thickness [um] by eta parameter value.
  ** d(eta) is a qudratic approximation
  ** // in format {p0, p1, p2} where p0, p1, p2 --- coefficients: eta = p0 + p1*d + p2*d^2 
  ** @param eta eta parameter value
  **/
  Double_t GetDeadLayerByEta (const Double_t eta);

private:
  SensorRunInfo* fSensor = nullptr;
};

Calibration::Calibration(const TString& rawDataPath) : TaskManager(rawDataPath) {
  fSensor = new SensorRunInfo();
}

void Calibration::SearchPeaks() {
  const TString spectraHistPath = fIOManager->GetPath(ROOT_HIST_SPECTRA_PATH, fRunId, fSensor);

  const TString peaksHistPath = fIOManager->GetPath(ROOT_HIST_PEAKS_PATH, fRunId, fSensor);
  const auto peakHists = fIOManager->CreateRootFile(peaksHistPath);

  std::vector<std::list<Double_t>> peaks;
  for (Int_t iStrip = 0; iStrip < fSensor->fStripAmount; iStrip++) {
    const TString histName = Form("strip_%d", iStrip);
    const auto histFile = fIOManager->OpenRootFile(spectraHistPath);
    const auto hist = static_cast<TH1D*>(GetObjectFromRootFile(histFile, histName));
    const auto peaksTSpec = GetPeaksTSpectrum(hist, fFitMinSigma, fFitPeakThreshold);
    auto stripPeaks = GetPeaks(hist, peaksTSpec);
    if (stripPeaks.size() == 4) {
      // delete the second element from the list
      auto itDelete = stripPeaks.begin();
      std::advance(itDelete, 1);
      stripPeaks.erase(itDelete);
    } else {
      Warning(
        "Calibration::SearchPeaks", "Found peaks amount doesn't equal 4 (%zu)", stripPeaks.size()
      );
      Warning("Calibration::SearchPeaks", "By default values are set to Nan");
      stripPeaks.clear();
      stripPeaks = std::list<Double_t>(3, std::numeric_limits<double>::quiet_NaN());
    }
    peaks.push_back(stripPeaks);
    histFile->Close();
  }

  const TString peakDataPath = fIOManager->GetPath(TXT_PEAK_DATA_PATH, fRunId, fSensor);
  // @TODO add methods to set peak markers on histograms
  auto peaksFile = fIOManager->CreateTextFile(peakDataPath);
  Dump2DVector(peaks, peaksFile);
  peaksFile.close();
}

Double_t Calibration::GetDeadLayerByEta (const Double_t eta) {
  // quad fit for elosses caliculated with 1e-5um step
  return (-5272.9763 + 19011.5041 * eta - 17102.8129 * eta * eta);
}


void Calibration::DeadLayerEstimation() {
  // read peaks from file
  const TString peakDataPath = fIOManager->GetPath(TXT_PEAK_DATA_PATH, fRunId, fSensor);
  auto peaksFile = fIOManager->OpenTextFile(peakDataPath);
  auto peaks = Read2DVectorFromFile<Double_t>(peakDataPath);
  
  vector<Double_t> deadVec;
  for (const auto stripPeaks: peaks) {
    const Double_t eta = (stripPeaks[2] - stripPeaks[1]) / (stripPeaks[2] - stripPeaks[0]);
    const Double_t deadLayer = GetDeadLayerByEta(eta); // [um]
    deadVec.push_back(deadLayer);
  }

  const TString deadPath = fIOManager->GetPath(TXT_DEAD_LAYER_PATH, fRunId, fSensor);
  auto file = fIOManager->CreateTextFile(deadPath);
  DumpVector(deadVec, file);
}

std::vector<Double_t> Calibration::GetAlphaEnergiesAfterDeadLayer (const Double_t dead) {
  std::vector<Double_t> energies;
  for (Int_t i = 0; i < fElossApprox.size(); i++) {
    const Double_t energy = fAlphaE[i] 
                          - fElossApprox[i][2] * pow(dead, 2) 
                          - fElossApprox[i][1] * dead 
                          - fElossApprox[i][0];
    energies.push_back(energy);
  }
  return energies;
}


void Calibration::CalcCalibrationCoefficients() {
  // read peaks from file
  const TString peakDataPath = fIOManager->GetPath(TXT_PEAK_DATA_PATH, fRunId, fSensor);
  auto peaks = Read2DVectorFromFile<Double_t>(peakDataPath);
  // read dead layers from file
  const TString deadDataPath = fIOManager->GetPath(TXT_DEAD_LAYER_PATH, fRunId, fSensor);
  auto deadVec = ReadVectorFromFile<Double_t>(deadDataPath);
  // calculate average dead layer
  Double_t sumDead = 0.;
  Int_t notNaNStrips = 0;
  for (const auto& dead: deadVec) {
    if (!std::isnan(dead)) {
      sumDead += dead;
      notNaNStrips++;
    }
  }
  const Double_t avgDead = sumDead / notNaNStrips;
  
  auto energies = GetAlphaEnergiesAfterDeadLayer(avgDead);

  std::vector<std::vector<Double_t>> coeffsAB;
  for (Int_t iStrip = 0; iStrip < peaks.size(); iStrip++) {
    auto gr = new TGraph(energies.size(), &peaks[iStrip][0], &energies[0]);
    TFitResultPtr fitRes = gr->Fit("pol1","S");
    const Double_t a = fitRes->Parameter(1);
    const Double_t b = fitRes->Parameter(0);
    std::vector<Double_t> coeffs = {a, b}; 
    coeffsAB.push_back(coeffs);
  }

  const TString deadPath = fIOManager->GetPath(TXT_DEAD_LAYER_PATH, fRunId, fSensor);
  auto fileDead = fIOManager->CreateTextFile(deadPath);
  DumpVector(deadVec, fileDead);
  fileDead.close();

  const TString calibPath = fIOManager->GetPath(TXT_CALIB_COEFF_PATH, fRunId, fSensor);
  auto fileCalib = fIOManager->CreateTextFile(calibPath);
  Dump2DVector(coeffsAB, fileCalib);
  fileCalib.close();

  PrintReport(peaks, deadVec, avgDead, coeffsAB);
}

void Calibration::PrintReport(const std::vector<std::vector<Double_t>>& peaks,
                              const std::vector<Double_t>& deadVec,
                              const Double_t avgDead,
                              const std::vector<std::vector<Double_t>>& coeffsAB) {
  const TString reportPath = fIOManager->GetPath(TXT_REPORT_PATH, fRunId, fSensor);
  auto report = fIOManager->CreateTextFile(reportPath);
  report << std::right;
  report << "Calibration results. " << endl << endl;
  report << "File ID is " << fRunId << endl;
  if (fPeakSearchMethod == SLIDING_WINDOW) {
    report << "Peak search method: SLIDING_WINDOW" << endl;
    report << "Sliding window algorithm parameters: " <<  endl;
    report << "  window width: " << fSlideWindowWidth <<  endl;
    report << "  search region around TSpectrum peak: +-" << fSearchRadius <<  endl;
    report << "  peak RMS: " << fFitMinSigma <<  endl;
    report << "  peak amplitude threshold (with respect to maximal): " 
               << fFitPeakThreshold <<  endl;
  }
  report << endl;
  report << "Determined peak positions (in [ADC-channels]):" << endl;
  report << "StripNb" << std::right 
         << setw(20) << "E_low" 
         << setw(20) << "E_middle" 
         << setw(20) << "E_high"  << endl;
  Int_t iStrip = 0;
  for (const auto& stripPeaks: peaks) {
    report << iStrip++ << std::right 
           << setw(20) << stripPeaks[0] 
           << setw(20) << stripPeaks[1] 
           << setw(20) << stripPeaks[2] << endl;
  }
  report << endl;
  report << "Dead layer estimation [um] by strips: " << endl;
  report << "StripNb" << setw(20) << "Dead layer"<< endl;
  iStrip = 0;
  for (const auto& dead: deadVec) {
    report << iStrip++ << std::right
           << setw(20) << dead << endl;
  }
  report << "Avg:" << setw(20) << avgDead << endl;
  report << endl;
  report << "Calibration coefficients: " << endl;  
  report << "StripNb" << std::right 
         << setw(20) << "a" 
         << setw(20) << "b"<< endl;
  iStrip = 0;
  for (const auto& stripCoeffsAB: coeffsAB) {
    report << iStrip++ << std::right
           << setw(20) << stripCoeffsAB[0] 
           << setw(20) << stripCoeffsAB[1] << endl;
  }
  report.close();
}

void Calibration::Exec() {
  SearchPeaks();
  DeadLayerEstimation();
  CalcCalibrationCoefficients();
}

class NonUniformityMapBuilder: public TaskManager, public PeakSearch {
public:
  NonUniformityMapBuilder() = default;
  ~NonUniformityMapBuilder() = default;

  void SetThickSensor(SensorRunInfo* sensor) {fMapSensors->at(0) = sensor;}
  void SetThinSensor(SensorRunInfo* sensor) {fMapSensors->at(1) = sensor;}
  void SetThickCalibSensor(SensorRunInfo* sensor) {fSensorCalib = sensor;}

  /**
  ** @brief Draws thick sensor pixels and saves histograms to the file.
  **
  ** Pixel - thick sensor strip data which is also registered in a thin sensor.
  ** Readout data are superior to noise thresholds in both stations.
  **/
  void DrawPixelSpectra();
  void SearchPixelHighEnergyPeak();
  void Exec();
private:
  // fMapSensors->at(0) - thick sensor info, fMapSensors->at(1) - thin sensor info
  std::vector<SensorRunInfo*>* fMapSensors = new std::vector<SensorRunInfo*>(2, nullptr); 
  SensorRunInfo* fSensorCalib = nullptr; // info about thick sensor from a calibration run
};

void NonUniformityMapBuilder::Exec() {
  DrawPixelSpectra();
  SearchPixelHighEnergyPeak();
}

void NonUniformityMapBuilder::DrawPixelSpectra() {
  // Get input tree
  const TString multSelectPath = fIOManager->GetPath(
    ROOT_MULT_SELECTED_PATH, fMapSensors->at(0)->fRunId, fMapSensors
  );
  cerr << "llogogogogoogogo" << multSelectPath << endl;
  auto multSelectFile = fIOManager->OpenRootFile(multSelectPath);
  auto tree = static_cast<TTree*>(GetObjectFromRootFile(multSelectFile));
  // Create output file
  const TString pixelSpectraPath = fIOManager->GetPath(
    ROOT_HIST_PIXEL_PATH, fMapSensors->at(0)->fRunId, fMapSensors
  );
  auto outFile = fIOManager->CreateRootFile(pixelSpectraPath);
  // Read thresholds
  const TString thresholdThickPath = fIOManager->GetPath(
    TXT_THRESHOLD_PATH, fMapSensors->at(0)->fRunId, fMapSensors->at(0)
  );
  const TString thresholdThinPath = fIOManager->GetPath(
    TXT_THRESHOLD_PATH, fMapSensors->at(1)->fRunId, fMapSensors->at(1)
  );
  const auto thresholdThick = ReadVectorFromFile<UShort_t>(thresholdThickPath);
  const auto thresholdThin = ReadVectorFromFile<UShort_t>(thresholdThinPath);
  
  DrawSensorSpectraByPixel(tree, fMapSensors->at(0), fMapSensors->at(1), 
                           thresholdThick, thresholdThin);
  multSelectFile->Close();
  outFile->Close();
}

void NonUniformityMapBuilder::SearchPixelHighEnergyPeak() {
  std::vector<std::vector<Double_t>> peaks;
  for (Int_t iStripThick = 0; iStripThick < fMapSensors->at(0)->fStripAmount; iStripThick++) {
    for (Int_t iStripThin = 0; iStripThin < fMapSensors->at(1)->fStripAmount; iStripThin++) {
      // tree
    }
  }
  const TString highEPeaksPath = fIOManager->GetPath(
    TXT_HIGH_E_PEAK_PATH, fRunId, fMapSensors->at(1)
  );
  auto peaksFile = fIOManager->CreateTextFile(highEPeaksPath);
  Dump2DVector(peaks, peaksFile);
}

} // namespace ERCalibrationSSD

using namespace ERCalibrationSSD;

void ssd_one_spectrum_calibration() {
  // Define input file path with raw data converted by TNEvent go4-based library
  const TString file_calib_path = 
    "/mnt/analysis_nas/exp201904/clb/oldLib/postClb/grouped/calib_1mm_90_since25to33.root";

  // [Prepare information about sensor in the calibration run]
  // Constructor parameters: 
  // * sensor branch name in raw data file
  // * stips amount
  // * bins amount in analysis histograms
  // * raw data file path
  //
  auto ssd_1m_1 = new SensorRunInfo("SSD_1m_1", 16, 1024, file_calib_path);
  auto ssd_1m_2 = new SensorRunInfo("SSD_1m_2", 16, 1024, file_calib_path);
  auto ssd_1m_3 = new SensorRunInfo("SSD_1m_3", 16, 1024, file_calib_path);
  auto ssd_1m_4 = new SensorRunInfo("SSD_1m_4", 16, 1024, file_calib_path);

  // [Preprocessing]
  // Constructor parameter is a raw data file path.
  // The base of a raw input file name is a run ID.
  // For example, if the input file path is /path/to/file/calib_1mm_90_all.root
  // then run ID is 'calib_1mm_90_all' which will be used as subfolder to
  // store results in a resulting directory
  //
  // For the calibration  only one sensor should be added to 
  // preprocessing by AddSensor() method
  auto prep_ssd_1m_1 = new Preprocessing(file_calib_path);
  prep_ssd_1m_1->AddSensor(ssd_1m_1);

  // Execution of preprocessing may be performed fully automaticaly by call Exec()
  // method
  // prep_ssd_1m_1->Exec();
  // or separately, if user one want to control each preprocessing step results.
  // Important that each the following steps can work only with a data produced by
  // precending method in order `ConvertTree() -> FindThresholds() -> MultiplicitySelection()`.
  // * Convertation of an input tree by leaving only the single branch conrned with
  //   analyzed sensor set by AddSensor() method.
  //   ouput file is stored by path
  //   ./result/[run_id]/input/input_[run_id]_[sensor_name].root
  prep_ssd_1m_1->ConvertTree();

  // * Find noise thresholds in each strip
  //   resulting file is stored by path
  //   ./result/[run_id]/[sensor_name]/txt/threshold_[run_id]_[sensor_name].txt
  prep_ssd_1m_1->FindThresholds();
 
  // * Leave for further analysis only the data with single multiplity - events there
  //   only one of strips has signal higher than a noise threshold.
  //   Strips spectra histograms can be viewed by path
  //  ./result/[run_id]/[sensor_name]/draw/spectra_[run_id]_[sensor_name].root
  //   Tree with a data is in the
  //  ./result/[run_id]/input/mult_one_[run_id]_[sensor_name].root
  prep_ssd_1m_1->MultiplicitySelection();

  // [Calibration] (http://er.jinr.ru/si_detector_calibration.html)
  auto calib_ssd_1m_1 = new Calibration(file_calib_path);
  calib_ssd_1m_1->SetSensor(ssd_1m_1);

  // [[Define peak search algoritm parameters]] 
  // Default values may be found in PeakSearch class documentation

  // Choose peak search method. Two option are avaliable: "sliding_window" or "gauss"
  calib_ssd_1m_1->SetPeakSearchMethod("sliding_window");
  
  // [[Set algorithm options]]
  
  // All the algoritms use TSpectrum peak search result as initial guess.
  // It's parameters are:
  // * Minimal peak standard deviation;
  calib_ssd_1m_1->SetFitMinSigma(6.);
  // * Relative peks height with respect to the highest one.
  calib_ssd_1m_1->SetFitPeakThreshold(0.7);

  // [[Algorithm specific parameters]]

  // If "sliding_window" set by SetPeakSearchMethod()
  // * Sliding window width [bins] (w_width) 
  calib_ssd_1m_1->SetSlideWindowWidth(10);
  // * Interval of rearch around TSpectrum found peak (tspec_val) 
  //   is defined by [tspec_val - w_width; tspec_val + w_width]
  calib_ssd_1m_1->SetSearchRadius(15);

  // If "gauss" set by SetPeakSearchMethod() 
  // the same option works
  // calib_ssd_1m_1->SetSearchRadius(10);

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
  calib_ssd_1m_1->Exec();
}
