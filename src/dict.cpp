/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

// Определяем сколько бит в процессоре
#if ((ULONG_MAX) == (UINT_MAX))
# define IS32BIT
#else
# define IS64BIT
#endif

#include <dict.hpp>

/**
 * start Метод инициализации работы тредпула
 */
void anyks::Dict::start() const {
	// Если тредпул ещё не создан
	if(this->tpool == nullptr){
		// Экранируем возможность ошибки памяти
		try {
			// Создаём тредпул
			this->tpool = new tpool_t;
			// Выполняем инициализацию тредпула
			this->tpool->init(this->threads);
		// Если происходит ошибка то игнорируем её
		} catch(const bad_alloc &) {
			// Выводим сообщение об ошибке, если режим отладки включён
			if(this->isOption(options_t::debug)) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "bad alloc for init threadpool");
			// Выходим из приложения
			exit(EXIT_FAILURE);
		}
	}
}
/**
 * finish Метод завершения работы тредпула
 */
void anyks::Dict::finish() const {
	// Если тредпул создан
	if(this->tpool != nullptr){
		// Ожидаем завершения обработки
		this->tpool->wait();
		// Удаляем выделенную память
		delete this->tpool;
		// Зануляем объект
		this->tpool = nullptr;
	}
}
/**
 * vecb Метод генерации бинарного эмбеддинга для слова
 * @param word слово для генерации эмбеддинга
 * @return     бинарный эмбеддинг слова
 */
const size_t anyks::Dict::vecb(const word_t & word) const noexcept {
	// Результат работы функции
	size_t result = 0;
	// Если слово передано
	if(!word.empty() && (word.length() <= MAX_WORD_LENGTH)){
		// Бинарный контейнер
		bitset <64> buffer;
		// Переходим по всей длине слова
		for(auto & letter : word){
			// Выполняем поиск буквы в словаре
			auto it = this->embedding.find(letter);
			// Если буква найдена в словаре
			if(it != this->embedding.end()) buffer.set(it->second);
		}
		// Выводим результат
		result = buffer.to_ullong();
	}
	// Выводим результат
	return result;
}
/**
 * vecsb Метод генерации всех возможных бинарных эмбеддингов для слова
 * @param word слово для генерации эмбеддинга
 * @return     список бинарных эмбеддингов слова
 */
const set <size_t> anyks::Dict::vecsb(const word_t & word) const noexcept {
	// Результат работы функции
	set <size_t> result;
	// Если слово передано
	if(!word.empty() && (word.length() <= MAX_WORD_LENGTH)){
		// Значение временного вектора
		size_t vector = 0;
		// Список векторов с удалёнными вариантами
		map <size_t, u_short> variants;
		// Получаем вектор слова
		const size_t vec = this->vecb(word);
		// Добавляем в результат первый вариант
		result.emplace(vec);
		// Формируем бинарный контейнер
		bitset <64> buffer = vec;
		// Выполняем генерацию вставок и удаления букв
		for(u_short i = 0; i < this->sizeEmbedding; i++){
			// Выполняем смену в буфере байта
			buffer.flip(i);
			// Получаем значение вектора
			vector = buffer.to_ullong();
			// Если бит не установлен
			if(!buffer.test(i)) variants.emplace(vector, i);
			// Добавляем в список полученный вариант
			result.emplace(vector);
			// Возвращаем значение байта обратно
			buffer.flip(i);
		}
		// Если список вариантов получен, генерируем список замещений букв
		if(!variants.empty()){
			// Переходим по всему списку вариантов
			for(auto & item : variants){
				// Получаем бинарные данные
				buffer = item.first;
				// Выполняем генерацию вставок и удаления букв
				for(u_short i = 0; i < this->sizeEmbedding; i++){
					// Если это не удалённый бит
					if((i != item.second) && !buffer.test(i)){
						// Выполняем смену в буфере байта
						buffer.flip(i);
						// Добавляем в список полученный вариант
						result.emplace(buffer.to_ullong());
						// Возвращаем значение байта обратно
						buffer.flip(i);
					}
				}
			}
		}
	}
	// Выводим результат
	return result;
}
/**
 * vec Метод генерации эмбеддинга для слова
 * @param word слово для генерации эмбеддинга
 * @return     эмбеддинг слова
 */
const vector <float> anyks::Dict::vec(const word_t & word) const noexcept {
	// Результат работы функции
	vector <float> result;
	// Если слово передано
	if(!word.empty() && (word.length() <= MAX_WORD_LENGTH)){
		// Создаем вектор нужного размера
		result.resize(this->sizeEmbedding, 0.0f);
		// Переходим по всем буквам слова
		for(size_t i = 0; i < word.length(); i++){
			// Извлекаем позицию буквы
			auto it = this->embedding.find(word.at(i));
			// Если такая буква существует в словаре эмбеддинга
			if(it != this->embedding.end()){
				// Получаем позицию буквы
				const size_t pos = it->second;
				// Если в позиции вектор больше 0
				if(result[pos] > 0.0f) result[pos] += (0.5f + ((i + 1.0f) * 0.01f));
				// Иначе устанавливаем значение вектора по умолчанию
				else result[pos] = (1.0f + ((i + 1.0f) * 0.01f));
			}
		}
	}
	// Выводим результат
	return result;
}
/**
 * correctly Метод проверки на верность слова
 * @param  word слово для проверки
 * @return      результат проверки
 */
const bool anyks::Dict::correctly(const word_t & word) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если слово передано
	if(!word.empty() && (word.length() <= MAX_WORD_LENGTH)){
		// Получаем первый символ слова
		const wchar_t first = word.front();
		// Определяем является ли последний символ дефисом
		const bool isHyphen = ((first == L'-') || (word.back() == L'-'));
		// Проверяем является ли первая буква слова числом
		result = !this->alphabet->isNumber(wstring(1, first));
		// Если длина слова больше 1-го символа
		if(word.length() > 1){
			// Если это число
			if(!result){
				// Если это сокращение и последний символ не дефис
				result = (!isHyphen && (word.find(L"-") != word_t::npos));
			// Проследим чтобы это был не дефис и не римская цифра
			} else result = (!isHyphen && (this->alphabet->roman2Arabic(word) < 1));
		// Если буква всего одна, проследим чтобы это был не дефис
		} else result = (result && !isHyphen);
	}
	// Выводим результат
	return result;
}
/**
 * isOption Метод проверки наличия опции
 * @param option опция для проверки
 * @return       результат проверки
 */
const bool anyks::Dict::isOption(const options_t option) const noexcept {
	// Выводим результат
	return this->options.test((u_short) option);
}
/**
 * clear Метод очистки словаря
 */
void anyks::Dict::clear() noexcept {
	// Очищаем параметры словаря
	this->params = {};
	// Очищаем объект индекса
	this->hnsw.clear();
	// Очищаем список слов
	this->words.clear();
	// Очищаем словарь
	this->lemmas.clear();
	// Очищаем список эмбеддингов
	this->embedding.clear();
	// Сбрасываем максимальное количество вариантов
	this->setNSWLibCount();
	// Сбрасываем размер эмбеддинга
	this->setSizeEmbedding();
}
/**
 * train Метод обучения
 * @param status функция вывода статуса модификации
 */
