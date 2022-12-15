#!/usr/bin/env bash

# Получаем корневую дирректорию
readonly ROOT=$(cd "$(dirname "$0")" && pwd)

# Запоминаем комманду
CMD=$1
# Получаем версию OS
OS=$(uname -a | awk '{print $1}')

if [[ $OS =~ "MINGW64" ]]; then
	OS="Windows"
fi

if [ -n "$CMD" ]; then
	if [ $CMD = "--clean" ] || [ $CMD = "--reset" ]; then
		# Очистка подпроекта
		clean_submodule(){
			cd "$ROOT/submodules/$1" || exit 1
			git clean -dfx
			git stash
			cd "$ROOT" || exit 1
		}

		# Очистка директории
		clean_directory(){
			git clean -dfx "$1"
		}

		if [ $CMD = "--reset" ]; then
			clean_submodule "alm"
			"$ROOT/submodules/alm/build_third_party.sh" --clean
		else
			rm -f "$ROOT/submodules/alm/.stamp_done"
			rm -rf "$ROOT/submodules/alm/build"
		fi

		rm -rf "$ROOT/third_party"

		printf "\n****************************************"
		printf "\n************   Success!!!   ************"
		printf "\n****************************************"
		printf "\n\n\n"

		exit 0
	elif [ $CMD != "--update" ]; then
		printf "Usage: config [options]\n"
		printf " --clean - Cleaning all submodules and build directory\n"

		exit 1
	fi
fi

# Устанавливаем систему сборки
if [[ $OS = "Windows" ]]; then
	export BUILD="mingw32-make"
else
	export BUILD="make"
fi

# Каталог для установки собранных библиотек
PREFIX="$ROOT/third_party"
export PKG_CONFIG_PATH="$PREFIX/lib/pkgconfig"

# Устанавливаем флаги глобального использования
# export CPPFLAGS=""
export CXXFLAGS="$CXXFLAGS -fPIC"
export CFLAGS="-I$PREFIX/include -fPIC"

export LDFLAGS="-L$PREFIX/lib"
export LD_LIBRARY_PATH="$PREFIX/lib"

# Создаём каталоги
mkdir -p "$PREFIX/bin"
mkdir -p "$PREFIX/lib"
mkdir -p "$PREFIX/include"

# Определяем количество логических ядер
if [ $OS = "Darwin" ]; then
	numproc=$(sysctl -n hw.logicalcpu)
elif [ $OS = "FreeBSD" ]; then
	numproc=$(sysctl -n hw.ncpu)
else
	numproc=$(nproc)
fi

# Применяем патчи
apply_patch(){
	patch="$ROOT/patches/$1/$2"
	if ! git apply --reverse --check "$patch" 2> /dev/null; then
		echo "applaying patch $patch"
		git apply "$patch" || exit 1
	else
		echo "patch $patch already applied"
	fi
}

if [[ $CMD != "--update" ]]; then
	# Выполняем синхронизацию сабмодулей
	git submodule sync
	# Инициализируем подпроекты
	git submodule update --init --recursive
fi

# Сборка ALM
src="$ROOT/submodules/alm"
if [ ! -f "$src/.stamp_done" ]; then
	printf "\n****** ANYKS LANGUAGE MODEL ******\n"
	cd "$src" || exit 1

	if [[ $CMD = "--update" ]]; then
		# Выполняем обновление репозитория
		git pull origin master
	else
		# Выполняем сборку зависимостей
		./build_third_party.sh
	fi

	# Создаём каталог сборки
	mkdir -p "build" || exit 1
	# Переходим в каталог
	cd "build" || exit 1

	# Удаляем старый файл кэша
	rm -rf "$ROOT/build/CMakeCache.txt"

	# Выполняем конфигурацию проекта
	if [[ $OS = "Windows" ]]; then
		cmake \
		 -DCMAKE_BUILD_TYPE=Release \
		 -DCMAKE_SYSTEM_NAME=Windows \
		 -DCMAKE_INSTALL_PREFIX="$PREFIX" \
		 -G "MinGW Makefiles" \
		 .. || exit 1
	else
		cmake \
		 -DCMAKE_BUILD_TYPE=Release \
		 -DCMAKE_INSTALL_PREFIX="$PREFIX" \
		 .. || exit 1
	fi

	# Выполняем сборку на всех логических ядрах
	$BUILD -j"$numproc" || exit 1
	# Выполняем установку проекта
	$BUILD install || exit 1

	# Помечаем флагом, что сборка и установка произведена
	touch "$src/.stamp_done"
	cd "$ROOT" || exit 1
fi

# Переименовываем расширение библиотек для Windows
if [ $OS = "Windows" ]; then # Windows
	for i in $(ls "$PREFIX/lib" | grep .a$);
	do
		mv "$PREFIX/lib/$i" "$PREFIX/lib/$(basename "$i" .a).lib"
	done
fi

printf "\n****************************************"
printf "\n************   Success!!!   ************"
printf "\n****************************************"
printf "\n\n\n"
