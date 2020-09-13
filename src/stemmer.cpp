/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <stemmer.hpp>

/**
 * exec Метод запуска внешнего приложения
 * @param cmd команда запуска
 * @return    ответ внешнего приложения
 */
/*
const string anyks::Stemmer::exec(const string & cmd) const noexcept {
	// Результат работы фнукции
	string result = "";
	// Если команда передана
	if(!cmd.empty()){
		// Буфер с адресом файла
		char buffer[128];
		// Выполняем запуск внешнего процесса
		FILE * pipe = popen(cmd.c_str(), "r");
		// Если процесс не отработал, выводим ошибку
		if(pipe == nullptr) throw runtime_error("popen() failed!");
		try {
			// Выполняем получение ответа скрипта в буфер
			while(fgets(buffer, sizeof(buffer), pipe) != nullptr) result.append(buffer);
			// Закрываем пайп
			pclose(pipe);
		// Если произошла ошибка
		} catch (...) {
			// Закрываем пайп
			pclose(pipe);
		}
	}
	// Выводим результат
	return result;
}
*/
/**
 * check Метод проверки слова на соответствие словаря
 * @param  word слово для проверки
 * @return      результат проверки
 */
const bool anyks::Stemmer::check(const wstring & word) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если слово передано
	if(!word.empty()){
		// Если это не первый или последний дефис
		result = ((word.front() != L'-') && (word.back() != L'-'));
		// Если слово прошло проверку
		if(result) result = !this->alphabet->isANumber(word);
	}
	// Выводим результат
	return result;
}
/**
 * setLMethod Метод установки функции получения леммы
 * @param fn функция для установки
 */
void anyks::Stemmer::setLMethod(stemming_t fn) noexcept {
	// Если функция передана, устанавливаем её
	if(fn != nullptr) this->lemmaFn = fn;
}
/**
 * setVMethod Метод установки функции подбора вариантов
 * @param fn функция для установки
 */
void anyks::Stemmer::setVMethod(variants_t fn) noexcept {
	// Если функция передана, устанавливаем её
	if(fn != nullptr) this->variantsFn = fn;
}
/**
 * setLScript Метод установки скрипта лемматизатора
 * @param script python скрипт для установки
 */
void anyks::Stemmer::setLScript(const string & script) noexcept {
// Если работа идет не изнутри Python
#ifndef NOPYTHON
	// Если скрипт передан, устанавливаем его
	if(!script.empty() && (this->script != nullptr)) this->lemmaScript = this->script->add(script, 2);
#endif
}
/**
 * setVScript Метод установки скрипта подбора вариантов
 * @param script python скрипт для установки
 */
void anyks::Stemmer::setVScript(const string & script) noexcept {
// Если работа идет не изнутри Python
#ifndef NOPYTHON
	// Если скрипт передан, устанавливаем его
	if(!script.empty() && (this->script != nullptr)) this->variantsScript = this->script->add(script, 2);
#endif
}
/**
 * setTokenizer Метод установки токенизатора
 * @param tokenizer указатель на токенизатор
 */
void anyks::Stemmer::setTokenizer(const tokenizer_t * tokenizer){
	// Если токенизатор передан
	if(tokenizer != nullptr){
		// Устанавливаем модуль генератора
		this->tokenizer = tokenizer;
// Если работа идет не изнутри Python
#ifndef NOPYTHON
		// Если внешняя функция не установлена
		if((this->lemmaFn == nullptr) && (this->variantsFn == nullptr) && (this->script == nullptr)){
			try {
				// Создаем объект модуля выполнения скриптов
				this->script = new python_t(this->tokenizer);
			// Если происходит ошибка то игнорируем её
			} catch(const bad_alloc&) {
				// Выходим из приложения
				exit(EXIT_FAILURE);
			}
		}
#endif
	}
}
/**
 * setAlphabet Метод установки алфавита
 * @param alphabet объект алфавита для установки
 */
void anyks::Stemmer::setAlphabet(const alphabet_t * alphabet) noexcept {
	// Запоминаем алфавит
	if(alphabet != nullptr) this->alphabet = alphabet;
}
/**
 * getPythonObj Метод извлечения внешнего объекта питона
 * @return внешний объект питона
 */
const anyks::python_t * anyks::Stemmer::getPythonObj() const noexcept {
	// Выводим созданный объект питона
	return this->script;
}
/**
 * lemma Метод получения леммы слова
 * @param  word слово для обработки
 * @param  code код языка передаваемого слова
 * @return      результат работы
 */
