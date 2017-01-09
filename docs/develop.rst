Разработка
==========

Структура сервера разработки
----------------------------

Git и Gitflow
-------------

Redmine
-------

Jenkins - сборки и тесты
------------------------

QA мониторы
-----------

Стандарты разработки
--------------------

.. _cmake_struct:

Структура cmake сценария для сборки библиотеки классов
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Все дириктории проекта ER кроме служебных: docs, gconfig, geometry, macro, parameters, templates, являются дирикторией исходных кодов одной библиотеки. Стандартный cmake сценарий сборки библиотеки выглядит так:

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

Для использования библиотеки в макросах ROOT ее нужно собрать с исопльзованием `специального инструмента и процедуры сборки. <https://root.cern.ch/root/htmldoc/guides/users-guide/AddingaClass.html>`_ Данные процесс автоматизирован с помощью функции 'GENERATE_LIBRARY()', которая находится в cmake модулях пакета FAIRroot.