void anyks::Dict::train(function <void (const u_short)> status) noexcept {
	// Если нужно сгенерировать индекс
	if(!this->words.empty() && !this->embedding.empty() &&
	this->hnsw.empty() && !this->isOption(options_t::onlytypos)){
		// Индекс перехода по списку
		size_t index = 0;
		// Текущий и предыдущий статус
		u_short actual = 0, past = 100;
		// Получаем количество слов в словаре
		const size_t count = this->words.size();
		// Если количество слов меньше или равно 1000, тогда обрабатываем в один заход
		if(count <= this->nswlibCount){
			// Список идентификаторов
			vector <size_t> ids(count);
			// Список эмбеддингов
			vector <vector <float>> data(count);
			// Переходим по всему списку слов
			for(auto & word : this->words){
				// Добавляем в список идентификатор
				ids[index] = word.first;
				// Добавляем в список эмбеддинг
				data[index] = this->vec(word.second);
				// Увеличиваем индекс смещения
				index++;
				// Если функция вывода статуса передана
				if(status != nullptr){
					// Выполняем расчёт текущего статуса
					actual = u_short(index / double(count) * 100.0);
					// Если статус обновился
					if(actual != past){
						// Запоминаем текущий статус
						past = actual;
						// Выводим статус извлечения
						status(actual);
					}
				}
			}
			// Если списки равнозначные
			if(ids.size() == data.size()){
				// Активируем индекс
				this->hnsw.init(NSWLIB_TYPE, this->sizeEmbedding);
				// Создаём индекс
				this->hnsw.index(data.size());
				// Добавляем в индекс данные
				this->hnsw.add(data, ids);
				// Устанавливаем сложность алгоритма
				this->hnsw.ef(NSWLIB_ACCURACY);
			}
		// Иначе выполняем вставки итерациями
		} else {
			// Список идентификаторов
			vector <size_t> ids;
			// Список эмбеддингов
			vector <vector <float>> data;
			// Получаем текущее значение итератора
			auto it = this->words.cbegin();
			// Размер блока, смещение и шаг
			size_t size = 0, step = this->nswlibCount;
			// Разбиваем вставки по MAX элементов
			while(index < count){
				// Получаем размер блока данных
				size = (count - index);
				// Выполняем корректировку размера
				size = (size >= step ? step : size);
				// Очищаем список идентификаторов
				ids.clear();
				// Очищаем список эмбеддингов
				data.clear();
				// Переходим по всему списку слов
				for(size_t i = index; i < count; i++){
					// Если вектор получен
					if(it != this->words.cend()){
						// Получаем эмбеддинг слова
						const auto & embedding = this->vec(it->second);
						// Если размер эмбеддинга соответствует
						if(embedding.size() == this->sizeEmbedding){
							// Добавляем в список идентификатор
							ids.push_back(it->first);
							// Добавляем в список эмбеддинг
							data.push_back(embedding);
							// Увеличиваем индекс смещения
							index++;
						}
						// Увеличиваем значение итератора
						it++;
						// Если кратность достигла предела, выходим
						if((index % step) == 0) break;
					// Выходим из цикла
					} else break;
				}
				// Если списки равнозначные
				if(!data.empty() && (ids.size() == data.size())){
					// Если индекс не инициализирован
					if(this->hnsw.empty()){
						// Активируем индекс
						this->hnsw.init(NSWLIB_TYPE, this->sizeEmbedding);
						// Создаём индекс
						this->hnsw.index(index);
					// Иначе перераспределяем индекс
					} else this->hnsw.resize(index);
					// Добавляем в индекс данные
					this->hnsw.add(data, ids);
					// Устанавливаем сложность алгоритма
					this->hnsw.ef(NSWLIB_ACCURACY);
					// Если функция вывода статуса передана
					if(status != nullptr){
						// Выполняем расчёт текущего статуса
						actual = u_short(index / double(count) * 100.0);
						// Если статус обновился
						if(actual != past){
							// Запоминаем текущий статус
							past = actual;
							// Выводим статус извлечения
							status(actual);
						}
					}
				}
				// Если мы дошли до конца, выходим из цикла
				if(it == this->words.cend()) break;
			}
		}
		// Выводим статус
		if(status != nullptr) status(100);
	}
}
/**
 * generateEmbedding Метод генерации эмбеддинга
 */
void anyks::Dict::generateEmbedding() noexcept {
	// Получаем данные алфавита
	const wstring & alphabet = this->alphabet->wget();
	// Если алфавит получен
	if(!alphabet.empty()){
		// Индекс в алфавите
		size_t i = 0, j = 0;
		// Очищаем собранный ранее Эмбеддинг
		this->embedding.clear();
		// Получаем количество букв в алфавите
		const size_t length = alphabet.length();
		// Выполняем сборку алфавита до тех пор, пока не соберём всё
		while(i < length){
			// Устанавливаем алфавит
			this->embedding.emplace(alphabet.at(i), j);
			// Если мы дошли до конца длины эмбеддинга, начинаем заново
			if(j == (this->sizeEmbedding - 3)) j = 0; else j++;
			// Увеличиваем количество обработанных букв
			i++;
		}
		// Извлекаем список разрешённых символов
		const auto & symbols = this->alphabet->getAllowed();
		// Если список разрешённых символов получен
		if(!symbols.empty()){
			// Добавляем все разрешённые символы
			for(auto & letter : symbols){
				// Если это цифра
				if(this->alphabet->isNumber(wstring(1, letter)))
					// Добавляем цифры
					this->embedding.emplace(letter, this->sizeEmbedding - 1);
				// Добавляем спецсимволы
				else this->embedding.emplace(letter, this->sizeEmbedding - 2);
			}
		}
	}
}
/**
 * bloom Метод инициализации фильтра Блума
 * @param status функция вывода статуса обработки
 */
void anyks::Dict::bloom(function <void (const u_short)> status) noexcept {
	// Если разрешено использовать фильтр Блума
	if(this->isOption(options_t::bloom) && !this->words.empty()){
		// Размер буфера слова
		size_t index = 0;
		// Текущий статус
		u_short actual = 0, past = 100;
		// Если фильтр Блума уже создан
		if(this->filter != nullptr) delete this->filter;
		// Выполняем создание фильтра Блума
		this->filter = new bloom_t(this->words.size(), 0.001);
		// Переходим по всему списку слов и добавляем в фильтр Блума
		for(auto & item : this->words){
			// Добавляем слово в фильтр Блума
			if(item.second.length() > 3) this->filter->insert(item.second.str());
			// Выводим статус
			if(status != nullptr){
				// Выполняем расчет статуса
				actual = ceil(index / double(this->words.size()) * 100.0);
				// Если статус больше 100, уменьшаем его
				actual = u_short(actual > 99 ? 99 : actual);
				// Если статус обновился
				if(actual != past){
					// Запоминаем текущий статус
					past = actual;
					// Выводим статус загрузки
					status(actual);
				}
			}
			// Увеличиваем значение индекса
			index++;
		}
		// Выводим статус окончательной загрузки
		if(status != nullptr) status(100);
	}
}
/**
 * stemming Метод инициализации стемминга
 * @param status функция вывода статуса обработки
 */
void anyks::Dict::stemming(function <void (const u_short)> status) noexcept {
	// Если разрешено использовать фильтр Блума
	if(this->isOption(options_t::stemming) && !this->words.empty()){
		// Размер буфера слова
		size_t index = 0;
		// Текущий статус
		u_short actual = 0, past = 100;
		// Переходим по всему списку слов и добавляем лемму
		for(auto & item : this->words){
			// Если слово больше 3-х символов
			if(item.second.length() > 3){
				// Получаем лемму слова
				const wstring & lemma = this->stem(item.second);
				// Если лемма не совпадает со словом
				if(item.second.compare(lemma) != 0) this->addLemma(lemma);
			}
			// Выводим статус
			if(status != nullptr){
				// Выполняем расчет статуса
				actual = ceil(index / double(this->words.size()) * 100.0);
				// Если статус больше 100, уменьшаем его
				actual = u_short(actual > 99 ? 99 : actual);
				// Если статус обновился
				if(actual != past){
					// Запоминаем текущий статус
					past = actual;
					// Выводим статус загрузки
					status(actual);
				}
			}
			// Увеличиваем значение индекса
			index++;
		}
		// Выводим статус окончательной загрузки
		if(status != nullptr) status(100);
	}
}
/**
 * addUWord Метод добавления слова, которое всегда начинается с заглавной буквы
 * @param word слово для добавления
 */
void anyks::Dict::addUWord(const string & word) noexcept {
	// Устанавливаем слово
	this->addUWord(this->alphabet->convert(word));
}
/**
 * addUWord Метод добавления слова, которое всегда начинается с заглавной буквы
 * @param word слово для добавления
 */
void anyks::Dict::addUWord(const wstring & word) noexcept {
	// Устанавливаем слово
	this->alt.addUWord(word);
}
/**
 * addUWord Метод добавления идентификатора слова, которое всегда начинается с заглавной буквы
 * @param idw идентификатор слова для добавления
 * @param ups регистры добавляемого слова
 */
