Разработка
==========

Структура сервера разработки
----------------------------


Git и Gitflow
-------------

Документирование
----------------

Документирование ER разделяется на документирование логики с помощью системы сборки документации 
sphinx-doc и автоматической генерации документации на класс по коду с помощью doxygen.

Исходники документации sphinx написаны в формате 
`reStructedText <http://www.sphinx-doc.org/en/1.5.1/rest.html>`_ и расположены в репозитории в папке
 docs.

По адресу er.jinr.ru расположена документация, собранная sphinx из ветки dev. Для обновления 
информации в ней необходимо отредактировать документацию в реозитории и запустить задачу обновления
 документации (update doc) в jenkins(er.jinr.ru:8080).

Redmine
-------

Jenkins - сборки и тесты
------------------------

QA мониторы
-----------

Стандарты разработки
--------------------

Лицензия
~~~~~~~~

ExpertRoot распространяется под лицензией LGPL <https://www.gnu.org/licenses/lgpl-3.0.ru.html>. 
В шапке всех исходных файлов должно быть добавлено:

::

  /********************************************************************************
   *              Copyright (C) Joint Institute for Nuclear Research              *
   *                                                                              *
   *              This software is distributed under the terms of the             * 
   *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
   *                  copied verbatim in the file "LICENSE"                       *
   ********************************************************************************/


Code convention
~~~~~~~~~~~~~~~

* Идентификаторы классов должны начинаться с префикса ER (``ERNeuRad``, ``ERGadast``, ...)
* Должно выдерживаться соотношение: один класс - два файла (``.h`` и ``.cxx``)
* Файлы включений должны быть расположены в следующей последовательности

  1. Заголовочный файл класса, если это ``.cxx``
  2. Заголовочные файлы библиотеки ``STL``
  3. Заголовочные файлы библиотеки ``Boost``
  4. Заголовочные файлы фреймворка ``Root``
  5. Заголовочные файлы фреймворка ``Geant``
  6. Заголовочные файлы фреймворка ``FairRoot``
  7. Заголовочные файлы пакета ``ExpertRoot``
* Между файлами включения разных библиотек должен быть отступ - пустая строка
  
::

  #include "ERNeuRad.h"

  #include <iostream>

  #include "TClonesArray.h"
  #include "TParticle.h"
  #include "TVirtualMC.h"
  #include "TGeoMatrix.h"
  #include "TString.h"

  #include "FairRootManager.h"
  #include "FairRun.h"
  #include "FairRunSim.h"
  #include "FairRuntimeDb.h"

  #include "ERNeuRadGeoPar.h"
  #include "ERMCEventHeader.h"

* В заголовочном файле должны быть подключены только те внешние заголовочные классы, которые ему необходимы. Подключать файлы для исходного файла в заголовочном нельзя.
* Объявления пространств имен должны быть расположены после включений заголовочных файлов, в той же последовательности, с теми же отступами и только в исходном файле. Объявления пространств имен в заголовочном файле не допускается.
* Код заголовочного файла должен быть обернут в директивы (идентификатор директивы формируется из названия класс и ``_H``): 

::

  #ifndef ERNeuRad_H
  #define ERNeuRad_H

  ...

  #endif //ERNeuRad_H

* В конце заголовочного и файла исходно кода должна быть пустая строка
* В заголовочном файле после include должен идти блок предварительной декларации всех используемых в нем классов в той же последовательности, что и заголовочные файлы (кроме классов от которых происходит наследование):

::

  class TClonesArray;
  class TLorentzVector;

  class FairVolume;

  class ERNeuRadStep;
  class ERNeuRadPoint;

* В определении класса последовательно должны быть введены следующие блоки:

  * Первый public:

    * Конструктор по умолчанию - конструктор без параметров. (Требование интерпретатора Root)
    * Конструкторы с параметрами
    * Деструктор (виртуальный, если класс является наследником)
    * Конструктор копирования и оператор присваиваивания. Обязательны для классов данных, для остальных - по необходимости.
    * Блок методов модификаторов, интерфейсы для изменения данных, настройки класса. Должен начинаться с комментария ``/* Modifiers */``. Каждый метод должен начинаться с префикса ``Set``.
    * Блок методов аксессоров, для получения данных класса. Должен начинаться с комментария ``/*Accessors*/``. Методы должны быть константными.
  * Второй public:

    * Виртуальные публичные методы класса.
    * Остальные публичные методы класса. Атрибут класса не может находится в блоке public. Каждому атрибуту класса данных необходимо предоставть аксессор и модификатор. В случае остальных классов - по необходимости.
  * protected методы (возникают исключительно в тот момент, когда оказались необходимы)
  * protected данные (так же как и в предыдущем пункте)
  * private методы
  * private данные
* В файле исходного кода реализации методов расположены в той же последовательности, что и в определении класса.
* Между методами необходимо добавлять разделитель в виде одной строки:

::

  //--------------------------------------------------------------------------------------------------