const wstring anyks::Stemmer::lemma(const wstring & word, const wstring & code) const noexcept {
	// Результат работы функции
	wstring result = L"";
	// Если слово передано и скрипт загружен
	if(!word.empty() && !code.empty() && ((this->lemmaFn != nullptr) || (this->lemmaScript > 0)) && this->check(word)){
		// Код языка к которому принадлежит слово
		wstring lang = code;
		// Если это русский язык, проверяем на латиницу
		if((lang.compare(L"en") != 0) && this->alphabet->isLatian(word)) lang = L"en";
		// Если внешняя функция установлена
		if(this->lemmaFn != nullptr) result = this->lemmaFn(word, lang);
// Если работа идет не изнутри Python
#ifndef NOPYTHON
		// Если скрипт Python установлен
		else if(this->script != nullptr){
			// Блокируем остальные потоки до завершения работы скрипта
			this->locker.lock();
			// Выполняем скрипт для получения вариантов
			result = this->script->run(
				this->lemmaScript, {
					this->alphabet->convert(word),
					this->alphabet->convert(lang)
				}
			);
			// Разблокируем остальные потоки
			this->locker.unlock();
		}
#endif
	}
	// Выводим результат
	return result;
}
/**
 * variants Метод получения вариантов слова
 * @param  word слово для обработки
 * @param  code код языка передаваемого слова
 * @return      результат работы
 */
const list <wstring> anyks::Stemmer::variants(const wstring & word, const wstring & code) const noexcept {
	// Результат работы функции
	list <wstring> result;
	// Если слово передано и скрипт загружен
	if(!word.empty() && !code.empty() && ((this->variantsFn != nullptr) || (this->variantsScript > 0)) && this->check(word)){
		// Код языка к которому принадлежит слово
		wstring lang = code;
		// Если это русский язык, проверяем на латиницу
		if((lang.compare(L"en") != 0) && this->alphabet->isLatian(word)) lang = L"en";
		// Если функция подборки вариантов установлена
		if(this->variantsFn != nullptr) result = this->variantsFn(word, lang);
// Если работа идет не изнутри Python
#ifndef NOPYTHON
		// Если функция подборки вариантов не передана
		else if(this->script != nullptr) {
			// Блокируем остальные потоки до завершения работы скрипта
			this->locker.lock();
			// Выполняем скрипт для получения вариантов
			const wstring & res = this->script->run(
				this->variantsScript, {
					this->alphabet->convert(word),
					this->alphabet->convert(lang)
				}
			);
			// Разблокируем остальные потоки
			this->locker.unlock();
			// Если результат получен
			if(!res.empty()) this->alphabet->split(res, L"|", result);
		}
#endif
		// Если результат получен
		if(!result.empty()){
			// Выполняем сортировку
			result.sort();
			// Удаляем все дубликаты
			result.unique();
		}
	}
	// Выводим результат
	return result;
}
/**
 * Stemmer Конструктор
 * @param alphabet объект алфавита
 */
anyks::Stemmer::Stemmer(const alphabet_t * alphabet) noexcept {
	// Устанавливаем объект алфавита
	this->setAlphabet(alphabet);
}
/**
 * Stemmer Конструктор
 * @param tokenizer указатель на токенизатор
 */
anyks::Stemmer::Stemmer(const tokenizer_t * tokenizer) noexcept {
	// Устанавливаем токенизатор
	this->setTokenizer(tokenizer);
}
/**
 * Stemmer Конструктор
 * @param alphabet  объект алфавита
 * @param tokenizer указатель на токенизатор
 */
anyks::Stemmer::Stemmer(const alphabet_t * alphabet, const tokenizer_t * tokenizer) noexcept {
	// Устанавливаем объект алфавита
	this->setAlphabet(alphabet);
	// Устанавливаем токенизатор
	this->setTokenizer(tokenizer);
}
/**
 * ~Stemmer Деструктор
 */
anyks::Stemmer::~Stemmer() noexcept {
// Если работа идет не изнутри Python
#ifndef NOPYTHON
	// Если объект создан, удаляем его
	if(this->script != nullptr) delete this->script;
#else
	/**
	 * Так и не понял этот баг
	 * Деструктор отрабатывает но из-за установленной функции lemmaFn поток блокируется намертво
	 * Игры с py::gil_scoped_release ничего не дали, блокировки в любом случае есть
	 * По этому просто выходим из приложения, по завершению работы деструктора
	 */
	exit(0);
#endif
}
