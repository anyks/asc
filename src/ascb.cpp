/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <ascb.hpp>

/**
 * read Метод чтения данных из файла в словарь
 */
void anyks::ASCb::read() noexcept {
	// Если основные объекты активны
	if((this->alm != nullptr) && (this->dict != nullptr) && (this->alphabet != nullptr) && (this->tokenizer != nullptr)){
		// Выполняем чтение данных словаря
		if(this->aspl->read() > 0){
			// Список пилотных слов
			set <wchar_t> pilots;
			// Список доменных зон
			set <string> domainZones;
			// Список пользовательских токенов и доменных зон
			vector <string> userTokensAlm;
			// Списки слов всегда начинающихся с заглавной буквы
			vector <size_t> uWords, uWordsCase;
			// Размер и опции языковой модели
			u_short sizeAlm = 0, optionsAlm = 0;
			// Список запрещёнынх токенов и токенов приводимых к <unk>
			set <token_t> tokensUnknownAlm, tokensDisableAlm;
			// Неизвестное слово, алфавит и локаль словаря
			string unknownAlm = "", alphabetDict = "", localeDict = "";
			// Словарь является зашифрованным и флаг апострофа в словаре
			bool encrypted = false, allowApostrophe = false, isALm2 = false;
			// Черный и белый списки слов словаря а также аббревиатур
			set <size_t> badwordsAlm, goodwordsAlm, abbrsTokenizer, suffixTokenizer;
			// Количество данных в словаре
			size_t countAltLetters = 0, countAltWords = 0, countAlm = 0, countNgrams = 0, countSubstitutes = 0;
			// Основные метаданные контейнера
			string code = "", name = "", author = "", lictype = "", lictext = "", contacts = "", copyright = "";
			// Если нужно получить размер шифрования
			if(!this->password.empty()){
				// Считываем размер шифрования
				this->aspl->get("aes", this->aes);
				// Устанавливаем размер шифрования
				this->aspl->setAES(this->aes);
			}{
				// Запоминаем, что словарь зашифрован
				this->aspl->get("encrypted", encrypted);
				// Если словарь зашифрован
				if(encrypted){
					// Если пароль существует
					if(!this->password.empty()){
						// Пароль сохраненный в словаре
						string password = "";
						// Считываем сохраненный пароль
						this->aspl->get("password", password, encrypted);
						// Если пароль не получен
						if(password.empty() || (this->password.compare(password) != 0)){
							// Выводим сообщение об ошибке
							this->alphabet->log("password '%s' is wrong\r\n", alphabet_t::log_t::error, this->logfile, this->password.c_str());
							// Выходим из приложения
							exit(EXIT_FAILURE);
						}
					// Если пароль не передан, выводим сообщение и выходим
					} else {
						// Выводим сообщение об ошибке
						this->alphabet->log("%s\r\n", alphabet_t::log_t::error, this->logfile, "password is empty");
						// Выходим из приложения
						exit(EXIT_FAILURE);
					}
				}
			}{
				// Дата генерации словаря
				time_t date = 0;
				// Количество слов во всех документах и количество всех документов в словаре
				size_t ad = 0, cw = 0;
				// Размер эмбеддинга
				size_t sizeEmbedding = 0;
				// Данные эмбеддинга
				string dataEmbedding = "";
				// Запоминаем количество слов в документах
				this->aspl->get("cw", cw);
				// Запоминаем количество документов в словаре
				this->aspl->get("ad", ad);
				// Запоминаем дату словаря
				this->aspl->get("date", date);
				// Запоминаем код словаря
				this->aspl->get("code", code);
				// Запоминаем название словаря
				this->aspl->get("name", name);
				// Запоминаем автора словаря
				this->aspl->get("author", author);
				// Запоминаем тип ALM
				this->aspl->get("almtype", isALm2);
				// Запоминаем размер словаря
				this->aspl->get("sizeAlm", sizeAlm);
				// Запоминаем тип лицензии
				this->aspl->get("lictype", lictype);
				// Запоминаем текст лицензии
				this->aspl->get("lictext", lictext);
				// Запоминаем контактную информацию автора
				this->aspl->get("contacts", contacts);
				// Запоминаем количество записанных буферов
				this->aspl->get("countAlm", countAlm);
				// Запоминаем список пилотных слов
				this->aspl->getValues("pilots", pilots);
				// Запоминаем список слов, которые всегда начинаются с заглавной буквы
				this->aspl->getValues("uwords", uWords);
				// Запоминаем список регистров слов, всегда начинающихся с заглавной буквы
				this->aspl->getValues("ucase", uWordsCase);
				// Запоминаем копирайт автора
				this->aspl->get("copyright", copyright);
				// Запоминаем опции языковой модели
				this->aspl->get("optionsAlm", optionsAlm);
				// Запоминаем количество обработанных N-грамм
				this->aspl->get("countNgrams", countNgrams);
				// Запоминаем размер эмбеддинга
				this->aspl->get("sizeEmbedding", sizeEmbedding);
				// Запоминаем количество альтернативных слов
				this->aspl->get("countAltWords", countAltWords);
				// Запоминаем локаль словаря
				this->aspl->get("locale", localeDict, encrypted);
				// Запоминаем чёрный список слов, языковой модели
				this->aspl->getValues("badwordsAlm", badwordsAlm);
				// Запоминаем белый список слов, языковой модели
				this->aspl->getValues("goodwordsAlm", goodwordsAlm);
				// Запоминаем разрешение использования апострофа
				this->aspl->get("allowApostrophe", allowApostrophe);
				// Запоминаем количество альтернативных букв
				this->aspl->get("countAltLetters", countAltLetters);
				// Запоминаем список доменных зон
				this->aspl->getStrings("zonesAlphabet", domainZones);
				// Запоминаем неизвестное слово, если такое есть
				this->aspl->get("unknownAlm", unknownAlm, encrypted);
				// Запоминаем алфавит словаря
				this->aspl->get("alphabet", alphabetDict, encrypted);
				// Запоминаем количество слов из смешанных алфавитов
				this->aspl->get("countSubstitutes", countSubstitutes);
				// Запоминаем список пользовательских токенов
				this->aspl->getStrings("userTokensAlm", userTokensAlm);
				// Запоминаем список аббревиатур
				this->aspl->getValues("abbrsTokenizer", abbrsTokenizer);
				// Запоминаем список суффиксов цифровых аббревиатур
				this->aspl->getValues("suffixTokenizer", suffixTokenizer);
				// Запоминаем данные эмбеддинга
				this->aspl->get("dataEmbedding", dataEmbedding, encrypted);
				// Запоминаем список токенов приводимых к <unk>
				this->aspl->getValues("tokensUnknownAlm", tokensUnknownAlm);
				// Запоминаем список запрещённых токенов
				this->aspl->getValues("tokensDisableAlm", tokensDisableAlm);
				// Устанавливаем дату генерации словаря
				if(date > 0) this->dict->setDate(date);
				// Если характеристики словаря получены, устанавливаем
				if((cw > 0) && (ad > 0)) this->dict->setAdCw(cw, ad);
				// Если апостроф разрешён, устанавливаем его
				if(allowApostrophe) this->alphabet->switchAllowApostrophe();
				// Если алфавит получен, устанавливаем его
				if(!alphabetDict.empty()){
					// Устанавливаем алфавит
					this->alphabet->set(alphabetDict);
					// Выполняем обновление словаря
					this->tokenizer->update();
				}
				// Если список пилотных слов получен, устанавливаем его
				if(!pilots.empty()) this->dict->setPilots(pilots);
				// Если список слов начинающихся с заглавной буквы получен
				if(!uWords.empty() && !uWordsCase.empty() && (uWords.size() == uWordsCase.size())){
					// Переходим по всему списку слов и добавляем их
					for(size_t i = 0; i < uWords.size(); i++){
						// Добавляем в список слова с их регистрами
						this->dict->addUWord(uWords.at(i), uWordsCase.at(i));
					}
				}
				// Если локаль получена, устанавливаем её
				if(!localeDict.empty()) this->alphabet->setlocale(localeDict);
				// Если код словаря получен, устанавливаем его
				if(!code.empty()) this->dict->setCode(this->alphabet->convert(code));
				// Если название словаря получено, устанавливаем его
				if(!name.empty()) this->dict->setName(this->alphabet->convert(name));
				// Если автор словаря получен, устанавливаем его
				if(!author.empty()) this->dict->setAuthor(this->alphabet->convert(author));
				// Если тип лицензии получен, устанавливаем его
				if(!lictype.empty()) this->dict->setLictype(this->alphabet->convert(lictype));
				// Если текст лицензии получен, устанавливаем его
				if(!lictext.empty()) this->dict->setLictext(this->alphabet->convert(lictext));
				// Если контактная информация автора передана, устанавливаем её
				if(!contacts.empty()) this->dict->setContacts(this->alphabet->convert(contacts));
				// Если копирайт автора получен, устанавливаем его
				if(!copyright.empty()) this->dict->setCopyright(this->alphabet->convert(copyright));
				// Если данные эмбеддинга получены
				if(!dataEmbedding.empty() && (sizeEmbedding > 0)){
					try {
						// Выполняем парсинг эмбеддинга
						const json & embedding = json::parse(dataEmbedding);
						// Устанавливаем данные эмбеддинга
						if(!embedding.empty()) this->dict->setEmbedding(embedding, sizeEmbedding);
					// Если возникает ошибка
					} catch(const exception & e) {
						// Выводим сообщение об ошибке
						this->alphabet->log("the JSON embedding [%s] is broken\r\n", alphabet_t::log_t::error, this->logfile, dataEmbedding.c_str());
						// Выходим из приложения
						exit(EXIT_FAILURE);
					}
				}
				// Если список альтернативных букв существует
				if(countAltLetters > 0){
					// Список альтернативных букв для записи
					vector <wchar_t> letters;
					// Извлекаем все альтернативные буквы
					for(size_t i = 0; i < countAltLetters; i++){
						// Очищаем список полученных букв
						letters.clear();
						// Добавляем буквы в словарь
						this->aspl->getValues(this->alphabet->format("altLetters_%zu", i), letters);
						// Если список альтернативных букв получен - устанавливаем
						if(letters.size() == 2) this->dict->addAlt(letters.front(), letters.back());
					}
				}
			}
			// Если словарь зашифрован а пароль не передан
			if(encrypted && this->password.empty()){
				// Выводим сообщение об ошибке
				this->alphabet->log("%s\r\n", alphabet_t::log_t::error, this->logfile, "password required for dictionary");
				// Выходим из приложения
				exit(EXIT_FAILURE);
			}
			// Если в бинарном словаре указана версия ALMv2 а установлена ALMv1, выводим сообщение и выходим
			if(isALm2 && !this->ALMv2){
				// Выводим сообщение об ошибке
				this->alphabet->log("%s\r\n", alphabet_t::log_t::error, this->logfile, "requires ALMv2 language model version");
				// Выходим из приложения
				exit(EXIT_FAILURE);
			// Если в бинарном словаре указана версия ALMv1 а установлена ALMv2, выводим сообщение и выходим
			} else if(!isALm2 && this->ALMv2){
				// Выводим сообщение об ошибке
				this->alphabet->log("%s\r\n", alphabet_t::log_t::error, this->logfile, "requires ALMv1 language model version");
				// Выходим из приложения
				exit(EXIT_FAILURE);
			}
			// Устанавливаем размер словаря
			if(sizeAlm > 0) this->alm->setSize(sizeAlm);
			// Если неизвестное слово существует, устанавливаем его
			if(!unknownAlm.empty()) this->alm->setUnknown(unknownAlm);
			// Если чёрный список существует, устанавливаем его
			if(!badwordsAlm.empty()) this->alm->setBadwords(badwordsAlm);
			// Если белый список существует, устанавливаем его
			if(!goodwordsAlm.empty()) this->alm->setGoodwords(goodwordsAlm);
			// Если список токенов для идентифицирования как <unk> существует, устанавливаем его
			if(!tokensUnknownAlm.empty()) this->alm->setTokensUnknown(tokensUnknownAlm);
			// Если список запрещённых токенов существует, устанавливаем его
			if(!tokensDisableAlm.empty()) this->alm->setTokensDisable(tokensDisableAlm);
			// Если опции языковой модели получены
			if(optionsAlm > 0){
				// Получаем опции языковой модели
				bitset <4> options = optionsAlm;
				// Если установлена опция - использования только слов из белого списка
				if(options.test(1)) this->alm->setOption(alm_t::options_t::onlyGood);
				// Если установлена опция - детектирования слова из смешанных словарей
				if(options.test(2)) this->alm->setOption(alm_t::options_t::mixdicts);
				// Если установлена опция - разрешающая загружать n-граммы из arpa так-как они есть
				if(options.test(3)) this->alm->setOption(alm_t::options_t::confidence);
			}
			// Если список пользовательских токенов получен
			if(!userTokensAlm.empty()){
				// Параметры индикаторы процесса
				size_t index = 0, status = 0, rate = 0;
				// Если отладка включена, выводим индикатор загрузки
				if((this->debug > 0) && (this->pss != nullptr)){
					// Очищаем предыдущий прогресс-бар
					this->pss->clear();
					// Устанавливаем заголовки прогресс-бара
					this->pss->title("Loading user token", "Loading user token is done");
					// Выводим индикатор прогресс-бара
					switch(this->debug){
						case 1: this->pss->update(); break;
						case 2: this->pss->status(); break;
					}
				}
				// Переходим по всему списку пользовательских токенов
				for(auto & token : userTokensAlm){
					// Устанавливаем пользовательский токен
					this->alm->setUserToken(token);
					// Если отладка включена
					if(this->debug > 0){
						// Увеличиваем индекс добавленного токена
						index++;
						// Подсчитываем статус выполнения
						status = u_short(index / double(userTokensAlm.size()) * 100.0);
						// Если процентное соотношение изменилось
						if(rate != status){
							// Запоминаем текущее процентное соотношение
							rate = status;
							// Если прогресс-бар передан
							if(this->pss != nullptr){
								// Отображаем ход процесса
								switch(this->debug){
									case 1: this->pss->update(status); break;
									case 2: this->pss->status(status); break;
								}
							// Выводим индикацию во внешнюю функцию
							} else this->progressFn(L"Loading user token", status);
						}
					}
				}
				// Если объект индикатора загрузки передан
				if(this->pss != nullptr){
					// Отображаем ход процесса
					switch(this->debug){
						case 1: this->pss->update(100); break;
						case 2: this->pss->status(100); break;
					}
				// Выводим индикацию во внешнюю функцию
				} else this->progressFn(L"Loading user token", 100);
			}{
				// Буфер бинарных данных словаря
				vector <char> buffer;
				// Выполняем загрузку данных обучения
				this->aspl->get("dictionary", buffer, encrypted);
				// Если буфер получен
				if(!buffer.empty()){
					// Если отладка включена, выводим индикатор загрузки
					if((this->debug > 0) && (this->pss != nullptr)){
						// Очищаем предыдущий прогресс-бар
						this->pss->clear();
						// Устанавливаем заголовки прогресс-бара
						this->pss->title("Loading dictionary", "Loading dictionary is done");
						// Выводим индикатор прогресс-бара
						switch(this->debug){
							case 1: this->pss->update(); break;
							case 2: this->pss->status(); break;
						}
					}
					// Выполняем установку данных обучения
					this->dict->load(buffer, true, [this](const u_short status) noexcept {
						// Если объект индикатора загрузки передан
						if(this->pss != nullptr){
							// Отображаем ход процесса
							switch(this->debug){
								case 1: this->pss->update(status); break;
								case 2: this->pss->status(status); break;
							}
						// Выводим индикацию во внешнюю функцию
						} else this->progressFn(L"Loading dictionary", status);
					});
					// Если объект индикатора загрузки передан
					if(this->pss != nullptr){
						// Отображаем ход процесса
						switch(this->debug){
							case 1: this->pss->update(100); break;
							case 2: this->pss->status(100); break;
						}
					// Выводим индикацию во внешнюю функцию
					} else this->progressFn(L"Loading dictionary", 100);
					// Очищаем буфер данных
					buffer.clear();
					/*
					// Выполняем загрузку данных индекса
					aspl.get("dictindex", buffer, encrypted);
					// Если буфер получен, выполняем загрузку индекса
					if(!buffer.empty()) dict.loadIndex(buffer);
					*/
					// Очищаем буфер данных
					vector <char> ().swap(buffer);
					// Если отладка включена, выводим индикатор загрузки
					if((this->debug > 0) && (this->pss != nullptr)){
						// Очищаем предыдущий прогресс-бар
						this->pss->clear();
						// Устанавливаем заголовки прогресс-бара
						this->pss->title("Loading Bloom filter", "Loading Bloom filter is done");
						// Выводим индикатор прогресс-бара
						switch(this->debug){
							case 1: this->pss->update(); break;
							case 2: this->pss->status(); break;
						}
					}
					// Выполняем загрузку фильтра Блума
					this->dict->bloom([this](const u_short status) noexcept {
						// Если объект индикатора загрузки передан
						if(this->pss != nullptr){
							// Отображаем ход процесса
							switch(this->debug){
								case 1: this->pss->update(status); break;
								case 2: this->pss->status(status); break;
							}
						// Выводим индикацию во внешнюю функцию
						} else this->progressFn(L"Loading Bloom filter", status);
					});
					// Если объект индикатора загрузки передан
					if(this->pss != nullptr){
						// Отображаем ход процесса
						switch(this->debug){
							case 1: this->pss->update(100); break;
							case 2: this->pss->status(100); break;
						}
					// Выводим индикацию во внешнюю функцию
					} else this->progressFn(L"Loading Bloom filter", 100);
					// Если отладка включена, выводим индикатор загрузки
					if((this->debug > 0) && (this->pss != nullptr)){
						// Очищаем предыдущий прогресс-бар
						this->pss->clear();
						// Устанавливаем заголовки прогресс-бара
						this->pss->title("Loading stemming", "Loading stemming, is done");
						// Выводим индикатор прогресс-бара
						switch(this->debug){
							case 1: this->pss->update(); break;
							case 2: this->pss->status(); break;
						}
					}
					// Выполняем загрузку стемминга
					this->dict->stemming([this](const u_short status) noexcept {
						// Если объект индикатора загрузки передан
						if(this->pss != nullptr){
							// Отображаем ход процесса
							switch(this->debug){
								case 1: this->pss->update(status); break;
								case 2: this->pss->status(status); break;
							}
						// Выводим индикацию во внешнюю функцию
						} else this->progressFn(L"Loading stemming", status);
					});
					// Если объект индикатора загрузки передан
					if(this->pss != nullptr){
						// Отображаем ход процесса
						switch(this->debug){
							case 1: this->pss->update(100); break;
							case 2: this->pss->status(100); break;
						}
					// Выводим индикацию во внешнюю функцию
					} else this->progressFn(L"Loading stemming", 100);
				// Сообщаем, что загрузить данные словаря не получилось
				} else {
					// Выводим сообщение об ошибке
					this->alphabet->log("%s\r\n", alphabet_t::log_t::error, this->logfile, "wrong dictionary data");
					// Выходим из приложения
					exit(EXIT_FAILURE);
				}
			}
			// Если количество записей в языковой модели получено
			if(countAlm > 0){
				// Буфер бинарных данных языковой модели
				vector <char> buffer;
				// Параметры индикаторы процесса
				size_t status = 0, rate = 0;
				// Если отладка включена, выводим индикатор загрузки
				if((this->debug > 0) && (this->pss != nullptr)){
					// Очищаем предыдущий прогресс-бар
					this->pss->clear();
					// Устанавливаем заголовки прогресс-бара
					this->pss->title("Loading language model", "Loading language model is done");
					// Выводим индикатор прогресс-бара
					switch(this->debug){
						case 1: this->pss->update(); break;
						case 2: this->pss->status(); break;
					}
				}
				// Извлекаем все данные языковой модели
				for(size_t i = 0; i < countAlm; i++){
					// Очищаем список бираных данных буфера
					buffer.clear();
					// Извлекаем дамп словаря
					this->aspl->get(this->alphabet->format("alm_%zu", i), buffer, encrypted);
					// Если буфер данных существует
					if(!buffer.empty()) (isALm2 ? this->alm->setBin2(buffer) : this->alm->setBin(buffer));
					// Если отладка включена
					if(this->debug > 0){
						// Подсчитываем статус выполнения
						status = u_short(i / double(countAlm) * 100.0);
						// Если процентное соотношение изменилось
						if(rate != status){
							// Запоминаем текущее процентное соотношение
							rate = status;
							// Если объект индикатора загрузки передан
							if(this->pss != nullptr){
								// Отображаем ход процесса
								switch(this->debug){
									case 1: this->pss->update(status); break;
									case 2: this->pss->status(status); break;
								}
							// Выводим индикацию во внешнюю функцию
							} else this->progressFn(L"Loading language model", status);
						}
					}
				}
				// Если объект индикатора загрузки передан
				if(this->pss != nullptr){
					// Отображаем ход процесса
					switch(this->debug){
						case 1: this->pss->update(100); break;
						case 2: this->pss->status(100); break;
					}
				// Выводим индикацию во внешнюю функцию
				} else this->progressFn(L"Loading language model", 100);
			}
			// Если количество альтернативных слов получено
			if(countAltWords > 0){
				// Список альтернативных слов для записи
				vector <string> words;
				// Параметры индикаторы процесса
				size_t status = 0, rate = 0;
				// Если отладка включена, выводим индикатор загрузки
				if((this->debug > 0) && (this->pss != nullptr)){
					// Очищаем предыдущий прогресс-бар
					this->pss->clear();
					// Устанавливаем заголовки прогресс-бара
					this->pss->title("Loading alternative words", "Loading alternative words is done");
					// Выводим индикатор прогресс-бара
					switch(this->debug){
						case 1: this->pss->update(); break;
						case 2: this->pss->status(); break;
					}
				}
				// Извлекаем все альтернативные слова
				for(size_t i = 0; i < countAltWords; i++){
					// Очищаем список полученных слов
					words.clear();
					// Извлекаем альтернативные слова
					this->aspl->getStrings(this->alphabet->format("altWords_%zu", i), words);
					// Если список альтернативных слов получен
					if(words.size() == 2) this->dict->addAlt(this->alphabet->convert(words.front()), this->alphabet->convert(words.back()));
					// Если отладка включена
					if(this->debug > 0){
						// Подсчитываем статус выполнения
						status = u_short(i / double(countAltWords) * 100.0);
						// Если процентное соотношение изменилось
						if(rate != status){
							// Запоминаем текущее процентное соотношение
							rate = status;
							// Если объект индикатора загрузки передан
							if(this->pss != nullptr){
								// Отображаем ход процесса
								switch(this->debug){
									case 1: this->pss->update(status); break;
									case 2: this->pss->status(status); break;
								}
							// Выводим индикацию во внешнюю функцию
							} else this->progressFn(L"Loading alternative words", status);
						}
					}
				}
				// Если объект индикатора загрузки передан
				if(this->pss != nullptr){
					// Отображаем ход процесса
					switch(this->debug){
						case 1: this->pss->update(100); break;
						case 2: this->pss->status(100); break;
					}
				// Выводим индикацию во внешнюю функцию
				} else this->progressFn(L"Loading alternative words", 100);
			}
			// Если список букв для исправления слов из смешанных алфавитов получен
			if(countSubstitutes > 0){
				// Список слов для записи
				vector <string> words;
				// Параметры индикаторы процесса
				size_t status = 0, rate = 0;
				// Список букв из смешанных словарей
				map <string, string> letters;
				// Если отладка включена, выводим индикатор загрузки
				if((this->debug > 0) && (this->pss != nullptr)){
					// Очищаем предыдущий прогресс-бар
					this->pss->clear();
					// Устанавливаем заголовки прогресс-бара
					this->pss->title("Loading substitutes letters", "Loading substitutes letters is done");
					// Выводим индикатор прогресс-бара
					switch(this->debug){
						case 1: this->pss->update(); break;
						case 2: this->pss->status(); break;
					}
				}
				// Извлекаем все буквы из смешанных словарей
				for(size_t i = 0; i < countSubstitutes; i++){
					// Очищаем список букв из смешанных словарей
					words.clear();
					// Извлекаем слова из смешанных словарей
					this->aspl->getStrings(this->alphabet->format("substitutes_%zu", i), words);
					// Если слова получены, добавляем полученные буквы
					if(!words.empty()) letters.emplace(words.front(), words.back());
					// Если отладка включена
					if(this->debug > 0){
						// Подсчитываем статус выполнения
						status = u_short(i / double(countSubstitutes) * 100.0);
						// Если процентное соотношение изменилось
						if(rate != status){
							// Запоминаем текущее процентное соотношение
							rate = status;
							// Если объект индикатора загрузки передан
							if(this->pss != nullptr){
								// Отображаем ход процесса
								switch(this->debug){
									case 1: this->pss->update(status); break;
									case 2: this->pss->status(status); break;
								}
							// Выводим индикацию во внешнюю функцию
							} else this->progressFn(L"Loading substitutes letters", status);
						}
					}
				}
				// Если список буквы из смешанных словарей получен, устанавливаем его
				if(!letters.empty()) this->alphabet->setSubstitutes(letters);
				// Очищаем буфер данных
				map <string, string> ().swap(letters);
				// Если объект индикатора загрузки передан
				if(this->pss != nullptr){
					// Отображаем ход процесса
					switch(this->debug){
						case 1: this->pss->update(100); break;
						case 2: this->pss->status(100); break;
					}
				// Выводим индикацию во внешнюю функцию
				} else this->progressFn(L"Loading substitutes letters", 100);
			}
			// Если список доменных зон получен, устанавливаем его
			if(!domainZones.empty()) this->alphabet->setzones(domainZones);
			// Если список аббревиатур получен, устанавливаем его
			if(!abbrsTokenizer.empty()) this->tokenizer->setAbbrs(abbrsTokenizer);
			// Если список суффиксов цифровых аббревиатур получен, устанавливаем его
			if(!suffixTokenizer.empty()) this->tokenizer->setSuffixes(suffixTokenizer);
		// Если бинарный файл словаря, не может быть прочитан
		} else {
			// Выводим сообщение об ошибке
			this->alphabet->log("%s\r\n", alphabet_t::log_t::error, this->logfile, "dictionary file is wrong");
			// Выходим из приложения
			exit(EXIT_FAILURE);
		}
	}
}
/**
 * write Метод записи данных словаря в файл
 */
