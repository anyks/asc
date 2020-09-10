/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef __ANYKS_STEMMER__
#define __ANYKS_STEMMER__

/**
 * Стандартная библиотека
 */
#include <list>
#include <mutex>
#include <string>
#include <functional>
/**
 * Наши модули
 */
#include <python.hpp>
#include <alphabet.hpp>
#include <tokenizer.hpp>

// Устанавливаем область видимости
using namespace std;

/**
 * anyks пространство имён
 */
namespace anyks {
	/**
	 * Stemmer Класс модуля стемминга
	 */
	typedef class Stemmer {
		private:
			// Мютекс для блокировки потока
			mutable mutex locker;
		public:
			// Тип функции для извлечения леммы
			typedef function <const wstring (const wstring &, const wstring &)> stemming_t;
			// Тип функции для подбора вариантов слова
			typedef function <const list <wstring> (const wstring &, const wstring &)> variants_t;
		private:
			// Название скрипта лемматизатора
			size_t lemmaScript = 0;
			// Название скрипта подбора вариантов
			size_t variantsScript = 0;
		private:
			// Функция для получения леммы
			stemming_t lemmaFn = nullptr;
			// Функция для подбора вариантов слова
			variants_t variantsFn = nullptr;
		private:
			// Объект исполнения внешних скриптов
			python_t * script = nullptr;
			// Объект алфавита
			const alphabet_t * alphabet = nullptr;
			// Объект токенизатора
			const tokenizer_t * tokenizer = nullptr;
		private:
			/**
			 * check Метод проверки слова на соответствие словаря
			 * @param  word слово для проверки
			 * @return      результат проверки
			 */
			const bool check(const wstring & word) const noexcept;
		public:
			/**
			 * setLMethod Метод установки функции получения леммы
			 * @param fn функция для установки
			 */
			void setLMethod(stemming_t fn) noexcept;
			/**
			 * setVMethod Метод установки функции подбора вариантов
			 * @param fn функция для установки
			 */
			void setVMethod(variants_t fn) noexcept;
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
		public:
			/**
			 * setTokenizer Метод установки токенизатора
			 * @param tokenizer указатель на токенизатор
			 */
			void setTokenizer(const tokenizer_t * tokenizer);
			/**
			 * setAlphabet Метод установки алфавита
			 * @param alphabet объект алфавита для установки
			 */
			void setAlphabet(const alphabet_t * alphabet) noexcept;
		public:
			/**
			 * getPythonObj Метод извлечения внешнего объекта питона
			 * @return внешний объект питона
			 */
			const python_t * getPythonObj() const noexcept;
			/**
			 * lemma Метод получения леммы слова
			 * @param  word слово для обработки
			 * @param  code код языка передаваемого слова
			 * @return      результат работы
			 */
			const wstring lemma(const wstring & word, const wstring & code) const noexcept;
			/**
			 * variants Метод получения вариантов слова
			 * @param  word слово для обработки
			 * @param  code код языка передаваемого слова
			 * @return      результат работы
			 */
			const list <wstring> variants(const wstring & word, const wstring & code) const noexcept;
		public:
			/**
			 * Stemmer Конструктор
			 */
			Stemmer() noexcept {};
			/**
			 * Stemmer Конструктор
			 * @param alphabet объект алфавита
			 */
			Stemmer(const alphabet_t * alphabet) noexcept;
			/**
			 * Stemmer Конструктор
			 * @param tokenizer указатель на токенизатор
			 */
			Stemmer(const tokenizer_t * tokenizer) noexcept;
			/**
			 * Stemmer Конструктор
			 * @param alphabet  объект алфавита
			 * @param tokenizer указатель на токенизатор
			 */
			Stemmer(const alphabet_t * alphabet, const tokenizer_t * tokenizer) noexcept;
			/**
			 * ~Stemmer Деструктор
			 */
			~Stemmer() noexcept;
	} stemmer_t;
};

#endif // __ANYKS_STEMMER__
