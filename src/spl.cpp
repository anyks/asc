/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <spl.hpp>


/**
 * pop Метод удаления первого слова
 */
void anyks::ASpell::Cache::pop() noexcept {
	// Если последовательность существует
	if(!this->seq.empty()) this->seq.erase(this->seq.begin());
	// Если список слов существует
	if(!this->words.empty()) this->words.erase(this->words.begin());
}
/**
 * clear Метод очистки данных
 */
void anyks::ASpell::Cache::clear() noexcept {
	// Очищаем список последовательности
	this->seq.clear();
	// Очищаем список слов
	this->words.clear();
	// Очищаем выделенную память списка последовательности
	vector <size_t> ().swap(this->seq);
	// Очищаем выделенную память списка слов
	vector <wstring> ().swap(this->words);
}
/**
 * push Метод добавления слова в список
 * @param idw  идентификатор слова
 * @param word слово для добавления
 */
void anyks::ASpell::Cache::push(const size_t idw, const wstring & word) noexcept {
	// Если размер последовательности выше разрешённой, удаляем излишки
	if(this->seq.size() >= size_t(size)) this->pop();
	// Добавляем в список идентификатор слова
	this->seq.push_back(idw);
	// Добавляем в список слово
	this->words.push_back(word);
}
/**
 * empty Метод проверки на сущестование данных
 * @return результат проверки
 */
const bool anyks::ASpell::Cache::empty() const noexcept {
	// Выводим результат првоеки
	return (this->seq.empty() && this->words.empty());
}
/**
 * gets Метод получения списка идентификаторов слов
 * @return список идентификаторов слов
 */
const vector <size_t> & anyks::ASpell::Cache::gets() const noexcept {
	// Выводим список последовательности
	return this->seq;
}
/**
 * getw Метод получения списка слов
 * @return список слов
 */
const vector <wstring> & anyks::ASpell::Cache::getw() const noexcept {
	// Выводим список слов
	return this->words;
}
/**
 * ~Cache Деструктор
 */
anyks::ASpell::Cache::~Cache() noexcept {
	// Выполняем очистку данных
	this->clear();
}
/**
 * idw Метод получения идентификатора слова
 * @param word слово для которого нужно получить идентификатор
 * @return     сгенерированный идентификатор слова
 */
const size_t anyks::ASpell::idw(const wstring & word) const noexcept {
	// Результат работы функции
	size_t result = idw_t::NIDW;
	// Если слово передано
	if(!word.empty()){
		// Выполняем проверку на существование слова
		result = this->alm->getIdw(word);
		// Если это слово, выполняем првоерку на существование слова
		if(((result == size_t(token_t::anum)) && (word.length() >= 6)) || !this->tokenizer->isToken(result)){
			// Выполняем провеку наличия слова в словаре
			result = this->dict->check(word, result);
		}
		// Если результат не получен, сбрасываем результат
		if((result == size_t(token_t::null)) || (result == size_t(token_t::unk))){
			// Формируем токен ответа
			result = (word.length() >= 6 ? idw_t::NIDW : (size_t) token_t::unk);
		}
	}
	// Выводим результат
	return result;
}
/**
 * isOption Метод проверки наличия опции
 * @param option  опция для проверки
 * @param options опции приложения
 * @return        результат проверки
 */
const bool anyks::ASpell::isOption(const options_t option, const u_short options) const noexcept {
	// Выполняем проверку наличия опции
	const bool result = bitset <11> (options).test((u_short) option);
	// Если флаг дал положительный результат и это режим отладки
	if(result && (option == options_t::debug)) this->logfile = nullptr;
	// Выводим результат
	return result;
}
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
const anyks::ASpell::backwrd_t anyks::ASpell::backword(const size_t idw, const wstring & word, const u_short options, size_t & pos1, size_t & pos2, vector <wstring> & words) const noexcept {
	// Получаем пустой идентификатор
	const size_t nidw = idw_t::NIDW;
	// Результат работы функции
	backwrd_t result = {nidw, {L"", L""}};
	// Если разрешено удалять сплиты в словах
	if(this->isOption(options_t::rsplit, options)){
		// Если слово передано и это не токен
		if(!word.empty() && (!this->tokenizer->isToken(idw) ||
		this->tokenizer->isAbbr(this->tokenizer->idw(word)))){
			// Добавляем слово в список
			words.push_back(word);
			// Если позиция пустая, устанавливаем её
			if(pos2 == 0) pos2 = pos1;
			// Если количество слов в списке больше 3-х, удаляем первое слово
			if(words.size() > 3){
				// Меняем позицию
				pos2++;
				// Удаляем первое слово в списке
				words.erase(words.begin());
			}
			// Если в списке слов больше одного
			if(words.size() > 1){
				// Переходим по всему списку слов
				for(auto & word : words){
					// Добавляем слово в результат
					result.second.first.append(word);
					// Если слово существует, добавляем разделитель
					if(!result.second.second.empty())
						// Добавляем разделитель
						result.second.second.append(1, L' ');
					// Добавляем слово в эталонный вариант
					result.second.second.append(word);
				}
			}
			// Проверяем существование слова
			result.first = this->dict->check(result.second.first);
			// Если вариант не найден, проверяем биграмму
			if((result.first == idw_t::NIDW) && (words.size() > 2)){
				// Формируем биграмму
				result.second.first = (words.at(1) + words.back());
				// Проверяем существование слова
				result.first = this->dict->check(result.second.first);
				// Если биграмма существует
				if(result.first != idw_t::NIDW){
					// Меняем позицию
					pos2++;
					// Удаляем первое слово в списке
					words.erase(words.begin());
					// Формируем эталонный вариант
					result.second.second = (words.front() + wstring(1, L' ') + words.back());
				}
			}
			// Если слово не получено, очищаем слово
			if(result.first == idw_t::NIDW){
				// Очищаем получившееся слово
				result.second.first.clear();
				// Очищаем эталонный вариант
				result.second.second.clear();
			}
		// Если это не слово
		} else {
			// Сбрасываем позицию
			pos2 = 0;
			// Очищаем список
			words.clear();
		}
	}
	// Выводим результат
	return result;
}
/**
 * check Метод проверки существования слова
 * @param word слово для проверки
 * @return     результат проверки
 */
const bool anyks::ASpell::check(const wstring & word) const  noexcept {
	// Результат работы функции
	bool result = false;
	// Если слово передано
	if(!word.empty() && (this->dict != nullptr) && (this->alm != nullptr) && (this->alphabet != nullptr)){
		// Отключаем флаг смешивания словарей
		const_cast <alm_t *> (this->alm)->unsetOption(alm_t::options_t::mixdicts);
		// Выполняем проверку на существование слова
		result = ((word.length() > 1 ? this->dict->check(word) : this->alm->getIdw(word)) != idw_t::NIDW);
	}
	// Выводим результат
	return result;
}
/**
 * setOption Метод установки опций модуля
 * @param option  опция для установки
 * @param options опции приложения
 * @return        список установленных опций
 */
const u_short anyks::ASpell::setOption(const options_t option, const u_short options) const noexcept {
	// Устанавливаем опции
	return bitset <11> (options).set((u_short) option).to_ulong();
}
/**
 * unsetOption Метод удаления опций модуля
 * @param option  опция для установки
 * @param options опции приложения
 * @return        список оставшихся опций
 */
