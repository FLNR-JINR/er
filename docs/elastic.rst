.. Автозамены
.. |empty| unicode:: U+2063
.. |theta| unicode:: U+03F4
.. |alpha| unicode:: U+03B1
.. |phi|  unicode:: U+03C6

Постановка задачи упругого рассеяния
------------------------------------

Ион **А** с энергией \ :sup:`A` \ **E**\ :sub:`0` , импульсом \ :sup:`A` \ **P**\ :sub:`0` \(**P**\ :sub:`x` , **P**\ :sub:`y` , **P**\ :sub:`z` ),
начальными пространственными координатами \ :sup:`A` \ **r**\ :sub:`0` \(**x**\ :sub:`0` , **y**\ :sub:`0` , **z**\ :sub:`0` ) налетает на покоящуюся мишень
состоящую из атомов **B**.
Ион **A** многократно рассеивается внутри объёма мишени, на некотором шаге выбивает ион **B**, и далее уже оба иона многократно рассеиваются пока не покинут объем мишени.
В результате - новые характеристики \ :sup:`A` \ **E**\, \ :sup:`A` \ **P(P**\ :sub:`x` , **P**\ :sub:`y` , **P**\ :sub:`z` ), \ :sup:`A` \ **r(x**, **y**, **z**)- для иона **А**,
и следующие для иона **B**: \ :sup:`B` \ **E**\, \ :sup:`B` \ **P(P**\ :sub:`x` , **P**\ :sub:`y` , **P**\ :sub:`z` ), \ :sup:`B` \ **r(x**, **y**, **z**).

    .. figure:: _static/fig1.png
        :height: 600px
        :width: 600 px
        :align: center

Рассмотрим добавления упругого рассеяния в сессию симуляции на примере рассеяния иона 15N на мишени 11B. Создаем класс коллекции взаимодействий и класс взаимодействия::

    ERDecayer* decayer = new ERDecayer();
    ERElasticScattering* scattering = new ERElasticScattering("15Nto15N11B");

15Nto15N11B - Имя взаимодействия.

Входные данные
--------------

Пользователь должен знать:

#. Следующие характеристики ионов:

    * A - атомная масса
    * Z - число протонов
    * Q - заряд

   Для налетающего иона, например для \ :sup:`15` \N, данные характеристике задаются так::

    scattering->SetInputIon(7,15,3); // Z = 7, A = 15, Q = 3

   А для иона мишени, например \ :sup:`11` \B так::

    scattering->SetTargetIon(5,11,5); // Z = 5, A = 11, Q = 5

#. Способ определения кинематических характеристик.

   По умолчанию, используется изотропное распределение угла рассеяния |theta|.

   Возможно задание кумулятивной функции распределения:

   Задаём имя текстового файла с данными о кумулятивной функции::

    scattering->SetThetaCDF("cos_tetta_cross.txt");

   Структура текстового файла должна быть следующая::

    4.5 0.0448573496944
    4.6 0.087071447189
    4.7 0.126858356665
    4.8 0.164410246312
    4.9 0.199891413123
    5.0 0.233459173925
    5.1 0.26523651571
    5.2 0.295359837723

    ... ...

    179.3 0.99999999789
    179.4 0.999999998493
    179.5 0.999999998995
    179.6 0.999999999397
    179.7 0.999999999698
    179.8 0.999999999899
    179.9 1.
    180.0 1.

   Где первый столбец соответствует углам |theta| в системе центра масс, а второй — значениям кумулятивной функции для данных значений углов |theta|.
   Текстовый файл с данными о кумулятивной функции должен лежать в директории **er/input** .

#. Способ определения координаты взаимодействия.

    * Для изотропного розыгрыша координаты взаимодействия внутри объёма мишени. Задать переднюю координату мишени и заднюю относительно глобальной оси **z**: ::

        scattering->SetUniformPos(-0.00035,0.00035);

    .. tip:: Использовать в случае **тонкой** мишени.


    .. figure:: _static/fig2.png
        :height: 600px
        :width: 600 px
        :align: center

    Обычно мишень располагается в начале глобальной системы координат, данный рисунок иллюстрирует этот случай.

    * Для экспоненциального розыгрыша координаты взаимодействия, используйте метод::

        SetExponentialPos(Double_t start, Double_t tau);

    .. tip:: Использовать в случае **толстой** мишени.

#. Точность необходимую для определения положение координаты взаимодействия.

    Для этого задаем шаг пророгации внутри объёма мишени::

        scattering->SetStep(0.00001); //0.1 micron