void anyks::Dict::addUWord(const size_t idw, const size_t ups) noexcept {
	// Добавляем идентификатор слова
	this->alt.addUWord(idw, ups);
}
/**
 * setUWords Метод добавления списка идентификаторов слов, которые всегда начинаются с заглавной буквы
 * @param words список идентификаторов слов для добавления
 */
void anyks::Dict::setUWords(const map <size_t, size_t> & words) noexcept {
	// Устанавливаем список слов
	this->alt.setUWords(words);
}
/**
 * setLMethod Метод установки функции получения леммы
 * @param fn функция для установки
 */
void anyks::Dict::setLMethod(stemmer_t::stemming_t fn) noexcept {
	// Устанавливаем функцию стемминга
	if(fn != nullptr) this->stemmer->setLMethod(fn);
}
/**
 * setVMethod Метод установки функции подбора вариантов
 * @param fn функция для установки
 */
void anyks::Dict::setVMethod(stemmer_t::variants_t fn) noexcept {
	// Устанавливаем функцию подбора вариантов
	if(fn != nullptr) this->stemmer->setVMethod(fn);
}
/**
 * setALM Метод установки языковой модели
 * @param alm объект языковой модели
 */
void anyks::Dict::setALM(alm_t * alm) noexcept {
	// Если языковая подель передана
	if(alm != nullptr){
		// Устанавливаем языковую модель
		this->alm = alm;
		// Устанавливаем функции получения и установки слова
		this->alm->setWordFn([this](const size_t idw){
			// Выводим результат
			return this->word(idw);
		}, [this](const size_t idw, const word_t & word){
			// Устанавливаем слово в словарь
			this->add(idw, word);
			// Добавляем альтернативное слово
			this->alt.add(word, true, idw);
		});
// Если работа идет не изнутри Python
#ifndef NOPYTHON
		// Получаем объект внешний объект питона
		const python_t * obj = this->stemmer->getPythonObj();
		// Устанавливаем объект питона
		if(obj != nullptr) this->alm->setPythonObj(const_cast <python_t *> (obj));
#endif
	}
}
/**
 * setDate Метод установки даты генерации словаря
 * @param date дата генерации словаря
 */
void anyks::Dict::setDate(const time_t date) noexcept {
	// Если дата передана, устанавливаем её
	if(date > 0) this->params.date = date;
}
/**
 * setCode Метод установки кода языка
 * @param code код языка словаря для установки
 */
void anyks::Dict::setCode(const wstring & code) noexcept {
	// Если код языка передан
	if(!code.empty()) this->params.code = this->alphabet->toLower(code);
}
/**
 * setName Метод установки названия словаря
 * @param name название словаря
 */
void anyks::Dict::setName(const wstring & name) noexcept {
	// Если имя передано
	if(!name.empty()) this->params.name = name;
}
/**
 * setPilot Метод установки пилотного слова
 * @param letter буква для установки
 */
void anyks::Dict::setPilot(const wchar_t letter) noexcept {
	// Добавляем букву в список
	if(letter > 0) this->pilots.emplace(letter);
}
/**
 * setLogfile Метод установки файла для вывода логов
 * @param logifle адрес файла для вывода отладочной информации
 */
void anyks::Dict::setLogfile(const char * logfile) noexcept {
	// Устанавливаем адрес log файла
	this->logfile = logfile;
}
/**
 * setOption Метод установки опций модуля
 * @param option опция для установки
 */
void anyks::Dict::setOption(const options_t option) noexcept {
// Если работа идет не изнутри Python
#ifndef NOPYTHON
	// Если это режим отладки
	if(option == options_t::debug){
		// Устанавливаем режим отладки модуля питона
		const_cast <python_t *> (this->stemmer->getPythonObj())->setDebug();
	}
#endif
	// Устанавливаем опции
	this->options.set((u_short) option);
}
/**
 * setAuthor Метод установки автора словаря
 * @param author автор словаря
 */
void anyks::Dict::setAuthor(const wstring & author) noexcept {
	// Если автор передан
	if(!author.empty()) this->params.author = author;
}
/**
 * setLScript Метод установки скрипта лемматизатора
 * @param script python скрипт для установки
 */
void anyks::Dict::setLScript(const string & script) noexcept {
	// Если скрипт передан, устанавливаем его
	if(!script.empty()) this->stemmer->setLScript(script);
}
/**
 * setVScript Метод установки скрипта подбора вариантов
 * @param script python скрипт для установки
 */
void anyks::Dict::setVScript(const string & script) noexcept {
	// Если скрипт передан, устанавливаем его
	if(!script.empty()) this->stemmer->setVScript(script);
}
/**
 * setLictype Метод установки типа лицензионной информации словаря
 * @param type тип лицензионного соглашения
 */
void anyks::Dict::setLictype(const wstring & type) noexcept {
	// Если тип лицензии передан
	if(!type.empty()) this->params.lictype = type;
}
/**
 * unsetOption Метод отключения опции модуля
 * @param option опция для отключения
 */
void anyks::Dict::unsetOption(const options_t option) noexcept {
	// Устанавливаем опции
	this->options.reset((u_short) option);
}
/**
 * setLictext Метод установки лицензионной информации словаря
 * @param license лицензионная информация словаря
 */
void anyks::Dict::setLictext(const wstring & license) noexcept {
	// Если лицензия передана
	if(!license.empty()) this->params.lictext = license;
}
/**
 * setThreads Метод установки количества потоков
 * @param threads количество потоков для работы
 */
void anyks::Dict::setThreads(const size_t threads) noexcept {
	// Устанавливаем новое количество потоков
	this->threads = (threads > 0 ? threads : thread::hardware_concurrency());
	// Устанавливаем ядра в индекс
	this->hnsw.setThreads(this->threads);
}
/**
 * setContacts Метод установки контактных данных автора словаря
 * @param contacts контактные данные автора словаря
 */
void anyks::Dict::setContacts(const wstring & contacts) noexcept {
	// Если контактные данные переданы
	if(!contacts.empty()) this->params.contacts = contacts;
}
/**
 * setNSWLibCount Метод установки максимального количества вариантов для анализа
 * @param count максимальное количество вариантов для анализа
 */
void anyks::Dict::setNSWLibCount(const size_t count) noexcept {
	// Если количество вариантов для анализа установлено
	if(count > 0) this->nswlibCount = count;
	// Если количество вариантов меньше установленных
	else this->nswlibCount = NSWLIB_COUNT;
}
/**
 * setCopyright Метод установки авторских прав на словарь
 * @param copyright авторские права на словарь
 */
void anyks::Dict::setCopyright(const wstring & copyright) noexcept {
	// Если копирайт передан
	if(!copyright.empty()) this->params.copyright = copyright;
}
/**
 * setSizeEmbedding Метод установки размера эмбеддинга
 * @param size размер эмбеддинга
 */
void anyks::Dict::setSizeEmbedding(const u_short size) noexcept {
/*
 * Определяем максимальный размер эмбеддинга
 */
#ifdef IS64BIT
	// Для 64-х битного процессора
	const u_short maxSize = 64;
#else
	// Для 32-х битного процессора
	const u_short maxSize = 32;
#endif
	// Запоминаем размер эмбеддинга
	if(size > 0)
		// Устанавливаем размер эмбеддинга
		this->sizeEmbedding = (size < maxSize ? size : maxSize);
	// Если размер эмбеддинга не передан, устанавливаем по умолчанию
	else this->sizeEmbedding = maxSize;
}
/**
 * setPilots Метод установки пилотных слов
 * @param letters список пилотных слов
 */
void anyks::Dict::setPilots(const set <wchar_t> & letters) noexcept {
	// Если список слов передан
	if(!letters.empty()) this->pilots = letters;
}
/**
 * setAlphabet Метод установки словаря
 * @param alphabet алфавит словаря
 */
void anyks::Dict::setAlphabet(const alphabet_t * alphabet) noexcept {
	// Если словарь передан
	if(alphabet != nullptr){
		// Устанавливаем алфавит
		this->alphabet = alphabet;
		// Устанавливаем алфавит в модуле альтернативных слов
		this->alt.setAlphabet(this->alphabet);
		// Устанавливаем алфавит в модуле стемминга
		this->stemmer->setAlphabet(this->alphabet);
		// Если эмбеддинг пустой
		if(this->embedding.empty()){
			// Устанавливаем размер эмбеддинга
			this->setSizeEmbedding();
			// Выполняем генерацию эмбеддинга
			this->generateEmbedding();
		}
	}
}
/**
 * setAdCw Метод установки характеристик словаря
 * @param cw количество обработанных слов
 * @param ad количество обработанных документов
 */
