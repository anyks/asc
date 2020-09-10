/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef __ANYKS_SCBIN__
#define __ANYKS_SCBIN__

/**
 * Стандартная библиотека
 */
#include <set>
#include <map>
#include <ctime>
#include <string>
#include <vector>
#include <bitset>
#include <cstring>
#include <functional>
#include <unordered_map>
/**
 * Наши модули
 */
#include <alm.hpp>
#include <dict.hpp>
#include <aspl.hpp>
#include <fsys.hpp>
#include <toolkit.hpp>
#include <alphabet.hpp>
#include <progress.hpp>
#include <tokenizer.hpp>

// Устанавливаем область видимости
using namespace std;
// Активируем пространство имён json
using json = nlohmann::json;

/**
 * anyks пространство имён
 */
namespace anyks {
	/**
	 * ASCb Класс бинарного словаря
	 */
	typedef class ASCb {
		private:
			// Режим отладки
			u_short debug = 0;
			// Флаг работы ALMv2
			bool ALMv2 = false;
			// Тип шифрования словаря
			aspl_t::types_t aes = aspl_t::types_t::aes128;
		private:
			// Адрес файла словаря
			string filename = "";
			// Пароль шифрования словаря
			string password = "";
			// Системная локаль приложения
			string locale = ALPHABET_LOCALE;
		private:
			// Список альтернативных букв
			map <wchar_t, wchar_t> altLetters;
			// Список альтернативных слов
			unordered_map <string, string> altWords;
			// Функция прогресс бара
			function <void (const wstring &, const u_short)> progressFn = nullptr;
		private:
			// Объект языковой модели
			alm_t * alm = nullptr;
			// Объект бинарного контейнера
			aspl_t * aspl = nullptr;
			// Объект словаря спеллчекера
			dict_t * dict = nullptr;
			// Прогресс бар
			progress_t * pss = nullptr;
			// Объект словаря
			alphabet_t * alphabet = nullptr;
			// Объект токенизатора
			tokenizer_t * tokenizer = nullptr;
		private:
			// Объект log файла
			const char * logfile = nullptr;
		public:
			/**
			 * read Метод чтения данных из файла в словарь
			 */
			void read() noexcept;
			/**
			 * write Метод записи данных словаря в файл
			 */
			void write() const noexcept;
		public:
			/**
			 * Метод инициализации
			 */
			void init();
			/**
			 * clear Метод очистки данных словаря
			 */
			void clear() noexcept;
			/**
			 * info Метод вывода инормационных данных словаря
			 */
			void info() const noexcept;
		public:
			/**
			 * addAlt Метод добавления альтернативной буквы
			 * @param letter буква для добавления
			 * @param alt    альтернативная буква для добавления
			 */
			void addAlt(const wchar_t letter, const wchar_t alt) noexcept;
			/**
			 * addAlt Метод добавления альтернативного слова
			 * @param word слово для добавления
			 * @param alt  альтернативное слово для добавления
			 */
			void addAlt(const string & word, const string & alt) noexcept;
		public:
			/**
			 * setAlm Метод установки объекта языковой модели
			 * @param alm объект языковой модели для установки
			 */
			void setAlm(alm_t * alm) noexcept;
			/**
			 * setDict Метод установки объекта словаря
			 * @param dict объект словаря для установки
			 */
			void setDict(dict_t * dict) noexcept;
			/**
			 * setAES Метод установки размера шифрования
			 * @param aes размер шифрования для установки
			 */
			void setAES(aspl_t::types_t aes) noexcept;
			/**
			 * setALMvType Метод установки типа ALM
			 * @param type тип версии ALM
			 */
			void setALMvType(const bool type) noexcept;
			/**
			 * setProgress Метод установки объекта прогресс-бара
			 * @param pss объект прогресс-бара
			 */
			void setProgress(progress_t * pss) noexcept;
			/**
			 * setDebug Метод установки флага отладки
			 * @param debug устанавливаемый флаг отладки
			 */
			void setDebug(const u_short debug) noexcept;
			/**
			 * setLocale Метод установки локали приложения
			 * @param locale локаль приложения для установки
			 */
			void setLocale(const string & locale) noexcept;
			/**
			 * setLogfile Метод установки файла для вывода логов
			 * @param logifle адрес файла для вывода отладочной информации
			 */
			void setLogfile(const char * logfile) noexcept;
			/**
			 * setAlphabet Метод установки объекта словаря
			 * @param alphabet объект словаря для установки
			 */
			void setAlphabet(alphabet_t * alphabet) noexcept;
			/**
			 * setPassword Метод установки пароля словаря
			 * @param password пароль словаря для установки
			 */
			void setPassword(const string & password) noexcept;
			/**
			 * setTokenizer Метод установки токенизатора
			 * @param tokenizer объект токенизатора
			 */
			void setTokenizer(tokenizer_t * tokenizer) noexcept;
			/**
			 * setFilename Метод установки адреса файла словаря
			 * @param filename адрес файла словаря
			 */
			void setFilename(const string & filename) noexcept;
			/**
			 * setProgressFn Метод установки внешнего прогресс-бара
			 * @param fn функция внешнего прогресс-бара
			 */
			void setProgressFn(function <void (const wstring &, const u_short)> fn) noexcept;
		public:
			/**
			 * ASCb Конструктор
			 */
			ASCb() noexcept;
			/**
			 * ASCb Конструктор
			 * @param filename адрес файла словаря
			 * @param password пароль бинарного словаря
			 * @param logifle  адрес файла для вывода отладочной информации
			 */
			ASCb(const string & filename, const string & password = "", const char * logfile = nullptr) noexcept;
			/**
			 * ~ASCb Деструктор
			 */
			~ASCb() noexcept;
	} ascb_t;
};

#endif // __ANYKS_SCBIN__
