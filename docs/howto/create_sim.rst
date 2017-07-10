Стандартный макрос симуляции детектора должен находится в папке ``~/expertroot/macro/det`` и называться ``sim.C``.

sim.C:

::

	void sim(Int_t nEvents = 1000){
	  //---------------------Files-----------------------------------------------
	  TString outFile= "sim.root";
	  TString parFile= "par.root";
	  // ------------------------------------------------------------------------

	  // -----   Timer   --------------------------------------------------------
	  TStopwatch timer;
	  timer.Start();
	  // ------------------------------------------------------------------------
	 
Создаем менеджер симуляции и указываем какой библиотекой будет осуществляться транспорт:

::

	  // -----   Create simulation run   ----------------------------------------
	  FairRunSim* run = new FairRunSim();
	  /** Select transport engine
	  * TGeant3
	  * TGeant4
	  **/
	  run->SetName("TGeant4");              // Transport engine
	  run->SetOutputFile(outFile.Data());          // Output file
	  // ------------------------------------------------------------------------

Создаем базу данных параметров

::

	  // -----   Runtime database   ---------------------------------------------
	  FairRuntimeDb* rtdb = run->GetRuntimeDb();
	  // ------------------------------------------------------------------------

Добавляем файл с материалами
	  
::

	  // -----   Create media   -------------------------------------------------
	  run->SetMaterials("media.geo");       // Materials
	  // ------------------------------------------------------------------------


Добавляем пассивный объем пещеры и активный детектор.

::

	  // -----   Create detectors  ----------------------------------------------	
	  FairModule* cave= new ERCave("CAVE");
	  cave->SetGeometryFileName("cave.geo");
	  run->AddModule(cave);
		
	  // Det definition
	  /* Select verbosity level
	   * 0 - only standard logs
	   * 1 - Print points after each event
	  */
	  Int_t verbose = 0;
	  ERDet* det= new ERDet("ERDet", kTRUE,verbose);
	  det->SetGeometryFileName("det.v1.geo.root");
	  run->AddModule(det);
	  // ------------------------------------------------------------------------
	
Инициализируем класс генератора событий:

::

	  // -----   Create PrimaryGenerator   --------------------------------------
	  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
	  Int_t pdgId = 2112; // neutron  beam
	  Double32_t theta1 = 0.;  // polar angle distribution
	  Double32_t theta2 = 7.;
	  Double32_t kin_energy = .500; //GeV
	  Double_t mass = TDatabasePDG::Instance()->GetParticle(pdgId)->Mass();
	  Double32_t momentum = TMath::Sqrt(kin_energy*kin_energy + 2.*kin_energy*mass); //GeV
	  FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, 1);
	  boxGen->SetThetaRange(theta1, theta1);
	  boxGen->SetPRange(momentum, momentum);
	  boxGen->SetPhiRange(90, 90);
	  boxGen->SetBoxXYZ(0.,0,0.6,0.6,0.);

	  primGen->AddGenerator(boxGen);
	  run->SetGenerator(primGen);
	  // ------------------------------------------------------------------------
		

Сохранение траекторий для event display и уровень подробности логов.

::

	  //-------Set visualisation flag to true------------------------------------
	  run->SetStoreTraj(kTRUE);
		
	  //-------Set LOG verbosity  ----------------------------------------------- 
	  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
	  
Запуск процедуры инициализации менеджера и инициализации базы данных параметров

::

	  // -----   Initialize simulation run   ------------------------------------
	  run->Init();
	  Int_t nSteps = -15000;
		
	  // -----   Runtime database   ---------------------------------------------
	  Bool_t kParameterMerged = kTRUE;
	  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
	  parOut->open(parFile.Data());
	  rtdb->setOutput(parOut);
	  rtdb->saveOutput();
	  rtdb->print();
	  // ---------------------------------------------------------
	  
Запуск на исполнение

::

	  // -----   Run simulation  ------------------------------------------------
	  run->Run(nEvents);
	  
	  // -----   Finish   -------------------------------------------------------
	  timer.Stop();
	  Double_t rtime = timer.RealTime();
	  Double_t ctime = timer.CpuTime();
	  cout << endl << endl;
	  cout << "Macro finished succesfully." << endl;
	  cout << "Output file is sim.root" << endl;
	  cout << "Parameter file is par.root" << endl;
	  cout << "Real time " << rtime << " s, CPU time " << ctime
			  << "s" << endl << endl;
	}