void anyks::Dict::setAdCw(const size_t cw, const size_t ad) noexcept {
	// Если статистика словаря передана, устанавливаем
	if(cw > 0) this->params.cw = cw;
	if(ad > 0) this->params.ad = ad;
}
/**
 * setTokenizer Метод установки токенизатора
 * @param tokenizer объект токенизатора
 */
void anyks::Dict::setTokenizer(const tokenizer_t * tokenizer) noexcept {
	// Устанавливаем объект токенизатора
	if(tokenizer != nullptr){
		// Устанавливаем токенизатор
		this->tokenizer = tokenizer;
		// Устанавливаем токенизатор в модуле альтернативных слов
		this->alt.setTokenizer(this->tokenizer);
		// Устанавливаем токенизатор для стеммера
		this->stemmer->setTokenizer(this->tokenizer);
	}
}
/**
 * setEmbedding Метод установки эмбеддинга
 * @param embedding эмбеддинг словаря
 * @param size      размер эмбеддинга
 */
void anyks::Dict::setEmbedding(const json & embedding, const u_short size) noexcept {
	// Если данные переданы
	if(!embedding.empty() && (size > 0)){
		// Удаляем предыдущий эмбеддинг
		this->embedding.clear();
		// Запоминаем размер эмбеддинга
		this->setSizeEmbedding(size);
		// Переходим по всему списку эмбеддинга
		for(auto & item : embedding.items()){
			// Устанавливаем эмбеддинг
			this->embedding.emplace(this->alphabet->convert(item.key())[0], item.value());
		}
	}
}
/**
 * addLemma Метод добавления леммы в словарь
 * @param lemma лемма для добавления
 */
void anyks::Dict::addLemma(const wstring & lemma) noexcept {
	// Если лемма не пустая
	if(!lemma.empty() && this->isOption(options_t::stemming)){
		// Итератор для подсчета длины n-граммы
		u_short i = 0;
		// Копируем основную карту
		lemma_t * obj = &this->lemmas;
		// Переходим по всему объекту
		for(auto & letter : lemma){
			// Добавляем букву в словарь
			auto ret = obj->emplace(letter, lemma_t());
			// Получаем блок структуры
			obj = &ret.first->second;
			// Если мы дошли до конца
			if(i == (lemma.length() - 1)) obj->emplace(this->elid, lemma_t());
			// Увеличиваем количество обработанных букв
			++i;
		}
	}
}
/**
 * addAlt Метод добавления альтернативной буквы
 * @param letter буква у которой есть альтернатива
 * @param alt    альтернативная буква
 */
void anyks::Dict::addAlt(const wchar_t letter, const wchar_t alt) noexcept {
	// Если буква и альтернативная буква переданы
	if((letter > 0) && (alt > 0)) this->alt.add(letter, alt);
}
/**
 * addAlt Метод добавления слова с альтернативной буквой
 * @param word    слово для добавления
 * @param alt     альтернативное слово для добавления
 * @param context слово является контексто-зависимым
 */
void anyks::Dict::addAlt(const wstring & word, const wstring & alt, const bool context) noexcept {
	// Если слово и альтернативное слово передано
	if(!word.empty() && !alt.empty()) this->alt.add(word, alt, context);
}
/**
 * addAlt Метод добавления слова с альтернативной буквой
 * @param word    слово для добавления
 * @param idw     идентификатор слова если есть
 * @param context слово является контексто-зависимым
 */
void anyks::Dict::addAlt(const wstring & word, const size_t idw, const bool context) noexcept {
	// Если альтернативное слово передано
	if(!word.empty()) this->alt.add(word, idw, context);
}
/**
 * find Метод поиска вариантов подходящих вариантов слов
 * @param  word слово для проверки
 * @param  dmp  сборщик результатов
 * @param  size размер последовательности, куда нужно добавить слово
 * @return      размер полученной последовательности и количество добавленных вариантов
 */
const pair <size_t, size_t> anyks::Dict::find(const word_t & word, dumper_t & dmp, const size_t size) const noexcept {
	// Результат работы функции
	pair <size_t, size_t> result = {0, 0};
	// Если список слов не пустой
	if(!this->words.empty() && (word.length() <= MAX_WORD_LENGTH)){
		// Объект левенштейна
		lev_t algorithms;
		// Создаём гипотезу
		dumper_t::awrd_t hypothesis;
		// Устанавливаем эталонную фразу
		hypothesis.etalon = word.wreal();
		// Выполняем получение максимального количества ошибок для слова
		const u_short errors = this->alphabet->errors(word);
		// Если количество ошибок больше 0
		if(errors > 0){
			/**
			 * findFn Функция извлечения слова из базы
			 * @param idw идентификатор слова
			 */
			auto findFn = [&](const size_t idw){
				// Получаем данные слова
				auto it = this->words.find(idw);
				// Если такое слово существует
				if(it != this->words.end()){
					// Выполняем блокировки потока
					this->locker.lock();
					// Устанавливаем идентификатор гипотезы
					hypothesis.idw = idw;
					// Получаем дистанцию
					hypothesis.lev = (errors > 1 ? algorithms.distance(word, it->second) : algorithms.damerau(word, it->second));
					// Отфильтровываем ненужные нам слова
					if(!it->second.empty() && (hypothesis.lev <= errors)){
						// Извлекаем слово из списка
						hypothesis.word = it->second;
						// Устанавливаем значение Танимото
						hypothesis.tmo = algorithms.tanimoto(word, hypothesis.word);
						// Если расстояние Левенштейна слишком большое, тогда Танимото должен быть больше 4.0
						if((hypothesis.lev <= 3) || (hypothesis.tmo >= 0.4)){
							// Если вывод отладочной информации разрешён
							if(this->isOption(options_t::debug)){
								// Выводим основное сообщение отладки
								this->alphabet->log("find word: [%s => %s]\r\n", alphabet_t::log_t::info, this->logfile, word.real().c_str(), hypothesis.word.real().c_str());
							}
							// Добавляем вариант в дампер
							const auto & res = dmp.smart(hypothesis, size);
							// Если вариант добавлен, запоминаем позицию
							if(res.second > 0){
								// Увеличиваем количество добавленных вариантов
								result.second++;
								// Запоминаем новую позицию
								result.first = res.first;
							}
						}
					}
					// Выполняем разблокировки потока
					this->locker.unlock();
				}
			};
			// Если нужно выполнять исправление опечаток
			if(this->isOption(options_t::onlytypos)){
				// Получаем список всех возможных векторов слова
				auto vectors = this->vecsb(word);
				// Если список получен
				if(!vectors.empty()){
					// Выполняем инициализацию пула потоков
					this->start();
					// Переходим по всему списку векторов
					for(auto & vector : vectors){
						// Получаем диапазон значений вектора
						auto ret = this->vectors.equal_range(vector);
						// Перебираем весь диапазон полученных векторов
						for(auto it = ret.first; it != ret.second; ++it){
							// Выполняем добавление полученного слова
							this->tpool->push(findFn, it->second);
						}
					}
					// Ожидаем завершения работы всех потоков
					this->finish();
				}
			// Если hnsw загружен
			} else if(!this->hnsw.empty()) {
				// Создаёс эмбеддинг слова
				const auto & embedding = this->vec(word);
				// Если эмбеддинг получен
				if(!embedding.empty()){
					// Множитель для расчёта максимального количества вариантов
					vector <double> factor = {0.25, 0.5, 0.75, 1};
					// Получаем количество элементов для выдачи
					const size_t count = pow(10, floor(log10(words.size())));
					// Получаем количество максимаольно-возможных вариантов для рассмотрения
					const size_t nswlibCount = (factor.at(errors - 1) * this->nswlibCount);
					// Запрашиваем nswlibCount вариантов
					const auto & res = this->hnsw.query({embedding}, (count < nswlibCount ? count : nswlibCount));
					// Если список вариантов получен
					if(!res.first.empty()){
						// Выполняем инициализацию пула потоков
						this->start();
						// Переходим по всему списку вариантов
						for(auto & idw : res.first) this->tpool->push(findFn, idw);
						// Ожидаем завершения работы всех потоков
						this->finish();
					}
				}
			}
		}
	}
	// Выводим результат
	return result;
}
/**
 * saveIndex Метод сохранения индекса в бинарном виде
 * @param buffer буфер данных индекса
 */
