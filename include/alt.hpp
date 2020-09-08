/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef __ANYKS_ALTERNATIVE__
#define __ANYKS_ALTERNATIVE__

/**
 * Стандартная библиотека
 */
#include <map>
#include <list>
#include <bitset>
#include <string>
/**
 * Наши модули
 */
#include <alphabet.hpp>
#include <tokenizer.hpp>

// Активируем стандартное пространство имён
using namespace std;

/**
 * anyks пространство имён
 */
namespace anyks {
	/**
	 * Alt Класс работы с альтернативными словами
	 */
	typedef class Alt {
		private:
			// Объект алфавита
			const alphabet_t * alphabet;
			// Объект токенизатора
			const tokenizer_t * tokenizer;
		private:
			// Список слов которые начинаются с заглавной буквы
			map <size_t, size_t> uwords;
			// Список альтернативных букв
			map <wchar_t, wchar_t> letters;
			// Список альтернативных слов, контексто-зависимых
			map <size_t, pair <size_t, wstring>> context;
			// Список альтернативных слов, контексто-независимых
			map <size_t, pair <size_t, wstring>> ncontext;
		public:
			/**
			 * empty Метод проверки на пустоту данных
			 * @return результат проверки
			 */
			const bool empty() const noexcept;
			/**
			 * uppers Метод извлечения регистров из слова
			 * @param word слово из которого нужно получить регистры
			 * @return     регистры полученные из слова
			 */
			const size_t uppers(const wstring & word) const noexcept;
		public:
			/**
			 * getUWords Метод извлечения списка идентификаторов слов, которые всегда начинаются с заглавной буквы
			 * @return список идентификаторов слов
			 */
			const map <size_t, size_t> & getUWords() const noexcept;
			/**
			 * getUWord Метод извлеченя регистров слова по его идентификатору
			 * @param idw идентификатор слова для проверки
			 * @return    регистры слова, если слово найдено
			 */
			const size_t getUWord(const size_t idw) const noexcept;
			/**
			 * getUWord Метод извлеченя регистров слова
			 * @param word слово для проверки
			 * @return     регистры слова, если слово найдено
			 */
			const size_t getUWord(const wstring & word) const noexcept;
		public:
			/**
			 * check Метод проверки соответствия слова на альтернативную букву
			 * @param idw идентификатор слова для проверки
			 * @return    результат проверки
			 */
			const bool check(const size_t idw) const noexcept;
			/**
			 * check Метод проверки соответствия слова на альтернативную букву
			 * @param word слово для проверки
			 * @return     результат проверки
			 */
			const bool check(const wstring & word) const noexcept;
			/**
			 * check Метод проверки существования альтернативной буквы
			 * @param  letter буква для проверки
			 * @return        результат проверки
			 */
			const bool check(const wchar_t letter) const noexcept;
			/**
			 * check Метод проверки соответствия слова на альтернативную букву
			 * @param idw     идентификатор слова для проверки
			 * @param context слово является контексто-зависимым
			 * @return        результат проверки
			 */
			const bool check(const size_t idw, const bool context) const noexcept;
			/**
			 * check Метод проверки соответствия слова на альтернативную букву
			 * @param word    слово для проверки
			 * @param context слово является контексто-зависимым
			 * @return        результат проверки
			 */
			const bool check(const wstring & word, const bool context) const noexcept;
		public:
			/**
			 * alt Метод получения альтернативной буквы
			 * @param  letter буква для проверки
			 * @return        альтернативная буква
			 */
			const wchar_t alt(const wchar_t letter) const noexcept;
			/**
			 * rel Метод получения реальной буквы из альтернативной
			 * @param  letter альтернативная буква
			 * @return        реальная буква
			 */
			const wchar_t rel(const wchar_t letter) const noexcept;
		public:
			/**
			 * clear Метод очистки данных
			 */
			void clear() noexcept;
		public:
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
			 * add Метод добавления альтернативной буквы
			 * @param letter буква у которой есть альтернатива
			 * @param alt    альтернативная буква
			 */
			void add(const wchar_t letter, const wchar_t alt) noexcept;
			/**
			 * add Метод добавления слова с альтернативной буквой
			 * @param word    слово для добавления
			 * @param alt     альтернативное слово для добавления
			 * @param context слово является контексто-зависимым
			 */
			void add(const wstring & word, const wstring & alt, const bool context = false) noexcept;
			/**
			 * add Метод добавления слова с альтернативной буквой
			 * @param word    слово для добавления
			 * @param idw     идентификатор слова если есть
			 * @param context слово является контексто-зависимым
			 */
			void add(const wstring & word, const size_t idw = idw_t::NIDW, const bool context = false) noexcept;
		public:
			/**
			 * setAlphabet Метод установки алфавита
			 * @param alphabet объект алфавита
			 */
			void setAlphabet(const alphabet_t * alphabet) noexcept;
			/**
			 * setTokenizer Метод установки токенизатора
			 * @param tokenizer объект токенизатора
			 */
			void setTokenizer(const tokenizer_t * tokenizer) noexcept;
		public:
			/**
			 * get Метод получения слово с альтернативной буквой
			 * @param idw     идентификатор слова для которого нужно получить слово с альтернативной буквой
			 * @param context слово является контексто-зависимым
			 * @return        слово с альтернативной буквой
			 */
			const pair <size_t, wstring> get(const size_t idw, const bool context = false) const noexcept;
			/**
			 * get Метод получения слово с альтернативной буквой
			 * @param word    слово для которого нужно получить слово с альтернативной буквой
			 * @param context слово является контексто-зависимым
			 * @return        слово с альтернативной буквой
			 */
			const pair <size_t, wstring> get(const wstring & word, const bool context = false) const noexcept;
		public:
			/**
			 * Alt Конструктор
			 */
			Alt() : alphabet(nullptr), tokenizer(nullptr) {};
			/**
			 * Alt Конструктор
			 * @param alphabet объект алфавита
			 */
			Alt(const alphabet_t * alphabet) : alphabet(alphabet), tokenizer(nullptr) {};
			/**
			 * Alt Конструктор
			 * @param tokenizer объект токенизатора
			 */
			Alt(const tokenizer_t * tokenizer) : alphabet(nullptr), tokenizer(tokenizer) {};
			/**
			 * Alt Конструктор
			 * @param alphabet  объект алфавита
			 * @param tokenizer объект токенизатора
			 */
			Alt(const alphabet_t * alphabet, const tokenizer_t * tokenizer) : alphabet(alphabet), tokenizer(tokenizer) {};
			/**
			 * ~Alt Деструктор
			 */
			~Alt();
	} alt_t;
};

#endif // __ANYKS_ALTERNATIVE__