#. Способ выбора диапазона розыгрыша углов |theta|\ :sub:`CM`  и |phi|\ :sub:`CM` для рассеявшихся ионов.

    По умолчанию |theta|\ :sub:`CM`  разыгрывается внутри диапазона от 0\ :sup:`0`  до 180\ :sup:`0` ,а  |phi|\ :sub:`CM`  от 0\ :sup:`0`  до 360\ :sup:`0` .
    Когда пользователю не нужен весь диапазон розыгрыша углов он может выбрать только его часть.

    Тогда ионы будут рассеиваться под углами из выбранного диапазона, что уменьшит время вычисления и повысит статистику.

    * Выбрать диапазон розыгрыша |theta|\ :sub:`CM`  можно:

        #. Явно::

            scattering->SetThetaRange(18.4, 19.4);

        #. Косвенно. Через следующие величины:

            * Координату |theta|\ :sub:`Lab` \  центра щели детектора

            * *Полуширину* выбираемого диапазона 0.5*d |theta| в лабораторной системе координат::

                scattering->SetDetectorsSlot(thetaCenter, dTheta);

            .. attention:: Важно выбирать полуширину с запасом

        Далее по формулам, которые приведены ниже рассчитываются: |theta|\ :sub:`CM1`  и |theta|\ :sub:`CM2` ,
        где |theta|\ :sub:`CM1`  и |theta|\ :sub:`CM2`  крайние значения расчитываемого диапозона.

        Когда M\ :sub:`1` \< M\ :sub:`2` \

        .. math::

            0 \leqslant \theta_1 \leqslant \pi\ ,\\

        а когда  M\ :sub:`1` \ > M\ :sub:`2` \

        .. math::

            0 \leqslant \theta_1 \leqslant \theta_{1max} \leqslant \frac{\pi}{2}\ ,\\

        в обоих случаях:

        .. math::

            \theta_{cm1} = \arccos\left [  -\frac{M_1}{M_2} + sin^2(\theta_1-d\theta) + cos(\theta_1-d\theta)\sqrt{1 - \frac{M_1^2}{M_2^2}sin^2(\theta_1-d\theta)} \right ]\ ,

            \theta_{cm2} = \arccos\left [  -\frac{M_1}{M_2} + sin^2(\theta_1+d\theta) + cos(\theta_1+d\theta)\sqrt{1 - \frac{M_1^2}{M_2^2}sin^2(\theta_1+d\theta)} \right ]\ .

        Когда M\ :sub:`1` \ = M\ :sub:`2` \

        .. math::

            0 \leqslant \theta_1 \leqslant \frac{\pi}{2}\ ,\\

        .. math::

            \theta_{cm1} = 2(\theta_1-d\theta) ,

            \theta_{cm2} = 2(\theta_1+d\theta) .


        Для первоначально покоившихся частиц (во всех случаях):

        .. math::

            \theta_{cm1} = \pi - 2(\theta_2-d\theta) ,

            \theta_{cm2} = \pi - 2(\theta_2+d\theta) .

        M\ :sub:`1`  - масса налетающей частицы,

        M\ :sub:`2`  - масса покоящейся частицы,

        |theta|\ :sub:`1`  - угол рассеяния налетающей частицы (Лаб. Сит.),

        |theta|\ :sub:`2`  - угол вылета покоящейся частицы (Лаб. Сит.).

        d |theta| - полуширина выбираемого диапазона (Лаб. Сит.).

    .. tip::
        Задавать диапазон розыгрыша косвенно: через координату |theta|\ :sub:`Lab` \  центра щели детектора и
        полуширину выбираемого диапазона d |theta| - предпочтительнее! Эти параметры всегда известны 
        пользователю, потому что все элементы геометрии позиционируется в Лаб. Сит.!
        
    * Диапазон розыгрыша для |phi| выбирается явно::

        scattering->SetPhiRange(-20., 20.);

Механизм работы класса
----------------------

Добавляем упругое рассеяние в коллекцию распадов::

    decayer->AddDecay(scattering);

Ниже опишем что будет происходить после этого.

Класс ElasticScattering содержит два основных метода: Intit(), Stepping().

Bool_t ElasticScattering::Init()
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Данный метод вызывается в самом начале симуляции, на этапе инициализации.
Здесь проверяется::

    if (!ERDecay::Init())
    {
        return kFALSE;
    }

    fTargetIonPDG = TDatabasePDG::Instance()->GetParticle(fTargetIonName);
    if ( ! fTargetIonPDG )
    {
        LOG(FATAL)<< "Target ion not found in pdg database!" << FairLogger::endl;
        return kFALSE;
    }

Считается диапазон розыгрыша углов |theta|\ :sub:`CM` \  - вызовом метода::

    ERElasticScattering::RangesCalculate(Doubleр_t iM, Double_t tM)