void anyks::Dict::saveIndex(vector <char> & buffer) const noexcept {
	// Очищаем буфер для сборка данных
	buffer.clear();
	// Выполняем извлечение буфера данных индекса
	if(!this->hnsw.empty()){
		// Буфер данных слов
		vector <char> data;
		// Выводим количество элементов в буфере
		const size_t elements = this->hnsw.getCurrentCount();
		// Получаем бинарные данные количества слов
		const char * bin = reinterpret_cast <const char *> (&elements);
		// Добавляем в буфер количество слов
		data.insert(data.end(), bin, bin + sizeof(elements));
		// Выполняем извлечение буфера данных
		this->hnsw.save(buffer);
		// Выполняем объединение двух буферов
		buffer.insert(buffer.begin(), data.begin(), data.end());
	}
}
/**
 * loadIndex Метод загрузки индекса в бинарном виде
 * @param buffer буфер данных индекса
 */
void anyks::Dict::loadIndex(const vector <char> & buffer) noexcept {
	// Если буфер получен
	if(!buffer.empty() && !this->isOption(options_t::onlytypos)){
		// Количество элементов и смещение в буфере
		size_t elements = 0, offset = 0;
		// Получаем данные буфера
		const char * data = buffer.data();
		// Извлекаем количество слов в буфере
		memcpy(&elements, data + offset, sizeof(elements));
		// Увеличиваем смещение
		offset += sizeof(elements);
		// Если количество элементов в буфере больше 0
		if(elements > 0){
			// Загружаем индекс
			this->hnsw.load(data + offset, elements);
			// Устанавливаем сложность алгоритма
			this->hnsw.ef(NSWLIB_ACCURACY);
		// Выводим сообщение, что индекс не получен
		} else this->alphabet->log("index data %s\r\n", alphabet_t::log_t::error, this->logfile, "is broken");
	}
}
/**
 * add Метод добавления слова в словарь
 * @param idw   идентификатор слова
 * @param word  слово для добавления
 * @param train нужно ли выполнить обучение
 */
void anyks::Dict::add(const size_t idw, const word_t & word, const bool train) noexcept {
	// Если данные переданы
	if((idw > 0) && !word.empty() && (word.length() <= MAX_WORD_LENGTH)){
		// Добавляем слово в хранилище слов
		this->words.emplace(idw, word);
		// Если нужно выполнять исправление опечаток
		if(this->isOption(options_t::onlytypos)){
			// Добавляем слово в список векторов
			this->vectors.emplace(this->vecb(word), idw);
		// Если нужно сгенерировать индекс
		} else if(train && !this->words.empty() && !this->embedding.empty()) {
			// Получаем эмбеддинг слова
			auto embedding = this->vec(word);
			// Если индекс не инициализирован
			if(this->hnsw.empty()){
				// Активируем индекс
				this->hnsw.init(NSWLIB_TYPE, this->sizeEmbedding);
				// Создаём индекс
				this->hnsw.index(this->words.size());
			// Иначе перераспределяем индекс
			} else this->hnsw.resize(this->words.size());
			// Добавляем в индекс данные
			this->hnsw.add({embedding}, {idw});
			// Устанавливаем сложность алгоритма
			this->hnsw.ef(NSWLIB_ACCURACY);
		}
	}
}
/**
 * readVocab Метод чтения словаря из файла
 * @param filename файл для чтения словаря
 * @param status   функция вывода статуса чтения
 */
void anyks::Dict::readVocab(const string & filename, function <void (const u_short)> status) noexcept {
	// Если адрес файла передан
	if(!filename.empty() && fsys_t::isfile(filename)){
		// Тип извлечения данных
		u_short type = 0;
		// Текущий и предыдущий статус
		u_short actual = 0, past = 100;
		// Количество обработанных данных
		size_t index = 0, pos = 0, loc = 0;
		// Выполняем считывание всех строк текста
		fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize) noexcept {
			// Если слово получено
			if(!text.empty()){
				// Определяем тип считываемых данных
				switch(type){
					// Если получено обозначение заголовка
					case 0: if(text.find("\\data\\") != string::npos) type++; break;
					// Если это тип считывания статистики
					case 1: {
						// Если мы дошли до считывания данных слов
						if(text.find("\\words:") != string::npos) type++;
						// Пытаемся прочитать статистику
						else if((pos = text.find("=")) != string::npos){
							// Извлекаем ключ
							const string & key = this->alphabet->trim(text.substr(0, pos));
							// Получаем значение записи
							const string & val = this->alphabet->trim(text.substr(pos + 1, text.length() - (pos + 1)));
							// Если это общее количество документов
							if(key.compare("ad") == 0) this->params.ad += stoull(val);
							// Если же это количество слов во всех документах
							else if(key.compare("cw") == 0) this->params.cw += stoull(val);
						}
					} break;
					// Если это тип считывания данных слова
					case 2: {
						// Если мы дошли до конца, выходим
						if(text.find("\\end\\") != string::npos) break;
						// Иначе считываем данные слова
						else if((pos = text.find("\t")) != string::npos){
							// Обнуляем локальную позицию
							loc = 0;
							// Считываем идентификатор слова
							const string & idWord = this->alphabet->trim(text.substr(loc, pos));
							// Запоминаем текущую позицию
							loc = pos;
							// Ищем значение слова
							if((pos = text.find("\t", pos + 1)) != string::npos){
								// Извлекаем данные слова
								word_t word = this->alphabet->trim(text.substr(loc + 1, pos - (loc + 1)));
								// Запоминаем текущую позицию
								loc = pos;
								// Ищем встречаемость слова
								if((pos = text.find("|", pos + 1)) != string::npos){
									// Извлекаем встречаемость слова
									const string & oc = this->alphabet->trim(text.substr(loc + 1, pos - (loc + 1)));
									// Запоминаем текущую позицию
									loc = pos;
									// Ищем встречаемость в документах
									if((pos = text.find("|", pos + 1)) != string::npos){
										// Получаем идентификатор слова
										const size_t idw = stoull(idWord);
										// Количество документов где встретилось слово
										const string & dc = this->alphabet->trim(text.substr(loc + 1, pos - (loc + 1)));
										// Выполняем поиск слова в словаре
										auto it = this->words.find(idw);
										// Если слово найдено
										if(it != this->words.end()){
											// Получаем метаданные слова
											auto meta = it->second.getmeta();
											// Добавляем в слово новые метаданные
											it->second.setmeta(meta.dc + stoull(dc), meta.oc + stoull(oc));
										// Если слово в словаре не найдено
										} else {
											// Добавляем в слово его метаданные
											word.setmeta(stoull(dc), stoull(oc));
											// Добавляем слово в словарь
											this->words.emplace(idw, word);
											// Если нужно выполнять исправление опечаток
											if(this->isOption(options_t::onlytypos)){
												// Добавляем слово в список векторов
												this->vectors.emplace(this->vecb(word), idw);
											}
										}
									}
								}
							}
						}
					} break;
				}
			}
			// Если функция вывода статуса передана
			if(status != nullptr){
				// Увеличиваем количество записанных n-грамм
				index += text.size();
				// Выполняем расчёт текущего статуса
				actual = u_short(index / double(fileSize) * 100.0);
				// Если статус обновился
				if(actual != past){
					// Запоминаем текущий статус
					past = actual;
					// Выводим статус извлечения
					status(actual);
				}
			}
		});
	// Выводим сообщение об ошибке
	} else this->alphabet->log("%s\r\n", alphabet_t::log_t::error, this->logfile, "vocab file is not exist");
}
/**
 * save Метод сохранения индекса
 * @param buffer буфер данных индекса
 * @param all    флаг предписывающий снятие полного дампа
 * @param status функция вывода статуса обработки
 */
