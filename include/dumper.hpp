/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef __ANYKS_DUMPER__
#define __ANYKS_DUMPER__

/**
 * Стандартная библиотека
 */
#include <map>
#include <list>
#include <bitset>
#include <string>
#include <vector>
#include <functional>
/**
 * Наши модули
 */
#include <alm.hpp>
#include <alt.hpp>
#include <app/asc.hpp>

// Активируем стандартное пространство имён
using namespace std;

/**
 * anyks пространство имён
 */
namespace anyks {
	/**
	 * Класс модуля сборки гипотез
	 */
	typedef class Dumper {
		public:
			// Основные опции
			enum class options_t : u_short {
				debug, // Флаг режима отладки
				alter, // Флаг разрешающий исправлять слова с альтернативными буквами
				uppers // Флаг разрешающий проставлять регистры букв в словах
			};
			/**
			 * Status структура статуса получения контекста
			 */
			typedef struct CStatus {
				size_t oclet;    // Количество одиночных букв следующих друг за другом.
				size_t bilet;    // Количество двухбуквенных слов следующих друг за другом
				size_t words;    // Количество слов в варианте
				size_t letters;  // Количество букв в варианте
				size_t maxwords; // Количество коротких слов
				size_t minwords; // Количество длинных слов
				double estimate; // Предварительная оценка
				/**
				 * CStatus Конструктор
				 */
				CStatus() : oclet(0), bilet(0), words(0), letters(0), maxwords(0), minwords(0), estimate(0.0) {}
			} __attribute__((packed)) cstatus_t;
			/**
			 * Awrd структура гипотезы слова
			 */
			typedef struct Awrd {
				bool best;      // Слово является наилучшим
				bool split;     // Слово является проспличенным
				bool ngram;     // N-грамма существует
				double tmo;     // Значение Танимото
				double wltf;    // Идентификатор слова
				u_short lev;    // Дистанция Дамерау-Левенштейна
				size_t idw;     // Идентификатор слова
				word_t word;    // Текущее слово
				wstring etalon; // Оригинальное слово
				/**
				 * Awrd Конструктор
				 */
				Awrd() : best(false), split(false), ngram(false), tmo(0.0), wltf(0.0), lev(MAX_LENGTH_TEXT), idw(idw_t::NIDW), word(L""), etalon(L"") {}
			} awrd_t;
		private:
			// Объект языковой модели
			const alm_t * alm;
			// Объект альтернативных слов
			const alt_t * alt;
			// Объект алфавита
			const alphabet_t * alphabet;
			// Объект log файла
			mutable const char * logfile;
			// Объект токенизатора
			const tokenizer_t * tokenizer;
		private:
			// Характеристики словаря
			size_t ad, cw;
			// Флаги параметров
			bitset <3> options;
			// Список промаркированных вариантов
			map <size_t, size_t> marks;
			// Список собранных вариантов
			mutable map <size_t, awrd_t> variants;
			// Список собранных последовательностей
			map <size_t, vector <size_t>> sequence;
			// Список цепочки последовательности
			map <size_t, vector <awrd_t>> hypseseq;
			// Список связей контекста
			map <size_t, vector <const awrd_t *>> links;
			// Список собранных гипотез
			map <size_t, map <size_t, awrd_t>> hypotheses;
			// Список лучших кандидатов
			mutable map <size_t, const awrd_t *> candidates;
		private:
			/**
			 * isOption Метод проверки наличия опции
			 * @param option опция для проверки
			 * @return       результат проверки
			 */
			const bool isOption(const options_t option) const noexcept;
		public:
			/**
			 * clear Метод очистки собранных данных
			 */
			void clear() noexcept;
			/**
			 * empty Метод проверки, является ли пустым блок с данными
			 * @return результат проверки
			 */
			const bool empty() const noexcept;
			/**
			 * best Метод получения лучшего результата
			 * @return наилучший вариант по мнению системы
			 */
			const pair <size_t, const awrd_t *> best() const noexcept;
			/**
			 * bests Метод вывода списка лучших кандидатов
			 * @return список лучших кандидатов
			 */
			const map <size_t, const awrd_t *> & bests() const noexcept;
			/**
			 * getSequence Метод извлечения собранной последовательности
			 * @param ids идентификатор последовательности
			 * @return    собранная последовательность
			 */
			const vector <size_t> & getSequence(const size_t ids) const noexcept;
			/**
			 * get Метод вывода списка всех кандидатов
			 * @param size размер последовательности
			 * @return     список всех собранных кандидатов
			 */
			const map <size_t, awrd_t> & get(const size_t size = 0) const noexcept;
		public:
			/**
			 * word Метод вывода слова из гипотезы
			 * @param hyps гипотеза из которой нужно сформировать слово
			 * @return     слово сформированное из гипотезы
			 */
			const wstring word(const pair <size_t, const awrd_t *> & hyps) const noexcept;
			/**
			 * ctx Метод сборки контекста из гипотезы
			 * @param hyps  гипотеза для сборки текста
			 * @param words список полученных слов
			 * @param info  список слов для которых выполнена коррекция
			 * @return      количество символов в обработанных словах и рейтинг варианта
			 */
			const cstatus_t ctx(const pair <size_t, const awrd_t *> & hyps, vector <wstring> & words, vector <pair <wstring, wstring>> * info = nullptr) const noexcept;
		public:
			/**
			 * alter Метод восстановления альтернативного слова
			 * @param hypothesis вариант гипотезы для обработки
			 * @param seq        последовательность для проверки
			 */
			void alter(const awrd_t * hypothesis, vector <size_t> & seq) const noexcept;
			/**
			 * ctxIt Метод перебора последовательно в обратном направлении всех вариантов последовательности
			 * @param hyps     гипотеза для переборки
			 * @param callback функция обратного вызова
			 */
			void ctxIt(const pair <size_t, const awrd_t *> & hyps, function <void (const size_t, const awrd_t *)> callback) const noexcept;
		public:
			/**
			 * setAlm Метод установки языковой модели
			 * @param alm объект языковой модели
			 */
			void setAlm(const alm_t * alm) noexcept;
			/**
			 * setAlt Метод установки объекта альтернативных вариантов
			 * @param alt объект альтернативных вариантов
			 */
			void setAlt(const alt_t * alt) noexcept;
			/**
			 * setLogfile Метод установки файла для вывода логов
			 * @param logifle адрес файла для вывода отладочной информации
			 */
			void setLogfile(const char * logfile) noexcept;
			/**
			 * setOption Метод установки опций модуля
			 * @param option опция для установки
			 */
			void setOption(const options_t option) noexcept;
			/**
			 * setAlphabet Метод установки алфавита
			 * @param alphabet объект алфавита
			 */
			void setAlphabet(const alphabet_t * alphabet) noexcept;
			/**
			 * setAdCw Метод установки характеристик словаря
			 * @param cw количество обработанных слов
			 * @param ad количество обработанных документов
			 */
			void setAdCw(const size_t cw, const size_t ad) noexcept;
			/**
			 * setTokenizer Метод установки токенизатора
			 * @param tokenizer объект токенизатора
			 */
			void setTokenizer(const tokenizer_t * tokenizer) noexcept;
		public:
			/**
			 * mark Метод маркировки варианта для последующей работы
			 * @param seq  собранная последовательность вариантов
			 * @param size размер последовательности, куда нужно добавить слово
			 */
			void mark(const vector <size_t> & seq, const size_t size = 0) noexcept;
		public:
			/**
			 * add Метод добавления слова
			 * @param word    параметры слова
			 * @param size    размер последовательности, куда нужно добавить слово
			 * @param replace разрешить заменять похожую последовательность
			 * @return        размер полученной последовательности
			 */
			const size_t add(const awrd_t & word, const size_t size = 0, const bool replace = false) noexcept;
			/**
			 * once Метод добавления слова один раз, в младшие секции
			 * @param word    параметры слова
			 * @param count   количество слов в тексте
			 * @param size    размер последовательности, куда нужно добавить слово
			 * @param replace разрешить заменять похожую последовательность
			 * @return        размер полученной последовательности
			 */
			const size_t once(const awrd_t & word, const size_t count = 1, const size_t size = 0, const bool replace = false) noexcept;
			/**
			 * add Метод добавления последовательности слов
			 * @param words   параметры списка слов
			 * @param lev     дистанция Дамерау-Левенштейна
			 * @param tmo     значение Танимото
			 * @param size    размер последовательности, куда нужно добавить слово
			 * @param replace разрешить заменять похожую последовательность
			 * @return        размер полученной последовательности
			 */
			const size_t add(const vector <awrd_t> & words, const u_short lev, const double tmo, const size_t size = 0, const bool replace = false) noexcept;
		public:
			/**
			 * smart Метод умного добавления слова
			 * @param word параметры слова
			 * @param size размер последовательности, куда нужно добавить слово
			 * @return     размер полученной последовательности
			 */
			const pair <size_t, size_t> smart(const awrd_t & word, const size_t size = 0) noexcept;
			/**
			 * set Метод установки нового слова в последовательность собираемых вариантов
			 * @param word    параметры слова
			 * @param ids     идентификатор предыдущей последовательности
			 * @param size    размер последовательности, куда нужно добавить слово
			 * @param replace разрешить заменять похожую последовательность
			 * @return        размер полученной последовательности
			 */
			const pair <size_t, size_t> set(const awrd_t & word, const size_t ids = idw_t::NIDW, const size_t size = 0, const bool replace = false) noexcept;
		public:
			/**
			 * Dumper Конструктор
			 */
			Dumper() : alm(nullptr), alt(nullptr), alphabet(nullptr), logfile(nullptr), tokenizer(nullptr), ad(0), cw(0) {};
			/**
			 * Dumper Конструктор
			 * @param alm объект языковой модели
			 */
			Dumper(const alm_t * alm) : alm(alm), alt(nullptr), alphabet(nullptr), logfile(nullptr), tokenizer(nullptr), ad(0), cw(0) {};
			/**
			 * Dumper Конструктор
			 * @param alt объект альтернативных вариантов
			 */
			Dumper(const alt_t * alt) : alm(nullptr), alt(alt), alphabet(nullptr), logfile(nullptr), tokenizer(nullptr), ad(0), cw(0) {};
			/**
			 * Dumper Конструктор
			 * @param tokenizer объект токенизатора
			 */
			Dumper(const tokenizer_t * tokenizer) : alm(nullptr), alt(nullptr), alphabet(nullptr), logfile(nullptr), tokenizer(tokenizer), ad(0), cw(0) {};
			/**
			 * Dumper Конструктор
			 * @param alm      объект языковой модели
			 * @param alt      объект альтернативных вариантов
			 * @param alphabet объект алфавита
			 */
			Dumper(const alm_t * alm, const alt_t * alt, const alphabet_t * alphabet) : alm(alm), alt(alt), alphabet(alphabet), logfile(nullptr), tokenizer(nullptr), ad(0), cw(0) {};
			/**
			 * Dumper Конструктор
			 * @param alm       объект языковой модели
			 * @param alt       объект альтернативных вариантов
			 * @param tokenizer объект токенизатора
			 */
			Dumper(const alm_t * alm, const alt_t * alt, const tokenizer_t * tokenizer) : alm(alm), alt(alt), alphabet(nullptr), logfile(nullptr), tokenizer(tokenizer), ad(0), cw(0) {};
			/**
			 * Dumper Конструктор
			 * @param alm       объект языковой модели
			 * @param alt       объект альтернативных вариантов
			 * @param alphabet  объект алфавита
			 * @param tokenizer объект токенизатора
			 */
			Dumper(const alm_t * alm, const alt_t * alt, const alphabet_t * alphabet, const tokenizer_t * tokenizer) : alm(alm), alt(alt), alphabet(alphabet), logfile(nullptr), tokenizer(tokenizer), ad(0), cw(0) {};
	} dumper_t;
};

#endif // __ANYKS_DUMPER__