где iM - масса налетающего иона, а tM - иона мишени.

.. note:: В случае явного определения диапазона розыгрыша |theta|\ :sub:`CM` \ , метод описанный выше не вызывается!

Так же здесь формируется кумулятивная функция. Из диапазона |theta|\ :sub:`CM` \  вычисляется соответствующий диапазон внутри кумулятивной функции.

Bool_t ElasticScattering::Steping()
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Вызывается на каждом шаге транспорта налетающего иона внутри объёма мишени.

Здесь разыгрываются углы вылета для ионов - налетающего и мишени, следующим образом::

    // Generate random angles theta and phi
    Double_t theta = ThetaGen();
    Double_t phi = fRnd->Uniform(fPhi1*DegToRad(), fPhi2*DegToRad());
    
Метод::

     ERElasticScattering::ThetaGen();

генерирует угол |theta|\ :sub:`CM` из кумулятивной функции.

Затем ионы с новыми характеристиками кидаются в стек частиц для дальнейшего транспорта.

Связь лабораторной и центра масс систем
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. figure:: _static/fig4.png
    :height: 600px
    :width:  600 px
    :align: center

На рисунке выше ион А упруго рассеивается на ионе B. В имплементации метода ElasticScattering::Steping() углы |theta|\ :sub:`CM` \
разыгрываются в системе координат (на рис., выше выделена красным) у которой ось z  направлена вдоль импульса первичного иона.
Необходимо  повернуть систему координат (2) так что бы ее ось z  совпала с-осью z  из (3).
С помощью методов TLorentzVector::

    TLorentzVector::RotateZ(-phi);
    TLorentzVector::RotateY(theta);
    TLorentzVector::RotateZ(phi);

В стек частиц упруго рассеявшаяся ионы необходимо передать с характеристиками в лабораторной системе координат: Px, Py, Pz и E - энергия.
Для перехода из системы центра масс в лабораторную используется Boost::

    TLorentzVector::Boost(TVector3 &);

Подробнее об этом можно прочесть здесь: https://root.cern/doc/v616/classTLorentzVector.html


Пример использования класса
---------------------------

Класс **ERElasticScattering** использовался для моделирования упругого рассеяния\ :sup:`15` \ N на \ :sup:`11` \ B.

