set(CMAKE_FIND_USE_SYSTEM_ENVIRONMENT_PATH FALSE)
if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(CMAKE_FIND_LIBRARY_PREFIXES "lib")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
endif()

# Поиск пути к заголовочным файлам
find_path(ALM_INCLUDE_DIR alm.hpp PATHS ${CMAKE_SOURCE_DIR}/third_party/include/alm/include NO_DEFAULT_PATH)
# Поиск библиотеки AWH
find_library(ALM_LIBRARY NAMES alm PATHS ${CMAKE_SOURCE_DIR}/third_party/lib NO_DEFAULT_PATH)

# Подключаем 'FindPackageHandle' для использования модуля поиска (find_package(<PackageName>))
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ALM REQUIRED_VARS
    ALM_LIBRARY
    ALM_INCLUDE_DIR

    FAIL_MESSAGE "Missing ALM. Run ./build_third_party.sh first"
)
