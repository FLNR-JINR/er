Interactions
============

В большинстве рассматриваемых физических экспериментов требуется восстановить параметры одного из продуктов взаимодействия
по параметрам первичного иона и продуктов, наблюдаемых в детекторах.

Для физики низких энергий критически важна точность определения параметров первичного иона перед взаимодействием.
Этот факт не позволяет начать моделирование с процесса транспорта продуктов, а требует детального рассмотрения
процесса пропагации первичного иона с учетом рассеяния и энергопотерь в детекторе пучка и мишени.

Необходимость использования специфических моделей взаимодействий и их каскадов делает невозможным 
использование Монте-Карло пакетов общего назначения:

* Geant (https://geant4.web.cern.ch/) ;
* Pythia  (http://home.thep.lu.se/~torbjorn/Pythia.html) .

Interactions - подсистема er, отвечающая за имплементацию моделирования реакций и распадов в
в общую схему процесса симуляций экспериментов, наследованную из пакета FairRoot.

Кроме того, Interactions может служить просто кинематическим кулькулятором взаимодействий.

Задачи Interactions можно разделить на две группы:

* Реализация довольно простых моделей взаимодействий:
	* Реакции 2 в 2
	* Распады, изотропные в ЦМ

но с существенно кастомизированными для задач физики низких энергий возможностями их параметризации;

* Реализация интерфейса между программными пакетами генерации событий, построенными на сложных теоретических моделях, и процессом симуляции всего эксперимента.

Ниже приведем иерархию классов подсистемы Interactions. Красным выделены абстрактные классы,
реализующие интерфейс и общую функциональность;
зеленым - классы, которые пользователь может напрямую использовать для конфигурации взаимодействия.

.. figure:: _images/interaction_classes.png
       :scale: 100 %
       :align: center
       :alt: Альтернативный текст


ERInteraction
-------------

ERInteraction - базовый абстрактный класс, предоставляющий общий интерфейс и общую функциональность для реализации взаимодействий в ExpertRoot.
На этом уровне иерархии классов находятся данные и функциональность, которые необходимы как для реакций, так и для распадов:

Участники взаимодействия
++++++++++++++++++++++++

Введены три типа участников взаимодействия: ERInteractionParticipant {kPROJECTILE, kTARGET, kEJECTILE};

Projectile и Target задаются либо с помощью зарядового Z и массового числа A, либо с помощью PDG кода:

::

  AddProjectile("Name", Z,A);
  AddProjectile("Name", pdg);
  AddTarget("Name", Z, A);
  AddTarget("Name", pdg);


По умолчанию ejectile транспортируется после взаимодействия (EREjectileTransported::kTRANSPORTED), но можно отменить
транспорт ejectile (EREjectileTransported::kNOTTRANSPORTED) в связи с тем, что он переходит в следующее взаимодействие
мгновенно (происходит распад, к примеру), или в транспорте нет необходимости, потому что Ejectile не интересует нас
ни как сигнал, ни как фон в данном сетапе.

::

  AddEjectile(Z, A, Q, kNOTTRANSPORTED);
  AddEjectile(pdg, kTRANSPORTED);

Для ejectile возможно задать спектр энергии возбуждения. Форма спектра задается либо с помощью взвешенного набора гауссиан:

:: 

  AddEjectile(Z, A, Q, kNOTTRANSPORTED, {{mean1, sigma1, weight1}, 
                                         {mean2, sigma2, weight2},  
                                         ...});

либо с помощью указания произвольной функции распределения TF1*:

::

  AddEjectile(Z, A, Q, kNOTTRANSPORTED, TF1* );

.. note::

  При указании энергии возбуждения, энергия основного состояния равно AEM * A.
  Для всех стабильных участников взаимодействия масса берется из Geant4.
  Значения используемых масс можно найти в логе.


Методы опредления координаты взаимодейсвия
++++++++++++++++++++++++++++++++++++++++++

* Способ определения координаты взаимодействия:
	* Распределение абсолютной координаты взаимодействия (Uniform, Exponential)
  * Распределение координаты взаимодействия с учетом реальной формы объема мишени. Задается Nuclear Interaction Length и Max Interaction Path Length.
  * По времени жизни в системе центра масс
	* Распределение энергии, до которой пропагируется первичный ион
* Способ разыгрывания кинематики: изотропное, неизотропное

Функциональность:

* Проверка входных данных:
	* Должен быть определен projectile, если нет родительского взаимодействия;
* Проверка, делаем ли взаимодействие на данном шаге 
* Остановка projectile трека
* Перевод выходов в лаб. СК
* Добавление выходов в стек треков


Последовательности взаимодействий
---------------------------------

Возможно задание последовательности или дерева взаимодействий. Ejectile материнского взаимодейсвия
может транспортироваться до позиции дочернего взаимодействия (EREjectileTransported::kTRANSPORTED) или дочернее взаимодейсвие начинается
в позиции материнского (EREjectileTransported::kNOTTRANSPORTED). В случае kNOTTRANSPORTED трек не создается.


ERReaction2to2
++++++++++++++

ERReaction2to2 - класс для реализации реакции 2 в 2 в общем случае.

Функциональность:

* Проверка входных данных:
	* Должен быть определен target
* Неизотропное распределение theta по CDF из файла (в  заданных диапазонах theta и phi)

ERReaction2to2Elastic
+++++++++++++++++++++

* Реализация упругого рассеяния 2 в 2

ERReaction2in2Inelastic
+++++++++++++++++++++++

* Надо прописывать конкретную физику "неупругости" - спектр возбуждения и, если есть, угловую зависимость этого спектра.

ERDecay
+++++++

* Способ определения координаты взаимодействия:
	* Время жизни иона

ERDecayIsotropic
++++++++++++++++

* Распределение энергии возбуждения входа (Возможно несколько с весами)

ERDecayExternal
+++++++++++++++

* Чтение результатов распада в ЦМ из внешнего файла

Запись взаимодействия в выходной файл
-------------------------------------

Параметры взаимодействия: координату, время, параметры частиц необходимо сохранять в выходной файл симуляции для дальнейшего
анализа. Ранее для сохранения использовались объекты header. В системе Interactions в выходной файл сохраняются сами объекты 
класса ERInteraction. Имя ветки соответствует имени взаимодействия. В случае каскада сохраняются отдельно
все взаимодействия каскада. Регистрацию объектов должен вызывать цикл внутри объекта ERInteractions (аналог текущего ERDecays).

Примеры использования
---------------------

* Упругое рассеяние 15N на 11B

::

	ERReaction2in2Elastic* reaction = new ERReaction2in2Elastic("15Nto15N11B");

	reaction->AddProjectile(7,15,7);
	reaction->AddTarget(5,11,5);

	reaction->SetUniformPos(-0.00035,0.00035);
	reaction->SetStep(0.00001); //0.1 micron
	reaction->SetInteractionVolume("targetB11");

	reaction->SetThetaCDF("cos_tetta_cross.txt");
	reaction->SetThetaRange(20., 21.);
	// reaction->SetLabThetaRange(20., 21.,kPROJECTILE); // theta range in lab for projectile
	// reaction->SetLabThetaRange(20., 21.,kTARGET); 
	reaction->SetPhiRange(0., 0.);



* Каскад реакции и распада для изучения 7H:

	* 8He + 2H → 3He + 7H
	* 7H → 3H + n +n +n +n

На моделирование каскада накладывается следующий набор условий:

1. Координата взаимодействия была разыграна по экспоненциальному закону.
2. 7H не транспортируется, распадается прямо в точке реакции.
3. Был задан спектр энергии возбуждения.
4. Реакция неизотропна; задана функция распределения угла theta.
5. Распад изотропен.

::

  ERReaction2to2* reaction = ERReaction2to2("8He + 2H → 3He + 7H");
  
  reaction->AddProjectile(2,8,2);
  reaction->AddTarget(1,2,1);
  reaction->AddEjectile(2,3,2);
  reaction->AddEjectile(1,7,1,kNOTTRANSPORTED); //don`t added to Geant track stack

  reaction->AddEjectileExitation(1,0.0004, 0.00002355, 1); //exitation distribution for second ejectile
  reaction->AddEjectileExitation(1,0.0012, 0.0002355, 1);

  reaction->SetExponentialPos(...);
  reaction->SetStep(1e-5);
  reaction->SetInteractionVolume("boxCD");

  reaction->SetThetaCDF("cos_tetta_cross.txt");

  ERDecayIsotropic* decay = ERDecayIsotropic("7H → 3H + n +n +n +n", reaction, 1)
  //second ejectile from parent reaction uses as projectile for decay

  decay->AddEjectile(1,3,1);
  decay->AddEjectile(2112);
  decay->AddEjectile(2112);
  decay->AddEjectile(2112);
  decay->AddEjectile(2112);


* Каскад с реакцией и распадом на лету для изучения 10Li:

	* 9Li + 2H → 10Li + 1H
	* 10Li → 9Li + n

На моделирование каскада накладывается следующий набор условий:

1. Координата взаимодействия была разыграна по равномерному закону.
2. После реакции Li10 транспортируется, до своей координаты распада.
3. Координата распада разыгрывается по времени жизни иона.
4. Пропагация протона после реакции не требуется.

::

  ERReaction2to2* reaction = ERReaction2to2("9Li + 2H → 10Li + 1H");
  
  reaction->AddProjectile(3,9,3);
  reaction->AddTarget(1,2,1);
  reaction->AddEjectile(3,10,3,kTRANSPORTED); //add to Geant track stack
  reaction->AddEjectile(1,1,1,kNOTTRANSPORTED); 

  reaction->SetUniformPos();
  reaction->SetStep(1e-5);
  reaction->SetInteractionVolume("target2H");

  ERDecayIsotropic* decay = ERDecayIsotropic("10Li → 9Li + n", reaction,0);

  decay->AddEjectile(3,9,3);
  decay->AddEjectile(2112);

  decay->SetPosByLifeTime(0.000001);


* Каскад с реакцией и распадом для изучения 10He:

	* 8He + 3H → 1H + 10He
	* 10He → 8He + n + n

На моделирование каскада накладывается следующий набор условий:

1. Координата взаимодействия была разыграна по равномерному закону.
2. После реакции 10He не транспортируется.
3. Распад загружается из внешнего текстового файла.

::

  ERReaction2to2* reaction = ERReaction2to2("8He + 3H → 1H + 10He");
  
  reaction->AddProjectile(2,8,2);
  reaction->AddTarget(1,3,1);
  reaction->AddEjectile(1,1,1); 
  reaction->AddEjectile(2,10,2,kNOTTRANSPORTED);
  
  reaction->SetUniformPos();
  reaction->SetStep(1e-5);
  reaction->SetInteractionVolume("target3H");

  ERDecayExternal* decay = ERDecayExternal("10He → 8He + n + n", reaction,1);

  decay->AddEjectile(2,8,2);
  decay->AddEjectile(2112);
  decay->AddEjectile(2112);

  decay->SetFile("10He_n_n_decays.txt");