void anyks::ASCb::write() const noexcept {
	// Если основные объекты активны
	if((this->alm != nullptr) && (this->dict != nullptr) && (this->alphabet != nullptr) && (this->tokenizer != nullptr)){
		// Буфер данных обучения словаря
		vector <char> buffer;
		// Устанавливаем тип шифрования
		if(!this->password.empty()){
			// Устанавливаем размер шифрования
			this->aspl->setAES(this->aes);
			// Запоминаем размер шифрования
			this->aspl->set("aes", this->aes);
			// Сохраняем пароль в словарь, в зашифрованном виде
			this->aspl->set("password", this->password, true);
		}{
			// Запоминаем тип ALM
			this->aspl->set("almtype", this->ALMv2);
			// Запоминаем дату словаря
			this->aspl->set("date", this->dict->getDate());
			// Запоминаем, что словарь зашифрован
			this->aspl->set("encrypted", !this->password.empty());
			// Устанавливаем версию словаря
			this->aspl->set("version", string(ANYKS_ASC_DICT_VERSION));
			// Устанавливаем код словаря
			this->aspl->set("code", this->alphabet->convert(this->dict->getCode()));
			// Устанавливаем название словаря
			this->aspl->set("name", this->alphabet->convert(this->dict->getName()));
			// Устанавливаем автора словаря
			this->aspl->set("author", this->alphabet->convert(this->dict->getAuthor()));
			// Устанавливаем тип лицензии
			this->aspl->set("lictype", this->alphabet->convert(this->dict->getLictype()));
			// Устанавливаем текст лицензии
			this->aspl->set("lictext", this->alphabet->convert(this->dict->getLictext()));
			// Устанавливаем контактную информацию автора
			this->aspl->set("contacts", this->alphabet->convert(this->dict->getContacts()));
			// Устанавливаем копирайт автора
			this->aspl->set("copyright", this->alphabet->convert(this->dict->getCopyright()));
			// Получаем данные словаря
			const auto & adcw = this->dict->getAdCw();
			// Устанавливаем количество слов в документах
			this->aspl->set("cw", adcw.first);
			// Устанавливаем количество документов в словаре
			this->aspl->set("ad", adcw.second);
			// Получаем данные эмбеддинга
			const auto & embedding = this->dict->getEmbedding();
			// Устанавливаем размер эмбеддинга
			this->aspl->set("sizeEmbedding", embedding.second);
			// Устанавливаем данные эмбеддинга
			this->aspl->set("dataEmbedding", embedding.first.dump(), !this->password.empty());
		}{
			// Если отладка включена, выводим индикатор загрузки
			if((this->debug > 0) && (this->pss != nullptr)){
				// Очищаем предыдущий прогресс-бар
				this->pss->clear();
				// Устанавливаем заголовки прогресс-бара
				this->pss->title("Dump dictionary", "Dump dictionary is done");
				// Выводим индикатор прогресс-бара
				switch(this->debug){
					case 1: this->pss->update(); break;
					case 2: this->pss->status(); break;
				}
			}
			// Выполняем извлечение данных обучения в буфер
			this->dict->save(buffer, true, [this](const u_short status) noexcept {
				// Если объект индикатора загрузки передан
				if(this->pss != nullptr){
					// Отображаем ход процесса
					switch(this->debug){
						case 1: this->pss->update(status); break;
						case 2: this->pss->status(status); break;
					}
				// Выводим индикацию во внешнюю функцию
				} else this->progressFn(L"Dump dictionary", status);
			});
			// Если буфер данных получен
			if(!buffer.empty()){
				// Выполняем сохранение данных обучения
				this->aspl->set("dictionary", buffer, !this->password.empty());
				/*
				// Выполняем получение данных индекса
				this->dict->saveIndex(buffer);
				// Если буфер данных получен, выполняем сохранение данных индекса
				if(!buffer.empty()) this->aspl->set("dictindex", buffer, !this->password.empty());
				*/
				// Очищаем буфер данных
				buffer.clear();
				// Очищаем буфер данных
				vector <char> ().swap(buffer);
			}
			// Если объект индикатора загрузки передан
			if(this->pss != nullptr){
				// Отображаем ход процесса
				switch(this->debug){
					case 1: this->pss->update(100); break;
					case 2: this->pss->status(100); break;
				}
			// Выводим индикацию во внешнюю функцию
			} else this->progressFn(L"Dump dictionary", 100);
		}{
			// Параметры индикаторы процесса
			size_t index = 0, status = 0, rate = 0, count = 0;
			// Если список альтернативных букв получен
			if(!this->altLetters.empty()){
				// Список альтернативных букв для записи
				vector <wchar_t> letters(2);
				// Получаем количество альтернативных букв
				count = this->altLetters.size();
				// Устанавливаем количество альтернативных букв
				this->aspl->set("countAltLetters", count);
				// Если отладка включена, выводим индикатор загрузки
				if((this->debug > 0) && (this->pss != nullptr)){
					// Очищаем предыдущий прогресс-бар
					this->pss->clear();
					// Устанавливаем заголовки прогресс-бара
					this->pss->title("Dump alternative letters", "Dump alternative letters is done");
					// Выводим индикатор прогресс-бара
					switch(this->debug){
						case 1: this->pss->update(); break;
						case 2: this->pss->status(); break;
					}
				}
				// Переходим по всем альтернативным буквам
				for(auto & item : this->altLetters){
					// Получаем первое слово
					letters.at(0) = item.first;
					// Получаем значение слова
					letters.at(1) = item.second;
					// Добавляем буквы в словарь
					this->aspl->setValues(this->alphabet->format("altLetters_%zu", index), letters);
					// Увеличиваем индекс обработанных букв
					index++;
					// Если отладка включена
					if(this->debug > 0){
						// Подсчитываем статус выполнения
						status = u_short(index / double(count) * 100.0);
						// Если процентное соотношение изменилось
						if(rate != status){
							// Запоминаем текущее процентное соотношение
							rate = status;
							// Если объект индикатора загрузки передан
							if(this->pss != nullptr){
								// Отображаем ход процесса
								switch(this->debug){
									case 1: this->pss->update(status); break;
									case 2: this->pss->status(status); break;
								}
							// Выводим индикацию во внешнюю функцию
							} else this->progressFn(L"Dump alternative letters", status);
						}
					}
				}
				// Если объект индикатора загрузки передан
				if(this->pss != nullptr){
					// Отображаем ход процесса
					switch(this->debug){
						case 1: this->pss->update(100); break;
						case 2: this->pss->status(100); break;
					}
				// Выводим индикацию во внешнюю функцию
				} else this->progressFn(L"Dump alternative letters", 100);
			}
			// Если список альтернативных слов получен
			if(!this->altLetters.empty() && !this->altWords.empty()){
				// Список альтернативных слов для записи
				vector <string> words(2);
				// Получаем количество альтернативных слов
				count = this->altWords.size();
				// Устанавливаем количество альтернативных слов
				this->aspl->set("countAltWords", count);
				// Если отладка включена, выводим индикатор загрузки
				if((this->debug > 0) && (this->pss != nullptr)){
					// Очищаем предыдущий прогресс-бар
					this->pss->clear();
					// Устанавливаем заголовки прогресс-бара
					this->pss->title("Dump alternative words", "Dump alternative words is done");
					// Выводим индикатор прогресс-бара
					switch(this->debug){
						case 1: this->pss->update(); break;
						case 2: this->pss->status(); break;
					}
				}
				// Переходим по всем альтернативным словам
				for(auto & item : this->altWords){
					// Получаем первое слово
					words.at(0) = item.first;
					// Получаем значение слова
					words.at(1) = item.second;
					// Добавляем слова в словарь
					this->aspl->setStrings(this->alphabet->format("altWords_%zu", index), words);
					// Увеличиваем индекс обработанных слов
					index++;
					// Если отладка включена
					if(this->debug > 0){
						// Подсчитываем статус выполнения
						status = u_short(index / double(count) * 100.0);
						// Если процентное соотношение изменилось
						if(rate != status){
							// Запоминаем текущее процентное соотношение
							rate = status;
							// Если объект индикатора загрузки передан
							if(this->pss != nullptr){
								// Отображаем ход процесса
								switch(this->debug){
									case 1: this->pss->update(status); break;
									case 2: this->pss->status(status); break;
								}
							// Выводим индикацию во внешнюю функцию
							} else this->progressFn(L"Dump alternative words", status);
						}
					}
				}
				// Если объект индикатора загрузки передан
				if(this->pss != nullptr){
					// Отображаем ход процесса
					switch(this->debug){
						case 1: this->pss->update(100); break;
						case 2: this->pss->status(100); break;
					}
				// Выводим индикацию во внешнюю функцию
				} else this->progressFn(L"Dump alternative words", 100);
			}
		}{
			// Параметры индикаторы процесса и количество N-грамм
			size_t index = 0, countNgrams = 0;
			// Если отладка включена, выводим индикатор загрузки
			if((this->debug > 0) && (this->pss != nullptr)){
				// Очищаем предыдущий прогресс-бар
				this->pss->clear();
				// Устанавливаем заголовки прогресс-бара
				this->pss->title("Dump language model", "Dump language model is done");
				// Выводим индикатор прогресс-бара
				switch(this->debug){
					case 1: this->pss->update(); break;
					case 2: this->pss->status(); break;
				}
			}
			// Выполняем извлечение данных языковой модели
			this->alm->getBin([&](const vector <char> & buffer, const size_t count, const u_short status){
				// Запоминаем количество обработанных N-грамм
				countNgrams = count;
				// Добавляем дамп в словарь
				this->aspl->set(this->alphabet->format("alm_%zu", index), buffer, !this->password.empty());
				// Увеличиваем индекс обработанных слов
				index++;
				// Если объект индикатора загрузки передан
				if(this->pss != nullptr){
					// Отображаем ход процесса
					switch(this->debug){
						case 1: this->pss->update(status); break;
						case 2: this->pss->status(status); break;
					}
				// Выводим индикацию во внешнюю функцию
				} else this->progressFn(L"Dump language model", status);
			});
			// Записываем количество записанных буферов
			this->aspl->set("countAlm", index);
			// Записываем количество обработанных N-грамм
			this->aspl->set("countNgrams", countNgrams);
			// Если объект индикатора загрузки передан
			if(this->pss != nullptr){
				// Отображаем ход процесса
				switch(this->debug){
					case 1: this->pss->update(100); break;
					case 2: this->pss->status(100); break;
				}
			// Выводим индикацию во внешнюю функцию
			} else this->progressFn(L"Dump language model", 100);
		}{
			// Устанавливаем размер словаря
			this->aspl->set("sizeAlm", this->alm->getSize());
			// Устанавливаем опции языковой модели
			this->aspl->set("optionsAlm", this->alm->getOptions());
			// Устанавливаем неизвестное слово, если такое есть
			this->aspl->set("unknownAlm", this->alm->getUnknown(), !this->password.empty());
			// Устанавливаем список пилотных слов
			this->aspl->setValues("pilots", this->dict->getPilots());
			// Устанавливаем чёрный список слов, языковой модели
			this->aspl->setValues("badwordsAlm", this->alm->getBadwords());
			// Устанавливаем белый список слов, языковой модели
			this->aspl->setValues("goodwordsAlm", this->alm->getGoodwords());
			// Устанавливаем список пользовательских токенов
			this->aspl->setStrings("userTokensAlm", this->alm->getUserTokens());
			// Устанавливаем список токенов приводимых к <unk>
			this->aspl->setValues("tokensUnknownAlm", this->alm->getTokensUnknown());
			// Устанавливаем список запрещённых токенов
			this->aspl->setValues("tokensDisableAlm", this->alm->getTokensDisable());
			// Устанавливаем список аббревиатур
			this->aspl->setValues("abbrsTokenizer", this->tokenizer->getAbbrs());
			// Устанавливаем список суффиксов цифровых аббревиатур
			this->aspl->setValues("suffixTokenizer", this->tokenizer->getSuffixes());
			// Устанавливаем разрешение использования апострофа
			this->aspl->set("allowApostrophe", this->alphabet->isAllowApostrophe());
			// Устанавливаем алфавит словаря
			this->aspl->set("alphabet", this->alphabet->get(), !this->password.empty());
			// Если локаль передана, устанавливаем локаль
			if(!this->locale.empty()) this->aspl->set("locale", this->locale, !this->password.empty());
			// Если же локаль не передана, устанавливаем локаль по умолчанию
			else this->aspl->set("locale", string(ALPHABET_LOCALE), !this->password.empty());
		}{
			// Получаем список слов начинающиеся с заглавной буквы
			const auto & res = this->dict->getUWords();
			// Если список получен
			if(!res.empty()){
				// Индекс обработанных слов
				size_t index = 0;
				// Списки слов всегда начинающихся с заглавной буквы
				vector <size_t> uWords(res.size()), uWordsCase(res.size());
				// Формируем списки слов и их регистров
				for(auto & item : res){
					// Добавляем в список слов, ещё одно слово
					uWords.at(index) = item.first;
					// Добавляем в список регистров, ещё регистры
					uWordsCase.at(index) = item.second;
					// Увеличиваем значение индекса
					index++;
				}
				// Устанавливаем список слов, которые всегда начинаются с заглавной буквы
				this->aspl->setValues("uwords", uWords);
				// Устанавливаем список регистров слов, которые всегда начинаются с заглавной буквы
				this->aspl->setValues("ucase", uWordsCase);
			}
		}{
			// Получаем список доменных зон
			const auto & res = this->alphabet->getzones();
			// Если список получен
			if(!res.empty()){
				// Список доменных зон
				vector <string> zones;
				// Переходим по всему списку зон
				for(auto & zone : res) zones.push_back(this->alphabet->convert(zone));
				// Если список зон сконвертирован
				if(!zones.empty()) this->aspl->setStrings("zonesAlphabet", zones);
			}
		}{
			// Получаем список букв для исправления слов из смешанных алфавитов
			const auto & res = this->alphabet->getSubstitutes();
			// Параметры индикаторы процесса
			size_t index = 0, status = 0, rate = 0, count = 0;
			// Если список получен
			if(!res.empty()){
				// Список слов для записи
				vector <string> words(2);
				// Получаем количество слов
				count = res.size();
				// Устанавливаем количество слов
				this->aspl->set("countSubstitutes", count);
				// Если отладка включена, выводим индикатор загрузки
				if((this->debug > 0) && (this->pss != nullptr)){
					// Очищаем предыдущий прогресс-бар
					this->pss->clear();
					// Устанавливаем заголовки прогресс-бара
					this->pss->title("Dump substitutes letters", "Dump substitutes letters is done");
					// Выводим индикатор прогресс-бара
					switch(this->debug){
						case 1: this->pss->update(); break;
						case 2: this->pss->status(); break;
					}
				}
				// Переходим по всем словам
				for(auto & item : res){
					// Получаем первое слово
					words.at(0) = item.first;
					// Получаем значение слова
					words.at(1) = item.second;
					// Добавляем слова в словарь
					this->aspl->setStrings(this->alphabet->format("substitutes_%zu", index), words);
					// Увеличиваем индекс обработанных слов
					index++;
					// Если отладка включена
					if(this->debug > 0){
						// Подсчитываем статус выполнения
						status = u_short(index / double(count) * 100.0);
						// Если процентное соотношение изменилось
						if(rate != status){
							// Запоминаем текущее процентное соотношение
							rate = status;
							// Если объект индикатора загрузки передан
							if(this->pss != nullptr){
								// Отображаем ход процесса
								switch(this->debug){
									case 1: this->pss->update(status); break;
									case 2: this->pss->status(status); break;
								}
							// Выводим индикацию во внешнюю функцию
							} else this->progressFn(L"Dump substitutes letters", status);
						}
					}
				}
				// Если объект индикатора загрузки передан
				if(this->pss != nullptr){
					// Отображаем ход процесса
					switch(this->debug){
						case 1: this->pss->update(100); break;
						case 2: this->pss->status(100); break;
					}
				// Выводим индикацию во внешнюю функцию
				} else this->progressFn(L"Dump substitutes letters", 100);
			}
		}
		// Выполняем запись данных словаря
		if(this->aspl->write() < 1){
			// Выводим сообщение об ошибке
			this->alphabet->log("%s\r\n", alphabet_t::log_t::error, this->logfile, "dictionary file is wrong");
			// Выходим из приложения
			exit(EXIT_FAILURE);
		}
	}
}
/**
 * Метод инициализации
 */