const u_short anyks::ASpell::unsetOption(const options_t option, const u_short options) const noexcept {
	// Удаляем опции
	return bitset <11> (options).reset((u_short) option).to_ulong();
}
/**
 * split Метод выполнения сплита слова на слова
 * @param text    текст для сплита
 * @param options опции приложения
 * @param words   список проспличенных вариантов
 */
void anyks::ASpell::split(const wstring & text, const u_short options, vector <wstring> & words) const noexcept {
	// Если текст передан
	if(!text.empty() && (this->dict != nullptr) && (this->alm != nullptr) && (this->alphabet != nullptr) && (this->tokenizer != nullptr)){
		// Очищаем переданный список слов
		words.clear();
		// Временное значение биграммы
		vector <size_t> tmp(2);
		// Список найденных пробелов
		set <size_t> posSpaces;
		// Получаем длину текста
		size_t size = text.length();
		// Количество пробелов в тексте и позиция добавления слова
		size_t spaces = 0, position = 0;
		// Получаем объект сборки гипотез
		dumper_t dmp = this->dict->dumper();
		// Если флаг исправления слов с альтернативными буквами активен
		if(this->isOption(options_t::alter, options)) dmp.setOption(dumper_t::options_t::alter);
		// Если флаг простановки регистра слов активен
		if(this->isOption(options_t::uppers, options)) dmp.setOption(dumper_t::options_t::uppers);
		/**
		 * validateFn Функция валидации подбираемого варианта
		 * @param word слово для валидации
		 * @param olet количество одиночных букв идущих друг за другом
		 * @return     результат валидации слова
		 */
		auto validateFn = [this](const wstring & word, const u_short olet) noexcept {
			// Результат работы функции
			pair <bool, token_t> result = {false, token_t::null};
			// Если слово передано
			if(!word.empty()){
				// Получаем длину слова
				const size_t length = word.length();
				// Если это не короткое слово
				if((result.first = (length > 3))) result.second = this->tokenizer->idt(word);
				// Если количество коротких слов идущих друг за другом меньше 3-х
				else if(olet < 4){
					// Получаем токен слова
					result.second = this->tokenizer->idt(word);
					// Если в слове всего 1-а буква
					if(length == 1){
						// Выполняем проверку на то, что это не слово
						result.first = ((result.second != token_t::null) && (result.second != token_t::unk));
						// Выполняем проверку на пилотное слово
						if(!result.first) result.first = this->dict->isPilot(word.front());
					// Если в слове больше 1-й буквы
					} else result.first = (length > 1);
				}
			}
			// Выводим результат
			return result;
		};
		/**
		 * findBestVariantFn Функция выбора лучшего варианта
		 */
		auto findBestVariantFn = [&spaces, &position, &dmp, &text, options, &words, this]() noexcept {
			// Если данные собраны
			if(!dmp.empty()){
				// Флаг найденного совпавшего варианта
				bool mode = false;
				// Список последовательности для проверки
				vector <size_t> seq;
				// Собранная последовательность слов
				vector <wstring> tmp;
				// Значение предыдущей частоты
				double logprob = MIN_WORD_WEIGHT;
				// Количество символов и слов в обработанных вариантах
				size_t countLetters = 0, countWords = text.length();
				// Рейтинг варианта
				double estimate1 = MIN_WORD_WEIGHT, estimate2 = MIN_WORD_WEIGHT;
				// Переходим по всему списку
				for(auto & item : dmp.get()){
					// Выполняем сборку контекста
					const auto & res = dmp.ctx(make_pair(item.first, &item.second), tmp);
					// Если количество символов совпадает
					if((res.letters + spaces) == text.length()){
						// Если количество коротких слов больше двух и их количество равно общему количеству слов, пропускаем вариант
						if((res.bilet > 2) && (res.bilet == tmp.size())) continue;
						// Запоминаем, что найден совпавший вариант
						mode = true;
						// Если количество слов в варианте, меньше чем у предыдущего варианта
						if((res.words < countWords) || ((res.words == countWords) && (estimate1 <= res.estimate))){
							// Получаем последовательность
							seq = dmp.getSequence(item.first);
							// Добавляем конец предложения
							if(seq.back() != size_t(token_t::finish)) seq.push_back(size_t(token_t::finish));
							// Добавляем начало предложения
							if(seq.front() != size_t(token_t::start)) seq.insert(seq.begin(), size_t(token_t::start));
							// Выполняем расчёт перплексии
							const auto & ppl = this->alm->perplexity(seq);
							// Если все варианты равны, тогда сравниваем по частоте последовательности
							if((res.words == countWords) && (estimate1 == res.estimate)){
								// Если частота последовательности текущего варианта лучше
								if(ppl.logprob > logprob){
									// Получаем готовый текст
									words = move(tmp);
									// Запоминаем частоту последовательности
									logprob = ppl.logprob;
								}
							// Получаем готовый текст
							} else words = move(tmp);
							// Запоминаем количество слов в варианте
							countWords = res.words;
							// Запоминаем рейтинг варианта
							estimate1 = res.estimate;
						}
					// Ищем лучшие варианты
					} else if(!mode && (res.letters >= countLetters) && (res.oclet == 0)) {
						// Если рейтинг меньше текущего варианта
						if((res.letters > countLetters) || (estimate2 <= res.estimate)){
							// Получаем готовый текст
							words = move(tmp);
							// Запоминаем рейтинг варианта
							estimate2 = res.estimate;
							// Запоминаем максимальное количество символов в словах
							countLetters = res.letters;
						}
					}
				}
				// Если вариант деффектный
				if(!mode && (words.size() > 1)){
					// Получаем недостающее слово
					const wstring & word = text.substr(countLetters + spaces);
					// Если слово получено
					if(word.length() > 3){
						// Если вывод отладочной информации разрешён
						if(this->isOption(options_t::debug, options)){
							// Выводим основное сообщение отладки
							this->alphabet->log("bad word in split: [%ls]\r\n", alphabet_t::log_t::warning, this->logfile, word.c_str());
						}
						/**
						 * defWordFn Функция формирования дефолтного варианта
						 * @param word  слово для добавления в список вариантов
						 * @param words список слов, куда нужно добавить вариант
						 */
						auto defWordFn = [](const wstring & word, vector <wstring> & words) noexcept {
							// Получаем длину последнего слова в списке
							const size_t back = words.back().length();
							// Получаем длину первого слова в списке
							const size_t front = words.front().length();
							// Первое оба слова больше 1-го символа
							if(((front > 3) && (back >= 3)) || ((front >= 3) && (back > 3))){
								// Добавляем слово в список слов
								words.push_back(word);
							// Очищаем собранный список слов
							} else words.clear();
						};
						// Если разрешено исправлять ошибки в сплитах
						if(this->isOption(options_t::esplit, options)){
							// Выполняем поиск варианта слова
							const auto & res = this->dict->find(word, dmp, position);
							// Если количество слов в списке не изменилось
							if(res.second < 1) defWordFn(word, words);
							// Если какой-то вариант найден
							else {
								// Выполняем получение списка слов
								dmp.ctx(dmp.best(), words);
								// Если такая последовательность не найдена, очищаем список слов
								if(!this->alm->check(words, true).first) words.clear();
							}
						// Если ошибки исправлять не разрешено
						} else defWordFn(word, words);
					// Очищаем собранный список слов
					} else words.clear();
				// Если вариант всего один, очищаем список слов
				} else if(words.size() == 1) words.clear();
			}
		};
		/**
		 * Прототип функции выполнения сплита в прямом и обратном направлении
		 * @param текст для обработки
		 * @param собранная последовательность
		 * @param идентификатор предыдущей последовательности
		 * @param позиция для добавления варианта
		 * @param количество собранных букв в тексте
		 * @param количество одиночных букв идущих друг за другом
		 */
		function <void (const wstring &, vector <size_t>, const size_t, const size_t, const size_t, const u_short)> splitFn;
		/**
		 * splitFn Функция выполнения сплита в прямом направлении
		 * @param text    текст для обработки
		 * @param seq     собранная последовательность
		 * @param ids     идентификатор предыдущей последовательности
		 * @param pos     позиция для добавления варианта
		 * @param letters количество собранных букв в тексте
		 * @param olet    количество одиночных букв идущих друг за другом
		 */
		splitFn = [&](const wstring & text, vector <size_t> seq, const size_t ids, const size_t pos, const size_t letters, const u_short olet) noexcept {
			// Если позиция больше текущей, устанавливаем
			if(pos > position) position = pos;
			// Если текст не пустой
			if(!text.empty()){
				// Вариант полученного слова
				wstring word = L"";
				// Флаг проверки слова на соответствие токену
				bool isToken = false;
				// Идентификатор токена
				token_t idt = token_t::null;
				// Результат валидации слова
				pair <bool, token_t> validate;
				// Идентификатор слова, смещение в тексте и длина слова
				size_t idw = idw_t::NIDW, offset = 0, length = 0;
				// Переходим по всему слову
				for(auto it = text.cbegin(); it != text.cend(); it++){
					// Устанавливаем идентификатор текущего слова
					idw = idw_t::NIDW;
					// Если буква является пробелом
					if(this->alphabet->isSpace(* it)){
						// Если слово уже существует
						if(!word.empty()) break;
						// Если текущая позиция еще не добавлена
						if(posSpaces.count(letters) < 1){
							// Увеличиваем количество найденных пробелов
							spaces++;
							// Запоминаем позицию пробела
							posSpaces.emplace(letters);
						}
						// Увеличиваем смещение
						offset++;
						// Пропускаем ход
						continue;
					}
					// Добавляем в слово
					word.append(1, * it);
					// Получаем длину слова
					length = word.length();
					// Если в конце слова находится дефис, пропускаем
					if((length > 1) && (word.back() == L'-')) continue;
					// Если количество коротких слов, идущих друг за другом, меньше 3, или слово длинее 3-х букв
					if((validate = validateFn(word, olet)).first){
						// Получаем идентификатор токена
						idt = validate.second;
						// Если токен существует
						if((idt != token_t::null) && (idt != token_t::unk) && (idt != token_t::abbr) && (idt != token_t::anum)){
							// Получаем следующий итератор
							auto nx = next(it, 1);
							// Если следующий символ существует является числом и текущий символ является числом, пропускаем текущую итерацию
							if(!((idt == token_t::num) && (nx != text.cend()) && this->alphabet->isNumber(wstring(1, * nx)))) idw = (size_t) idt;
							// Если - это отрицательное число, выходим
							if((idt == token_t::num) && (word.front() == L'-')) break;
						// Если слово является числовой или текстовой аббревиатурой
						} else if((idt == token_t::abbr) && this->tokenizer->isSuffix(word)) idw = (size_t) token_t::abbr;
						// Если идентификатор слова не получен, проверяем наличие слова в словаре
						if((idw == idw_t::NIDW) && (idt != token_t::num)) idw = this->dict->isWord(word);
						// Если слово или токен существуют
						if(idw != idw_t::NIDW){
							// Выполняем проверку на токен
							isToken = this->tokenizer->isToken(idw);
							// Если последовательность уже существует
							if(!seq.empty()){
								// Формируем биграмму для проверки
								tmp.at(1) = idw;
								tmp.at(0) = seq.back();
								// Если слово не является токеном и оно равно предыдущему, пропускаем
								if(!isToken && (tmp.front() == tmp.back())) continue;
							}
							// Добавляем в список последовательности следующий индекс слова
							seq.push_back(idw);
							// Если последовательность существует
							if((seq.size() == 1) || isToken || this->alm->check(tmp, (u_short) 2)){
								// Создаём гипотезу
								dumper_t::awrd_t hypothesis;
								// Устанавливаем идентификатор слова
								hypothesis.idw = idw;
								// Устанавливаем эталонную фразу
								hypothesis.etalon = word;
								// Если вариант собран, маркируем его как лучший
								if((letters + length + spaces) == size) dmp.mark(seq, pos);
								// Выполняем добавления очередного варианта
								const auto & res = dmp.set(move(hypothesis), ids, pos, (idw == size_t(token_t::num)) || (idw == size_t(token_t::abbr)));
								// Если слово добавлено
								if((res.first != idw_t::NIDW) && (res.second > 0)){
									// Добавляем вариант слова
									splitFn(text.substr(length + offset), seq, res.first, res.second, (letters + length), (length < 4 ? olet + 1 : 0));
								}
							}
							// Удаляем последний элемент в списке последовательности
							seq.pop_back();
						}
					}
				}
			}
		};
		{
			// Переводим текст в нижний регистр
			const wstring & tmp = this->alphabet->toLower(text);
			// Если текст получен
			if(!tmp.empty()){
				// Выполняем поиск вариантов
				splitFn(tmp, {}, idw_t::NIDW, 1, 0, 0);
				// Извлекаем лучший вариант
				findBestVariantFn();
			}
		}
	}
}
/**
 * erratum Метод поиска в тексте опечаток
 * @param text    текст для сборки данных
 * @param options опции приложения
 * @param words   список слов в которых найдены ошибки
 */
