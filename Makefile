# Определяем тип операционной системы
OS := $(shell uname -s)

# Название приложения
NAME = asc

# Если это MacOS X (Сборка через make)
ifeq ($(OS), Darwin)
	# Компилятор
	CC = clang++

	# Версия
	PYVER = 8
	# Питон
	PYTHON = python3.$(PYVER)
	# Путь питона
	PYPATH = /System/Volumes/Data/usr/local/Cellar/python@3.$(PYVER)/3.$(PYVER).5/Frameworks/Python.framework/Versions/3.$(PYVER)

	# Сторонние модули
	LIBS = \
		-lm \
		-lz \
		-lssl \
		-flto \
		-lstdc++ \
		-lcrypto \
		-lpthread \
		-ljemalloc \
		-l$(PYTHON) \
		-L$(PYPATH)/lib \
		-L/usr/lib \
		-L/usr/local/lib \
		-L/usr/local/Cellar/openssl@1.1/1.1.1g/lib

	# Расширение библиотеки
	EXT = dylib

	# Библиотеки для тестовой сборки
	UNITLIBS = \
		./gtest/mac/libgtest.a \
		./gtest/mac/libgtest_main.a

	# Заголовочные файлы
	INCLUDE = \
		-I./ \
		-I./alm \
		-I./include \
		-I./alm/include \
		-I./contrib/include \
		-I./alm/contrib/include \
		-I/usr/include \
		-I/usr/local/include \
		-I./gtest/include \
		-I$(PYPATH)/include/$(PYTHON) \
		-I/usr/local/Cellar/openssl@1.1/1.1.1g/include

	# brew install google-perftools

	# Отладчик
	DEBUG = -glldb
endif

# Если это FreeBSD (Сборка через gmake)
ifeq ($(OS), FreeBSD)
	# Компилятор
	CC = clang++

	# Версия
	PYVER = 7
	# Питон
	PYTHON = python3.$(PYVER)m

	# Сторонние модули
	LIBS = \
		-lm \
		-lz \
		-lssl \
		-flto \
		-lstdc++ \
		-lcrypto \
		-lpthread \
		-ljemalloc \
		-l$(PYTHON) \
		-L/usr/lib \
		-L/usr/local/lib

	# Расширение библиотеки
	EXT = so

	# Библиотеки для тестовой сборки
	UNITLIBS = \
		./gtest/freebsd/libgtest.a \
		./gtest/freebsd/libgtest_main.a

	# Заголовочные файлы
	INCLUDE = \
		-I./ \
		-I./alm \
		-I./include \
		-I./alm/include \
		-I./contrib/include \
		-I./alm/contrib/include \
		-I/usr/include \
		-I/usr/local/include \
		-I./gtest/include \
		-I/usr/local/include/$(PYTHON)

	# pkg install google-perftools-2.7_1

	# Отладчик
	DEBUG = -glldb
endif

# Если это Linux
ifeq ($(OS), Linux)
	# Компилятор
	CC = gcc

	# Версия
	PYVER = 7
	# Питон
	PYTHON = python3.$(PYVER)m

	# Сторонние модули
	LIBS = \
		-lm \
		-lz \
		-lssl \
		-flto \
		-lstdc++ \
		-lcrypto \
		-lpthread \
		-ljemalloc \
		-l$(PYTHON) \
		-L/usr/lib \
		-L/usr/local/lib

	# Расширение библиотеки
	EXT = so

	# Библиотеки для тестовой сборки
	UNITLIBS = \
		./gtest/linux/libgtest.a \
		./gtest/linux/libgtest_main.a

	# Заголовочные файлы
	INCLUDE = \
		-I./ \
		-I./alm \
		-I./include \
		-I./alm/include \
		-I./contrib/include \
		-I./alm/contrib/include \
		-I/usr/include \
		-I/usr/local/include \
		-I./gtest/include \
		-I/usr/include/$(PYTHON)

	# apt install libgoogle-perftools-dev

	# Отладчик
	DEBUG = -ggdb3
endif

# Бинарный файл
BIN = ./bin
# Каталог сборки
BUILD = ./build

