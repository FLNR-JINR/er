
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
		ERDetPoint.cxx
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
	#pragma link C++ class ERDetPoint+;

Если мы хотим указать, что объект данного класса будет писаться в root файл, необходимо добавить ``+`` после названия класса.

Минимальный набор файлов библиотеки классов детектора для симуляции:

::

	CMakeLists.txt
	ERDetLinkDef.h
	ERDet.h
	ERDet.cxx
	ERDetPoint.h
	ERDetPoint.cxx

Чтобы добавить библиотеку в процесс общей сборки expertroot нужно добавить строку:

::

	add_subdirectory (det)

в файл ``~/expertroot/CMakeLists.txt`` туда где вызываюстя подобные команды (на данный момент после ``add_subdirectory (beamtime)``).

При создании новых классов не забывайте вносить их в ``CMakeLists.txt`` и в ``LinkDef.h`` файлы.
 