void anyks::ASpell::erratum(const wstring & text, const u_short options, vector <wstring> & words) const noexcept {
	// Если текст передан
	if(!text.empty() && (this->dict != nullptr) && (this->alm != nullptr) && (this->alphabet != nullptr) && (this->tokenizer != nullptr)){
		// Очищаем переданный список слов
		words.clear();
		// Список слов для удаления пробелов
		vector <wstring> parts;
		// Позиция добавления слов
		size_t pos1 = 1, pos2 = 0;
		// Кэш последовательности
		cache_t cache(this->alm->getSize());
		// Отключаем флаг смешивания словарей
		const_cast <alm_t *> (this->alm)->unsetOption(alm_t::options_t::mixdicts);
		/**
		 * modeFn Функция обработки разбитого текста
		 * @param word  слово для обработки
		 * @param ctx   контекст к которому принадлежит слово
		 * @param reset флаг сброса контекста
		 * @param stop  флаг завершения обработки
		 */
		auto modeFn = [&](const wstring & wrd, const vector <string> & ctx, const bool reset, const bool stop) noexcept {
			// Если это сброс контекста, очищаем список слов
			if(reset) parts.clear();
			// Если слово получено
			if(!wrd.empty()){
				// Получаем слово в нижнем регистре
				const wstring & word = this->alphabet->toLower(wrd);
				// Выполняем проверку на существование слова
				const size_t idw = this->idw(word);
				// Если вывод отладочной информации разрешён
				if(this->isOption(options_t::debug, options)){
					// Если слово принято
					if(idw != idw_t::NIDW)
						// Выводим основное сообщение отладки
						this->alphabet->log("word accepted: [%ls | %u]\r\n", alphabet_t::log_t::info, this->logfile, word.c_str(), idw);
					// Если слово не принято
					else this->alphabet->log("word not accepted: [%ls]\r\n", alphabet_t::log_t::warning, this->logfile, word.c_str());
				}
				// Если слово не существует, добавляем его в список
				if(idw == idw_t::NIDW){
					// Очищаем кэш последовательности
					cache.clear();
					// Добавляем слово в список неизвестных
					words.push_back(word);
				// Если слово найдено и оно не является токеном
				} else if(!this->tokenizer->isToken(idw)) {
					// Добавляем слово в кэш
					cache.push(idw, word);
					// Получаем список последовательности
					const auto & seq = cache.gets();
					// Если список не является юниграммой
					if((seq.size() > 1) && !this->alm->check(seq, (u_short) 2)){
						// Получаем текст ответа
						const wstring & text = this->tokenizer->restore(cache.getw());
						// Если текст получен
						if(!text.empty()) words.push_back(text);
						// Очищаем кэш последовательности
						cache.clear();
					}
				// Очищаем кэш последовательности
				} else cache.clear();
				// Получаем слово разделённое пробелами
				const auto & bwrd = this->backword(idw, word, options, pos1, pos2, parts);
				// Если слово получено
				if(bwrd.first != idw_t::NIDW) words.push_back(bwrd.second.second);
			}
			// Если это конец, очищаем список слов
			if(stop) parts.clear();
			// Выводим результат
			return true;
		};
		// Выполняем разбивку текста на токены
		this->tokenizer->run(text, modeFn);
	}
}
/**
 * hyphen Метод выполнения сплита слов по дефисам на слова
 * @param text    текст для сплита
 * @param options опции приложения
 * @param seq     список последовательности
 * @param words   список проспличенных вариантов
 */