# Конфиг для сборки отладочной версии
CONFDEV = -std=c++11 -O0 -fno-permissive -Wno-pedantic -Wno-unknown-attributes
# Конфиг для стандартной сборки
CONFIG = -std=c++11 -O2 -pipe -mfma -mrdrnd -mf16c -fno-permissive -Wno-pedantic -Wno-unknown-attributes

# ./app/alm.cpp

# Правило сборки
all:
	mkdir -p $(BUILD) && \
	mkdir -p $(BIN)/libs && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./app/asc.cpp -o $(BUILD)/main.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/alt.cpp -o $(BUILD)/alt.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/spl.cpp -o $(BUILD)/spl.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/dict.cpp -o $(BUILD)/dict.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/hnsw.cpp -o $(BUILD)/hnsw.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/ascb.cpp -o $(BUILD)/ascb.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/bloom.cpp -o $(BUILD)/bloom.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/dumper.cpp -o $(BUILD)/dumper.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/stemmer.cpp -o $(BUILD)/stemmer.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/idw.cpp -o $(BUILD)/idw.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/nwt.cpp -o $(BUILD)/nwt.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/env.cpp -o $(BUILD)/env.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/alm.cpp -o $(BUILD)/alm.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/alm1.cpp -o $(BUILD)/alm1.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/alm2.cpp -o $(BUILD)/alm2.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/arpa.cpp -o $(BUILD)/arpa.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/ablm.cpp -o $(BUILD)/ablm.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/python.cpp -o $(BUILD)/python.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/toolkit.cpp -o $(BUILD)/toolkit.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/alphabet.cpp -o $(BUILD)/alphabet.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/progress.cpp -o $(BUILD)/progress.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/tokenizer.cpp -o $(BUILD)/tokenizer.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/collector.cpp -o $(BUILD)/collector.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/levenshtein.cpp -o $(BUILD)/levenshtein.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/contrib/src/cityhash/city.cc -o $(BUILD)/cityhash.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/contrib/src/bigint/BigInteger.cc -o $(BUILD)/BigInteger.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/contrib/src/bigint/BigUnsigned.cc -o $(BUILD)/BigUnsigned.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/contrib/src/bigint/BigIntegerUtils.cc -o $(BUILD)/BigIntegerUtils.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/contrib/src/bigint/BigUnsignedInABase.cc -o $(BUILD)/BigUnsignedInABase.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/contrib/src/bigint/BigIntegerAlgorithms.cc -o $(BUILD)/BigIntegerAlgorithms.o && \
	$(CC) $(LIBS) -shared -fPIC -o $(BIN)/libs/libspl.$(EXT) $(BUILD)/spl.o $(BUILD)/alt.o $(BUILD)/idw.o $(BUILD)/nwt.o $(BUILD)/env.o \
	$(BUILD)/cityhash.o $(BUILD)/bloom.o $(BUILD)/dict.o $(BUILD)/dumper.o $(BUILD)/stemmer.o $(BUILD)/hnsw.o $(BUILD)/arpa.o $(BUILD)/ablm.o \
	$(BUILD)/python.o $(BUILD)/toolkit.o $(BUILD)/alm.o $(BUILD)/alm1.o $(BUILD)/alm2.o $(BUILD)/tokenizer.o $(BUILD)/alphabet.o \
	$(BUILD)/levenshtein.o $(BUILD)/collector.o $(BUILD)/progress.o $(BUILD)/ascb.o $(BUILD)/BigInteger.o $(BUILD)/BigUnsigned.o \
	$(BUILD)/BigIntegerUtils.o $(BUILD)/BigUnsignedInABase.o $(BUILD)/BigIntegerAlgorithms.o && \
	ar rcs $(BIN)/libs/libspl.a $(BUILD)/spl.o $(BUILD)/alt.o $(BUILD)/idw.o $(BUILD)/nwt.o $(BUILD)/env.o $(BUILD)/bloom.o \
	$(BUILD)/alm.o $(BUILD)/alm1.o $(BUILD)/alm2.o $(BUILD)/cityhash.o $(BUILD)/arpa.o $(BUILD)/dict.o $(BUILD)/dumper.o \
	$(BUILD)/stemmer.o $(BUILD)/hnsw.o $(BUILD)/ablm.o $(BUILD)/python.o $(BUILD)/toolkit.o $(BUILD)/collector.o \
	$(BUILD)/tokenizer.o $(BUILD)/alphabet.o $(BUILD)/levenshtein.o $(BUILD)/progress.o $(BUILD)/ascb.o $(BUILD)/BigInteger.o \
	$(BUILD)/BigUnsigned.o $(BUILD)/BigIntegerUtils.o $(BUILD)/BigUnsignedInABase.o $(BUILD)/BigIntegerAlgorithms.o && \
	$(CC) $(BUILD)/main.o -L$(BIN)/libs -Wl,-rpath,$(BIN)/libs -lspl $(LIBS) -o $(BIN)/$(NAME) && \
	rm -rf $(BUILD)

