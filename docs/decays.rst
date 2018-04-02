Interactions
============

Пакет ExpertRoot обладает собственным подходом добавления взаимодействий в процесс транспорта частиц. Этот подход основан на возможности пользователя написать свою собственную обработку Stepping Action библиотеки транспорта и возможности добавлять в стэк треков новые треки.

У такого подхода есть два достоинства:

1. Исследователь может заложить только ту информацию о взаимодействии, которой обладает.
2. Взаимодействие встроено в процесс транспорта, поэтому нет необходимости заранее наигрывать параметры первичного иона в момент взаимодействия.

Elastic scattering
------------------

Для моделирования упругого рассеяния иона на мишени создан класс `ERElasticScattering`. Он позволяет разыгрывать угол theta рассеяния изотропно или по заданному файлу с функцией CDF(comulative distribution function) угла theta.

Рассмотрим добавления данного взаимодейтсвия в сессию симуляции на примере рассейния иона 15N на мишени 11B. Создаем класс коллекции взаимодействий и класс взаимодействия:

::

  ERDecayer* decayer = new ERDecayer();
  ERElasticScattering* scattering = new ERElasticScattering("15Nto15N11B");

`15Nto15N11B` - Имя взаимодействия.
Задаем налетающий ион и ион мишений:

::

  scattering->SetInputIon(7,15,7);
  scattering->SetTargetIon(5,11,5);

Задаем 

::

  scattering->SetUniformPos(-0.00035,0.00035);
  scattering->SetStep(0.00001); //0.1 micron
  scattering->SetDecayVolume("targetB11");

Interaction class
-----------------

Реализация своего взаимодействия добавляется с помощью создания класса, унаследованного от `ERDecay`. Коды распадов расположены в дирeктории `decays`.

В новом классе необходимо написать реализации методов: `Init`, `BeginEvent`, `FinishEvent`, `Stepping`.

В методе `Init` необходимо проверить все ли ионы и частицы, которые учавствуют в распаде добавлены в объект распада и в Root библиотеку частиц - `TDatabasePDG`.

::

  if (fInputIon) {
    fInputIonPDG = TDatabasePDG::Instance()->GetParticle(fInputIonName);
    if ( ! fInputIonPDG ) {
        std::cerr  << "ERTextDecay: Ion " << fInputIonName << " not found in database!"<< endl;
        return kFALSE;
    }
  }
  else{
    std::cerr  << "Input ion not defined"<< endl;
    return kFALSE;
  }

Также необходимо проверить все ли входные текстовые файлы и другие настройки распада указаны.

::

  if (fFileName == "") {
    cerr << "File for " << fName << " decay not defined!" << endl;
    return kFALSE;
  }

Метод `BeginEvent` вызывается в начале каждого события. Стандартно в этом методе необходимо реинициализировать часть переменных распада и разыграть вероятностные характеристики распада, в частности, позицию распада.

::

  fDecayFinish = kFALSE;
  if (fUniform){
    fDecayPosZ = fRnd->Uniform(fUniformA, fUniformB);
  }
  if (fExponential){
    fDecayPosZ = fExponentialStart + fRnd->Exp(fExponentialTau);
  }

Метод `Stepping` вызывается на каждом шаге транспорта всех частиц. В нём закладывается условие распада и добавляются новые треки в стек треков. Типичная структура этого метода представлена далее.

Проверка того, что мы рассматриваем трекинг интересующего нас иона и установка малого шага трека для того чтобы максимально точно выбрать позицию распада:

::

  if(!fDecayFinish && gMC->TrackPid() == fInputIonPDG->PdgCode()){
    gMC->SetMaxStep(0.01);
    gMC->TrackPosition(fDecayPos);

Провека что распад выполниться именно на этом шаге:

::

    if (fDecayPos.Z() > fDecayPosZ){
      gMC->TrackMomentum(fInputIonV);
      //Add new ion
      Int_t newTrackNb;
      vector<TLorentzVector> decay = fDecays[gMC->CurrentEvent()];

Добавление новых частиц в стек для трнспорта:

::

      gMC->GetStack()->PushTrack(1,gMC->GetStack()->GetCurrentTrackNumber(), fOutputIonPDG->PdgCode(),
                           outputIonV.Px(),outputIonV.Py(),outputIonV.Pz(),
                           outputIonV.E(), fDecayPos.X(), fDecayPos.Y(), fDecayPos.Z(),
                           gMC->TrackTime(), 0., 0., 0.,
                           kPDecay, newTrackNb, fOutputIonPDG->Mass(), 0);

Окончание распада. Остановка первичного иона. Возвращение максимального шага транспорта. Сохранение
характеристик распада в объект `MCEventHeader` .

::

      fDecayFinish = kTRUE;
      gMC->StopTrack();
      gMC->SetMaxStep(10000.);
      SaveToEventHeader();
    }
  }
  return kTRUE;

Decay definition in macro
-------------------------

Инициализация менеджера запуска.

