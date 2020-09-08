/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <alt.hpp>

/**
 * empty Метод проверки на пустоту данных
 * @return результат проверки
 */
const bool anyks::Alt::empty() const noexcept {
	// Выводим результат проверки
	return (this->letters.empty() || (this->ncontext.empty() && this->context.empty()));
}
/**
 * uppers Метод извлечения регистров из слова
 * @param word слово из которого нужно получить регистры
 * @return     регистры полученные из слова
 */
const size_t anyks::Alt::uppers(const wstring & word) const noexcept {
	// Результат работы функции
	size_t result = 0;
	// Если слово передано
	if(!word.empty() && (word.length() <= 35)){
		// Позиция дефиса в слове
		size_t pos = 0;
		// Создаем блок байтов
		bitset <35> data;
		// Устанавливаем первый бит
		data.set(0);
		// Выполняем поиск дефиса
		while((pos = word.find(L'-', pos)) != wstring::npos){
			// Устанавливаем позицию в слове
			if((pos + 1) < word.length()) data.set(pos + 1);
			// Увеличиваем позицию
			pos++;
		}
		// Получаем регистры слова
		result = data.to_ullong();
	}
	// Выводим результат
	return result;
}
/**
 * getUWords Метод извлечения списка идентификаторов слов, которые всегда начинаются с заглавной буквы
 * @return список идентификаторов слов
 */
const map <size_t, size_t> & anyks::Alt::getUWords() const noexcept {
	// Выводим список собранных слов
	return this->uwords;
}
/**
 * getUWord Метод извлеченя регистров слова по его идентификатору
 * @param idw идентификатор слова для проверки
 * @return    регистры слова, если слово найдено
 */
const size_t anyks::Alt::getUWord(const size_t idw) const noexcept {
	// Результат работы функции
	size_t result = 0;
	// Выполняем поиск слова
	auto it = this->uwords.find(idw);
	// Если слово найдено, выводим регистры слова
	if(it != this->uwords.end()) result = it->second;
	// Выводим результат
	return result;
}
/**
 * getUWord Метод извлеченя регистров слова
 * @param word слово для проверки
 * @return     регистры слова, если слово найдено
 */
const size_t anyks::Alt::getUWord(const wstring & word) const noexcept {
	// Выводим результат проверки
	return this->getUWord(this->tokenizer->idw(word));
}
/**
 * check Метод проверки соответствия слова на альтернативную букву
 * @param idw идентификатор слова для проверки
 * @return    результат проверки
 */
const bool anyks::Alt::check(const size_t idw) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если идентификатор передан
	if((idw > 0) && (idw != idw_t::NIDW)){
		// Выводим результат
		result = ((this->ncontext.count(idw) > 0) || (this->context.count(idw) > 0));
	}
	// Выводим результат
	return result;
}
/**
 * check Метод проверки соответствия слова на альтернативную букву
 * @param word слово для проверки
 * @return     результат проверки
 */
const bool anyks::Alt::check(const wstring & word) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если слово передано
	if(!word.empty() && (this->tokenizer != nullptr)){
		// Выполняем проверку слова
		result = this->check(this->tokenizer->idw(word));
	}
	// Выводим результат
	return result;
}
/**
 * check Метод проверки существования альтернативной буквы
 * @param  letter буква для проверки
 * @return        результат проверки
 */
const bool anyks::Alt::check(const wchar_t letter) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если буква передана
	if((letter > 0) && !this->letters.empty()){
		// Выполняем проверку
		result = (this->letters.count(letter) > 0);
	}
	// Выводим результат
	return result;
}
/**
 * check Метод проверки соответствия слова на альтернативную букву
 * @param idw     идентификатор слова для проверки
 * @param context слово является контексто-зависимым
 * @return        результат проверки
 */
const bool anyks::Alt::check(const size_t idw, const bool context) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если идентификатор передан
	if((idw > 0) && (idw != idw_t::NIDW)){
		// Выводим результат
		result = (context ? (this->context.count(idw) > 0) : (this->ncontext.count(idw) > 0));
	}
	// Выводим результат
	return result;
}
/**
 * check Метод проверки соответствия слова на альтернативную букву
 * @param word    слово для проверки
 * @param context слово является контексто-зависимым
 * @return        результат проверки
 */
const bool anyks::Alt::check(const wstring & word, const bool context) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если слово передано
	if(!word.empty() && (this->tokenizer != nullptr)){
		// Выполняем проверку слова
		result = this->check(this->tokenizer->idw(word), context);
	}
	// Выводим результат
	return result;
}
/**
 * alt Метод получения альтернативной буквы
 * @param  letter буква для проверки
 * @return        альтернативная буква
 */