void anyks::Dict::save(vector <char> & buffer, const bool all, function <void (const u_short)> status) const noexcept {
	// Очищаем буфер для сборка данных
	buffer.clear();
	// Выполняем извлечение буфера данных индекса
	if(all && !this->hnsw.empty()){
		// Буфер данных слов
		vector <char> data;
		// Выводим количество элементов в буфере
		const size_t elements = this->hnsw.getCurrentCount();
		// Получаем бинарные данные количества слов
		const char * bin = reinterpret_cast <const char *> (&elements);
		// Добавляем в буфер количество слов
		data.insert(data.end(), bin, bin + sizeof(elements));
		// Выполняем извлечение буфера данных
		this->hnsw.save(buffer);
		// Выполняем объединение двух буферов
		buffer.insert(buffer.begin(), data.begin(), data.end());
	}
	// Если буфер получен и список слов заполнен
	if((!all || !buffer.empty()) && !this->words.empty()){
		// Обрабатываемое слово
		word_t word = L"";
		// Буфер данных слов
		vector <char> data, dump;
		// Размер буфера слова
		size_t size = 0, index = 0;
		// Текущий статус
		u_short actual = 0, past = 100;
		// Выводим количество элементов в буфере
		const size_t elements = this->words.size();
		// Получаем бинарные данные количества слов
		const char * bin = reinterpret_cast <const char *> (&elements);
		// Добавляем в буфер количество слов
		data.insert(data.end(), bin, bin + sizeof(elements));
		// Переходим по всем словам
		for(auto & item : this->words){
			// Получаем слово
			word = item.second;
			// Получаем дамп слова
			dump = word.dump();
			// Получаем размер буфера слова
			size = dump.size();
			// Создаём бинарные данные идентификатора слова
			bin = reinterpret_cast <const char *> (&item.first);
			// Добавляем в буфер идентификатор слова
			data.insert(data.end(), bin, bin + sizeof(item.first));
			// Создаём бинарные данные размера блока слова
			bin = reinterpret_cast <const char *> (&size);
			// Добавляем в буфер размер дампа слова
			data.insert(data.end(), bin, bin + sizeof(size));
			// Добавляем в буфер данные слова
			data.insert(data.end(), dump.begin(), dump.end());
			// Выводим статус
			if(status != nullptr){
				// Выполняем расчет статуса
				actual = ceil(index / double(elements) * 100.0);
				// Если статус больше 100, уменьшаем его
				actual = u_short(actual > 99 ? 99 : actual);
				// Если статус обновился
				if(actual != past){
					// Запоминаем текущий статус
					past = actual;
					// Выводим статус загрузки
					status(actual);
				}
			}
			// Увеличиваем значение индекса
			index++;
		}
		// Объединяем полученные данные
		if(!buffer.empty() && !data.empty())
			// Выполняем объединение двух буферов
			buffer.insert(buffer.begin(), data.begin(), data.end());
		// Выполняем выдачу только бинарного буфера слов
		else if(!data.empty()) buffer.assign(data.begin(), data.end());
		// Выводим статус
		if(status != nullptr) status(100);
	}
}
/**
 * load Метод загрузки индекса
 * @param buffer буфер данных индекса
 * @param all    флаг предписывающий загрузки полного дампа
 * @param status функция вывода статуса обработки
 */
void anyks::Dict::load(const vector <char> & buffer, const bool all, function <void (const u_short)> status) noexcept {
	// Если буфер индекса передан
	if(!buffer.empty()){
		// Слово для установки
		word_t word = L"";
		// Получаем данные буфера
		const char * data = buffer.data();
		// Количество элементов в буфере, смещение, размер блок слова и его идентификатор
		size_t elements = 0, offset = 0, size = 0, idw = 0;
		// Извлекаем количество слов в буфере
		memcpy(&elements, data + offset, sizeof(elements));
		// Увеличиваем смещение
		offset += sizeof(elements);
		// Если количество слов получено
		if(elements > 0){
			// Индекс обработанных данных
			size_t index = 0;
			// Очищаем индекс
			this->hnsw.clear();
			// Выделяем память для списка слов
			this->words.reserve(elements);
			// Текущий статус
			u_short actual = 0, past = 100;
			// Активируем индекс
			this->hnsw.init(NSWLIB_TYPE, this->sizeEmbedding);
			/**
			 * setWordFn Функция добавления слова в память
			 * @param idw  идентификатор слова
			 * @param word слово для добавления
			 */
			auto setWordFn = [&](const size_t idw, const word_t word){
				// Выполняем блокировки потока
				this->locker.lock();
				// Увеличиваем значение индекса
				index++;
				// Если идентификатор и слово переданы
				if((idw > 0) && !word.empty()){
					// Устанавливаем слово в словарь
					this->words[idw] = word;
					// Если нужно выполнять исправление опечаток
					if(this->isOption(options_t::onlytypos)){
						// Добавляем слово в список векторов
						this->vectors.emplace(this->vecb(word), idw);
					}
				}
				// Выводим статус
				if(status != nullptr){
					// Выполняем расчет статуса
					actual = ceil(index / double(elements) * 100.0);
					// Если статус больше 100, уменьшаем его
					actual = u_short(actual > 99 ? 99 : actual);
					// Если статус обновился
					if(actual != past){
						// Запоминаем текущий статус
						past = actual;
						// Выводим статус загрузки
						status(actual);
					}
				}
				// Выполняем разблокировки потока
				this->locker.unlock();
			};
			// Выполняем инициализацию пула потоков
			this->start();
			// Переходим по всему списку слов
			for(size_t i = 0; i < elements; i++){
				// Получаем идентификатор слова
				memcpy(&idw, data + offset, sizeof(idw));
				// Увеличиваем смещение
				offset += sizeof(idw);
				// Получаем размер буфера слова
				memcpy(&size, data + offset, sizeof(size));
				// Увеличиваем смещение
				offset += sizeof(size);
				// Если идентификатор слова и его данные существуют
				if((idw > 0) && (size > 0)){
					// Устанавливаем данные слова
					word.set(data + offset, size);
					// Выполняем загрузку данных
					this->tpool->push(setWordFn, idw, word);
					// Увеличиваем смещение
					offset += size;
				}
			}
			// Ожидаем завершения работы всех потоков
			this->finish();
			// Если нужно загрузить и индекс обучения тоже
			if(all && !this->isOption(options_t::onlytypos)){
				// Количество элементов в буфере
				size_t elements = 0;
				// Извлекаем количество слов в буфере
				memcpy(&elements, data + offset, sizeof(elements));
				// Увеличиваем смещение
				offset += sizeof(elements);
				// Если количество элементов в буфере больше 0
				if(elements > 0){
					// Загружаем индекс
					this->hnsw.load(data + offset, elements);
					// Устанавливаем сложность алгоритма
					this->hnsw.ef(NSWLIB_ACCURACY);
				// Выводим сообщение, что индекс не получен
				} else this->alphabet->log("index data %s\r\n", alphabet_t::log_t::error, this->logfile, "is broken");
			}
			// Выводим статус
			if(status != nullptr) status(100);
		}
	}
}
/**
 * empty Метод проверки пустой ли словарь
 * @return результат проверки
 */
const bool anyks::Dict::empty() const noexcept {
	// Выводим результат проверки
	return this->words.empty();
}
/**
 * isIdw Метод точной проверки существования идентификатора слова
 * @param idw идентификатор слова для проверки
 * @return    результат проверки
 */
const bool anyks::Dict::isIdw(const size_t idw) const noexcept {
	// Выводим результат проверки
	return (this->words.count(idw) > 0);
}
/**
 * isPilot Метод проверки на пилотное слово
 * @param letter слово для првоерки
 * @return       результат проверки
 */
const bool anyks::Dict::isPilot(const wchar_t letter) const noexcept {
	// Выполняем проверку на пилотное слово
	return (this->pilots.count(letter) > 0);
}
/**
 * isLemma Метод проверки на существование леммы
 * @param lemma лемма для проверки
 * @return      результат проверки
 */
