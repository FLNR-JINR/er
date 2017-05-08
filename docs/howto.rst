How-to
======

Создать модель детектора
------------------------

Модель детектора состоит из описания геометрии и материалов.

Задание материалов
~~~~~~~~~~~~~~~~~~

Все материалы, учавствующие в моделировании, описаны в файле geometry/media.geo.

Формат файла тут: https://fairroot.gsi.de/?q=node/34.

Создание геометрии
~~~~~~~~~~~~~~~~~~

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


Создать библиотеку классв для детектора
---------------------------------------

Для каждого детектора создается его библиотека классов, в которую входят:

	* Классы симуляций детектора, унаследованные от ``ERDetector`` ;
	* Классы диджитизации, реконструкции и анализа, унаследованные от ``FairTask``;
	* Классы, описывающие множества параметров детектора, унаследованные от ``FairParGenericSet``;
	* Классы данных детектора, унаследованные напрямую от TObject или от ``FairMCPoint``, ``FairHit``, если это поинт или хит соотвественно.

Классы каждой библиотеки хранятся в директории первого уровня:

::

	cd ~/expertroot
	mkdir det
	cd det

Сборка библиотеки описывается файлом CMakeLists.txt, имеющим следующую структуру:

::
	
	# Create a library called "libDet" which includes the source files given in
	# the array .
	# The extension is already found.  Any number of sources could be listed here.

	set(INCLUDE_DIRECTORIES
	${BASE_INCLUDE_DIRECTORIES}
	${ROOT_INCLUDE_DIR}
	${Boost_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/det
	${CMAKE_SOURCE_DIR}/base
	)

	include_directories( ${INCLUDE_DIRECTORIES})

	set(LINK_DIRECTORIES
	${BASE_LINK_DIRECTORIES}
	${FAIRROOT_LIBRARY_DIR}
	) 

	link_directories( ${LINK_DIRECTORIES})

	set(SRCS
		ERDet.cxx
	)

	# fill list of header files from list of source files
	# by exchanging the file extension
	CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

	Set(LINKDEF ERDetLinkDef.h)
	Set(LIBRARY_NAME Det)
	Set(DEPENDENCIES ERBase ERData Base Core Geom)

	GENERATE_LIBRARY()


В переменную ``INCLUDE_DIRECTORIES`` следует добавить все директории с необходимыми файлами включений. Директории внешних пакетов: ``${BASE_INCLUDE_DIRECTORIES}, ${ROOT_INCLUDE_DIR}, ${Boost_INCLUDE_DIRS}`` стоит добавлять всегда. В переменную LINK_DIRECTORIES сдедует добавить директории библиотек. Переменные ``${BASE_LINK_DIRECTORIES}, ${FAIRROOT_LIBRARY_DIR}`` стоит добавлять всегда. В переменной ``SRCS`` следует перечислить все ``*.cxx`` исходные файлы библиотеки. В переменной ``LINKDEF`` служебный файл для интерпретатора ``cling``. В переменной ``DEPENDENCIES`` указать какие именно библиотеки необходимо подключить. Функция ``GENERATE_LIBRARY()`` автоматически сформирует библиотеку добавив операции записи в root файл объектам данным, и добавив необходимые объекты - словари, с помощью которых интерпретатор вызывает нужные методы классов из библиотек.

Служебный файл ``ERDetLinkDef.h`` будет иметь следующую структуру:

::

	#pragma link off all globals;
	#pragma link off all classes;
	#pragma link off all functions;

	#pragma link C++ class ERDet;

Если мы хотим указать, что объект данного класса будет писаться в root файл, необходимо добавить ``+`` после названия класса.

::

	#pragma link C++ class ERDetPoint+;

Минимальный набор файлов библиотеки классов детектора:

::

	CMakeLists.txt
	ERDetLinkDef.h
	ERDet.h
	ERDet.cxx

Чтобы добавить библиотеку в процесс общей сборки expertroot нужно добавить строку:

::

	add_subdirectory (det)

в файл ``~/expertroot/CMakeLists.txt`` туда где вызываюстя подобные команды (на данный момент после ``add_subdirectory (beamtime)``).

Создать симуляцию детектора
---------------------------

Класс симуляции
~~~~~~~~~~~~~~~
Процедура симуляции детектора заключается в записи данных из процедуры транспорта частиц через чувствительные (Sensitive) объёмы детектора. Главным атрибутом этих данных является потеря энергии частици (energy loss - eloss), так как она является источником для дальнейшего моделирования цифрового отклика детектора. Традиционно запись информации происходит в коллекцию объектов, называемых поинтами (Point). Поинт - прямолинейный отрезок трека в активном объёме. Поинт создается набором шагов транспорта от рождения или входа трек в чувствительный объём до выхода или конца трека. На запись поинтов в файл обычно устанавливают порог на eloss. 
Для создания симуляции детектора необходимо создать класс - населдник класса ``ERDetector`` и написать реализации методов:

	* ProcessHits - вызывается на каждом шаге транспорт внутри активного объёма детектора. В данном методе заложена процедура записи данных из объекта Virtual Monte Carlo (TVirtualMC - https://root.cern.ch/doc/master/classTVirtualMC.html) в коллекции выходных данных (обычно поинтов, но возможно и другое).
	* BeginEvent - вызывается в начале события
	* EndOfEvent - вызывается в конце события
	* Register - для связывания объектов и веток во выходном файле. Вызывается при run->Init().
	* GetCollection - возвращает коллекцию поинтов
	* Print - для вывода информации о событии
	* Reset - для обнуления всех коллекций и объектов между событиями
	* CopyClones - Copies the hit collection with a given track index offset
	* ConstructGeometry - процедура постпроения геометрии
	* Initialize - инициализация объекта. Вызывается при run->Init().
	* CheckIfSensitive - установка чувствительных объёмов в геометрии. Вызывается при run->Init(). Рекурсивно опрашивает для всех объёмов в геометрии.


Типовая структура ERDet.h файла класса выглядит так:

::

	// -------------------------------------------------------------------------
	// -----                        ERDet header file                      -----
	// -----                  Created data  by developer name              -----
	// -------------------------------------------------------------------------


	#ifndef ERDet_H
	#define ERDet_H

Указываем имя разработчика и дату начала разработки. Также используем стандартную кончтрукцию, чтобы избежать многократного включения в другие файлы. 

::

	#include "ERDetector.h"
	#include "ERDetPoint.h"
	#include "TLorentzVector.h"
	#include "TClonesArray.h"

	class ERDet : public ERDetector
	{

Объявляем класс ``ERDet``, унаслеованные от ``ERDetector``. Объявляем класс поинта; его реализация будет показана далее. 
  
::

	public:

	  /** Default constructor **/
	  ERDet();
	  
	  
	  /** Standard constructor.
	   *@param name    ERDet ERDet name
	   *@param active  sensitivity flag
	   *@param verbose Verbosity level. 1 - only standart logs, 2 - Print points after each event, 3 - GEANT Step information
	   **/
	  ERDet(const char* name, Bool_t active, Int_t verbose);
	  
	  
	  /** Destructor **/
	  virtual ~ERDet();
	  
	  
	  /** Virtual method ProcessHits
	   **   
	   ** Defines the action to be taken when a step is inside the
	   ** active volume. Creates a ERDetPoint and adds it to the
	   ** collection.
	   *@param vol  Pointer to the active volume
	   **/
	  virtual Bool_t ProcessHits(FairVolume* vol = 0);
	  
	  
	  /** Virtual method BeginEvent
	   **
	   **/
	  virtual void BeginEvent();
	  
	  /** Virtual method EndOfEvent
	   **
	   ** If verbosity level is set, print point collection at the
	   ** end of the event.
	   **/
	  virtual void EndOfEvent();
	  
	  /** Virtual method Register
	   **
	   ** Registers the point collection in the ROOT manager.
	   **/
	  virtual void Register();
	  
	  
	  /** Accessor to the point collection **/
	  virtual TClonesArray* GetCollection(Int_t iColl) const;
	  
	  
	  /** Virtual method Print
	   **
	   ** Screen output of hit collection.
	   **/
	  virtual void Print(Option_t *option="") const;
	  
	  
	  /** Virtual method Reset
	   **
	   ** Clears the point collection
	   **/
	  virtual void Reset();
	  
	  
	  /** Virtual method CopyClones
	   **
	   ** Copies the hit collection with a given track index offset
	   *@param cl1     Origin
	   *@param cl2     Target
	   *@param offset  Index offset
	   **/
	  virtual void CopyClones(TClonesArray* cl1, TClonesArray* cl2,
				  Int_t offset);
	  
	  
	  /** Virtaul method Construct geometry
	   **
	   ** Constructs the ERDet geometry
	   **/
	  virtual void ConstructGeometry();
	  
	   /** Virtaul method Initialize
	   **
	   ** Initialize ERDet data
	   **/
	  virtual void Initialize();

	  /** Virtaul method CheckIfSensitive 
		**Check whether a volume is sensitive.
	  ** @param(name)  Volume name
	  ** @value        kTRUE if volume is sensitive, else kFALSE
	  **
	  ** The decision is based on the volume name.
	  **/
	  virtual Bool_t CheckIfSensitive(std::string name);

Объявляем публичные методы, описанные выше.

::

	private:
	  TClonesArray*  fDetPoints;     //!  The point collection

	  Int_t          fEventID;           //!  event index
	  Int_t          fTrackID;           //!  track index
	  Int_t          fMot0TrackID;       //!  mother track index
	  Int_t       	 fPID;               //!  particle PDG
	  TLorentzVector fPosIn, fPosOut;     //!  position
	  TLorentzVector fMomIn, fMomOut;     //!  momentum
	  Double32_t     fTime;              //!  time
	  Double32_t     fLength;            //!  length
	  Double32_t     fELoss;             //!  energy loss

	private:
	  /** Private method AddPoint
	   **
	   ** Adds a ERDetPoint to the Point Collection
	   **/
	  
	  ERDetPoint* AddPoint(Int_t eventID, Int_t trackID,
				  Int_t mot0trackID,
				  Double_t mass,
				  TVector3 posIn,
				  TVector3 pos_out, TVector3 momIn,
				  TVector3 momOut, Double_t time,
				  Double_t length, Double_t eLoss);
	  ClassDef(ERDet,1);
	};

	#endif

Объявляем ``fDetPoints`` - коллекцию поинтов, метод для добавления поинтов в коллекцию. Объявляем набор переменнных, хранящих текущее состояние поинта внутри активного объекма детектора. Используем мароподстановку ClassDef(ERDet,1); для добавления функциональности Root объекта.

Типовые реализации методов приведены далее. Их необходимо добавить в файл ERDet.cxx.

::

	#include "ERDet.h"

	#include "TVirtualMC.h"
	#include "TParticle.h"

	// -----   Default constructor   -------------------------------------------
	ERDet::ERDet() : 
	  ERDetector("ERDet", kTRUE),
	  fDetPoints(NULL)
	{
	  fDetPoints = new TClonesArray("ERDetPoint");
	  //Это нужно сделать для того, чтобы геометрия в симуляции автоматом писалась в файл runtime db
	  flGeoPar = new TList();
	  flGeoPar->SetName( GetName());

	  fVerboseLevel = 1;
	}
	// -------------------------------------------------------------------------

	// -----   Standard constructor   ------------------------------------------
	ERDet::ERDet(const char* name, Bool_t active, Int_t verbose) 
	  : ERDetector(name, active,1),
	  fDetPoints(NULL)
	  {
	  fDetPoints = new TClonesArray("ERDetPoint");
	  //Это нужно сделать для того, чтобы геометрия в симуляции автоматом писалась в файл runtime db
	  flGeoPar = new TList();
	  flGeoPar->SetName( GetName());

	  fVerboseLevel = verbose;
	}
	// -------------------------------------------------------------------------

Реализуем конструкторы класса. Конструкцией ``: FairDetector(...)`` передаем параметры конструктор ``FairDetector`` от которого отнаследован ``ERDetector``.
Важное требование ``FairRoot`` и ``Root``  - все указатели должны быть инициализированы в констукторе объекта. Поэтому в список инициализации добавлено ``fDetPoints(NULL)``. Необходимо также инициализировать список геометрических параметров, объявленный в ``FairDetector``: ``flGeoPar = new TList();flGeoPar->SetName( GetName());``.

::
	
	// -------------------------------------------------------------------------
	ERDet::~ERDet() {
	  if (fDetPoints) {
	    fDetPoints->Delete();
	    delete fDetPoints;
	  }
	}

В деструкторе очищаем коллекцию поинтов (вызовется деструктор поинта для кадого) и удаляем саму коллекцию.

::

	void ERDet::Initialize()
	{
	  FairDetector::Initialize();
	}

В инициализации просто вызываем метод инициализации из ``FairDetector``.

Остальные методы кроме ProcessHits приведены без комментариев.

::

	void ERDet::BeginEvent() {
	}
	// -------------------------------------------------------------------------
	void ERDet::EndOfEvent() {
	  if (fVerboseLevel > 0)
		Print();
	  Reset();
	}
	// -------------------------------------------------------------------------
	void ERDet::Register() {
	  FairRootManager* ioman = FairRootManager::Instance();
	  if (!ioman)
		Fatal("Init", "IO manager is not set");	
	  ioman->Register("DetPoint","Det", fDetPoints, kTRUE);
	}
	// ----------------------------------------------------------------------------
	TClonesArray* ERDet::GetCollection(Int_t iColl) const {
	  if (iColl == 0) 
	    return fDetPoints;
	  else 
	    return NULL;
	}
	// ----------------------------------------------------------------------------

	// -----   Public method Print   ----------------------------------------------
	void ERDet::Print(Option_t *option) const
	{
	  for (Int_t iPoint = 0; iPoint < fDetPoints->GetEntriesFast(); iPoint++){
	    ERDetPoint* point = (ERDetPoint*)fDetPoints->At(iPoint);
	    point->Print();
	  }
	}
	// ----------------------------------------------------------------------------
	void ERDet::Reset() {
	  fDetPoints->Clear();
	}
	// ----------------------------------------------------------------------------

	// -----   Public method CopyClones   -----------------------------------------
	void ERDet::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
	  Int_t nEntries = cl1->GetEntriesFast();
	  std::cout << "Det: " << nEntries << " entries to add" << std::endl;
	  TClonesArray& clref = *cl2;
	  ERDetPoint* oldpoint = NULL;
	  for (Int_t i=0; i<nEntries; i++) {
	  oldpoint = (ERDetPoint*) cl1->At(i);
	   Int_t index = oldpoint->GetTrackID() + offset;
	   oldpoint->SetTrackID(index);
	   new (clref[cl2->GetEntriesFast()]) ERDetPoint(*oldpoint);
	  }
	  std::cout << "Det: " << cl2->GetEntriesFast() << " merged entries" << std::endl;
	}
	// ----------------------------------------------------------------------------
	  ERDetPoint* ERDet::AddPoint(Int_t eventID, Int_t trackID,
				  Int_t mot0trackID,
				  Int_t pid,
				  TVector3 posIn,
				  TVector3 posOut, TVector3 momIn,
				  TVector3 momOut, Double_t time,
				  Double_t length, Double_t eLoss){
	  TClonesArray& clref = *fDetPoints;
	  Int_t size = clref.GetEntriesFast();
	  return new(clref[size]) ERDetPoint(eventID, trackID, mot0trackID,pid,posIn,posOut,
	  									momIn,momOut,time,length,eLoss);
	}
	// ----------------------------------------------------------------------------
	Bool_t ERDet::CheckIfSensitive(std::string name)
	{
	  TString volName = name;
	  if(volName.Contains("fiber")) {
	    return kTRUE;
	  }
	  return kFALSE;
	}
	// ----------------------------------------------------------------------------
	ClassImp(ERDet)

Макроподстановка ``ClassImp(ERDet)`` необходима для добавления реализации служебных методов.

Метод ProcessHits
+++++++++++++++++

::

	Bool_t ERDet::ProcessHits(FairVolume* vol) {

В методе ``ProcessHits`` объявляем переменные для хранения текущего состояния поинта. Переменные статические так как при формаировании одного поинта метод может быть вызван множество раз.

::

	if ( gMC->IsTrackEntering() ) { // Return true if this is the first step of the track in the current volume
	    fELoss  = 0.;
	    fEventID = gMC->CurrentEvent();
	    gMC->TrackPosition(fPosIn);
	    gMC->TrackMomentum(fMomIn);
	    fTrackID  = gMC->GetStack()->GetCurrentTrackNumber();
	    fTime   = gMC->TrackTime() * 1.0e09;  // Return the current time of flight of the track being transported
	    fLength = gMC->TrackLength(); // Return the length of the current track from its origin (in cm)
	    fMot0TrackID  = gMC->GetStack()->GetCurrentTrack()->GetMother(0);
	    fPID = gMC->TrackPid();
  	}

Условие начала поинта. Поинт начинается при первом шаге в чувствительном объёме. Инициализируем переменные, которые можно инициализировать в начале поинта.

::

	fELoss += gMC->Edep(); // GeV //Return the energy lost in the current step

Инкрементация ``eLoss`` поинта происходит на каждом шаге от его начала до окончания.

::

		if (gMC->IsTrackExiting()    || //Return true if this is the last step of the track in the current volume 
		    gMC->IsTrackStop()       || //Return true if the track energy has fallen below the threshold
		    gMC->IsTrackDisappeared()) 
		  { 
		    gMC->TrackPosition(fPosOut);
		    gMC->TrackMomentum(fMomOut);

Условие окончания поинта. Инициализируем переменные окончания поинта.

::

			if (fELoss > 0.){
				      AddPoint( fEventID, fTrackID, fMot0TrackID, fPID,
				                TVector3(fPosIn.X(),   fPosIn.Y(),   fPosIn.Z()),
				                TVector3(fPosOut.X(),  fPosOut.Y(),  fPosOut.Z()),
				                TVector3(fMomIn.Px(),  fMomIn.Py(),  fMomIn.Pz()),
				                TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
				                fTime, fLength, fELoss);
		    }
		}
		return kTRUE;
	}
Условие записи поинта в выходной файл. По умолчанию необходимо, чтобы на одном из шагов был Eloss у частицы.

.. note::
	
	Каждое из условий: начала поинта, окончания и записи в файл можно менять. Также можно менять структуру данных поинта. Это будет продемонстрировано далее.


Как работать с процессами в ProcessHits
++++++++++++++++++++++++++++++++++++++

ProcessHits вызывается при каждом шаге в чувствительном объёме. На каждом шаге обновляется структура gMC, в которой в том числе записан список произошедших процессов. Вывести информацию о процессах в консоль можно, к примеру следующим образом.

::

	TArrayI processesID;  
	gMC->StepProcesses(processesID);
	std::cerr << gMC->TrackPid() << " " << gMC->Edep() << " " ;
	for (Int_t i = 0;i<processesID.GetSize();i++){
		std::cerr << TMCProcessName[processesID[i]]  << " ";
	}
	std::cerr << std::endl;

В массив целых чисел ``processesID`` будет записан список идентификаторов процессов. С помощью структуры данных ``TMCProcessName`` идетификаторы будут преобразованы к строковым выражениям.

В результате можно будет наблюдать таблиицу из PDG частицы, Edep на данном шаге и списка произошедших процессов.

Пример неупругого взаимодействия нейтрона в сцинциляторе

::

	2112 0 No active process 
	2112 0 Hadronic inelastic Primary particle emission Primary particle emission 
	2212 0 No active process 
	2212 0.0015359 Energy loss Multiple scattering Energy loss Primary particle emission Primary particle emission 
	2212 0.000867295 Energy loss Multiple scattering Energy threshold Primary particle emission Primary particle emission 
	1000020040 0 No active process 
	1000020040 0.001234 Energy loss Multiple scattering Energy loss Primary particle emission Primary particle emission 
	1000020040 0.00136297 Energy loss Multiple scattering Energy loss Primary particle emission Primary particle emission 
	1000020040 0.00146174 Energy loss Multiple scattering Energy loss Primary particle emission Primary particle emission 
	1000020040 0.000321879 Energy loss Multiple scattering Energy threshold Primary particle emission Primary particle emission 
	1000010020 0 No active process 
	1000010020 0.00177477 Energy loss Multiple scattering Energy loss Primary particle emission Primary particle emission 
	1000010020 0.00185553 Energy loss Multiple scattering Energy loss Primary particle emission Primary particle emission 
	1000010020 0.00193789 Energy loss Multiple scattering Energy loss Primary particle emission Primary particle emission 
	1000010020 0.00202412 Energy loss Multiple scattering Energy loss Primary particle emission Primary particle emission 
	1000010020 0 No active process 
	1000010020 0.00175385 Energy loss Multiple scattering Energy loss Primary particle emission Primary particle emission 
	2212 0 No active process 
	2212 0.0159552 Energy loss Multiple scattering Transportation Primary particle emission Primary particle emission 
	2112 0 No active process 
	2112 0 Transportation Primary particle emission Primary particle emission 
	2112 0 No active process 
	2112 0 Hadronic elastic Primary particle emission Primary particle emission 
	2112 0.000594615 Energy threshold Primary particle emission Primary particle emission 
	2112 0 No active process 
	2112 0 Transportation Primary particle emission Primary particle emission 
	2112 0 No active process 
	2112 0 Transportation Primary particle emission Primary particle emission 
	2212 0 No active process 
	2212 0.00136197 Energy loss Multiple scattering Energy loss Primary particle emission Primary particle emission 
	2212 0.00141336 Energy loss Multiple scattering Energy loss Primary particle emission Primary particle emission 
	2212 0.00132469 Energy loss Multiple scattering Energy loss Primary particle emission Primary particle emission 
	2212 0.00143044 Energy loss Multiple scattering Energy loss Primary particle emission Primary particle emission 
	2212 0.00132046 Energy loss Multiple scattering Energy loss Primary particle emission Primary particle emission 
	2212 0.00025315 Energy loss Multiple scattering Energy threshold Primary particle emission Primary particle emission 
	11 0 No active process 
	11 0.00245072 Energy loss Multiple scattering Energy loss Primary particle emission Primary particle emission 
	11 0.00121991 Energy loss Multiple scattering Energy loss Primary particle emission Primary particle emission 
	11 0.00146832 Energy loss Multiple scattering Energy loss Primary particle emission Primary particle emission 
	11 0.000707945 Energy loss Multiple scattering Energy loss Primary particle emission Primary particle emission 
	11 0.000584947 Energy loss Multiple scattering Energy loss Primary particle emission Primary particle emission 
	11 0.000423729 Energy loss Multiple scattering Energy loss Primary particle emission Primary particle emission 
	11 0.000434988 Energy loss Multiple scattering Energy loss Primary particle emission Primary particle emission 
	11 4.95559e-05 Energy loss Multiple scattering Transportation Primary particle emission Primary particle emission

Пример упругого взаимодействия нейтрона в сцинциляторе.

::




Как реализовать закон Биркса в ProcessHits
++++++++++++++++++++++++++++++++++++++++++
Для детекторов на базе сцинциляторов из симуляции необходим не только ``eloss``, но еще и световыход - `light yield`. Световыход зависит от плотности ионизации и может изменяться по длине поинта. Поэтому данное вычисление нельзя перенести, например, в диджитизацию. Далее будет изложена реализация закона Биркса, взятая из Geant4.

:: 

  // Set constants for Birk's Law implentation
  static const Double_t dP = 1.032 ;
  static const Double_t BirkC1 =  0.013/dP;
  static const Double_t BirkC2 =  9.6e-6/(dP * dP);

  static Double_t       lightYield;

Необходимо ввести ряд констант и переменную, в которой будет храниться суммарный световыход поинта, в начале метода ``ProcessHits``. 

::

	if ( gMC->IsTrackEntering() ) { // Return true if this is the first step of the track in the current volume
	    ...
	    lightYield = 0.;
	    ...
	}

Данную переменную надо обнулить в начале поинта. 

::

	// Correction for all charge states
  	if (gMC->TrackCharge()!=0) { // Return the charge of the track currently transported
	    Double_t BirkC1Mod = 0;
	    // Apply correction for higher charge states
	      if (TMath::Abs(gMC->TrackCharge())>=2)
	        BirkC1Mod=BirkC1*7.2/12.6;
	      else
	        BirkC1Mod=BirkC1;

	    if (gMC->TrackStep()>0)
	    {
	      Double_t dedxcm=gMC->Edep()*1000./gMC->TrackStep(); //[MeV/cm]
	      curLightYield=gMC->Edep()*1000./(1.+BirkC1Mod*dedxcm+BirkC2*dedxcm*dedxcm); //[MeV]
	      curLightYield /= 1000.; //[GeV]
	      lightYield+=curLightYield;
	    }
  	}

Реализация закона. Первое условие - работаем с заряженной частицей. Второе условие - величина шага больше нуля(чтобы не делить на ноль). Таким образом в случае, если энерговыделение произошло на шаге длинной равной нулю, а такое может быть к примеру когда происходит родить вторичную ниже порога на рождение. 

::

	AddPoint( eventID, trackID, mot0TrackID, mass,
                TVector3(posIn.X(),   posIn.Y(),   posIn.Z()),
                TVector3(posOut.X(),  posOut.Y(),  posOut.Z()),
                TVector3(momIn.Px(),  momIn.Py(),  momIn.Pz()),
                TVector3(momOut.Px(), momOut.Py(), momOut.Pz()),
                time, length, eLoss, lightYield);

Для записи в файл, данный атрибут необходимо добавить в класс поинта, конструктор поинта, и метод ``AddPoint``.


Класс Point
~~~~~~~~~~~~

Типовой класс поинт выглядит следующим образом:

ERDetPoint.h:

:: 

	// -------------------------------------------------------------------------
	// -----                      ERDetPoint header file                -----
	// -----                  Created data  developerName                  -----
	// -------------------------------------------------------------------------


	/**  ERDetPoint.h
	 **/


	#ifndef ERDetPoint_H
	#define ERDetPoint_H


	#include "TObject.h"
	#include "TVector3.h"

	#include "FairMCPoint.h"


	class ERDetPoint : public FairMCPoint 
	{

	 public:

	  /** Default constructor **/
	  ERDetPoint();


	  /** Constructor with arguments
	   *@param EventID  Index of Event
	   *@param trackID  Index of MCTrack
	   *@param mot0trackID Index of Mother MCTrack
	   *@param pid particle ID
	   *@param posIn    Ccoordinates at entrance of point [cm]
	   *@param posOut   Coordinates at exit of point [cm]
	   *@param momIn    Momentum of track at entrance [GeV]
	   *@param momOut   Momentum of track at exit [GeV]
	   *@param tof      Time since event start [ns]
	   *@param length   Track length since creation [cm]
	   *@param eLoss    Energy deposit [KeV]
	   **/
	  ERDetPoint(Int_t eventID, Int_t trackID,
			  Int_t mot0trackID,
			  Int_t pid,
			  TVector3 posIn,
			  TVector3 posOut, TVector3 momIn, TVector3 momOut,
			  Double_t tof, Double_t length, Double_t eLoss);


	  /** Copy constructor **/
	  ERDetPoint(const ERDetPoint&);


	  /** Destructor **/
	  virtual ~ERDetPoint();


	  ERDetPoint& operator=(const ERDetPoint&) { return *this; }


	  /** Accessors **/
	  Int_t GetEventID()            const { return fEventID; }
	  Int_t GetMot0TrackID()        const { return fMot0TrackID; }
	  Double_t GetXIn()             const { return fX; }
	  Double_t GetYIn()             const { return fY; }
	  Double_t GetZIn()             const { return fZ; }
	  Double_t GetXOut()            const { return fX_out; }
	  Double_t GetYOut()            const { return fY_out; }
	  Double_t GetZOut()            const { return fZ_out; }
	  Double_t GetPxOut()           const { return fPx_out; }
	  Double_t GetPyOut()           const { return fPy_out; }
	  Double_t GetPzOut()           const { return fPz_out; }
	  Int_t GetPID()            const { return fPid; }

	  void PositionIn(TVector3& pos)  { pos.SetXYZ(fX, fY, fZ); }
	  void PositionOut(TVector3& pos) { pos.SetXYZ(fX_out,fY_out,fZ_out); }
	  void MomentumOut(TVector3& mom) { mom.SetXYZ(fPx_out,fPy_out,fPz_out); }

	  Int_t StilbenNr() const {return fStilbenNr;}
	  Float_t LightYield() const {return fLightYield;}

	  /** Point coordinates at given z from linear extrapolation **/
	  Double_t GetX(Double_t z) const;
	  Double_t GetY(Double_t z) const;


	  /** Check for distance between in and out **/
	  Bool_t IsUsable() const;

	  /** Output to screen **/
	  virtual void Print(const Option_t* opt = 0) const;

	 protected:
	  
	  Int_t fEventID;
	  Int_t fMot0TrackID;
	  Int_t fPid;
	  Double32_t fX_out,  fY_out,  fZ_out;
	  Double32_t fPx_out, fPy_out, fPz_out;
	  Int_t fStilbenNr;
	  Float_t fLightYield;

	  ClassDef(ERDetPoint,1)
	};
	#endif

ERDetPoint.cxx:

::

	// -------------------------------------------------------------------------
	// -----                      ERDetPoint source file                   -----
	// -------------------------------------------------------------------------

	#include "ERDetPoint.h"
	#include <iostream>
	using namespace std;
	// -----   Default constructor   -------------------------------------------
	ERDetPoint::ERDetPoint()
	  : FairMCPoint(),
	    fX_out(0.), fY_out(0.), fZ_out(0.),
	    fPx_out(0.), fPy_out(0.), fPz_out(0.),
	    fStilbenNr(-1)
	{
	}
	// -------------------------------------------------------------------------
	ERDetPoint::ERDetPoint(Int_t eventID, Int_t trackID,
			  Int_t mot0trackID,
			  Int_t pid,
			  TVector3 posIn,
			  TVector3 posOut, TVector3 momIn, TVector3 momOut,
			  Double_t tof, Double_t length, Double_t eLoss)
	  : FairMCPoint(trackID, -1., posIn, momIn, tof, length, eLoss),
	    fEventID(eventID),
	    fPid(pid),
	    fX_out(posOut.X()), fY_out(posOut.Y()), fZ_out(posOut.Z()),
	    fPx_out(momOut.X()), fPy_out(momOut.Y()), fPz_out(momOut.Z())
	{
	}
	// -------------------------------------------------------------------------
	ERDetPoint::ERDetPoint(const ERDetPoint& right)
	  : FairMCPoint(right),
	    fPid(right.fPid),
	    fX_out(right.fX_out), fY_out(right.fY_out), fZ_out(right.fZ_out),
	    fPx_out(right.fPx_out), fPy_out(right.fPy_out), fPz_out(right.fPz_out)
	{
	}
	// -------------------------------------------------------------------------
	ERDetPoint::~ERDetPoint()
	{
	}
	// -------------------------------------------------------------------------
	void ERDetPoint::Print(const Option_t* opt /* = 0*/) const
	{
	  cout << "-I- ERDetPoint: track " << fTrackID << " mother track = " << fMot0TrackID << endl;
	  cout << "    particle ID " << fPid << endl;
	  cout << "    Position (" << fX << ", " << fY << ", " << fZ << ") cm" << endl;
	  cout << "    Momentum (" << fPx << ", " << fPy << ", " << fPz << ") GeV" << endl;
	  cout << "    Time " << fTime << " ns,  Length " << fLength << " cm" << endl;
	  cout << "    Energy loss " << fELoss << " keV "<< endl;
	}
	// -------------------------------------------------------------------------
	// -----   Point x coordinate from linear extrapolation   ------------------
	Double_t ERDetPoint::GetX(Double_t z) const
	{
	  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fX_out+fX)/2.;
	  Double_t dz = fZ_out - fZ;
	  return ( fX + (z-fZ) / dz * (fX_out-fX) );
	}
	// -------------------------------------------------------------------------

	// -----   Point y coordinate from linear extrapolation   ------------------
	Double_t ERDetPoint::GetY(Double_t z) const
	{
	  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fY_out+fY)/2.;
	  Double_t dz = fZ_out - fZ;
	  //  if ( TMath::Abs(dz) < 1.e-3 ) return (fY_out+fY)/2.;
	  return ( fY + (z-fZ) / dz * (fY_out-fY) );
	}
	// -------------------------------------------------------------------------
	// -----   Public method IsUsable   ----------------------------------------
	Bool_t ERDetPoint::IsUsable() const
	{
	  Double_t dz = fZ_out - fZ;
	  if ( TMath::Abs(dz) < 1.e-4 ) return kFALSE;
	  return kTRUE;
	}
	// -------------------------------------------------------------------------
	ClassImp(ERDetPoint)



Класс MCHeader
~~~~~~~~~~~~~~

Макрос симуляции
~~~~~~~~~~~~~~~~

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
	  //gMC->SetMaxNStep(nSteps);
		
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




Создать диджитизацию детектора
------------------------------

Класс диджитизации
~~~~~~~~~~~~~~~~~~

Класс digi
~~~~~~~~~~

Класс Setup
~~~~~~~~~~~

Макрос диджитизации
~~~~~~~~~~~~~~~~~~~

Создать реконструкцию детектора
-------------------------------

Класс реконструкции
~~~~~~~~~~~~~~~~~~~

Класс хита
~~~~~~~~~~

Класс Header
~~~~~~~~~~~~

Макрос реконструкции
~~~~~~~~~~~~~~~~~~~~

Работать с базой данных параметров
----------------------------------

Про ContFactory

Создать генератор
-----------------

Создать распад
--------------

Использовать HPC решения
------------------------

Использование PROOF
~~~~~~~~~~~~~~~~~~~

Использование FairMQ
~~~~~~~~~~~~~~~~~~~~