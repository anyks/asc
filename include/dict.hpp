/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef __ANYKS_DICTIONARY__
#define __ANYKS_DICTIONARY__

/**
 * Стандартная библиотека
 */
#include <set>
#include <map>
#include <list>
#include <mutex>
#include <cmath>
#include <ctime>
#include <bitset>
#include <locale>
#include <vector>
#include <string>
#include <functional>
#include <unordered_map>
/**
 * Наши модули
 */
#include <alm.hpp>
#include <alt.hpp>
#include <hnsw.hpp>
#include <bloom.hpp>
#include <dumper.hpp>
#include <stemmer.hpp>
#include <levenshtein.hpp>
#include <nlohmann/json.hpp>

// Активируем стандартное пространство имён
using namespace std;
// Активируем json в качестве объекта пространства имён
using json = nlohmann::json;

/**
 * anyks пространство имён
 */
namespace anyks {
	/**
	 * Класс словаря
	 */
	typedef class Dict {
		public:
			// Основные опции
			enum class options_t : u_short {
				debug,    // Флаг режима отладки
				bloom,    // Флаг разрешающий использовать фильтр Блума
				stemming, // Флаг активации стемминга
				onlytypos // Флаг исправления только опечаток
			};
		private:
			/**
			 * Params Структура парамеров
			 */
			typedef struct Params {
				// Дата генерации словаря
				time_t date;
				// Характеристики словаря
				size_t ad, cw;
				// Название словаря
				wstring name;
				// Код языка словаря
				wstring code;
				// Автор словаря
				wstring author;
				// Тип лицензионного соглашения
				wstring lictype;
				// Лицензионная информация
				wstring lictext;
				// Контактные данные автора
				wstring contacts;
				// Авторское правоs
				wstring copyright;
				/**
				 * Params Конструктор
				 */
				Params() : date(time(nullptr)), ad(0), cw(0) {}
			} param_t;
			/**
			 * Lemma Структура лемм словаря
			 */
			typedef struct Lemma: map <wchar_t, Lemma> {} __attribute__((packed)) lemma_t;
		private:
			// Индекс поиска нужных вариантов
			hnsw_t hnsw;
			// Параметры словаря
			param_t params;
			// Объект альтернативных слов
			mutable alt_t alt;
			// Мютекс блокировки потока
			mutable mutex locker;
			// Словарь слов
			mutable lemma_t lemmas;
			// Список пилотных слов
			mutable set <wchar_t> pilots;
			// Эмбеддинг индексировани
			map <wchar_t, u_short> embedding;
			// Списко бинарных векторов средних слов
			multimap <size_t, size_t> vectors;
			// Коллекция всех слов - словаря
			unordered_map <size_t, word_t> words;
			// Количество потоков для работы
			size_t threads = thread::hardware_concurrency();
		private:
			// Идентификатор конца строки
			wchar_t elid;
			// Флаги параметров
			bitset <4> options;
			// Размер эмбеддинга
			u_short sizeEmbedding = 0;
			// Количество вариантов для анализа
			size_t nswlibCount = NSWLIB_COUNT;
		private:
			// Объект языковой модели
			alm_t * alm = nullptr;
			// Фильтр Блума
			bloom_t * filter = nullptr;
			// Объект стемминга
			stemmer_t * stemmer = nullptr;
			// Объект log файла
			const char * logfile = nullptr;
			// Создаем тредпул
			mutable tpool_t * tpool = nullptr;
			// Объект алфавита
			const alphabet_t * alphabet = nullptr;
			// Объект токенизатора
			const tokenizer_t * tokenizer = nullptr;
		private:
			/**
			 * start Метод инициализации работы тредпула
			 */
			void start() const;
			/**
			 * finish Метод завершения работы тредпула
			 */
			void finish() const;
			/**
			 * vecb Метод генерации бинарного эмбеддинга для слова
			 * @param word слово для генерации эмбеддинга
			 * @return     бинарный эмбеддинг слова
			 */
			const size_t vecb(const word_t & word) const noexcept;
			/**
			 * vecsb Метод генерации всех возможных бинарных эмбеддингов для слова
			 * @param word слово для генерации эмбеддинга
			 * @return     список бинарных эмбеддингов слова
			 */
			const set <size_t> vecsb(const word_t & word) const noexcept;
			/**
			 * vec Метод генерации эмбеддинга для слова
			 * @param word слово для генерации эмбеддинга
			 * @return     эмбеддинг слова
			 */
			const vector <float> vec(const word_t & word) const noexcept;
		private:
			/**
			 * correctly Метод проверки на верность слова
			 * @param  word слово для проверки
			 * @return      результат проверки
			 */
			const bool correctly(const word_t & word) const noexcept;
			/**
			 * isOption Метод проверки наличия опции
			 * @param option опция для проверки
			 * @return       результат проверки
			 */
			const bool isOption(const options_t option) const noexcept;
		public:
			/**
			 * clear Метод очистки словаря
			 */
			void clear() noexcept;
			/**
			 * generateEmbedding Метод генерации эмбеддинга
			 */
			void generateEmbedding() noexcept;
			/**
			 * train Метод обучения
			 * @param status функция вывода статуса модификации
			 */
			void train(function <void (const u_short)> status = nullptr) noexcept;
			/**
			 * bloom Метод инициализации фильтра Блума
			 * @param status функция вывода статуса обработки
			 */
			void bloom(function <void (const u_short)> status = nullptr) noexcept;
			/**
			 * stemming Метод инициализации стемминга
			 * @param status функция вывода статуса обработки
			 */
			void stemming(function <void (const u_short)> status = nullptr) noexcept;
		public:
			/**
			 * addUWord Метод добавления слова, которое всегда начинается с заглавной буквы
			 * @param word слово для добавления
			 */
			void addUWord(const string & word) noexcept;
			/**
			 * addUWord Метод добавления слова, которое всегда начинается с заглавной буквы
			 * @param word слово для добавления
			 */
			void addUWord(const wstring & word) noexcept;
			/**
			 * addUWord Метод добавления идентификатора слова, которое всегда начинается с заглавной буквы
			 * @param idw идентификатор слова для добавления
			 * @param ups регистры добавляемого слова
			 */
			void addUWord(const size_t idw, const size_t ups) noexcept;
			/**
			 * setUWords Метод добавления списка идентификаторов слов, которые всегда начинаются с заглавной буквы
			 * @param words список идентификаторов слов для добавления
			 */
			void setUWords(const map <size_t, size_t> & words) noexcept;
		public:
			/**
			 * setLMethod Метод установки функции получения леммы
			 * @param fn функция для установки
			 */
			void setLMethod(stemmer_t::stemming_t fn) noexcept;
			/**
			 * setVMethod Метод установки функции подбора вариантов
			 * @param fn функция для установки
			 */
			void setVMethod(stemmer_t::variants_t fn) noexcept;
		public:
			/**
			 * setALM Метод установки языковой модели
			 * @param alm объект языковой модели
			 */
			void setALM(alm_t * alm) noexcept;
			/**
			 * setDate Метод установки даты генерации словаря
			 * @param date дата генерации словаря
			 */
			void setDate(const time_t date) noexcept;
			/**
			 * setCode Метод установки кода языка
			 * @param code код языка словаря для установки
			 */
			void setCode(const wstring & code) noexcept;
			/**
			 * setName Метод установки названия словаря
			 * @param name название словаря
			 */
			void setName(const wstring & name) noexcept;
			/**
			 * setPilot Метод установки пилотного слова
			 * @param letter буква для установки
			 */
			void setPilot(const wchar_t letter) noexcept;
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
			 * setAuthor Метод установки автора словаря
			 * @param author автор словаря
			 */
			void setAuthor(const wstring & author) noexcept;
			/**
			 * setLScript Метод установки скрипта лемматизатора
			 * @param script python скрипт для установки
			 */
			void setLScript(const string & script) noexcept;
			/**
			 * setVScript Метод установки скрипта подбора вариантов
			 * @param script python скрипт для установки
			 */
			void setVScript(const string & script) noexcept;
			/**
			 * setLictype Метод установки типа лицензионной информации словаря
			 * @param type тип лицензионного соглашения
			 */
			void setLictype(const wstring & type) noexcept;
			/**
			 * unsetOption Метод отключения опции модуля
			 * @param option опция для отключения
			 */
			void unsetOption(const options_t option) noexcept;
			/**
			 * setLictext Метод установки лицензионной информации словаря
			 * @param license лицензионная информация словаря
			 */
			void setLictext(const wstring & license) noexcept;
			/**
			 * setThreads Метод установки количества потоков
			 * @param threads количество потоков для работы
			 */
			void setThreads(const size_t threads = 0) noexcept;
			/**
			 * setContacts Метод установки контактных данных автора словаря
			 * @param contacts контактные данные автора словаря
			 */
			void setContacts(const wstring & contacts) noexcept;
			/**
			 * setNSWLibCount Метод установки максимального количества вариантов для анализа
			 * @param count максимальное количество вариантов для анализа
			 */
			void setNSWLibCount(const size_t count = 0) noexcept;
			/**
			 * setCopyright Метод установки авторских прав на словарь
			 * @param copyright авторские права на словарь
			 */
			void setCopyright(const wstring & copyright) noexcept;
			/**
			 * setSizeEmbedding Метод установки размера эмбеддинга
			 * @param size размер эмбеддинга
			 */
			void setSizeEmbedding(const u_short size = 0) noexcept;
			/**
			 * setPilots Метод установки пилотных слов
			 * @param letters список пилотных слов
			 */
			void setPilots(const set <wchar_t> & letters) noexcept;
			/**
			 * setAlphabet Метод установки словаря
			 * @param alphabet алфавит словаря
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
			/**
			 * setEmbedding Метод установки эмбеддинга
			 * @param embedding эмбеддинг словаря
			 * @param size      размер эмбеддинга
			 */
			void setEmbedding(const json & embedding, const u_short size) noexcept;
		public:
			/**
			 * addLemma Метод добавления леммы в словарь
			 * @param lemma лемма для добавления
			 */
			void addLemma(const wstring & lemma) noexcept;
			/**
			 * addAlt Метод добавления альтернативной буквы
			 * @param letter буква у которой есть альтернатива
			 * @param alt    альтернативная буква
			 */
			void addAlt(const wchar_t letter, const wchar_t alt) noexcept;
			/**
			 * addAlt Метод добавления слова с альтернативной буквой
			 * @param word    слово для добавления
			 * @param alt     альтернативное слово для добавления
			 * @param context слово является контексто-зависимым
			 */
			void addAlt(const wstring & word, const wstring & alt, const bool context = false) noexcept;
			/**
			 * addAlt Метод добавления слова с альтернативной буквой
			 * @param word    слово для добавления
			 * @param idw     идентификатор слова если есть
			 * @param context слово является контексто-зависимым
			 */
			void addAlt(const wstring & word, const size_t idw = idw_t::NIDW, const bool context = false) noexcept;
		public:
			/**
			 * find Метод поиска вариантов подходящих вариантов слов
			 * @param  word слово для проверки
			 * @param  dmp  сборщик результатов
			 * @param  size размер последовательности, куда нужно добавить слово
			 * @return      размер полученной последовательности и количество добавленных вариантов
			 */
			const pair <size_t, size_t> find(const word_t & word, dumper_t & dmp, const size_t size = 0) const noexcept;
		public:
			/**
			 * saveIndex Метод сохранения индекса в бинарном виде
			 * @param buffer буфер данных индекса
			 */
			void saveIndex(vector <char> & buffer) const noexcept;
			/**
			 * loadIndex Метод загрузки индекса в бинарном виде
			 * @param buffer буфер данных индекса
			 */
			void loadIndex(const vector <char> & buffer) noexcept;
			/**
			 * add Метод добавления слова в словарь
			 * @param idw   идентификатор слова
			 * @param word  слово для добавления
			 * @param train нужно ли выполнить обучение
			 */
			void add(const size_t idw, const word_t & word, const bool train = false) noexcept;
			/**
			 * readVocab Метод чтения словаря из файла
			 * @param filename файл для чтения словаря
			 * @param status   функция вывода статуса чтения
			 */
			void readVocab(const string & filename, function <void (const u_short)> status = nullptr) noexcept;
			/**
			 * save Метод сохранения индекса
			 * @param buffer буфер данных индекса
			 * @param all    флаг предписывающий снятие полного дампа
			 * @param status функция вывода статуса обработки
			 */
			void save(vector <char> & buffer, const bool all = false, function <void (const u_short)> status = nullptr) const noexcept;
			/**
			 * load Метод загрузки индекса
			 * @param buffer буфер данных индекса
			 * @param all    флаг предписывающий загрузки полного дампа
			 * @param status функция вывода статуса обработки
			 */
			void load(const vector <char> & buffer, const bool all = false, function <void (const u_short)> status = nullptr) noexcept;
		public:
			/**
			 * empty Метод проверки пустой ли словарь
			 * @return результат проверки
			 */
			const bool empty() const noexcept;
			/**
			 * isIdw Метод точной проверки существования идентификатора слова
			 * @param idw идентификатор слова для проверки
			 * @return    результат проверки
			 */
			const bool isIdw(const size_t idw) const noexcept;
			/**
			 * isPilot Метод проверки на пилотное слово
			 * @param letter слово для првоерки
			 * @return       результат проверки
			 */
			const bool isPilot(const wchar_t letter) const noexcept;
			/**
			 * isLemma Метод проверки на существование леммы
			 * @param lemma лемма для проверки
			 * @return      результат проверки
			 */
			const bool isLemma(const wstring & lemma) const noexcept;
		public:
			/**
			 * isWord Метод точной проверки существования слова в словаре
			 * @param  word слово для проверки
			 * @return      идентификатор искомого слова
			 */
			const size_t isWord(const word_t & word) const noexcept;
			/**
			 * check Метод проверки существования слова в словаре
			 * @param  word слово для проверки
			 * @param  idw  идентификатор слова, если есть
			 * @return      идентификатор искомого слова
			 */
			const size_t check(const word_t & word, const size_t idw = idw_t::NIDW) const noexcept;
		public:
			/**
			 * dumper Метод создания сборщика гипотез
			 * @return сборщик гипотез
			 */
			dumper_t dumper() const noexcept;
			/**
			 * getDate Метод получения даты создания словаря
			 * @return дата создания словаря в timeStamp
			 */
			const time_t getDate() const noexcept;
			/**
			 * getCode Метод получения кода языка
			 * @return код языка
			 */
			const wstring & getCode() const noexcept;
			/**
			 * getName Метод получения названия словаря
			 * @return название словаря
			 */
			const wstring & getName() const noexcept;
			/**
			 * getAuthor Метод получения автора словаря
			 * @return автор словаря
			 */
			const wstring & getAuthor() const noexcept;
			/**
			 * getLictype Метод получения типа лицензионной информации словаря
			 * @return тип лицензионного соглашения
			 */
			const wstring & getLictype() const noexcept;
			/**
			 * getLictext Метод получения лицензионной информации словаря
			 * @return лицензионная информация словаря
			 */
			const wstring & getLictext() const noexcept;
			/**
			 * getContacts Метод получения контактных данных автора словаря
			 * @return контактные данные автора словаря
			 */
			const wstring & getContacts() const noexcept;
			/**
			 * setCopyright Метод получения авторских прав на словарь
			 * @return авторские права на словарь
			 */
			const wstring & getCopyright() const noexcept;
			/**
			 * word Метод получения слова по его идентификатору
			 * @param  idw идентификатор слова
			 * @return     запрашиваемое слово
			 */
			const word_t * word(const size_t idw) const noexcept;
			/**
			 * stem Метод извлечения лемма/стеммы
			 * @param  word слово для обработки
			 * @return      полученная лемма/стемма
			 */
			const wstring stem(const wstring & word) const noexcept;
		public:
			/**
			 * getPilots Метод получения пилотных слов
			 */
			const set <wchar_t> & getPilots() const noexcept;
			/**
			 * getAdCw Метод получения характеристик словаря
			 * @return характеристики словаря
			 */
			const pair <size_t, size_t> getAdCw() const noexcept;
			/**
			 * getEmbedding Метод получения эмбеддинга словаря
			 * @return эмбеддинг словаря
			 */
			const pair <json, size_t> getEmbedding() const noexcept;
			/**
			 * getUWords Метод извлечения списка идентификаторов слов, которые всегда начинаются с заглавной буквы
			 * @return список идентификаторов слов
			 */
			const map <size_t, size_t> & getUWords() const noexcept;
			/**
			 * all Метод извлечения всех слов
			 * @return список найденных слов
			 */
			const unordered_map <size_t, word_t> & all() const noexcept;
			/**
			 * variants Метод получения вариантов слова
			 * @param word слово для которого нужно получить варианты
			 * @return     список подобранных вариантов слова
			 */
			const list <wstring> variants(const wstring & word) const noexcept;
		public:
			/**
			 * Dict Конструктор
			 */
			Dict() noexcept;
			/**
			 * Dict Конструктор
			 * @param alm объект языковой модели
			 */
			Dict(alm_t * alm) noexcept;
			/**
			 * Dict Конструктор
			 * @param alphabet объект алфавита
			 */
			Dict(const alphabet_t * alphabet) noexcept;
			/**
			 * Dict Конструктор
			 * @param tokenizer объект токенизатора
			 */
			Dict(const tokenizer_t * tokenizer) noexcept;
			/**
			 * Dict Конструктор
			 * @param alm      объект языковой модели
			 * @param alphabet объект алфавита
			 */
			Dict(alm_t * alm, const alphabet_t * alphabet) noexcept;
			/**
			 * Dict Конструктор
			 * @param alm       объект языковой модели
			 * @param tokenizer объект токенизатора
			 */
			Dict(alm_t * alm, const tokenizer_t * tokenizer) noexcept;
			/**
			 * Dict Конструктор
			 * @param alm       объект языковой модели
			 * @param alphabet  объект алфавита
			 * @param tokenizer объект токенизатора
			 */
			Dict(alm_t * alm, const alphabet_t * alphabet, const tokenizer_t * tokenizer) noexcept;
			/**
			 * ~Dict деструктор
			 */
			~Dict() noexcept;
	} dict_t;
};

#endif // __ANYKS_DICTIONARY__