const bool anyks::Dict::isLemma(const wstring & lemma) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если лемма передана
	if(!lemma.empty() && this->isOption(options_t::stemming)){
		// Итератор для подсчета длины n-граммы
		u_short i = 0;
		// Копируем основной объект
		lemma_t * obj = &this->lemmas;
		// Переходим по всему объекту
		for(auto & letter : lemma){
			// Выполняем поиск нашего слова
			auto it = obj->find(letter);
			// Если слово найдено
			if(it != obj->end()){
				// Получаем блок структуры
				obj = &it->second;
				// Если мы дошли до конца
				result = ((i == (lemma.length() - 1)) && (obj->count(this->elid) > 0));
			// Выходим из цикла
			} else break;
			// Увеличиваем количество обработанных букв
			++i;
		}
	}
	// Выводим результат
	return result;
}
/**
 * isWord Метод точной проверки существования слова в словаре
 * @param  word слово для проверки
 * @return      идентификатор искомого слова
 */
const size_t anyks::Dict::isWord(const word_t & word) const noexcept {
	// Результат работы функции
	size_t result = idw_t::NIDW;
	// Если слово передано
	if(!word.empty()){
		// Получаем идентификатор слова
		const size_t idw = this->tokenizer->idw(word);
		// Если идентификатор слова найден
		if(this->isIdw(idw)) result = idw;
	}
	// Выводим результат
	return result;
}
/**
 * check Метод проверки существования слова в словаре
 * @param  word слово для проверки
 * @param  idw  идентификатор слова, если есть
 * @return      идентификатор искомого слова
 */
const size_t anyks::Dict::check(const word_t & word, const size_t idw) const noexcept {
	// Результат работы функции
	size_t result = idw_t::NIDW;
	// Если слово передано
	if(!word.empty() && (word.length() <= MAX_WORD_LENGTH) && (word.find(L' ') == word_t::npos)){
		// Если это не корректное слово
		if(!this->correctly(word) || !this->alphabet->isAllowed(word)){
			// Выводим идентификатор слова
			return (idw == idw_t::NIDW ? this->alm->getIdw(word) : idw);
		// Если это нормальное слово, которое нужно проверить
		} else {
			// Если нужно использовать стемминг
			if(this->isOption(options_t::stemming)){
				// Получаем лемму
				const wstring & lemma = this->stem(word);
				// Если лемма не равна исходному слову
				if(!lemma.empty() && (word.compare(lemma) != 0)){
					// Если лемма получена
					if(this->isLemma(lemma)) return (idw == idw_t::NIDW ? this->tokenizer->idw(word) : idw);
				}
			}
			// Если нужно проверить по фильтру Блума
			if(this->isOption(options_t::bloom) && (word.length() > 3)){
				// Проверяем слово в фильтре
				if(this->filter->contains(word.str())) return (idw == idw_t::NIDW ? this->tokenizer->idw(word) : idw);
			// Если нужно проверить слово в словаре
			} else {
				// Получаем идентификатор слова
				const size_t id = (idw == idw_t::NIDW ? this->tokenizer->idw(word) : idw);
				// Если идентификатор существует, выводим его
				if(this->isIdw(id)) return id;
			}
		}
	}
	// Выводим результат
	return result;
}
/**
 * dumper Метод создания сборщика гипотез
 * @return сборщик гипотез
 */
anyks::dumper_t anyks::Dict::dumper() const noexcept {
	// Создаём сборщик гипотез
	dumper_t result(this->alm, &this->alt, this->alphabet, this->tokenizer);
	// Устанавливаем файл для логов
	result.setLogfile(this->logfile);
	// Устанавливаем характеристики словаря
	result.setAdCw(this->params.cw, this->params.ad);
	// Устанавливаем режим отладки
	if(this->isOption(options_t::debug)) result.setOption(dumper_t::options_t::debug);
	// Выводим результат
	return result;
}
/**
 * getDate Метод получения даты создания словаря
 * @return дата создания словаря в timeStamp
 */
const time_t anyks::Dict::getDate() const noexcept {
	// Выводим результат
	return this->params.date;
}
/**
 * getCode Метод получения кода языка
 * @return код языка
 */
const wstring & anyks::Dict::getCode() const noexcept {
	// Выводим код языка
	return this->params.code;
}
/**
 * getName Метод получения названия словаря
 * @return название словаря
 */
const wstring & anyks::Dict::getName() const noexcept {
	// Выводим код языка
	return this->params.name;
}
/**
 * getAuthor Метод получения автора словаря
 * @return автор словаря
 */
const wstring & anyks::Dict::getAuthor() const noexcept {
	// Выводим данные автора
	return this->params.author;
}
/**
 * getLictype Метод получения типа лицензионной информации словаря
 * @return тип лицензионного соглашения
 */
const wstring & anyks::Dict::getLictype() const noexcept {
	// Выводим тип лицензии
	return this->params.lictype;
}
/**
 * getLictext Метод получения лицензионной информации словаря
 * @return лицензионная информация словаря
 */
const wstring & anyks::Dict::getLictext() const noexcept {
	// Выводим данные лицензии
	return this->params.lictext;
}
/**
 * getContacts Метод получения контактных данных автора словаря
 * @return контактные данные автора словаря
 */
const wstring & anyks::Dict::getContacts() const noexcept {
	// Выводим данные контактов
	return this->params.contacts;
}
/**
 * setCopyright Метод получения авторских прав на словарь
 * @return авторские права на словарь
 */
const wstring & anyks::Dict::getCopyright() const noexcept {
	// Выводим данные копирайта
	return this->params.copyright;
}
/**
 * word Метод получения слова по его идентификатору
 * @param  idw идентификатор слова
 * @return     запрашиваемое слово
 */
const anyks::word_t * anyks::Dict::word(const size_t idw) const noexcept {
	// Получаем данные слова
	auto it = this->words.find(idw);
	// Если слово найдено, выводим его
	return (it != this->words.end() ? &it->second : nullptr);
}
/**
 * stem Метод извлечения лемма/стеммы
 * @param  word слово для обработки
 * @return      полученная лемма/стемма
 */
const wstring anyks::Dict::stem(const wstring & word) const noexcept {
	// Результат работы функции
	wstring result = L"";
	// Получаем длину слова
	const size_t length = word.length();
	// Если слово передано
	if(!word.empty() && (length <= MAX_WORD_LENGTH) && (length > 2)){
		// Текущая буква
		wchar_t letter = 0;
		// Флаг соответствия слова словарю
		bool conform = false;
		// Переводим слово в нижний регистр
		const wstring & tmp = this->alphabet->toLower(word);
		// Переходим по всему слову и проверяем букву на соответствие
		for(size_t i = 0, j = (length - 1); j > ((length / 2) - 1); i++, j--){
			// Получаем текущую букву
			letter = tmp.at(i);
			// Проверяем соответствует ли буква
			conform = ((letter == L'-') || this->alphabet->isLetter(letter));
			// Проверяем вторую букву в слове
			if(conform && (i != j)){
				// Получаем текущую букву
				letter = tmp.at(j);
				// Проверяем соответствует ли буква
				conform = ((letter == L'-') || this->alphabet->isLetter(letter));
			}
			// Если буква не соответствует, выходим
			if(!conform) break;
		}
		// Если слово соответствует, выполняем стемминг
		if(conform){
			// Выполняем стемминг
			result = this->stemmer->lemma(word, this->getCode());
			// Если вывод отладочной информации разрешён
			if(this->isOption(options_t::debug)){
				// Выводим основное сообщение отладки
				this->alphabet->log("stemming: [%ls => %ls]\r\n", alphabet_t::log_t::info, this->logfile, word.c_str(), result.c_str());
			}
		}
	}
	// Выводим результат
	return result;
}
/**
 * getPilots Метод получения пилотных слов
 */
const set <wchar_t> & anyks::Dict::getPilots() const noexcept {
	// Выводим список пилотных слов
	return this->pilots;
}
/**
 * getAdCw Метод получения характеристик словаря
 * @return характеристики словаря
 */
const pair <size_t, size_t> anyks::Dict::getAdCw() const noexcept {
	// Выводим характеристики словаря
	return make_pair(this->params.cw, this->params.ad);
}
/**
 * getEmbedding Метод получения эмбеддинга словаря
 * @return эмбеддинг словаря
 */
