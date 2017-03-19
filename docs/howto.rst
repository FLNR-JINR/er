How-to
======

Создать модель детектора
------------------------

Модель детектора состоит из описания геометрии и материалов.

Задание материалов
~~~~~~~~~~~~~~~~~~

Создание геометрии
~~~~~~~~~~~~~~~~~~

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


Типовая структура *.h файла класса выглядит так:

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

	class ERDet : public ERDetector
	{

Объявляем класс ``ERDet``, унаслеованные от ``ERDetector``. Объявляем класс поинта; его реализация будет показана далее. 
  
::

	public:

	  /** Default constructor **/
	  ERND();
	  
	  
	  /** Standard constructor.
	   *@param name    ERND ERND name
	   *@param active  sensitivity flag
	   *@param verbose Verbosity level. 1 - only standart logs, 2 - Print points after each event, 3 - GEANT Step information
	   **/
	  ERND(const char* name, Bool_t active, Int_t verbose);
	  
	  
	  /** Destructor **/
	  virtual ~ERND();
	  
	  
	  /** Virtual method ProcessHits
	   **   
	   ** Defines the action to be taken when a step is inside the
	   ** active volume. Creates a ERNDPoint and adds it to the
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
	   ** Constructs the ERND geometry
	   **/
	  virtual void ConstructGeometry();
	  
	   /** Virtaul method Initialize
	   **
	   ** Initialize ERND data
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

Объявляем ``fDetPoints`` - коллекцию поинтов, метод для добавления поинтов в коллекцию. Используем мароподстановку ClassDef(ERDet,1); для добавления функциональности Root объекта.


Типовые реализации методов приведены далее.

::

	// -----   Default constructor   -------------------------------------------
	ERDet::ERDet() : 
	  FairDetector("ERDet", kTRUE),
	  fDetPoints(NULL)
	{
	  fNDPoints = new TClonesArray("ERNDPoint");
	  flGeoPar = new TList();
	  flGeoPar->SetName( GetName());
	  fVerboseLevel = 1;
	}
	// -------------------------------------------------------------------------

	// -----   Standard constructor   ------------------------------------------
	ERDet::ERDet(const char* name, Bool_t active, Int_t verbose) 
	  : FairDetector(name, active,verbose),
	  fDetPoints(NULL)
	  {
	  fNDPoints = new TClonesArray("ERNDPoint");
	  flGeoPar = new TList();
	  flGeoPar->SetName( GetName());
	}
	// -------------------------------------------------------------------------

Реализуем конструкторы класса. Конструкцией ``: FairDetector(...)`` передаем параметры конструктор ``FairDetector`` от которого отнаследован ``ERDetector``.
Важное требование ``FairRoot`` и ``Root``  - все указатели должны быть инициализированы в констукторе объекта. Поэтому в список инициализации добавлено ``fDetPoints(NULL)``. Необходимо также инициализировать список геометрических параметров, объявленный в ``FairDetector``: ``flGeoPar = new TList();flGeoPar->SetName( GetName());``.

::
	
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

Метод ProcessHits
+++++++++++++++++

::

	Bool_t ERND::ProcessHits(FairVolume* vol) {
	  static Int_t          eventID;           //!  event index
	  static Int_t          trackID;           //!  track index
	  static Int_t          mot0TrackID;       //!  mother track index
	  static Double_t       mass;              //!  mass
	  static TLorentzVector posIn, posOut;     //!  position
	  static TLorentzVector momIn, momOut;     //!  momentum
	  static Double32_t     time;              //!  time
	  static Double32_t     length;            //!  length
	  static Double32_t     eLoss;             //!  energy loss

В методе ``ProcessHits`` объявляем переменные для хранения текущего состояния поинта. Переменные статические так как при формаировании одного поинта метод может быть вызван множество раз.

::
	  
	  if ( gMC->IsTrackEntering() ) { // Return true if this is the first step of the track in the current volume
	    eLoss  = 0.;
	    eventID = gMC->CurrentEvent();
	    gMC->TrackPosition(posIn);
	    gMC->TrackMomentum(momIn);
	    trackID  = gMC->GetStack()->GetCurrentTrackNumber();
	    time   = gMC->TrackTime() * 1.0e09;  // Return the current time of flight of the track being transported
	    length = gMC->TrackLength(); // Return the length of the current track from its origin (in cm)
	    mot0TrackID  = gMC->GetStack()->GetCurrentTrack()->GetMother(0);
	    mass = gMC->ParticleMass(gMC->TrackPid()); // GeV/c2ы
	  }

Условие начала поинта. Поинт начинается при первом шаге в чувствительном объёме. Инициализируем переменные, которые можно инициализировать в начале поинта.

::

	  eLoss += gMC->Edep(); // GeV //Return the energy lost in the current step

Инкрементация ``eLoss`` поинта происходит на каждом шаге от его начала до окончания.

::

		if (gMC->IsTrackExiting()    || //Return true if this is the last step of the track in the current volume 
		    gMC->IsTrackStop()       || //Return true if the track energy has fallen below the threshold
		    gMC->IsTrackDisappeared()) 
		{ 
		    gMC->TrackPosition(posOut);
		    gMC->TrackMomentum(momOut);

Условие окончания поинта. Инициализируем переменные окончания поинта.

::

			if (eLoss > 0.){
			      AddPoint( eventID, trackID, mot0TrackID, mass,
			                TVector3(posIn.X(),   posIn.Y(),   posIn.Z()),
			                TVector3(posOut.X(),  posOut.Y(),  posOut.Z()),
			                TVector3(momIn.Px(),  momIn.Py(),  momIn.Pz()),
			                TVector3(momOut.Px(), momOut.Py(), momOut.Pz()),
			                time, length, eLoss);
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

Для записи в файл, данный атрибут необходмо добавить в класс поинта, конструктор поинта, и метод ``AddPoint``.


Класс Point
~~~~~~~~~~~~

Класс MCHeader
~~~~~~~~~~~~~~

Макрос симуляции
~~~~~~~~~~~~~~~~

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