::

  void decay(Int_t nEvents = 10) {
    //---------------------Files-----------------------------------------------
    TString outFile= "sim.root";
    TString parFile= "par.root";
    // ------------------------------------------------------------------------

    // -----   Timer   --------------------------------------------------------
    TStopwatch timer;
    timer.Start();
    // ------------------------------------------------------------------------

    // -----   Create simulation run   ----------------------------------------
    ERRunSim* run = new ERRunSim();
    run->SetName("TGeant4");
    run->SetOutputFile(outFile.Data());
    // ------------------------------------------------------------------------

    // -----   Runtime database   ---------------------------------------------
    FairRuntimeDb* rtdb = run->GetRuntimeDb();
    // ------------------------------------------------------------------------

    // -----   Create media   -------------------------------------------------
    run->SetMaterials("media.geo");       // Materials
    // ------------------------------------------------------------------------

Если необходимо сохранять параметры распада в выходной файл необходимо добавить `MCEventHeader`.

::

  //-------- Set MC event header --------------------------------------------
  ERDecayMCEventHeader* header = new ERDecayMCEventHeader();
  run->SetMCEventHeader(header);
  //-------------------------------------------------------------------------

Добавить необходимую геометрию в событие:

::

  // -----   Create detectors  ----------------------------------------------	
  FairModule* cave= new ERCave("CAVE");
  cave->SetGeometryFileName("cave.geo");
  run->AddModule(cave);

  FairModule* target = new ERTarget("Target", kTRUE, 1);
  target->SetGeometryFileName("Li10_target.geo.root");
  run->AddModule(target);

  // ------------------------------------------------------------------------

Добавить ERDecayer и добавить в него сконфигурированные распады:

::

  //------    ER Deacayer   -------------------------------------------------
  ERDecayer* decayer = new ERDecayer();
  ERDecay* targetDecay = new ERDecayLi9DetoLi10_Li9n_p();
  decayer->AddDecay(targetDecay);
  //-------------------------------------------------------------------------

Добавить генератор первичного иона.

::

      // -----   Create PrimaryGenerator   --------------------------------------
      FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

      ERIonGenerator* ionGenerator = new ERIonGenerator("Li9",3,9,3,1);
      Double32_t kin_energy = 0.025*9; //GeV
      Double_t mass = ionGenerator->Ion()->GetMass();
      Double32_t momentum = TMath::Sqrt(kin_energy*kin_energy + 2.*kin_energy*mass); //GeV
      ionGenerator->SetPRange(momentum, momentum);
      Double32_t theta1 = 0.;  // polar angle distribution
      Double32_t theta2 = 0.0001*TMath::RadToDeg();
      ionGenerator->SetThetaRange(theta1, theta2);
      ionGenerator->SetPhiRange(0, 360);
      ionGenerator->SetBoxXYZ(-0.4,-0.4,0.4,0.4,-10);
      primGen->AddGenerator(ionGenerator);

      run->SetGenerator(primGen);
      // ------------------------------------------------------------------------

Закончить инициализацию и запустить расчёт:

::

    //-------------------------------------------------------------------------
    // -----   Runtime database   ---------------------------------------------
    Bool_t kParameterMerged = kTRUE;
    FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
    parOut->open(parFile.Data());
    rtdb->setOutput(parOut);
    rtdb->saveOutput();
    rtdb->print();
    //-------------------------------------------------------------------------

    // -----   Run simulation  ------------------------------------------------
    run->Run(nEvents);
    //-------------------------------------------------------------------------

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
    //-------------------------------------------------------------------------
  }

ERTextDecay - decay from text file
----------------------------------

`ERTextDecay` - класс распада, в котором чтение результатов распада происходит из текстового файла следующей структуры:

::

   10000    8.00    1.00    1.00    0.0000   10.0000    0.1653    0.3100   90.0000  -45.0000    1.0000    0.8000    1.0000
 0.7206406E+01 -0.1754454E+02  0.5639178E+02 -0.1327796E+03  0.7652269E+00 -0.3139226E+02  0.7600432E+02  0.1677931E+02 -0.2499952E+02  0.5677531E+02  0.2169820E+00
 0.9067860E+01 -0.2866158E+02 -0.3239947E+02 -0.9974271E+02 -0.1909192E+02 -0.4091810E+01  0.1072426E+03  0.4775351E+02  0.3649128E+02 -0.7499936E+01  0.1777150E+00
 0.7098366E+01 -0.4391382E+02 -0.1458367E+02  0.1816315E+02  0.9634398E+02  0.1606081E+02 -0.2588241E+02 -0.5243016E+02 -0.1477139E+01  0.7719253E+01  0.2744438E+00

Первая строчка является шапкой файла и параметрами генерации. Данная информация не используется при реализации распада в ExpertRoot. Далее каждая строка в файле задает событие. Первое число - энергия возбуждения, также не используется в добавлении распада в транспорт. Далее записаны вектора импульсов выходов распада. Последняя колонка - угол реакции - тоже не используется.

Все импульсы указываются в системе центра масс распада. При чтении из файла импульсы переводятся в лабораторную СК с учётом импульса первичного иона в момент распада.

Все файлы распадов должны находится в папке `input`.

Для добавления данного распада в событие необходимо добавить в макрос:

::

  //------    ER Deacayer   -------------------------
  ERDecayer* decayer = new ERDecayer();
  ERTextDecay* decay = new ERTextDecay("10Heto8He2n");
  decay->SetInputIon(2,10,2);
  decay->SetOutputIon(2,8,2);
  decay->AddOutputParticle(2212);
  decay->AddOutputParticle(2212);
  decay->SetDecayPosZ(2.);
  decay->SetFileName("generator_10He_decay.dat");
  decayer->AddDecay(decay);
  run->SetDecayer(decayer);

При инициализации необходимо указать входной ион, выходной ион и набор выходных частиц через массовые числа или pdg. Далее необходимо указать позицию распада по Z и файл распада с импульсами выходных частиц.

Позицию распада также можно задать с помощью равномерного распределения. Это уместно для тонкой мишени.

::

  SetUniformPos(Double_t a, Double_t b);

И с помошью экспоненциального распределения. Это уместно для толстой мишени.

::

  SetExponentialPos(Double_t start, Double_t tau);