const wchar_t anyks::Alt::alt(const wchar_t letter) const noexcept {
	// Резузльтат работы функции
	wchar_t result = 0;
	// Если буква передана
	if(letter > 0){
		// Ищем букву в списке
		auto it = this->letters.find(letter);
		// Выводим результат
		result = (it != this->letters.end() ? it->second : result);
	}
	// Выводим в результат
	return result;
}
/**
 * rel Метод получения реальной буквы из альтернативной
 * @param  letter альтернативная буква
 * @return        реальная буква
 */
const wchar_t anyks::Alt::rel(const wchar_t letter) const noexcept {
	// Резузльтат работы функции
	wchar_t result = 0;
	// Если альтернативная буква передана
	if((letter > 0) && !this->letters.empty()){
		// Переходим по всему списку альтернативных букв
		for(auto & item : this->letters){
			// Если буква найдена
			if(letter == item.second){
				// Запоминаем результат
				result = item.first;
				// Выходим из цикла
				break;
			}
		}
	}
	// Выводим в результат
	return result;
}
/**
 * clear Метод очистки данных
 */
void anyks::Alt::clear() noexcept {
	// Очищаем список слов, начинающихся с заглавной буквы
	this->uwords.clear();
	// Очищаем список собранных букв
	this->letters.clear();
	// Очищаем список контекстно-зависимых слов
	this->context.clear();
	// Очищаем список не контекстно-зависимых слов
	this->ncontext.clear();
}
/**
 * addUWord Метод добавления слова, которое всегда начинается с заглавной буквы
 * @param word слово для добавления
 */
void anyks::Alt::addUWord(const wstring & word) noexcept {
	// Выполняем добавление слова в список, если слово передано
	if(word.size() > 3) this->addUWord(this->tokenizer->idw(word), this->uppers(word));
}
/**
 * addUWord Метод добавления идентификатора слова, которое всегда начинается с заглавной буквы
 * @param idw идентификатор слова для добавления
 * @param ups регистры добавляемого слова
 */
void anyks::Alt::addUWord(const size_t idw, const size_t ups) noexcept {
	// Если идентификатор передан
	if((idw != 0) && (idw != idw_t::NIDW) && (ups > 0)) this->uwords.emplace(idw, ups);
}
/**
 * setUWords Метод добавления списка идентификаторов слов, которые всегда начинаются с заглавной буквы
 * @param words список идентификаторов слов для добавления
 */
void anyks::Alt::setUWords(const map <size_t, size_t> & words) noexcept {
	// Если список слов передан
	if(!words.empty()) this->uwords = words;
}
/**
 * add Метод добавления альтернативной буквы
 * @param letter буква у которой есть альтернатива
 * @param alt    альтернативная буква
 */
void anyks::Alt::add(const wchar_t letter, const wchar_t alt) noexcept {
	// Если буквы переданы
	if((letter > 0) && (alt > 0)) this->letters.emplace(letter, alt);
}
/**
 * add Метод добавления слова с альтернативной буквой
 * @param word    слово для добавления
 * @param alt     альтернативное слово для добавления
 * @param context слово является контексто-зависимым
 */
void anyks::Alt::add(const wstring & word, const wstring & alt, const bool context) noexcept {
	// Если слова переданы
	if(!word.empty() && !alt.empty() && (this->tokenizer != nullptr)){
		// Получаем новое значение альтернативного слова
		wstring tmpAlt = this->alphabet->toLower(alt);
		// Получаем новое значение слова
		const wstring & tmpWord = this->alphabet->toLower(word);
		// Если слова получены
		if(!tmpAlt.empty() && !tmpWord.empty()){
			// Флаг соответствия слова
			bool mode = false;
			// Переходим по всем альтернативным буквам
			for(auto & item : this->letters){
				// Выполняем поиск альтернативной буквы в слове
				mode = ((tmpWord.find(item.first) != wstring::npos) && (tmpAlt.find(item.second) != wstring::npos));
				// Если буква найдена, выходим
				if(mode) break;
			}
			// Если слово соответствует
			if(mode){
				// Получаем идентификатор альтернативного слова
				const size_t ida = this->tokenizer->idw(tmpAlt);
				// Получаем идентификатор слова
				const size_t idw = this->tokenizer->idw(tmpWord);
				// Если идентификаторы получены
				if((ida > 0) && (idw > 0) && (ida != idw_t::NIDW) && (idw != idw_t::NIDW)){
					// Если это контексто-зависимое слово
					if(context) this->context.emplace(idw, make_pair(ida, move(tmpAlt)));
					// Если это не контексто-зависимое слово
					else this->ncontext.emplace(idw, make_pair(ida, move(tmpAlt)));
				}
			}
		}
	}
}
/**
 * add Метод добавления слова с альтернативной буквой
 * @param word    слово для добавления
 * @param idw     идентификатор слова если есть
 * @param context слово является контексто-зависимым
 */