Ниже представлен управляющий макрос для симуляции::

    void sim(Int_t nEvents = 100, Int_t index = 0, TString outDir="output", Double_t angle = 20.)
    {
        gRandom->SetSeed(index);

        //---------------------Files-----------------------------------------------
        TString outFile;
        outFile.Form("%s/sim_%d.root", outDir.Data(), index);
        TString parFile;
        parFile.Form("%s/par_%d.root", outDir.Data(), index);
        // ------------------------------------------------------------------------

        // -----  Timer  --------------------------------------------------------
        TStopwatch timer;
        timer.Start();

        // -----  Create simulation run  ----------------------------------------
        ERRunSim* run = new ERRunSim();
        /** Select transport engine
        * TGeant3
        * TGeant4
        **/
        run->SetName("TGeant4");              // Transport engine
        run->SetOutputFile(outFile.Data());          // Output file

        // ------------------------------------------------------------------------
        // -----  Runtime database  ---------------------------------------------
        FairRuntimeDb* rtdb = run->GetRuntimeDb();
        // ------------------------------------------------------------------------

        // -----  Create media  -------------------------------------------------
        run->SetMaterials("N15.media.geo");      // Materials
        // ------------------------------------------------------------------------

        //-------- Set MC event header --------------------------------------------
        ERDecayMCEventHeader* header = new ERDecayMCEventHeader();
        run->SetMCEventHeader(header);
        //-------------------------------------------------------------------------

        // -----  Create detectors  ----------------------------------------------
        FairModule* cave= new ERCave("CAVE");
        cave->SetGeometryFileName("cave.geo");
        run->AddModule(cave);

        FairModule* collimator = new ERCollimator(); // "N15B11_collimator", "N15B11_collimator"
        collimator->SetGeometryFileName("N15.collimator.root");
        run->AddModule(collimator);

        ERDetector* target = new ERTarget("N15B11_target", kTRUE, 1);
        target->SetGeometryFileName("N15.target.root");
        run->AddModule(target);

        FairDetector* detector = new ERN15B11Detector("N15B11detector", kTRUE, 1);
        detector->SetGeometryFileName("N15B11_detector.geo.root");
        run->AddModule(detector);

        //------    ER Decayer  -------------------------------------------------
        //Ion 15N
        Int_t A = 15;
        Int_t Z = 7;
        Int_t Q = 3;

        ERDecayer* decayer = new ERDecayer();
        ERElasticScattering* scattering = new ERElasticScattering("15Nto15N11B");

        scattering->SetInputIon(Z,A,Q);
        scattering->SetTargetIon(5, 11, 5); // 11B
        scattering->SetThetaCDF("cos_tetta_cross.txt");
        scattering->SetUniformPos(-0.00035,0.00035);
        scattering->SetStep(0.00001); //0.1 micron
        scattering->SetDecayVolume("cave"); //targetB11
        scattering->SetDetectorsSlot(angle, 4.*0.262822833);
        scattering->SetPhiRange(-20., 20.);

        decayer->AddDecay(scattering);
        run->SetDecayer(decayer);

        // -----  Create PrimaryGenerator  --------------------------------------
        FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
        ERIonMixGenerator* generator = new ERIonMixGenerator("15N", Z, A, Q, 1);
        generator->SetKinERange(0.0427094, 0.0436017); // 0.0427094 : 0.0436017

        Double32_t theta = 0.;
        Double32_t sigmaTheta = 5e-3*TMath::RadToDeg();
        generator->SetThetaSigma(theta, sigmaTheta); // theta = 0., sigma = 5 mrad
        generator->SetPhiRange(0., 180.); // 0 : 180

        Double32_t distanceToTarget = 50.; // work: 50 cm, test 0.5 micron: 0.00005+0.00035
        generator->SetBoxXYZ(-0.5, -0.5, 0.5, 0.5, -distanceToTarget); // Xmin = -0.5, Ymin = -0.5, Xmax = 0.5, , Ymax = 0.5, Z

        primGen->AddGenerator(generator);

        run->SetGenerator(primGen);
        // ------------------------------------------------------------------------

        //-------Set visualisation flag to true------------------------------------
        run->SetStoreTraj(kFALSE); // or kTRUE

        //-------Set LOG verbosity  -----------------------------------------------
        FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
        FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");

        //------- Initialize simulation run ---------------------------------------
        run->Init();
        Int_t nSteps = -15000;

        //--- Runtime database ----------------------------------------------------
        Bool_t kParameterMerged = kTRUE;    /** @brief Returns curent theta in CM for Primary Ion. **/
        FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
        parOut->open(parFile.Data());
        rtdb->setOutput(parOut);
        rtdb->saveOutput();
        rtdb->print();

        // -----  Run simulation  ------------------------------------------------
        run->Run(nEvents);

        // -----  Finish  -------------------------------------------------------
        timer.Stop();
        Double_t rtime = timer.RealTime();
        Double_t ctime = timer.CpuTime();
        cout<< endl << endl;
        cout<< "Macro finished succesfully." << endl;
        cout<< "Output file is sim.root" << endl;
        cout<< "Parameter file is par.root" << endl;
        cout<< "Real time " << rtime << " s, CPU time " << ctime
        << "s" << endl << endl;
    }

Результаты моделирования
~~~~~~~~~~~~~~~~~~~~~~~~

.. figure:: _static/fig7.png
    :height: 600px
    :width:  600 px
    :align: center

    На данном рисунке мы отоброзили как некоторые физические факторы влияют на угловое разрешение.

    #. Мы начали с идеальной реакции с идеальным пучком, без мишени, без коллиматора, но с реалистичной щелью детектора и последовательно добавили следующие реалистичные свойства:

    #. 11B мишень толщиной 7 микрон
    #. Энергетическое размытие пучка иона 15N (42 : 43 МэВ)
    #. Размытие угла  пучка иона 15N (sigma = 5 мрад)
    #. Размытие угла  пучка иона 15N (0 : 2)
    #. X размытие пятна пучка на мишени (-0.5 : 0.5 см)
    #. Y размытие пятна пучка на мишени (-0.5 : 0.5 см)
    #. Коллиматор пучка (размер отверстия 1.5 см)

.. figure:: _static/fig6.png
    :height: 600px
    :width:  600 px
    :align: center

    Измеренное дифференциальное сечение как функция угла рассеяния  в ЦМ

    На данном рисунке представлено сравнение входного дифференциального сечения и выходного  (полученного по результатам симуляции).
    Здесь можно видеть два главных эффекта:

    #. Полученная зависимость становиться менее крутой в передних углах.
    #. Волновое поведение исходной кривой становится менее выраженным.

В результате работы было продемонстрировано, что восстановленное дифференциальное сечение немного отличается от входного
Основной причиной этого различия является размер пятна пучка на мишени
Влияние длины щели оказалось незначительно, следовательно, она может быть увеличено для лучшей эффективности обнаружения
Разработанное программное обеспечение будет использоваться для планирования и анализа подобных экспериментов в будущем