void anyks::ASpell::hyphen(const wstring & text, const u_short options, vector <size_t> & seq, vector <wstring> & words) const noexcept {
	// Если текст передан
	if(!text.empty() && (this->dict != nullptr) && (this->alm != nullptr) && (this->alphabet != nullptr) && (this->tokenizer != nullptr)){
		// Если количество дефисов в слове больше 1
		if(this->alphabet->countLetter(text, L'-') > 1){
			// Очищаем список последовательности
			seq.clear();
			// Очищаем список вариантов слов
			words.clear();
			// Слово для добавления
			wstring word = L"";
			// Идентификатор слова
			size_t idw = idw_t::NIDW;
			// Позиция дефиза в тексте
			size_t pos1 = 0, pos2 = 0;
			// Выполняем поиск дефиза пока он есть
			while((pos2 = text.find(L"-", pos1)) != wstring::npos){
				// Получаем слово
				word = text.substr(pos1, pos2 - pos1);
				// Если не нужно учитывать регистры
				if(!this->isOption(options_t::uppers, options)){
					// Переводим слово в нижний регистр
					word = this->alphabet->toLower(word);
				}
				// Получаем идентификатор слова
				idw = this->alm->getIdw(word);
				// Если это нормальное слово и оно существует
				if(!this->tokenizer->isToken(idw) && this->dict->isIdw(idw)){
					// Добавляем слово в список последовательности
					seq.push_back(idw);
					// Добавляем слово в список
					words.push_back(word);
					// Увеличиваем текущую позицию
					pos1 = (pos2 + 1);
				// Иначе выходим из цикла
				} else {
					// Очищаем список последовательности
					seq.clear();
					// Очищаем список собранных слов
					words.clear();
					// Выходим из цикла
					break;
				}
			}
			// Если список получен
			if(!words.empty() && !seq.empty()){
				// Получаем слово
				word = text.substr(pos1);
				// Если не нужно учитывать регистры
				if(!this->isOption(options_t::uppers, options)){
					// Переводим слово в нижний регистр
					word = this->alphabet->toLower(word);
				}
				// Получаем идентификатор слова
				idw = this->alm->getIdw(word);
				// Если это нормальное слово и оно существует
				if(!this->tokenizer->isToken(idw) && this->dict->isIdw(idw)){
					// Добавляем слово в список последовательности
					seq.push_back(idw);
					// Добавляем слово в список
					words.push_back(word);
				// Иначе очищаем список собранных слов
				} else {
					// Очищаем список последовательности
					seq.clear();
					// Очищаем список собранных слов
					words.clear();
				}
			}
			// Если список получен и такая последовательность не существует, очищаем собранный список слов
			if(!seq.empty() && !this->alm->check(seq, (u_short) 2)){
				// Очищаем список последовательности
				seq.clear();
				// Очищаем список собранных слов
				words.clear();
			}
		}
	}
}
/**
 * spell Метод исправления опечаток в большом тексте
 * @param text    текст для корректировки
 * @param options опции приложения
 * @param info    список слов для которых выполнена коррекция
 */