void anyks::ASCb::init(){
	// Если объект бинарного контейнера еще не инициализирован
	if(this->aspl == nullptr){
		// Экранируем возможность ошибки памяти
		try {
			// Выполняем инициализацию объекта бинарного контейнера
			this->aspl = new aspl_t(this->filename, this->password);
		// Если происходит ошибка то игнорируем её
		} catch(const bad_alloc &) {
			// Выходим из приложения
			exit(EXIT_FAILURE);
		}
	}
}
/**
 * clear Метод очистки данных словаря
 */
void anyks::ASCb::clear() noexcept {
	// Очищаем режим отладки
	this->debug = 0;
	// Очищаем имя файла бинарного контейнера
	this->filename = "";
	// Очищаем пароль шифрования словаря
	this->password = "";
	// Очищаем флаг работы ALMv2
	this->ALMv2 = false;
	// Очищаем список альтернативных слов
	this->altWords.clear();
	// Очищаем список альтернативных букв
	this->altLetters.clear();
	// Очищаем системную локаль приложения
	this->locale = ALPHABET_LOCALE;
	// Очищаем тип шифрования словаря
	this->aes = aspl_t::types_t::aes128;
	// Удаляем выделенную память для бинарного контейнера
	if(this->aspl != nullptr){
		// Очищаем выделенную память
		delete this->aspl;
		// Зануляем объект
		this->aspl = nullptr;
	}
}
/**
 * info Метод вывода инормационных данных словаря
 */
