/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef __ANYKS_SPELLCHECKER__
#define __ANYKS_SPELLCHECKER__

/**
 * Стандартная библиотека
 */
#include <set>
#include <ctime>
#include <bitset>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
/**
 * Наши модули
 */
#include <dict.hpp>

// Активируем стандартное пространство имён
using namespace std;

/**
 * anyks пространство имён
 */
namespace anyks {
	/**
	 * ASpell Класс спеллчекера
	 */
	typedef class ASpell {
		private:
			/**
			 * Cache Класс кэша последовательности
			 */
			typedef class Cache {
				private:
					// Размер N-граммы
					u_short size;
					// Список идентификаторов слов
					vector <size_t> seq;
					// Список слов
					vector <wstring> words;
				public:
					/**
					 * pop Метод удаления первого слова
					 */
					void pop() noexcept;
					/**
					 * clear Метод очистки данных
					 */
					void clear() noexcept;
					/**
					 * push Метод добавления слова в список
					 * @param idw  идентификатор слова
					 * @param word слово для добавления
					 */
					void push(const size_t idw, const wstring & word) noexcept;
				public:
					/**
					 * empty Метод проверки на сущестование данных
					 * @return результат проверки
					 */
					const bool empty() const noexcept;
					/**
					 * gets Метод получения списка идентификаторов слов
					 * @return список идентификаторов слов
					 */
					const vector <size_t> & gets() const noexcept;
					/**
					 * getw Метод получения списка слов
					 * @return список слов
					 */
					const vector <wstring> & getw() const noexcept;
				public:
					/**
					 * Cache Конструктор
					 * @param size размер N-граммы
					 */
					Cache(const u_short size = 3) noexcept: size(size) {};
					/**
					 * ~Cache Деструктор
					 */
					~Cache() noexcept;
			} cache_t;
		public:
			// Основные опции
			enum class options_t : u_short {
				debug,   // Флаг режима отладки
				alter,   // Флаг разрешающий исправлять слова с альтернативными буквами
				split,   // Флаг разрешающий сплит слов
				esplit,  // Флаг разрешающий сплит слов с ошибками
				rsplit,  // Флаг разрешающий удалять сплит слов
				uppers,  // Флаг разрешающий проставлять регистры букв в словах
				hyphen,  // Флаг разрешающий выполнять сплиты слов по дефисам
				skipupp, // Флаг разрешающий пропускать слова в верхнем регистре
				skiplat, // Флаг разрешающий пропускать слова содержащие латиницу
				skiphyp, // Флаг разрешающий пропускать слова содержащие дефисы
				wordrep  // Флаг разрешающий удаление одинаковых слов следующих друг за другом
			};
		private:
			/**
			 * Seq структура параметров слова
			 */
			typedef struct Seq {
				size_t idw;
				wstring word;
				wstring orig;
			} seq_t;
			/**
			 * Тип возвращаемого значения метода backword
			 */
			typedef pair <size_t, pair <wstring, wstring>> backwrd_t;
		private:
			// Объект языковой модели
			const alm_t * alm = nullptr;
			// Объект словаря
			const dict_t * dict = nullptr;
			// Объект алфавита
			const alphabet_t * alphabet = nullptr;
			// Объект log файла
			mutable const char * logfile = nullptr;
			// Объект токенизатора
			const tokenizer_t * tokenizer = nullptr;
		private:
			/**
			 * idw Метод получения идентификатора слова
			 * @param word слово для которого нужно получить идентификатор
			 * @return     сгенерированный идентификатор слова
			 */
			const size_t idw(const wstring & word) const noexcept;
			/**
			 * isOption Метод проверки наличия опции
			 * @param option  опция для проверки
			 * @param options опции приложения
			 * @return        результат проверки
			 */
			const bool isOption(const options_t option, const u_short options) const noexcept;
			/**
			 * backword Метод формирования слова с пробелами
			 * @param idw     идентификатор слова
			 * @param word    слово для добавления
			 * @param options опции приложения
			 * @param pos1    первая основная позиция добавления слова
			 * @param pos2    вторая обратная позиция для добавления получившегося слова
			 * @param words   список собранных слов
			 * @return        полученное слово
			 */
			const backwrd_t backword(const size_t idw, const wstring & word, const u_short options, size_t & pos1, size_t & pos2, vector <wstring> & words) const noexcept;
		public:
			/**
			 * check Метод проверки существования слова
			 * @param word слово для проверки
			 * @return     результат проверки
			 */
			const bool check(const wstring & word) const  noexcept;
			/**
			 * setOption Метод установки опций модуля
			 * @param option  опция для установки
			 * @param options опции приложения
			 * @return        список установленных опций
			 */
			const u_short setOption(const options_t option, const u_short options = 0) const noexcept;
			/**
			 * unsetOption Метод удаления опций модуля
			 * @param option  опция для установки
			 * @param options опции приложения
			 * @return        список оставшихся опций
			 */
			const u_short unsetOption(const options_t option, const u_short options = 0) const noexcept;
		public:
			/**
			 * split Метод выполнения сплита слова на слова
			 * @param text    текст для сплита
			 * @param options опции приложения
			 * @param words   список проспличенных вариантов
			 */
			void split(const wstring & text, const u_short options, vector <wstring> & words) const noexcept;
			/**
			 * erratum Метод поиска в тексте опечаток
			 * @param text    текст для сборки данных
			 * @param options опции приложения
			 * @param words   список слов в которых найдены ошибки
			 */
			void erratum(const wstring & text, const u_short options, vector <wstring> & words) const noexcept;
			/**
			 * hyphen Метод выполнения сплита слов по дефисам на слова
			 * @param text    текст для сплита
			 * @param options опции приложения
			 * @param seq     список последовательности
			 * @param words   список проспличенных вариантов
			 */
			void hyphen(const wstring & text, const u_short options, vector <size_t> & seq, vector <wstring> & words) const noexcept;
			/**
			 * spell Метод исправления опечаток в большом тексте
			 * @param text    текст для корректировки
			 * @param options опции приложения
			 * @param info    список слов для которых выполнена коррекция
			 */
			void spell(wstring & text, const u_short options, vector <vector <pair <wstring, wstring>>> * info = nullptr) const noexcept;
			/**
			 * analyze Метод проведения анализа текста
			 * @param text    текст для проведения анализа
			 * @param options опции приложения
			 * @param info    список слов для которых выполнена коррекция
			 */
			void analyze(const wstring & text, const u_short options, vector <unordered_map <wstring, set <wstring>>> & info) const noexcept;
		public:
			/**
			 * setAlm Метод установки языковой модели
			 * @param alm объект языковой модели
			 */
			void setAlm(const alm_t * alm) noexcept;
			/**
			 * setDict Метод установки словаря
			 * @param dict объект словаря
			 */
			void setDict(const dict_t * dict) noexcept;
			/**
			 * setLogfile Метод установки файла для вывода логов
			 * @param logifle адрес файла для вывода отладочной информации
			 */
			void setLogfile(const char * logfile) noexcept;
			/**
			 * setAlphabet Метод установки словаря
			 * @param alphabet объект алфавита словаря
			 */
			void setAlphabet(const alphabet_t * alphabet) noexcept;
			/**
			 * setTokenizer Метод установки токенизатора
			 * @param tokenizer объект токенизатора
			 */
			void setTokenizer(const tokenizer_t * tokenizer) noexcept;
		public:
			/**
			 * ASpell Конструктор
			 */
			ASpell() noexcept {};
			/**
			 * ASpell Конструктор
			 * @param dict объект словаря
			 */
			ASpell(const dict_t * dict) noexcept;
			/**
			 * ASpell Конструктор
			 * @param dict объект словаря
			 * @param alm  объект языковой модели
			 */
			ASpell(const dict_t * dict, const alm_t * alm) noexcept;
			/**
			 * ASpell Конструктор
			 * @param dict      объект словаря
			 * @param alm       объект языковой модели
			 * @param alphabet  объект алфавита
			 * @param tokenizer объект токенизатора
			 */
			ASpell(const dict_t * dict, const alm_t * alm, const alphabet_t * alphabet, const tokenizer_t * tokenizer) noexcept;
	} spell_t;
};

#endif // __ANYKS_SPELLCHECKER__