void anyks::ASpell::spell(wstring & text, const u_short options, vector <vector <pair <wstring, wstring>>> * info) const noexcept {
	// Если текст передан
	if(!text.empty() && (this->dict != nullptr) && (this->alm != nullptr) && (this->alphabet != nullptr) && (this->tokenizer != nullptr)){
		// Объект левенштейна
		lev_t algorithms;
		// Предыдущее слово
		word_t lastWord = L"";
		// Позиция добавления слов
		size_t pos1 = 1, pos2 = 0;
		// Флаг добавления дублирующего кандидата
		bool duoCandidate = false;
		// Создаём объект гипотезы
		dumper_t::awrd_t hypothesis;
		// Очищаем список слов для которых выполняется коррекция
		if(info != nullptr) info->clear();
		// Получаем объект сборки гипотез
		dumper_t dmp = this->dict->dumper();
		// Кэш последовательности
		cache_t cache(this->alm->getSize());
		// Список полученных слов и слов для удаления пробелов
		vector <wstring> words, result, parts;
		// Создаём кандидатов с неправельным контекстом
		vector <pair <wstring, wstring>> noContext;
		// Отключаем флаг смешивания словарей
		const_cast <alm_t *> (this->alm)->unsetOption(alm_t::options_t::mixdicts);
		// Если флаг исправления слов с альтернативными буквами активен
		if(this->isOption(options_t::alter, options)) dmp.setOption(dumper_t::options_t::alter);
		// Если флаг простановки регистра слов активен
		if(this->isOption(options_t::uppers, options)) dmp.setOption(dumper_t::options_t::uppers);
		/**
		 * resFn Функция вывода результата
		 * @param word слово токена-разделителя
		 */
		auto resFn = [&](const wstring & word = L"") noexcept {
			// Создаём кандидата для получения статистики
			vector <pair <wstring, wstring>> candidate;
			// Выполняем получение списка слов
			dmp.ctx(dmp.best(), words, (info != nullptr ? &candidate : nullptr));
			// Если список слов получен
			if(!words.empty()){
				// Выполняем сбор полной последовательности
				result.insert(result.end(), words.begin(), words.end());
				// Если кандидаты получены
				if((info != nullptr) && !candidate.empty()){
					// Если список вариантов с неверным контекстом получен, добавляем его в полученный список
					if(!noContext.empty()){
						// Добавляем список вариантов в полученный список
						candidate.insert(candidate.end(), noContext.begin(), noContext.end());
						// Очищаем список кандидатов с неверным контекстом
						noContext.clear();
					}
					// Добавляем полученный вариант статистики
					if(!duoCandidate) info->push_back(move(candidate));
					// Добавляем в список статистики, дополнительные варианты
					else info->back().insert(info->back().end(), candidate.begin(), candidate.end());
				}
				// Если вывод отладочной информации разрешён
				if(this->isOption(options_t::debug, options)){
					// Выводим основное сообщение отладки
					this->alphabet->log("spell result: [%ls]\r\n", alphabet_t::log_t::info, this->logfile, this->tokenizer->restore(words).c_str());
				}
				// Очищаем список собранных слов
				words.clear();
			}
			// Добавляем токен-разделитель в список
			if(!word.empty()) result.push_back(word);
			// Обнуляем позиции
			pos1 = 1;
			pos2 = 0;
			// Очищаем объект сборки гипотез
			dmp.clear();
			// Очищаем список собранных слов
			parts.clear();
			// Очищаем предыдущее слово
			lastWord.clear();
			// Сбрасываем флаг добавления дубликата
			duoCandidate = false;
		};
		/**
		 * modeFn Функция обработки разбитого текста
		 * @param word  слово для обработки
		 * @param ctx   контекст к которому принадлежит слово
		 * @param reset флаг сброса контекста
		 * @param stop  флаг завершения обработки
		 */
		auto modeFn = [&](const wstring & wrd, const vector <string> & ctx, const bool reset, const bool stop) noexcept {
			// Если слово получено
			if(!wrd.empty()){
				// Получаем слово в нижнем регистре
				const wstring & word = this->alphabet->toLower(wrd);
				// Выполняем проверку на существование слова
				const size_t idw = this->idw(word);
				// Проверяем идентификатор на валидность слова
				if((idw != idw_t::NIDW) && !this->tokenizer->isIdWord(idw)){
					// Отправляем результат
					resFn(wrd);
					// Выводим результат
					return true;
				}
				// Флаги не было обработанных и существующих слов
				bool mode = false, exist = (((idw == size_t(token_t::unk)) || (idw == size_t(token_t::null))) ? false : true);
				// Если вывод отладочной информации разрешён
				if(this->isOption(options_t::debug, options)){
					// Если слово принято
					if(idw != idw_t::NIDW)
						// Выводим основное сообщение отладки
						this->alphabet->log("word accepted: [%ls | %u]\r\n", alphabet_t::log_t::info, this->logfile, word.c_str(), idw);
					// Если слово не принято
					else this->alphabet->log("word not accepted: [%ls]\r\n", alphabet_t::log_t::warning, this->logfile, word.c_str());
				}
				// Если нужно удалять повторяющиеся слова
				if(this->isOption(options_t::wordrep, options) && !this->tokenizer->isToken(idw)){
					// Если текущее слово и предыдущее совпадают
					if(!lastWord.empty() && (lastWord.compare(word) == 0)){
						// Если разрешено собирать статистику
						if(info != nullptr){
							// Добавляем полученный вариант статистики
							if(!duoCandidate){
								// Создаём кандидата для добавления в список статистики
								vector <pair <wstring, wstring>> candidate = {{(lastWord.wreal() + wstring(1, L' ') + word), word}};
								// Добавляем кандидата в список статистики
								info->push_back(move(candidate));
							// Добавляем в список статистики, дополнительные варианты
							} else info->back().emplace_back((lastWord.wreal() + wstring(1, L' ') + word), word);
							// Запоминаем что дубликат добавлен
							duoCandidate = true;
						}
						// Выводим результат
						return true;
					}
					// Запоминаем предыдущее слово
					lastWord = word;
				}
				// Если нужно пропускать слова с дефисами
				if(this->isOption(options_t::skiphyp, options) && !this->tokenizer->isToken(idw)){
					// Если в слове встретились дефисы
					if(word.find(L'-') != wstring::npos) goto addAsIt;
				}
				// Если нужно пропускать слова в верхнем регистре
				if(this->isOption(options_t::skipupp, options) && !this->tokenizer->isToken(idw)){
					// Если слова не изменились, значит всё слово в верхнем регистре
					if(this->alphabet->toUpper(word).compare(word) == 0) goto addAsIt;
				}
				// Если нужно пропускать слова с латинскими символами
				if(this->isOption(options_t::skiplat, options) && !this->tokenizer->isToken(idw)){
					// Если в слове встретились латинские символы
					if(this->alphabet->checkLatian(this->alphabet->toLower(word))) goto addAsIt;
				}{
					// Получаем слово разделённое пробелами
					const auto & bwrd = this->backword(idw, word, options, pos1, pos2, parts);
					// Если слово получено
					if((bwrd.first != idw_t::NIDW) && !this->tokenizer->isToken(bwrd.first) && this->dict->isIdw(bwrd.first)){
						// Устанавливаем идентификатор слова
						hypothesis.idw = bwrd.first;
						// Устанавливаем эталонную фразу
						hypothesis.etalon = bwrd.second.second;
						// Устанавливаем полученное слово
						hypothesis.word = (* this->dict->word(bwrd.first));
						// Выполняем расчёт дистанции Дамерау-Левенштейна
						hypothesis.lev = algorithms.damerau(bwrd.second.first, bwrd.second.second);
						// Выполняем расчёт значения Танимото
						hypothesis.tmo = algorithms.tanimoto(bwrd.second.first, bwrd.second.second);
						// Добавляем гипотезу в сборщик гипотез
						dmp.once(hypothesis, parts.size(), pos2);
					}
				}
				// Если слово не существует, пытаемся его исправить.
				if((word.length() > 1) && (idw == idw_t::NIDW)){
					// Очищаем кэш последовательности
					if(info != nullptr) cache.clear();
					// Выполняем поиск варианта слова
					const auto & res = this->dict->find(word, dmp, pos1);
					// Если разрешено выполнять сплиты по дефисам
					if(this->isOption(options_t::hyphen, options)){
						// Список последовательности
						vector <size_t> seq;
						// Выполняем сплит слов
						this->hyphen(word, this->unsetOption(options_t::uppers, options), seq, words);
						// Если сплит выполнен
						if(!seq.empty() && !words.empty()){
							// Запоминаем, что сплит выполнен
							mode = true;
							// Получившийся текст и временное слово
							wstring text = L"", tmp = L"";
							// Список собранных гипотез
							vector <dumper_t::awrd_t> hypotheses(words.size());
							// Переходим по всему списку
							for(size_t i = 0; i < words.size(); i++){
								// Получаем временное слово
								tmp = move(words.at(i));
								// Если текст не пустой, добавляем разделитель
								if(!text.empty()) text.append(1, L' ');
								// Добавляем слово в список
								text.append(tmp);
								// Сбрасываем значение Танимото
								hypothesis.tmo = 0.0;
								// Устанавливаем флаг сплитов
								hypothesis.split = true;
								// Устанавливаем идентификатор слова
								hypothesis.idw = seq.at(i);
								// Сбрасываем дистанцию Дамерау-Левенштейна
								hypothesis.lev = MAX_LENGTH_TEXT;
								// Устанавливаем эталонную фразу
								hypothesis.etalon = (i == 0 ? wrd : L"");
								// Если идентификатор существует
								if(this->dict->isIdw(hypothesis.idw)){
									// Устанавливаем полученное слово
									hypothesis.word = * this->dict->word(hypothesis.idw);
								// Устанавливаем слово, как есть
								} else hypothesis.word = move(word_t(tmp));
								// Устанавливаем полученную гипотезу
								hypotheses.at(i) = hypothesis;
							}
							// Добавляем список гипотез в список
							pos1 = dmp.add(hypotheses, algorithms.damerau(word, text), algorithms.tanimoto(word, text), pos1);
						}
					}
					// Если разрешено выполнять сплиты
					if(!mode && this->isOption(options_t::split, options)){
						// Удаляем опцию исправления слов с альтернативными буквами
						u_short opts = this->unsetOption(options_t::alter, options);
						// Удаляем опцию простановки регистров в словах
						opts = this->unsetOption(options_t::uppers, opts);
						// Выполняем сплит слова
						this->split(word, opts, words);
						// Если сплит выполнен
						if(words.size() > 1){
							// Запоминаем, что сплит выполнен
							mode = true;
							// Получившийся текст и временное слово
							wstring text = L"", tmp = L"";
							// Список собранных гипотез
							vector <dumper_t::awrd_t> hypotheses(words.size());
							// Переходим по всему списку
							for(size_t i = 0; i < words.size(); i++){
								// Получаем временное слово
								tmp = move(words.at(i));
								// Если текст не пустой, добавляем разделитель
								if(!text.empty()) text.append(1, L' ');
								// Добавляем слово в список
								text.append(tmp);
								// Сбрасываем значение Танимото
								hypothesis.tmo = 0.0;
								// Устанавливаем флаг сплитов
								hypothesis.split = true;
								// Сбрасываем дистанцию Дамерау-Левенштейна
								hypothesis.lev = MAX_LENGTH_TEXT;
								// Устанавливаем идентификатор слова
								hypothesis.idw = this->alm->getIdw(tmp);
								// Устанавливаем эталонную фразу
								hypothesis.etalon = (i == 0 ? wrd : L"");
								// Если идентификатор существует
								if(this->dict->isIdw(hypothesis.idw)){
									// Устанавливаем полученное слово
									hypothesis.word = * this->dict->word(hypothesis.idw);
								// Устанавливаем слово, как есть
								} else hypothesis.word = move(word_t(tmp));
								// Устанавливаем полученную гипотезу
								hypotheses.at(i) = hypothesis;
							}
							// Добавляем список гипотез в список
							pos1 = dmp.add(hypotheses, algorithms.damerau(word, text), algorithms.tanimoto(word, text), pos1);
						}
					}
					// Если сплит выполнять не удалось
					if(!mode){
						// Если вариант не был добавлен, добавляем как есть
						mode = (res.second > 0);
						// Если слово небыло найдено, устанавливаем флаг
						exist = (res.second > 0);
						// Запоминаем позицию варианта
						pos1 = (res.second > 0 ? res.first : pos1);
					}
				}
				// Устанавливаем метку перехода
				addAsIt:
				// Иначе добавляем слово как оно есть
				if(!mode){
					// Сбрасываем значение Танимото
					hypothesis.tmo = 0.0;
					// Устанавливаем идентификатор слова
					hypothesis.idw = idw;
					// Устанавливаем эталонную фразу
					hypothesis.etalon = wrd;
					// Сбрасываем флаг сплитов
					hypothesis.split = false;
					// Сбрасываем дистанцию Дамерау-Левенштейна
					hypothesis.lev = MAX_LENGTH_TEXT;
					// Сбрасываем значение слова
					hypothesis.word = (exist ? L"" : NOTFOUND);
					// Добавляем вариант слова
					pos1 = dmp.add(hypothesis, pos1);
					// Если слово существует
					if((info != nullptr) && (idw != idw_t::NIDW)){
						// Если это не спец-токен
						if(!this->tokenizer->isToken(idw)){
							// Добавляем слово в кэш
							cache.push(idw, word);
							// Получаем список последовательности
							const auto & seq = cache.gets();
							// Если список не является юниграммой
							if((seq.size() > 1) && !this->alm->check(seq, (u_short) 2)){
								// Получаем список слов
								const auto & words = cache.getw();
								// Если список получен, получаем строку
								if(!words.empty()){
									// Получаем текст ответа
									const wstring & text = this->tokenizer->restore(words);
									// Если текст получен
									if(!text.empty()) noContext.push_back(make_pair(text, NOTFOUND));
								}
								// Очищаем кэш последовательности
								cache.clear();
							}
						// Очищаем кэш последовательности
						} else cache.clear();
					}
				}
			}
			// Если это конец, отправляем результат
			if(stop) resFn();
			// Выводим результат
			return true;
		};
		// Выполняем разбивку текста на токены
		this->tokenizer->run(text, modeFn);
		// Выводим полученный результат
		if(!result.empty()) text = this->tokenizer->restore(result);
	}
}
/**
 * analyze Метод проведения анализа текста
 * @param text    текст для проведения анализа
 * @param options опции приложения
 * @param info    список слов для которых выполнена коррекция
 */