void anyks::ASCb::info() const noexcept {
	// Выполняем чтение данных словаря
	if(this->aspl->read() > 0){
		// Список пилотных слов
		set <wchar_t> pilots;
		// Список доменных зон
		set <string> domainZones;
		// Список пользовательских токенов и доменных зон
		vector <string> userTokensAlm, alts;
		// Тип шифрования и размер языковой модели
		u_short aes = 0, sizeAlm = 0, optionsAlm = 0;
		// Словарь является зашифрованным и флаг апострофа в словаре
		bool encrypted = false, allowApostrophe = false, isALm2 = false;
		// Версия словаря, неизвестное слово, алфавит и локаль словаря
		string version = "", unknownAlm = "", alphabetDict = "", localeDict = "";
		// Черный и белый списки слов словаря а также аббревиатур
		set <size_t> badwordsAlm, goodwordsAlm, abbrsTokenizer, suffixTokenizer, uWords;
		// Количество данных в словаре
		size_t countAltLetters = 0, countAltWords = 0, countNgrams = 0, countSubstitutes = 0;
		// Основные метаданные контейнера
		string code = "", name = "", author = "", lictype = "", lictext = "", contacts = "", copyright = "";
		// Если нужно получить размер шифрования
		if(!this->password.empty()) this->aspl->get("aes", aes);
		{
			// Дата генерации словаря
			time_t date = 0;
			// Количество слов во всех документах и количество всех документов в словаре
			size_t ad = 0, cw = 0;
			// Размер эмбеддинга
			size_t sizeEmbedding = 0;
			// Запоминаем количество слов в документах
			this->aspl->get("cw", cw);
			// Запоминаем количество документов в словаре
			this->aspl->get("ad", ad);
			// Запоминаем дату словаря
			this->aspl->get("date", date);
			// Запоминаем код словаря
			this->aspl->get("code", code);
			// Запоминаем название словаря
			this->aspl->get("name", name);
			// Запоминаем автора словаря
			this->aspl->get("author", author);
			// Запоминаем тип ALM
			this->aspl->get("almtype", isALm2);
			// Запоминаем размер словаря
			this->aspl->get("sizeAlm", sizeAlm);
			// Запоминаем версию словаря
			this->aspl->get("version", version);
			// Запоминаем тип лицензии
			this->aspl->get("lictype", lictype);
			// Запоминаем текст лицензии
			this->aspl->get("lictext", lictext);
			// Запоминаем контактную информацию автора
			this->aspl->get("contacts", contacts);
			// Запоминаем список пилотных слов
			this->aspl->getValues("pilots", pilots);
			// Запоминаем список слов, которые всегда начинаются с заглавной буквы
			this->aspl->getValues("uwords", uWords);
			// Запоминаем копирайт автора
			this->aspl->get("copyright", copyright);
			// Запоминаем, что словарь зашифрован
			this->aspl->get("encrypted", encrypted);
			// Запоминаем опции языковой модели
			this->aspl->get("optionsAlm", optionsAlm);
			// Запоминаем количество обработанных N-грамм
			this->aspl->get("countNgrams", countNgrams);
			// Запоминаем размер эмбеддинга
			this->aspl->get("sizeEmbedding", sizeEmbedding);
			// Запоминаем количество альтернативных слов
			this->aspl->get("countAltWords", countAltWords);
			// Запоминаем локаль словаря
			this->aspl->get("locale", localeDict, encrypted);
			// Запоминаем чёрный список слов, языковой модели
			this->aspl->getValues("badwordsAlm", badwordsAlm);
			// Запоминаем белый список слов, языковой модели
			this->aspl->getValues("goodwordsAlm", goodwordsAlm);
			// Запоминаем разрешение использования апострофа
			this->aspl->get("allowApostrophe", allowApostrophe);
			// Запоминаем количество альтернативных букв
			this->aspl->get("countAltLetters", countAltLetters);
			// Запоминаем список доменных зон
			this->aspl->getStrings("zonesAlphabet", domainZones);
			// Запоминаем неизвестное слово, если такое есть
			this->aspl->get("unknownAlm", unknownAlm, encrypted);
			// Запоминаем алфавит словаря
			this->aspl->get("alphabet", alphabetDict, encrypted);
			// Запоминаем количество слов из смешанных алфавитов
			this->aspl->get("countSubstitutes", countSubstitutes);
			// Запоминаем список пользовательских токенов
			this->aspl->getStrings("userTokensAlm", userTokensAlm);
			// Запоминаем список аббревиатур
			this->aspl->getValues("abbrsTokenizer", abbrsTokenizer);
			// Запоминаем список суффиксов цифровых аббревиатур
			this->aspl->getValues("suffixTokenizer", suffixTokenizer);
			// Если алфавит получен, устанавливаем его
			if(!encrypted && !alphabetDict.empty()) this->alphabet->set(alphabetDict);
			// Если локаль получена, устанавливаем её
			if(!encrypted && !localeDict.empty()) this->alphabet->setlocale(localeDict);
			// Если список альтернативных букв существует
			if(countAltLetters > 0){
				// Список альтернативных букв для записи
				vector <wchar_t> letters;
				// Извлекаем все альтернативные буквы
				for(size_t i = 0; i < countAltLetters; i++){
					// Очищаем список полученных букв
					letters.clear();
					// Добавляем буквы в словарь
					this->aspl->getValues(this->alphabet->format("altLetters_%zu", i), letters);
					// Если список альтернативных букв получен
					if(letters.size() == 2){
						// Добавляем строку в список альтернативных букв
						alts.push_back(this->alphabet->format("%ls => %ls", wstring(1, letters.front()).c_str(), wstring(1, letters.back()).c_str()));
					}
				}
			}
			// Формируем информационную строку
			string info = "\033c\r\n\r\n* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\r\n\r\n";
			// Если код словаря переда, устанавливаем
			if(!code.empty()) info.append(this->alphabet->format("* Code: %s\r\n\r\n", this->alphabet->toUpper(code).c_str()));
			// Если версия словаря получена, устанавливаем
			if(!version.empty()) info.append(this->alphabet->format("* Version: %s\r\n\r\n", version.c_str()));
			// Если название словаря передано, устанавливаем
			if(!name.empty()) info.append(this->alphabet->format("* Dictionary name: %s\r\n\r\n", name.c_str()));
			// Если локаль передана, устанавливаем
			if(!encrypted && !localeDict.empty()) info.append(this->alphabet->format("* Locale: %s\r\n", localeDict.c_str()));
			// Если алфавит передан, устанавливаем
			if(!encrypted && !alphabetDict.empty()) info.append(this->alphabet->format("* Alphabet: %s\r\n\r\n", alphabetDict.c_str()));
			// Если локаль передана, устанавливаем разделитель
			else if(!localeDict.empty()) info.append("\r\n");
			// Если дата генерации словаря получена
			if(date > 0){
				// Создаем буфер для хранения даты
				char buffer[80];
				// Заполняем его нулями
				memset(buffer, 0, sizeof(buffer));
				// Получаем структуру локального времени
				struct tm * timeinfo = localtime(&date);
				// Копируем в буфер полученную дату и время
				const int length = strftime(buffer, sizeof(buffer), "%m/%d/%Y %H:%M:%S", timeinfo);
				// Если дата создана
				if(length > 0){
					// Создаем строку с датой
					const string zdate(buffer, length);
					// Устанавливаем дату генерации словаря
					info.append(this->alphabet->format("* Build date: %s\r\n\r\n", zdate.c_str()));
				}
			}
			// Если размер шифрования передан, устанавливаем
			if(aes > 0) info.append(this->alphabet->format("* AES: %zu\r\n", aes));
			// Если словарь зашифрован
			if(encrypted) info.append("* Encrypted: YES\r\n\r\n");
			// Если словарь не зашифрован
			else info.append("* Encrypted: NO\r\n\r\n");
			// Выводим тип языковой модели
			if(isALm2) info.append("* ALM type: ALMv2\r\n\r\n");
			// Выводим тип языковой модели, используемый в словаре
			else info.append("* ALM type: ALMv1\r\n\r\n");
			// Если апостроф разрешён, устанавливаем его
			if(allowApostrophe) info.append("* Allow apostrophe: YES\r\n\r\n");
			// Если апостроф не разрешён
			else info.append("* Allow apostrophe: NO\r\n\r\n");
			// Если неизвестное слово есть, устанавливаем
			if(!encrypted && !unknownAlm.empty()) info.append(this->alphabet->format("* Unknown word: %s\r\n\r\n", unknownAlm.c_str()));
			// Если количество слов во всех документах получены, устанавливаем
			if(cw > 0) info.append(this->alphabet->format("* Count words: %zu\r\n", cw));
			// Если количество документов в словаре получено, устанавливаем
			if(ad > 0) info.append(this->alphabet->format("* Count documents: %zu\r\n\r\n", ad));
			// Если количество слов больше 0, устанавливаем разделитель
			else if(cw > 0) info.append("\r\n");
			// Если опции языковой модели получены, устанавливаем
			if(optionsAlm > 0){
				// Получаем опции языковой модели
				bitset <4> options = optionsAlm;
				// Если установлена опция - использования только слов из белого списка
				if(options.test(1)) info.append("* Only good words: YES\r\n");
				// Если опция не установлена
				else info.append("* Only good words: NO\r\n");
				// Если установлена опция - детектирования слова из смешанных словарей
				if(options.test(2)) info.append("* Mix words in dicts: YES\r\n");
				// Если опция не установлена
				else info.append("* Mix words in dicts: NO\r\n");
				// Если установлена опция - разрешающая загружать n-граммы из arpa так-как они есть
				if(options.test(3)) info.append("* Confidence arpa: YES\r\n\r\n");
				// Если опция не установлена
				else info.append("* Confidence arpa: NO\r\n\r\n");
			}
			// Если список слов, начинающихся с заглавной буквы получен, устанавливаем количество слов
			if(!uWords.empty()) info.append(this->alphabet->format("* Count upper words: %zu\r\n", uWords.size()));
			// Если список пилотных слов получен, устанавливаем количество пилотных слов
			if(!pilots.empty()) info.append(this->alphabet->format("* Count pilots words: %zu\r\n", pilots.size()));
			// Если чёрный список существует, устанавливаем количество слов в чёрном списке
			if(!badwordsAlm.empty()) info.append(this->alphabet->format("* Count bad words: %zu\r\n", badwordsAlm.size()));
			// Если белый список существует, устанавливаем количество слов в белом списке
			if(!goodwordsAlm.empty()) info.append(this->alphabet->format("* Count good words: %zu\r\n", goodwordsAlm.size()));
			// Если количество букв из смешанных алфавитов существуют, устанавливаем
			if(countSubstitutes > 0) info.append(this->alphabet->format("* Count substitutes: %zu\r\n", countSubstitutes));
			// Если список аббревиатур существует, устанавливаем количество аббревиатур
			if(!abbrsTokenizer.empty() || !suffixTokenizer.empty()) info.append(this->alphabet->format("* Count abbreviations: %zu\r\n\r\n", abbrsTokenizer.size() + suffixTokenizer.size()));
			// Если список аббревиатур не получен, но существуют черный или белый списки, устанавливаем разделитель
			else if(!badwordsAlm.empty() || !goodwordsAlm.empty() || (countSubstitutes > 0)) info.append("\r\n");
			// Если список доменных зон существует, устанавливаем количество доменных зон
			if(!domainZones.empty()) info.append(this->alphabet->format("* Count domain zones: %zu\r\n\r\n", domainZones.size()));
			// Если список пользовательских токенов существует
			if(!userTokensAlm.empty()){
				// Список токенов
				string tokens = "";
				// Переходим по всему списку пользовательских токенов
				for(auto & token : userTokensAlm){
					// Если список токенов уже не пустой, добавляем разделитель
					if(!tokens.empty()) tokens.append(" | ");
					// Добавляем токен в список токенов
					tokens.append(this->alphabet->format("<%s>", token.c_str()));
				}
				// Если список токенов получен, устанавливаем его
				if(!tokens.empty()) info.append(this->alphabet->format("* User tokens: %s\r\n", tokens.c_str()));
			}
			// Если альтернативные буквы получены
			if(!alts.empty()){
				// Строка с альтернативными буквами
				string str = "";
				// Переходим по всему списку альтернативных букв
				for(auto & alt : alts){
					// Если список альтернативных букв уже не пустой, добавляем разделитель
					if(!str.empty()) str.append(" | ");
					// Добавляем альтернативные буквы
					str.append(alt);
				}
				// Если список альтернативных букв получен
				if(!str.empty()) info.append(this->alphabet->format("* Alternatives: %s\r\n", str.c_str()));
			}
			// Если список альтернативных слов существует, добавляем количество альтернативных слов
			if(countAltWords > 0) info.append(this->alphabet->format("* Count alternatives words: %zu\r\n\r\n", countAltWords));
			// Если список альтернативных букв существует, добавляем разделитель
			else if(!alts.empty()) info.append("\r\n");
			// Если размер эмбеддинга получен, устанавливаем его
			if(sizeEmbedding > 0) info.append(this->alphabet->format("* Size embedding: %zu\r\n\r\n", sizeEmbedding));
			// Если размер N-Граммы языковой модели передан, устанавливаем его
			if(sizeAlm > 0) info.append(this->alphabet->format("* Length n-gram: %hu\r\n", sizeAlm));
			// Если количество N-грамм в языковой модели получено, устанавливаем
			if(countNgrams > 0) info.append(this->alphabet->format("* Count n-grams: %zu\r\n\r\n", countNgrams));
			// Если размер N-Граммы языковой модели передан, добавляем разделитель
			else if(sizeAlm > 0) info.append("\r\n");
			// Если автор словаря существует, устанавливаем его
			if(!author.empty()) info.append(this->alphabet->format("* Author: %s\r\n\r\n", author.c_str()));
			// Если контакты автора словаря указаны, устанавливаем их
			if(!contacts.empty()) info.append(this->alphabet->format("* Contacts: %s\r\n\r\n", contacts.c_str()));
			// Если копирайт автора словаря указан, устанавливаем его
			if(!copyright.empty()) info.append(this->alphabet->format("* Copyright ©: %s\r\n\r\n", copyright.c_str()));
			// Если тип лицензии указан, устанавливаем его
			if(!lictype.empty()) info.append(this->alphabet->format("* License type: %s\r\n", lictype.c_str()));
			// Если текст лицензии существует, устанавливаем его
			if(!lictext.empty()) info.append(this->alphabet->format("* License text:\r\n%s\r\n", lictext.c_str()));
			// Добавляем завершающий разделитель
			info.append("\r\n* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\r\n\r\n");
			// Выводим получившийся результат
			if(!info.empty()) this->alphabet->log("%s", alphabet_t::log_t::null, this->logfile, info.c_str());
		}
	}
}
/**
 * addAlt Метод добавления альтернативной буквы
 * @param letter буква для добавления
 * @param alt    альтернативная буква для добавления
 */