void anyks::Alt::add(const wstring & word, const size_t idw, const bool context) noexcept {
	// Если слово не пустое
	if(!word.empty() && !this->letters.empty() && (this->alphabet != nullptr) && (this->tokenizer != nullptr)){
		// Флаг соответствия слова
		bool mode = false;
		// Позиция в слове альтернативной буквы
		size_t pos = 0, ida = idw_t::NIDW;
		// Получаем слово в нижнем регистре
		wstring tmp = this->alphabet->toLower(word);
		// Переходим по всем альтернативным буквам
		for(auto & item : this->letters){
			// Сбрасываем флаг
			mode = false;
			// Выполняем замену в слове буквы
			while((pos = tmp.find(item.second, pos)) != wstring::npos){
				// Устанавливаем флаг
				mode = true;
				// Выполняем замену букв
				tmp.replace(pos, 1, 1, item.first);
				// Увеличиваем значение позиции
				pos++;
			}
			// Если слово соответствует
			if(mode){
				// Получаем идентификатор слова
				ida = this->tokenizer->idw(tmp);
				// Если слово можно добавить, добавляем в список слов
				if((ida > 0) && (ida != idw_t::NIDW) && (context ? (this->ncontext.count(ida) < 1) : (this->context.count(ida) < 1))){
					// Если - это, контексто-зависимое слово
					if(context)
						// Добавляем слово в список
						this->context.emplace(ida, make_pair(((idw > 0) && (idw != idw_t::NIDW) ? idw : this->tokenizer->idw(word)), word));
					// Если - это, не контексто-зависимое слово, добавляем в список
					else this->ncontext.emplace(ida, make_pair(((idw > 0) && (idw != idw_t::NIDW) ? idw : this->tokenizer->idw(word)), word));
				}
			}
		}
	}
}
/**
 * setAlphabet Метод установки алфавита
 * @param alphabet объект алфавита
 */
void anyks::Alt::setAlphabet(const alphabet_t * alphabet) noexcept {
	// Устанавливаем алфавит
	if(alphabet != nullptr) this->alphabet = alphabet;
}
/**
 * setTokenizer Метод установки токенизатора
 * @param tokenizer объект токенизатора
 */
void anyks::Alt::setTokenizer(const tokenizer_t * tokenizer) noexcept {
	// Устанавливаем объект токенизатора
	if(tokenizer != nullptr) this->tokenizer = tokenizer;
}
/**
 * get Метод получения слово с альтернативной буквой
 * @param idw     идентификатор слова для которого нужно получить слово с альтернативной буквой
 * @param context слово является контексто-зависимым
 * @return        слово с альтернативной буквой
 */
const pair <size_t, wstring> anyks::Alt::get(const size_t idw, const bool context) const noexcept {
	// Получаем пустой идентификатор
	const size_t nidw = idw_t::NIDW;
	// Результат работы функции
	pair <size_t, wstring> result = make_pair(nidw, L"");
	// Если слово передано
	if((idw > 0) && (idw != nidw)){
		// Выполняем поиск идентификатора слова
		auto it = (context ? this->context.find(idw) : this->ncontext.find(idw));
		// Если слово найдено
		if(it != (context ? this->context.end() : this->ncontext.end())){
			// Выводим результат
			result = it->second;
		}
	}
	// Выводим результат
	return result;
}
/**
 * get Метод получения слово с альтернативной буквой
 * @param word    слово для которого нужно получить слово с альтернативной буквой
 * @param context слово является контексто-зависимым
 * @return        слово с альтернативной буквой
 */
const pair <size_t, wstring> anyks::Alt::get(const wstring & word, const bool context) const noexcept {
	// Получаем пустой идентификатор
	const size_t nidw = idw_t::NIDW;
	// Результат работы функции
	pair <size_t, wstring> result = make_pair(nidw, L"");
	// Если слово передано
	if(!word.empty() && (this->tokenizer != nullptr)) result = this->get(this->tokenizer->idw(word), context);
	// Выводим результат
	return result;
}
/**
 * ~Alt Деструктор
 */
anyks::Alt::~Alt(){
	// Очищаем собранные данные
	this->clear();
}
