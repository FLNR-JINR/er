На данный момент симуляции возможны с двумя форматами геометрий: root и gdml.

Пример создания геометрии с помощью макроса будет приведен ниже. Все макросы, создающие геометрию, должны быть расположены в директроии ``/macro/geo/`` и быть названы по шаблону ``create_det_geo_v1.C``. Файл с результирующей геометрие должен находиться в ``geometry`` и называться по шаблону det.v1.geo.root.

В примере будет рассмотрен простой нейтронный детектор из 4 волокон квадратного сечения, расположенных вдоль пучка.

Инициализируаем глобальное смещение и поворот детектроа.

::

	void create_det_geo_v1()
	{
	  // Create a global translation
	  Float_t global_X = 0.;
	  Float_t global_Y = 0.;
	  Float_t global_Z = 0.;
	  //Create gloabal Rotation
	  TGeoRotation *fGlobalRotation = new TGeoRotation();
	  fGlobalRotation->RotateX(0.);
	  fGlobalRotation->RotateY(0.);
	  fGlobalRotation->RotateZ(0.);
	  // Create a zero rotation
	  TGeoRotation *fZeroRotation = new TGeoRotation();
	  fZeroRotation->RotateX(0.);
	  fZeroRotation->RotateY(0.);
	  fZeroRotation->RotateZ(0.);

Цепляем глобальные менеджер геометрии и читаем файл материалов.

::

	  TGeoManager*   gGeoMan = NULL;
	  // -------   Load media from media file   -----------------------------------
	  FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
	  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
	  TString geoPath = gSystem->Getenv("VMCWORKDIR");
	  TString medFile = geoPath + "/geometry/media.geo";
	  geoFace->setMediaFile(medFile);
	  geoFace->readMedia();
	  gGeoMan = gGeoManager;
	  // --------------------------------------------------------------------------

Задаемся путем где будет лежать файл результирующей геометрии.

::

	  // -------   Geometry file name (output)   ----------------------------------
	  TString geoFileName = geoPath + "/geometry/det.v1.geo.root";
	  // --------------------------------------------------------------------------

Некоторый костыль. Превращение объекта материала FairRoot в объект материала Root.
	  
::

	  // -----------------   Get and create the required media    -----------------
	  FairGeoMedia*   geoMedia = geoFace->getMedia();
	  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

	  FairGeoMedium* mBC408      = geoMedia->getMedium("BC408");
	  if ( ! mBC408 ) Fatal("Main", "FairMedium BC408 not found");
	  geoBuild->createMedium(mBC408);
	  TGeoMedium* pMed37 = gGeoMan->GetMedium("BC408");
	  if ( ! pMed37 ) Fatal("Main", "Medium BC408 not found");
	  
	  FairGeoMedium* vacuum      = geoMedia->getMedium("vacuum");
	  if ( ! vacuum ) Fatal("Main", "FairMedium vacuum not found");
	  geoBuild->createMedium(vacuum);
	  TGeoMedium* pMed0 = gGeoMan->GetMedium("vacuum");
	  if ( ! pMed0 ) Fatal("Main", "Medium vacuum not found");
	  // --------------------------------------------------------------------------

Создаем объемы геометрии. Верхний объем всегда Assembly и называется TOP. Объем детектра тоже Assembly.
	  
::

	  //------------------------- VOLUMES -----------------------------------------
	  
	  // --------------   Create geometry and top volume  -------------------------
	  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
	  gGeoMan->SetName("DETgeom");
	  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
	  gGeoMan->SetTopVolume(top);
	  TGeoVolume* det = new TGeoVolumeAssembly("det");
	  // --------------------------------------------------------------------------

	  //------------------ BC408  fiber  -----------------------------------------
	  Double_t fiber_X = 0.6;   //cm
	  Double_t fiber_Y = 0.6;   //cm
	  Double_t fiber_Z = 100.;  //cm
	  fiber_X /= 2.;
	  fiber_Y /= 2.;
	  fiber_Z /= 2.;
	  TGeoVolume *fiber = gGeoManager->MakeBox("fiber", pMed37, fiber_X, fiber_Y, fiber_Z);

Создаем структуру. Указываем как объёмы вложены друг в друга.

::

	  //------------------ STRUCTURE  -----------------------------------------
	  //------------------ Add fibers to det  -----------------------------
	  Int_t fibers_in_det_X_Nb = 2;
	  Int_t fibers_in_det_Y_Nb = 2;
	  
	  Double_t det_X = fiber_X * fibers_in_det_X_Nb;
	  Double_t det_Y = fiber_Y * fibers_in_det_Y_Nb;
	  Double_t det_Z = fiber_Z; 
	  Int_t i_fiber = 0;
	  for (Int_t i_Y_fiber = 0; i_Y_fiber < fibers_in_det_Y_Nb; i_Y_fiber++){
	    for (Int_t i_X_fiber = 0; i_X_fiber < fibers_in_det_X_Nb; i_X_fiber++){
	      Double_t fiber_in_det_X_trans = det_X - fiber_X*2*(i_X_fiber)-fiber_X;
	      Double_t fiber_in_det_Y_trans = det_Y - fiber_Y*2*(i_Y_fiber)-fiber_Y;
	      Double_t fiber_in_det_Z_trans = 0.;
	      det->AddNode( fiber, i_fiber, new TGeoCombiTrans(fiber_in_det_X_trans, 
	                                                            fiber_in_det_Y_trans,
	                                                            fiber_in_det_Z_trans, 
	                                                            fZeroRotation));
	      i_fiber++;
	    }
	  }
	  top->AddNode(det, 1, new TGeoCombiTrans(global_X,global_Y,global_Z,fGlobalRotation));

Проверяем ошибки в геометрии. Записываем ее в файл.

::

	  // ---------------   Finish   -----------------------------------------------
	  gGeoMan->CloseGeometry();
	  gGeoMan->CheckOverlaps(0.001);
	  gGeoMan->PrintOverlaps();
	  gGeoMan->Test();

	  TFile* geoFile = new TFile(geoFileName, "RECREATE");
	  top->Write();
	  geoFile->Close();
	  // --------------------------------------------------------------------------
	}