# Правила сборки под Dev
dev:
	mkdir -p $(BUILD) && \
	mkdir -p $(BIN)/libs && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./app/asc.cpp -o $(BUILD)/main.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./src/alt.cpp -o $(BUILD)/alt.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./src/spl.cpp -o $(BUILD)/spl.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./src/dict.cpp -o $(BUILD)/dict.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./src/hnsw.cpp -o $(BUILD)/hnsw.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./src/ascb.cpp -o $(BUILD)/ascb.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./src/bloom.cpp -o $(BUILD)/bloom.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./src/dumper.cpp -o $(BUILD)/dumper.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./src/stemmer.cpp -o $(BUILD)/stemmer.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./alm/src/idw.cpp -o $(BUILD)/idw.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./alm/src/nwt.cpp -o $(BUILD)/nwt.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./alm/src/env.cpp -o $(BUILD)/env.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./alm/src/alm.cpp -o $(BUILD)/alm.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./alm/src/alm1.cpp -o $(BUILD)/alm1.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./alm/src/alm2.cpp -o $(BUILD)/alm2.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./alm/src/arpa.cpp -o $(BUILD)/arpa.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./alm/src/ablm.cpp -o $(BUILD)/ablm.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./alm/src/python.cpp -o $(BUILD)/python.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./alm/src/toolkit.cpp -o $(BUILD)/toolkit.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./alm/src/alphabet.cpp -o $(BUILD)/alphabet.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./alm/src/progress.cpp -o $(BUILD)/progress.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./alm/src/tokenizer.cpp -o $(BUILD)/tokenizer.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./alm/src/collector.cpp -o $(BUILD)/collector.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./alm/src/levenshtein.cpp -o $(BUILD)/levenshtein.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./alm/contrib/src/cityhash/city.cc -o $(BUILD)/cityhash.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./alm/contrib/src/bigint/BigInteger.cc -o $(BUILD)/BigInteger.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./alm/contrib/src/bigint/BigUnsigned.cc -o $(BUILD)/BigUnsigned.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./alm/contrib/src/bigint/BigIntegerUtils.cc -o $(BUILD)/BigIntegerUtils.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./alm/contrib/src/bigint/BigUnsignedInABase.cc -o $(BUILD)/BigUnsignedInABase.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./alm/contrib/src/bigint/BigIntegerAlgorithms.cc -o $(BUILD)/BigIntegerAlgorithms.o && \
	ar rcs $(BIN)/libs/libspl.a $(BUILD)/spl.o $(BUILD)/alt.o $(BUILD)/idw.o $(BUILD)/nwt.o $(BUILD)/env.o $(BUILD)/bloom.o \
	$(BUILD)/alm.o $(BUILD)/alm1.o $(BUILD)/alm2.o $(BUILD)/cityhash.o $(BUILD)/arpa.o $(BUILD)/dict.o $(BUILD)/dumper.o \
	$(BUILD)/stemmer.o $(BUILD)/hnsw.o $(BUILD)/ablm.o $(BUILD)/python.o $(BUILD)/toolkit.o $(BUILD)/collector.o \
	$(BUILD)/tokenizer.o $(BUILD)/alphabet.o $(BUILD)/levenshtein.o $(BUILD)/progress.o $(BUILD)/ascb.o $(BUILD)/BigInteger.o \
	$(BUILD)/BigUnsigned.o $(BUILD)/BigIntegerUtils.o $(BUILD)/BigUnsignedInABase.o $(BUILD)/BigIntegerAlgorithms.o && \
	$(CC) $(DEBUG) $(BUILD)/main.o -L$(BIN)/libs -Wl,-rpath,$(BIN)/libs -lspl $(LIBS) -o $(BIN)/$(NAME) && \
	rm -rf $(BUILD)