void anyks::ASpell::analyze(const wstring & text, const u_short options, vector <unordered_map <wstring, set <wstring>>> & info) const noexcept {
	// Если текст передан
	if(!text.empty() && (this->dict != nullptr) && (this->alm != nullptr) && (this->alphabet != nullptr) && (this->tokenizer != nullptr)){
		// Очищаем список анализируемых слов
		info.clear();
		// Объект левенштейна
		lev_t algorithms;
		// Предыдущее слово
		word_t lastWord = L"";
		// Позиция добавления слов
		size_t pos1 = 1, pos2 = 0;
		// Флаг добавления дублирующего кандидата
		bool duoCandidate = false;
		// Создаём объект гипотезы
		dumper_t::awrd_t hypothesis;
		// Список полученных слов и слов для удаления пробелов
		vector <wstring> words, parts;
		// Получаем объект сборки гипотез
		dumper_t dmp = this->dict->dumper();
		// Кэш последовательности
		cache_t cache(this->alm->getSize());
		// Создаём кандидатов с неправельным контекстом
		unordered_map <wstring, set <wstring>> noContext;
		// Отключаем флаг смешивания словарей
		const_cast <alm_t *> (this->alm)->unsetOption(alm_t::options_t::mixdicts);
		// Если флаг исправления слов с альтернативными буквами активен
		if(this->isOption(options_t::alter, options)) dmp.setOption(dumper_t::options_t::alter);
		// Если флаг простановки регистра слов активен
		if(this->isOption(options_t::uppers, options)) dmp.setOption(dumper_t::options_t::uppers);
		/**
		 * resFn Функция вывода результата
		 */
		auto resFn = [&]() noexcept {
			// Получаем список лучших вариантов
			const auto & bests = dmp.bests();
			// Если список лучших вариантов получен
			if(!bests.empty()){
				// Список полученных кандидатов
				vector <pair <wstring, wstring>> candidates;
				// Список вариантов кандидата
				unordered_map <wstring, set <wstring>> candidate;
				// Переходим по всем вариантам
				for(auto & item : bests){
					// Очищаем список кандидатов
					candidates.clear();
					// Выполняем сборку кандидатов
					dmp.ctx(item, words, &candidates);
					// Если кандидаты получены
					if(!candidates.empty()){
						// Переходим по всему списку кандидатов
						for(auto & item : candidates){
							// Если вариант получен
							if(!item.first.empty()){
								// Ищем вариант в списке
								auto it = candidate.find(item.first);
								// Если вариант получен
								if(it != candidate.end()) it->second.emplace(item.second);
								// Иначе создаём новый вариант
								else {
									// Создаём новый список вариантов
									set <wstring> words = {item.second};
									// Добавляем список вариантов
									candidate.emplace(item.first, move(words));
								}
							}
						}
					}
				}
				// Если список вариантов с неверным контекстом получен, добавляем его в полученный список
				if(!noContext.empty()){
					// Добавляем список вариантов в полученный список
					candidate.insert(noContext.begin(), noContext.end());
					// Очищаем список кандидатов с неверным контекстом
					noContext.clear();
				}
				// Добавляем полученный вариант аналитики
				if(!duoCandidate) info.push_back(move(candidate));
				// Добавляем в список аналитики, дополнительные варианты
				else info.back().insert(candidate.begin(), candidate.end());
			}
			// Обнуляем позиции
			pos1 = 1;
			pos2 = 0;
			// Очищаем объект сборки гипотез
			dmp.clear();
			// Очищаем список собранных слов
			parts.clear();
			// Очищаем предыдущее слово
			lastWord.clear();
			// Сбрасываем флаг добавления дубликата
			duoCandidate = false;
		};
		/**
		 * modeFn Функция обработки разбитого текста
		 * @param word  слово для обработки
		 * @param ctx   контекст к которому принадлежит слово
		 * @param reset флаг сброса контекста
		 * @param stop  флаг завершения обработки
		 */
		auto modeFn = [&](const wstring & wrd, const vector <string> & ctx, const bool reset, const bool stop) noexcept {
			// Если слово получено
			if(!wrd.empty()){
				// Получаем слово в нижнем регистре
				const wstring & word = this->alphabet->toLower(wrd);
				// Выполняем проверку на существование слова
				const size_t idw = this->idw(word);
				// Проверяем идентификатор на валидность слова
				if((idw != idw_t::NIDW) && !this->tokenizer->isIdWord(idw)){
					// Отправляем результат
					resFn();
					// Выводим результат
					return true;
				}
				// Флаги не было обработанных и существующих слов
				bool mode = false, exist = (((idw == size_t(token_t::unk)) || (idw == size_t(token_t::null))) ? false : true);
				// Если вывод отладочной информации разрешён
				if(this->isOption(options_t::debug, options)){
					// Если слово принято
					if(idw != idw_t::NIDW)
						// Выводим основное сообщение отладки
						this->alphabet->log("word accepted: [%s | %u]\r\n", alphabet_t::log_t::info, this->logfile, word.c_str(), idw);
					// Если слово не принято
					else this->alphabet->log("word not accepted: [%s]\r\n", alphabet_t::log_t::warning, this->logfile, word.c_str());
				}
				// Если нужно удалять повторяющиеся слова
				if(this->isOption(options_t::wordrep, options) && !this->tokenizer->isToken(idw)){
					// Если текущее слово и предыдущее совпадают
					if(!lastWord.empty() && (lastWord.compare(word) == 0)){
						// Список слов для добавления
						set <wstring> words = {word};
						// Список вариантов кандидата
						unordered_map <wstring, set <wstring>> candidate;
						// Создаём кандидата для добавления в список аналитики
						candidate.emplace((lastWord.wreal() + wstring(1, L' ') + word), move(words));
						// Если нужно выводить список обработанных вариантов
						if(!duoCandidate) info.push_back(move(candidate));
						// Добавляем в список аналитики, дополнительные варианты
						else info.back().insert(candidate.begin(), candidate.end());
						// Запоминаем что дубликат добавлен
						duoCandidate = true;
						// Выводим результат
						return true;
					}
					// Запоминаем предыдущее слово
					lastWord = word;
				}
				// Если нужно пропускать слова с дефисами
				if(this->isOption(options_t::skiphyp, options) && !this->tokenizer->isToken(idw)){
					// Если в слове встретились латинские символы
					if(word.find(L'-') != wstring::npos) goto addAsIt;
				}
				// Если нужно пропускать слова в верхнем регистре
				if(this->isOption(options_t::skipupp, options) && !this->tokenizer->isToken(idw)){
					// Если слова не изменились, значит всё слово в верхнем регистре
					if(this->alphabet->toUpper(word).compare(word) == 0) goto addAsIt;
				}
				// Если нужно пропускать слова с латинскими символами
				if(this->isOption(options_t::skiplat, options) && !this->tokenizer->isToken(idw)){
					// Если в слове встретились латинские символы
					if(this->alphabet->checkLatian(this->alphabet->toLower(word))) goto addAsIt;
				}{
					// Получаем слово разделённое пробелами
					const auto & bwrd = this->backword(idw, word, options, pos1, pos2, parts);
					// Если слово получено
					if((bwrd.first != idw_t::NIDW) && !this->tokenizer->isToken(bwrd.first) && this->dict->isIdw(bwrd.first)){
						// Устанавливаем идентификатор слова
						hypothesis.idw = bwrd.first;
						// Устанавливаем эталонную фразу
						hypothesis.etalon = bwrd.second.second;
						// Устанавливаем полученное слово
						hypothesis.word = (* this->dict->word(bwrd.first));
						// Выполняем расчёт дистанции Дамерау-Левенштейна
						hypothesis.lev = algorithms.damerau(bwrd.second.first, bwrd.second.second);
						// Выполняем расчёт значения Танимото
						hypothesis.tmo = algorithms.tanimoto(bwrd.second.first, bwrd.second.second);
						// Добавляем гипотезу в сборщик гипотез
						dmp.once(hypothesis, parts.size(), pos2);
					}
				}
				// Если слово не существует, пытаемся его исправить.
				if((word.length() > 1) && (idw == idw_t::NIDW)){
					// Очищаем кэш последовательности
					cache.clear();
					// Выполняем поиск варианта слова
					const auto & res = this->dict->find(word, dmp, pos1);
					// Если разрешено выполнять сплиты по дефисам
					if(this->isOption(options_t::hyphen, options)){
						// Список последовательности
						vector <size_t> seq;
						// Выполняем сплит слов
						this->hyphen(word, this->unsetOption(options_t::uppers, options), seq, words);
						// Если сплит выполнен
						if(!seq.empty() && !words.empty()){
							// Запоминаем, что сплит выполнен
							mode = true;
							// Получившийся текст и временное слово
							wstring text = L"", tmp = L"";
							// Список собранных гипотез
							vector <dumper_t::awrd_t> hypotheses(words.size());
							// Переходим по всему списку
							for(size_t i = 0; i < words.size(); i++){
								// Получаем временное слово
								tmp = move(words.at(i));
								// Если текст не пустой, добавляем разделитель
								if(!text.empty()) text.append(1, L' ');
								// Добавляем слово в список
								text.append(tmp);
								// Сбрасываем значение Танимото
								hypothesis.tmo = 0.0;
								// Устанавливаем флаг сплитов
								hypothesis.split = true;
								// Устанавливаем идентификатор слова
								hypothesis.idw = seq.at(i);
								// Сбрасываем дистанцию Дамерау-Левенштейна
								hypothesis.lev = MAX_LENGTH_TEXT;
								// Устанавливаем эталонную фразу
								hypothesis.etalon = (i == 0 ? wrd : L"");
								// Если идентификатор существует
								if(this->dict->isIdw(hypothesis.idw)){
									// Устанавливаем полученное слово
									hypothesis.word = * this->dict->word(hypothesis.idw);
								// Устанавливаем слово, как есть
								} else hypothesis.word = move(word_t(tmp));
								// Устанавливаем полученную гипотезу
								hypotheses.at(i) = hypothesis;
							}
							// Добавляем список гипотез в список
							pos1 = dmp.add(hypotheses, algorithms.damerau(word, text), algorithms.tanimoto(word, text), pos1);
						}
					}
					// Если разрешено выполнять сплиты
					if(!mode && this->isOption(options_t::split, options)){
						// Удаляем опцию исправления слов с альтернативными буквами
						u_short opts = this->unsetOption(options_t::alter, options);
						// Удаляем опцию простановки регистров в словах
						opts = this->unsetOption(options_t::uppers, opts);
						// Выполняем сплит слова
						this->split(word, opts, words);
						// Если сплит выполнен
						if(words.size() > 1){
							// Запоминаем, что сплит выполнен
							mode = true;
							// Получившийся текст и временное слово
							wstring text = L"", tmp = L"";
							// Список собранных гипотез
							vector <dumper_t::awrd_t> hypotheses(words.size());
							// Переходим по всему списку
							for(size_t i = 0; i < words.size(); i++){
								// Получаем временное слово
								tmp = move(words.at(i));
								// Если текст не пустой, добавляем разделитель
								if(!text.empty()) text.append(1, L' ');
								// Добавляем слово в список
								text.append(tmp);
								// Сбрасываем значение Танимото
								hypothesis.tmo = 0.0;
								// Устанавливаем флаг сплитов
								hypothesis.split = true;
								// Сбрасываем дистанцию Дамерау-Левенштейна
								hypothesis.lev = MAX_LENGTH_TEXT;
								// Устанавливаем идентификатор слова
								hypothesis.idw = this->alm->getIdw(tmp);
								// Устанавливаем эталонную фразу
								hypothesis.etalon = (i == 0 ? wrd : L"");
								// Если идентификатор существует
								if(this->dict->isIdw(hypothesis.idw)){
									// Устанавливаем полученное слово
									hypothesis.word = * this->dict->word(hypothesis.idw);
								// Устанавливаем слово, как есть
								} else hypothesis.word = move(word_t(tmp));
								// Устанавливаем полученную гипотезу
								hypotheses.at(i) = hypothesis;
							}
							// Добавляем список гипотез в список
							pos1 = dmp.add(hypotheses, algorithms.damerau(word, text), algorithms.tanimoto(word, text), pos1);
						}
					}
					// Если сплит выполнять не удалось
					if(!mode){
						// Если вариант не был добавлен, добавляем как есть
						mode = (res.second > 0);
						// Если слово небыло найдено, устанавливаем флаг
						exist = (res.second > 0);
						// Запоминаем позицию варианта
						pos1 = (res.second > 0 ? res.first : pos1);
					}
				}
				// Устанавливаем метку перехода
				addAsIt:
				// Иначе добавляем слово как оно есть
				if(!mode){
					// Сбрасываем значение Танимото
					hypothesis.tmo = 0.0;
					// Устанавливаем идентификатор слова
					hypothesis.idw = idw;
					// Устанавливаем эталонную фразу
					hypothesis.etalon = wrd;
					// Сбрасываем флаг сплитов
					hypothesis.split = false;
					// Сбрасываем дистанцию Дамерау-Левенштейна
					hypothesis.lev = MAX_LENGTH_TEXT;
					// Сбрасываем значение слова
					hypothesis.word = (exist ? L"" : NOTFOUND);
					// Добавляем вариант слова
					pos1 = dmp.add(hypothesis, pos1);
					// Если слово существует
					if(idw != idw_t::NIDW){
						// Если это не спец-токен
						if(!this->tokenizer->isToken(idw)){
							// Добавляем слово в кэш
							cache.push(idw, word);
							// Получаем список последовательности
							const auto & seq = cache.gets();
							// Если список не является юниграммой
							if((seq.size() > 1) && !this->alm->check(seq, (u_short) 2)){
								// Получаем список слов
								const auto & words = cache.getw();
								// Если список получен, получаем строку
								if(!words.empty()){
									// Результат несуществующего варианта
									set <wstring> noExist = {NOTFOUND};
									// Получаем текст ответа
									const wstring & text = this->tokenizer->restore(words);
									// Если текст получен
									if(!text.empty()) noContext.emplace(text, move(noExist));
								}
								// Очищаем кэш последовательности
								cache.clear();
							}
						// Очищаем кэш последовательности
						} else cache.clear();
					}
				}
			}
			// Если это конец, отправляем результат
			if(stop) resFn();
			// Выводим результат
			return true;
		};
		// Выполняем разбивку текста на токены
		this->tokenizer->run(text, modeFn);
	}
}
/**
 * setAlm Метод установки языковой модели
 * @param alm объект языковой модели
 */