* Все методы класса начинаются с буквы верхнего регистра. Нижние подчеркивания в названиях методов не допускаются. В абревиатуре только первая буква пишется заглавной (``ER`` является исключением).
* Все атрибуты(данные) класса начинаются с префикса ``f``.
* Все переменные начинаются с буквы нижнего регистра.
* Отступы выполняются двумя пробелами. Использование знака табуляции для разметки недопустимо. Для удобства необходимо настроить свои текстовые редакторы на змену табляции двумя пробелами.
* Открывающая фигурная скобка должна стоять в той же строке, что и оператор. Исключением является скобка после списка инициализации. Закрывающая - в отдельной строке.
* Ширина строки не должна превышать 100 символов. Это также удобно настроить в текстовом редакторе.

.. _cmake_struct:

Структура cmake сценария для сборки библиотеки классов
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Все директории проекта ER кроме служебных: docs, gconfig, geometry, macro, parameters, templates,
 являются директорией исходных кодов одной библиотеки. Стандартный cmake сценарий сборки библиотеки
 выглядит так:

::

  # Create a library called "libNeuRad" which includes the source files given in
  # the array .
  # The extension is already found.  Any number of sources could be listed here.

  set(INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
  ${ROOT_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/ERData/NeuRadData/
  ${CMAKE_SOURCE_DIR}/ERData/
  ${CMAKE_SOURCE_DIR}/NeuRad/
  ${CMAKE_SOURCE_DIR}/ERBase/
  )

  include_directories( ${INCLUDE_DIRECTORIES})

  set(LINK_DIRECTORIES
  ${BASE_LINK_DIRECTORIES}
  ${FAIRROOT_LIBRARY_DIR}

  ) 

  link_directories( ${LINK_DIRECTORIES})

  set(SRCS
    ERNeuRad.cxx
    ERNeuRadDigitizer.cxx
    ERNeuRadContFact.cxx
    ERNeuRadDigiPar.cxx
    ERNeuRadGeoPar.cxx
    ERNeuRadSetup.cxx
    ERNeuRadHitFinder.cxx
    ERNeuRadHitFinderMF.cxx
    ERNeuRadHitFinderWBT.cxx
    ERNeuRadMatcher.cxx
  )

  # fill list of header files from list of source files
  # by exchanging the file extension
  CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

  Set(LINKDEF ERNeuRadLinkDef.h)
  Set(LIBRARY_NAME NeuRad)
  Set(DEPENDENCIES ERBase ERData Base Core Geom)

  GENERATE_LIBRARY()

Для использования библиотеки в макросах ROOT ее нужно собрать с исопльзованием 
`специального инструмента и процедуры сборки.
<https://root.cern.ch/root/htmldoc/guides/users-guide/AddingaClass.html>`_ Данные процесс
автоматизирован с помощью функции ``GENERATE_LIBRARY()``, которая находится в cmake модулях пакета
``FAIRroot``.

Сценарий начинается с инициализации списка директорий include файлов: 

::

  set(INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
  ${ROOT_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/ERData/NeuRadData/
  ${CMAKE_SOURCE_DIR}/ERData/
  ${CMAKE_SOURCE_DIR}/NeuRad/
  ${CMAKE_SOURCE_DIR}/ERBase/
  )

  include_directories( ${INCLUDE_DIRECTORIES})


Переменные ``BASE_INCLUDE_DIRECTORIES, ROOT_INCLUDE_DIR, Boost_INCLUDE_DIRS`` определены в корневом
cmake сценарии проекта и модулях, отвечающих за поиск соответствующих пакетов в системе. Например
``~/fair_install/fairroot_inst/share/fairbase/cmake/modules/FindROOT.cmake``.

Далее инициализируется список директорий с библиотеками для линковки.

::

  set(LINK_DIRECTORIES
  ${BASE_LINK_DIRECTORIES}
  ${FAIRROOT_LIBRARY_DIR}

  ) 

  link_directories( ${LINK_DIRECTORIES})

Далее инициализуется список исходников, которые будут включены в библиотеку.

:: 
  
  set(SRCS
    ERNeuRad.cxx
    ERNeuRadDigitizer.cxx
    ERNeuRadContFact.cxx
    ERNeuRadDigiPar.cxx
    ERNeuRadGeoPar.cxx
    ERNeuRadSetup.cxx
    ERNeuRadHitFinder.cxx
    ERNeuRadHitFinderMF.cxx
    ERNeuRadHitFinderWBT.cxx
    ERNeuRadMatcher.cxx
  )

  # fill list of header files from list of source files
  # by exchanging the file extension
  CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Назначается LinkDef файл, имя библиотеки и список библиотек для линковки.

:: 

  Set(LINKDEF ERNeuRadLinkDef.h)
  Set(LIBRARY_NAME NeuRad)
  Set(DEPENDENCIES ERBase ERData Base Core Geom)

Вызывается функция ``GENERATE_LIBRARY()``.

::

  GENERATE_LIBRARY()