# Правила статической сборки
static:
	mkdir -p $(BUILD) && \
	mkdir -p $(BIN)/libs && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./app/asc.cpp -o $(BUILD)/main.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/alt.cpp -o $(BUILD)/alt.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/spl.cpp -o $(BUILD)/spl.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/dict.cpp -o $(BUILD)/dict.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/hnsw.cpp -o $(BUILD)/hnsw.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/ascb.cpp -o $(BUILD)/ascb.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/bloom.cpp -o $(BUILD)/bloom.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/dumper.cpp -o $(BUILD)/dumper.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/stemmer.cpp -o $(BUILD)/stemmer.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/idw.cpp -o $(BUILD)/idw.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/nwt.cpp -o $(BUILD)/nwt.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/env.cpp -o $(BUILD)/env.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/alm.cpp -o $(BUILD)/alm.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/alm1.cpp -o $(BUILD)/alm1.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/alm2.cpp -o $(BUILD)/alm2.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/arpa.cpp -o $(BUILD)/arpa.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/ablm.cpp -o $(BUILD)/ablm.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/python.cpp -o $(BUILD)/python.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/toolkit.cpp -o $(BUILD)/toolkit.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/alphabet.cpp -o $(BUILD)/alphabet.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/progress.cpp -o $(BUILD)/progress.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/tokenizer.cpp -o $(BUILD)/tokenizer.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/collector.cpp -o $(BUILD)/collector.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/levenshtein.cpp -o $(BUILD)/levenshtein.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/contrib/src/cityhash/city.cc -o $(BUILD)/cityhash.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/contrib/src/bigint/BigInteger.cc -o $(BUILD)/BigInteger.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/contrib/src/bigint/BigUnsigned.cc -o $(BUILD)/BigUnsigned.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/contrib/src/bigint/BigIntegerUtils.cc -o $(BUILD)/BigIntegerUtils.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/contrib/src/bigint/BigUnsignedInABase.cc -o $(BUILD)/BigUnsignedInABase.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/contrib/src/bigint/BigIntegerAlgorithms.cc -o $(BUILD)/BigIntegerAlgorithms.o && \
	ar rcs $(BIN)/libs/libspl.a $(BUILD)/spl.o $(BUILD)/alt.o $(BUILD)/idw.o $(BUILD)/nwt.o $(BUILD)/env.o $(BUILD)/bloom.o \
	$(BUILD)/alm.o $(BUILD)/alm1.o $(BUILD)/alm2.o $(BUILD)/cityhash.o $(BUILD)/arpa.o $(BUILD)/dict.o $(BUILD)/dumper.o \
	$(BUILD)/stemmer.o $(BUILD)/hnsw.o $(BUILD)/ablm.o $(BUILD)/python.o $(BUILD)/toolkit.o $(BUILD)/collector.o \
	$(BUILD)/tokenizer.o $(BUILD)/alphabet.o $(BUILD)/levenshtein.o $(BUILD)/progress.o $(BUILD)/ascb.o $(BUILD)/BigInteger.o \
	$(BUILD)/BigUnsigned.o $(BUILD)/BigIntegerUtils.o $(BUILD)/BigUnsignedInABase.o $(BUILD)/BigIntegerAlgorithms.o && \
	$(CC) $(BUILD)/main.o -L$(BIN)/libs -Wl,-rpath,$(BIN)/libs -lspl $(LIBS) -o $(BIN)/$(NAME) && \
	rm -rf $(BUILD)