void anyks::ASpell::setAlm(const alm_t * alm) noexcept {
	// Устанавливаем объект языковой модели
	if(alm != nullptr) this->alm = alm;
}
/**
 * setDict Метод установки словаря
 * @param dict объект словаря
 */
void anyks::ASpell::setDict(const dict_t * dict) noexcept {
	// Устанавливаем объект словаря
	if(dict != nullptr) this->dict = dict;
}
/**
 * setLogfile Метод установки файла для вывода логов
 * @param logifle адрес файла для вывода отладочной информации
 */
void anyks::ASpell::setLogfile(const char * logfile) noexcept {
	// Устанавливаем адрес log файла
	this->logfile = logfile;
}
/**
 * setAlphabet Метод установки словаря
 * @param alphabet объект алфавита словаря
 */
void anyks::ASpell::setAlphabet(const alphabet_t * alphabet) noexcept {
	// Устанавливаем объект алфавита
	if(alphabet != nullptr) this->alphabet = alphabet;
}
/**
 * setTokenizer Метод установки токенизатора
 * @param tokenizer объект токенизатора
 */
void anyks::ASpell::setTokenizer(const tokenizer_t * tokenizer) noexcept {
	// Устанавливаем объект токенизатора
	if(tokenizer != nullptr) this->tokenizer = tokenizer;
}
/**
 * ASpell Конструктор
 * @param dict объект словаря
 */
anyks::ASpell::ASpell(const dict_t * dict) noexcept {
	// Устанавливаем объект словаря
	this->setDict(dict);
}
/**
 * ASpell Конструктор
 * @param dict объект словаря
 * @param alm  объект языковой модели
 */
anyks::ASpell::ASpell(const dict_t * dict, const alm_t * alm) noexcept {
	// Устанавливаем объект языковой модели
	this->setAlm(alm);
	// Устанавливаем объект словаря
	this->setDict(dict);
}
/**
 * ASpell Конструктор
 * @param dict      объект словаря
 * @param alm       объект языковой модели
 * @param alphabet  объект алфавита
 * @param tokenizer объект токенизатора
 */
anyks::ASpell::ASpell(const dict_t * dict, const alm_t * alm, const alphabet_t * alphabet, const tokenizer_t * tokenizer) noexcept {
	// Устанавливаем объект языковой модели
	this->setAlm(alm);
	// Устанавливаем объект словаря
	this->setDict(dict);
	// Устанавливаем объект алфавита
	this->setAlphabet(alphabet);
	// Устанавливаем объект токенизатора
	this->setTokenizer(tokenizer);
}