void anyks::ASCb::addAlt(const wchar_t letter, const wchar_t alt) noexcept {
	// Если буквы переданы, устанавливаем их
	if((letter > 0) && (alt > 0)) this->altLetters.emplace(letter, alt);
}
/**
 * addAlt Метод добавления альтернативного слова
 * @param word слово для добавления
 * @param alt  альтернативное слово для добавления
 */
void anyks::ASCb::addAlt(const string & word, const string & alt) noexcept {
	// Если слово и альтернативное слово переданы
	if(!word.empty() && !alt.empty()) this->altWords.emplace(word, alt);
}
/**
 * setAlm Метод установки объекта языковой модели
 * @param alm объект языковой модели для установки
 */
void anyks::ASCb::setAlm(alm_t * alm) noexcept {
	// Если объект языковой модели передан, устанавливаем его
	if(alm != nullptr) this->alm = alm;
}
/**
 * setDict Метод установки объекта словаря
 * @param dict объект словаря для установки
 */
void anyks::ASCb::setDict(dict_t * dict) noexcept {
	// Если объект словаря передан, устанавливаем его
	if(dict != nullptr) this->dict = dict;
}
/**
 * setAES Метод установки размера шифрования
 * @param aes размер шифрования для установки
 */
void anyks::ASCb::setAES(aspl_t::types_t aes) noexcept {
	// Устанавливаем размер шифрования
	this->aes = aes;
}
/**
 * setALMvType Метод установки типа ALM
 * @param type тип версии ALM
 */
