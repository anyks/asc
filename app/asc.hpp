/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

// Название языковой модели
#define ANYKS_ASC_NAME "asc"
// Версия приложения
#define ANYKS_ASC_VERSION "1.2.6"
// Версия словаря
#define ANYKS_ASC_DICT_VERSION "1.0.1"
// Автор приложения
#define ANYKS_ASC_AUTHOR "Yuriy Lobarev"
// Телефон автора
#define ANYKS_ASC_PHONE "+7 (910) 983-95-90"
// Телеграм-контакт
#define ANYKS_ASC_CONTACT "@forman"
// Адрес электронной почты
#define ANYKS_ASC_EMAIL "forman@anyks.com"
// Адрес сайта автора
#define ANYKS_ASC_SITE "https://anyks.com"

/** ПАРАМЕТРЫ СЛОВ **/
// Неизвестный вариант
#define NOTFOUND L"-"
// Максимальная длина текста для обработки
#define MAX_LENGTH_TEXT 1024
// Максимально-возможный вес слова
#define MAX_WORD_WEIGHT 1e+20f
// Минимально-возможный вес слова
#define MIN_WORD_WEIGHT -1e+20f

/** ПАРАМЕТРЫ NSWLIB **/
// Тип алгоритма nswlib
#define NSWLIB_TYPE "l2"
// Количество элементов к выдачи
#define NSWLIB_COUNT 30000
// Точность nswlib
#define NSWLIB_ACCURACY 10