const pair <json, size_t> anyks::Dict::getEmbedding() const noexcept {
	// Результат работы функции
	pair <json, size_t> result;
	// Если объект эмбеддинга существует
	if(!this->embedding.empty()){
		// Создаём промежуточный формат эмбеддинга
		map <string, size_t> embedding;
		// Переходим по всему списку
		for(auto & item : this->embedding){
			// Заполняем эмбеддинг данными
			embedding.emplace(this->alphabet->convert(wstring({item.first})), item.second);
		}
		// Если эмбеддинг получен
		if(!embedding.empty()){
			// Создаём объект JSON
			json obj(embedding);
			// Формируем результат
			result = make_pair(move(obj), this->sizeEmbedding);
		}
	}
	// Выводим данные эмбеддинга
	return result;
}
/**
 * getUWords Метод извлечения списка идентификаторов слов, которые всегда начинаются с заглавной буквы
 * @return список идентификаторов слов
 */
const map <size_t, size_t> & anyks::Dict::getUWords() const noexcept {
	// Выводим список слов, начинающихся с заглавной буквы
	return this->alt.getUWords();
}
/**
 * all Метод извлечения всех слов
 * @return список найденных слов
 */
const unordered_map <size_t, anyks::word_t> & anyks::Dict::all() const noexcept {
	// Выводим результат
	return this->words;
}
/**
 * variants Метод получения вариантов слова
 * @param word слово для которого нужно получить варианты
 * @return     список подобранных вариантов слова
 */
const list <wstring> anyks::Dict::variants(const wstring & word) const noexcept {
	// Результат работы функции
	list <wstring> result;
	// Если слово передано
	if(!word.empty() && (word.length() <= MAX_WORD_LENGTH)){
		// Выполняем подбор вариантов
		result = this->stemmer->variants(word, this->getCode());
		// Если вывод отладочной информации разрешён
		if(this->isOption(options_t::debug)){
			// Список вариантов
			wstring words = L"";
			// Переходим по всему списку вариантов
			for(auto & item : result){
				// Если слово уже не пустое, добавляем разделитель
				if(!words.empty()) words.append(L"|");
				// Добавляем в список вариантов, полученный вариант
				words.append(item);
			}
			// Выводим основное сообщение отладки
			this->alphabet->log("stemming: [%ls => %ls]\r\n", alphabet_t::log_t::info, this->logfile, word.c_str(), words.c_str());
		}
	}
	// Выводим результат
	return result;
}
/**
 * Dict Конструктор
 */
anyks::Dict::Dict() noexcept : elid(L'¶') {
	// Экранируем возможность ошибки памяти
	try {
		// Устанавливаем название словаря
		this->params.name = L"asc";
		// Создаем объект модуля выполнения скриптов
		this->stemmer = new stemmer_t();
	// Если происходит ошибка то игнорируем её
	} catch(const bad_alloc &) {
		// Выходим из приложения
		exit(EXIT_FAILURE);
	}
}
/**
 * Dict Конструктор
 * @param alm объект языковой модели
 */
anyks::Dict::Dict(alm_t * alm) noexcept : elid(L'¶') {
	// Если языковая модель существует
	if(alm != nullptr){
		// Экранируем возможность ошибки памяти
		try {
			// Устанавливаем название словаря
			this->params.name = L"asc";
			// Создаем объект модуля выполнения скриптов
			this->stemmer = new stemmer_t();
		// Если происходит ошибка то игнорируем её
		} catch(const bad_alloc &) {
			// Выходим из приложения
			exit(EXIT_FAILURE);
		}
		// Запоминаем объект языковой модели
		this->setALM(alm);
	// Выходим из приложения
	} else exit(EXIT_FAILURE);
}
/**
 * Dict Конструктор
 * @param alphabet объект алфавита
 */
anyks::Dict::Dict(const alphabet_t * alphabet) noexcept : elid(L'¶') {
	// Если алфавит существует
	if(alphabet != nullptr){
		// Экранируем возможность ошибки памяти
		try {
			// Устанавливаем название словаря
			this->params.name = L"asc";
			// Создаем объект модуля выполнения скриптов
			this->stemmer = new stemmer_t(alphabet);
		// Если происходит ошибка то игнорируем её
		} catch(const bad_alloc &) {
			// Выходим из приложения
			exit(EXIT_FAILURE);
		}
		// Запоминаем объект алфавита
		this->setAlphabet(alphabet);
	// Выходим из приложения
	} else exit(EXIT_FAILURE);
}
/**
 * Dict Конструктор
 * @param tokenizer объект токенизатора
 */
anyks::Dict::Dict(const tokenizer_t * tokenizer) noexcept : elid(L'¶') {
	// Если токенизатор существует
	if(tokenizer != nullptr){
		// Экранируем возможность ошибки памяти
		try {
			// Устанавливаем название словаря
			this->params.name = L"asc";
			// Создаем объект модуля выполнения скриптов
			this->stemmer = new stemmer_t(tokenizer);
		// Если происходит ошибка то игнорируем её
		} catch(const bad_alloc &) {
			// Выходим из приложения
			exit(EXIT_FAILURE);
		}
		// Запоминаем объект токенизатора
		this->setTokenizer(tokenizer);
	// Выходим из приложения
	} else exit(EXIT_FAILURE);
}
/**
 * Dict Конструктор
 * @param alm      объект языковой модели
 * @param alphabet объект алфавита
 */
anyks::Dict::Dict(alm_t * alm, const alphabet_t * alphabet) noexcept : elid(L'¶') {
	// Если алфавит существует
	if((alm != nullptr) && (alphabet != nullptr)){
		// Экранируем возможность ошибки памяти
		try {
			// Устанавливаем название словаря
			this->params.name = L"asc";
			// Создаем объект модуля выполнения скриптов
			this->stemmer = new stemmer_t(alphabet);
		// Если происходит ошибка то игнорируем её
		} catch(const bad_alloc &) {
			// Выходим из приложения
			exit(EXIT_FAILURE);
		}
		// Запоминаем объект языковой модели
		this->setALM(alm);
		// Запоминаем объект алфавита
		this->setAlphabet(alphabet);
	// Выходим из приложения
	} else exit(EXIT_FAILURE);
}
/**
 * Dict Конструктор
 * @param alm       объект языковой модели
 * @param tokenizer объект токенизатора
 */
anyks::Dict::Dict(alm_t * alm, const tokenizer_t * tokenizer) noexcept : elid(L'¶') {
	// Если токенизатор существует
	if((alm != nullptr) && (tokenizer != nullptr)){
		// Экранируем возможность ошибки памяти
		try {
			// Устанавливаем название словаря
			this->params.name = L"asc";
			// Создаем объект модуля выполнения скриптов
			this->stemmer = new stemmer_t(tokenizer);
		// Если происходит ошибка то игнорируем её
		} catch(const bad_alloc &) {
			// Выходим из приложения
			exit(EXIT_FAILURE);
		}
		// Запоминаем объект языковой модели
		this->setALM(alm);
		// Запоминаем объект токенизатора
		this->setTokenizer(tokenizer);
	// Выходим из приложения
	} else exit(EXIT_FAILURE);
}
/**
 * Dict Конструктор
 * @param alm       объект языковой модели
 * @param alphabet  объект алфавита
 * @param tokenizer объект токенизатора
 */
anyks::Dict::Dict(alm_t * alm, const alphabet_t * alphabet, const tokenizer_t * tokenizer) noexcept : elid(L'¶') {
	// Если токенизатор и алфавит существуют
	if((alm != nullptr) && (alphabet != nullptr) && (tokenizer != nullptr)){
		// Экранируем возможность ошибки памяти
		try {
			// Устанавливаем название словаря
			this->params.name = L"asc";
			// Создаем объект модуля выполнения скриптов
			this->stemmer = new stemmer_t(alphabet, tokenizer);
		// Если происходит ошибка то игнорируем её
		} catch(const bad_alloc &) {
			// Выходим из приложения
			exit(EXIT_FAILURE);
		}
		// Запоминаем объект языковой модели
		this->setALM(alm);
		// Запоминаем объект алфавита
		this->setAlphabet(alphabet);
		// Запоминаем объект токенизатора
		this->setTokenizer(tokenizer);
	// Выходим из приложения
	} else exit(EXIT_FAILURE);
}
/**
 * ~Dict деструктор
 */
anyks::Dict::~Dict() noexcept {
	// Если фильтр Блума создан
	if(this->filter != nullptr) delete this->filter;
	// Если стемминг подключен, удаляем его
	if(this->stemmer != nullptr) delete this->stemmer;
}