void anyks::ASCb::setALMvType(const bool type) noexcept {
	// Устанавливаем тип языковой модели
	this->ALMv2 = type;
}
/**
 * setProgress Метод установки объекта прогресс-бара
 * @param pss объект прогресс-бара
 */
void anyks::ASCb::setProgress(progress_t * pss) noexcept {
	// Если объект прогресс-бара передан, устанавливаем его
	if(pss != nullptr) this->pss = pss;
}
/**
 * setDebug Метод установки флага отладки
 * @param debug устанавливаемый флаг отладки
 */
void anyks::ASCb::setDebug(const u_short debug) noexcept {
	// Устанавливаем режим отладки
	this->debug = debug;
}
/**
 * setLocale Метод установки локали приложения
 * @param locale локаль приложения для установки
 */
void anyks::ASCb::setLocale(const string & locale) noexcept {
	// Если локаль передана, устанавливаем её
	if(!locale.empty()) this->locale = locale;
}
/**
 * setLogfile Метод установки файла для вывода логов
 * @param logifle адрес файла для вывода отладочной информации
 */
void anyks::ASCb::setLogfile(const char * logfile) noexcept {
	// Если адреса файла для вывода отладочной информации передан
	if(logfile != nullptr) this->logfile = logfile;
}
/**
 * setAlphabet Метод установки объекта словаря
 * @param alphabet объект словаря для установки
 */