py:
	mkdir -p $(BIN) && \
	mkdir -p $(BUILD)/pybind11 && \
	cp -rf ./python/pybind11/* $(BUILD)/pybind11/ && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/spl.cpp -o $(BUILD)/pybind11/spl.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/alt.cpp -o $(BUILD)/pybind11/alt.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/dict.cpp -o $(BUILD)/pybind11/dict.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/hnsw.cpp -o $(BUILD)/pybind11/hnsw.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/ascb.cpp -o $(BUILD)/pybind11/ascb.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/bloom.cpp -o $(BUILD)/pybind11/bloom.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/dumper.cpp -o $(BUILD)/pybind11/dumper.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/stemmer.cpp -o $(BUILD)/pybind11/stemmer.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/app/alm.cpp -o $(BUILD)/pybind11/main.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/idw.cpp -o $(BUILD)/pybind11/idw.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/nwt.cpp -o $(BUILD)/pybind11/nwt.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/env.cpp -o $(BUILD)/pybind11/env.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/alm.cpp -o $(BUILD)/pybind11/alm.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/alm1.cpp -o $(BUILD)/pybind11/alm1.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/alm2.cpp -o $(BUILD)/pybind11/alm2.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/arpa.cpp -o $(BUILD)/pybind11/arpa.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/ablm.cpp -o $(BUILD)/pybind11/ablm.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/ascb.cpp -o $(BUILD)/pybind11/ascb.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/python.cpp -o $(BUILD)/pybind11/python.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/toolkit.cpp -o $(BUILD)/pybind11/toolkit.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/alphabet.cpp -o $(BUILD)/pybind11/alphabet.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/progress.cpp -o $(BUILD)/pybind11/progress.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/tokenizer.cpp -o $(BUILD)/pybind11/tokenizer.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/collector.cpp -o $(BUILD)/pybind11/collector.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/src/levenshtein.cpp -o $(BUILD)/pybind11/levenshtein.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/contrib/src/cityhash/city.cc -o $(BUILD)/pybind11/cityhash.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/contrib/src/bigint/BigInteger.cc -o $(BUILD)/pybind11/BigInteger.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/contrib/src/bigint/BigUnsigned.cc -o $(BUILD)/pybind11/BigUnsigned.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/contrib/src/bigint/BigIntegerUtils.cc -o $(BUILD)/pybind11/BigIntegerUtils.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/contrib/src/bigint/BigUnsignedInABase.cc -o $(BUILD)/pybind11/BigUnsignedInABase.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./alm/contrib/src/bigint/BigIntegerAlgorithms.cc -o $(BUILD)/pybind11/BigIntegerAlgorithms.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c $(BUILD)/pybind11/palm.cxx -o $(BUILD)/pybind11/palm.o && \
	$(CC) $(LIBS) -shared -fPIC -o $(BIN)/$(NAME).so $(BUILD)/pybind11/palm.o $(BUILD)/pybind11/spl.o $(BUILD)/pybind11/alt.o \
	$(BUILD)/pybind11/idw.o $(BUILD)/pybind11/dict.o $(BUILD)/pybind11/dumper.o $(BUILD)/pybind11/hnsw.o $(BUILD)/pybind11/bloom.o \
	$(BUILD)/pybind11/stemmer.o $(BUILD)/pybind11/nwt.o $(BUILD)/pybind11/env.o $(BUILD)/pybind11/arpa.o $(BUILD)/pybind11/ablm.o \
	$(BUILD)/pybind11/python.o $(BUILD)/pybind11/toolkit.o $(BUILD)/pybind11/alm.o $(BUILD)/pybind11/alm1.o $(BUILD)/pybind11/alm2.o \
	$(BUILD)/pybind11/tokenizer.o $(BUILD)/pybind11/alphabet.o $(BUILD)/pybind11/levenshtein.o $(BUILD)/pybind11/collector.o \
	$(BUILD)/pybind11/progress.o $(BUILD)/pybind11/ascb.o $(BUILD)/pybind11/cityhash.o $(BUILD)/pybind11/BigInteger.o \
	$(BUILD)/pybind11/BigUnsigned.o $(BUILD)/pybind11/BigIntegerUtils.o $(BUILD)/pybind11/BigUnsignedInABase.o $(BUILD)/pybind11/BigIntegerAlgorithms.o && \
	rm -rf $(BUILD)

# Правило очистки
clean:
	rm -rf $(BIN) && rm -rf $(BUILD)