void anyks::ASCb::setAlphabet(alphabet_t * alphabet) noexcept {
	// Если объект алфавита передан, устанавливаем его
	if(alphabet != nullptr) this->alphabet = alphabet;
}
/**
 * setPassword Метод установки пароля словаря
 * @param password пароль словаря для установки
 */
void anyks::ASCb::setPassword(const string & password) noexcept {
	// Если пароль шифрования передан, устанавливаем его
	if(!password.empty()) this->password = password;
}
/**
 * setTokenizer Метод установки токенизатора
 * @param tokenizer объект токенизатора
 */
void anyks::ASCb::setTokenizer(tokenizer_t * tokenizer) noexcept {
	// Если объект токенизатора передан, устанавливаем его
	if(tokenizer != nullptr) this->tokenizer = tokenizer;
}
/**
 * setFilename Метод установки адреса файла словаря
 * @param filename адрес файла словаря
 */
void anyks::ASCb::setFilename(const string & filename) noexcept {
	// Если адрес файла передан, устанавливаем его
	if(!filename.empty()) this->filename = filename;
}
/**
 * setProgressFn Метод установки внешнего прогресс-бара
 * @param fn функция внешнего прогресс-бара
 */
void anyks::ASCb::setProgressFn(function <void (const wstring &, const u_short)> fn) noexcept {
	// Зануляем основной пргресс-бар
	this->pss = nullptr;
	// Устанавливаем функцию внешнего прогресс-бара
	this->progressFn = fn;
}
/**
 * ASCb Конструктор
 */
anyks::ASCb::ASCb() noexcept {
	// Устанавливаем внешнюю болванку прогресс-бара
	this->progressFn = [](const wstring & text, const u_short status) noexcept {
		// Блокируем варнинги на неиспользуемые переменные
		(void) text;
		(void) status;
	};
}
/**
 * ASCb Конструктор
 * @param filename адрес файла словаря
 * @param password пароль бинарного словаря
 * @param logifle  адрес файла для вывода отладочной информации
 */
anyks::ASCb::ASCb(const string & filename, const string & password, const char * logfile) noexcept {
	// Устанавливаем адрес файла для вывода отладочной информации
	this->setLogfile(logfile);
	// Устанавливаем имя файла словаря
	this->setFilename(filename);
	// Устанавливаем пароль шифрования словаря
	this->setPassword(password);
	// Устанавливаем внешнюю болванку прогресс-бара
	this->progressFn = [](const wstring & text, const u_short status) noexcept {
		// Блокируем варнинги на неиспользуемые переменные
		(void) text;
		(void) status;
	};
}
/**
 * ~ASCb Деструктор
 */
anyks::ASCb::~ASCb() noexcept {
	// Выполняем очистику выделенных решений
	this->clear();
}
