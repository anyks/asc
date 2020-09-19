/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <dumper.hpp>

/**
 * isOption Метод проверки наличия опции
 * @param option опция для проверки
 * @return       результат проверки
 */
const bool anyks::Dumper::isOption(const options_t option) const noexcept {
	// Выполняем проверку наличия опции
	const bool result = this->options.test((u_short) option);
	// Если флаг дал положительный результат и это режим отладки
	if(result && (option == options_t::debug)) this->logfile = nullptr;
	// Выводим результат
	return result;
}
/**
 * clear Метод очистки собранных данных
 */
void anyks::Dumper::clear() noexcept {
	// Очищаем список связей
	this->links.clear();
	// Очищаем список промаркированных вариантов
	this->marks.clear();
	// Очищаем список собранных вариантов
	this->variants.clear();
	// Очищаем цепочки последовательностей
	this->hypseseq.clear();
	// Очищаем список последовательности
	this->sequence.clear();
	// Очищаем список лучших кандидатов
	this->candidates.clear();
	// Очищаем список гипотез
	this->hypotheses.clear();
}
/**
 * empty Метод проверки, является ли пустым блок с данными
 * @return результат проверки
 */
const bool anyks::Dumper::empty() const noexcept {
	// Выводим результат
	return (this->sequence.empty() && this->hypotheses.empty() && this->links.empty());
}
/**
 * best Метод получения лучшего результата
 * @return наилучший вариант по мнению системы
 */
const pair <size_t, const anyks::Dumper::awrd_t *> anyks::Dumper::best() const noexcept {
	// Результат работы функции
	pair <size_t, const awrd_t *> result = {0, nullptr};
	// Если данные собрены
	if(!this->empty() && (this->alm != nullptr)){
		// Получаем список гипотез
		const auto & hypotheses = this->get();
		// Если гипотезы получены
		if(!hypotheses.empty()){
			/* 
			 * EXT FREQ LEV TMO WLTF
			 *  0   0    0   0   0
			 */
			typedef struct Variant {
				// Буфер формирования рейтинга
				bitset <5> buffer;
				// Дистанция Левенштейна
				u_short lev;
				// Значение Танимото, общий рейтинг всех слов и значение частоты последовательности
				double tmo, wltf, logprob;
				/**
				 * Variant Конструктор
				 */
				Variant() : lev(MAX_LENGTH_TEXT), tmo(0.0), wltf(MIN_WORD_WEIGHT), logprob(MIN_WORD_WEIGHT) {}
			} var_t;
			// Очищаем список лучших кандидатов
			this->candidates.clear();
			// Варианты для обработки
			var_t var, var1, var2;
			// Лучший кандидат и текущий кандидат
			string bestCandidate = "";
			// Последовательность для проверки
			vector <size_t> * seq = nullptr;
			// Значение лучшей частоты последовательности и Танимото
			double logprob = MIN_WORD_WEIGHT;
			// Флаги добавления последовательностей
			bool start = false, finish = false;
			// Флаг существования последовательности
			bool exist = false, onewrd = false;
			// Количество лучших биграмм и триграмм
			size_t bigram = 0, trigram = 0, count = 0;
			// Получаем граммность языковой модели
			const u_short size = this->alm->getSize();
			// Флаги стадий проверки характеристик
			bool mode1 = false, mode2 = false, distmo = false;
			// Разница - дистанции в полученных характеристиках
			double distLev = 0.0, distTmo = 0.0, distWid = 0.0;
			/**
			 * resFn Функция формирования результата
			 * @param ids идентификатор последовательности
			 * @param hyp данные лучшей гипотезы
			 * @param seq последовательность текущего варианта
			 */
			auto resFn = [&result, &bestCandidate, this](const size_t ids, const awrd_t * hyp, const vector <size_t> & seq) noexcept {
				// Запоминаем текущее значение идентификатора
				result.first = ids;
				// Запоминаем текущее значение варианта
				result.second = hyp;
				// Добавляем лучший вариант
				this->candidates.emplace(ids, hyp);
				// Если вывод отладочной информации разрешён, запоминаем кандидата
				if(this->isOption(options_t::debug) && !seq.empty()){
					// Очищаем лучшего кандидата
					bestCandidate.clear();
					// Переходим по всему списку последовательности
					for(auto & idw : seq){
						// Добавляем разделитель
						if(!bestCandidate.empty()) bestCandidate.append(1, ' ');
						// Добавляем слово в текст
						bestCandidate.append(this->alm->word(idw).real());
					}
				}
			};
			/**
			 * printFn Функция вывода отладочной информации о текущем кандидате
			 * @param wltf    рейтинг текущего варианта
			 * @param logprob частота текущего варианта
			 * @param tmo     значение Танимото текущего варианта
			 * @param lev     дистанция Левенштейна текущего варианта
			 * @param seq     последовательность текущего варианта
			 */
			auto printFn = [&exist, this](const double wltf, const double logprob, const double tmo, const u_short lev, const vector <size_t> & seq) noexcept {
				// Если вывод отладочной информации разрешён
				if(this->isOption(options_t::debug) && !seq.empty()){
					// Текстовая строка кандидата
					string candidate = "";
					// Переходим по всему списку последовательности
					for(auto & idw : seq){
						// Добавляем разделитель
						if(!candidate.empty()) candidate.append(1, ' ');
						// Добавляем слово в текст
						candidate.append(this->alm->word(idw).real());
					}
					// Выводим основное сообщение отладки
					this->alphabet->log(
						"best item: [%s]\r\n"
						"* Exist = %s\r\n"
						"* WLTF = %4.8f\r\n"
						"* logprob = %4.8f\r\n"
						"* Tanimoto = %4.8f\r\n"
						"* Levenshtein = %u\r\n",
						alphabet_t::log_t::info,
						this->logfile,
						candidate.c_str(),
						(exist ? "YES" : "NO"),
						wltf, logprob, tmo, lev
					);
				}
			};
			// Переходим по всем гипотезам
			for(auto & item : hypotheses){
				// Ищем последовательность
				auto it = this->sequence.find(item.first);
				// Если последовательность найдена
				if(it != this->sequence.end()){
					// Сбрасываем буфер
					var1 = {};
					// Если предыдущий вариант существовал, восстанавливаем его
					if(var2.buffer.test(4)){
						// Сбрасываем второй рейтинг
						var2.buffer.reset();
						// Восстанавливаем рейтинг
						var2.buffer.set(4);
					// Сбрасываем второй рейтинг
					} else var2.buffer.reset();
					// Получаем последовательность
					seq = const_cast <vector <size_t> *> (&it->second);
					// Получаем количество слов в списке
					count = seq->size();
					// Проверяем существует ли цепочка одиночных слов
					if(!onewrd) onewrd = (count == 1);
					// Выполняем проверку существования N-граммы
					if(!onewrd) exist = this->alm->check(* seq, (u_short) 2);
					// Если это - лучший вариант
					if(item.second.lev == 0){
						// Запоминаем, что вариант существует
						exist = true;
						// Устанавливаем флаг
						var1.buffer.set(4);
					}
					// if(!onewrd) exist = this->alm->check(* seq, u_short((size > 3) && (count > 3) ? 3 : 2));
					// Если предыдущий вариант существовал, то и текущий тоже должен существовать
					if((var2.buffer.test(4) && exist) || onewrd || !var2.buffer.test(4)){
						// Если предыдущий вариант не существовал, сбрасываем рейтинги
						if(exist && !onewrd && !var2.buffer.test(4)){
							// Сбрасываем первый рейтинг
							var1.buffer.reset();
							// Сбрасываем второй рейтинг
							var2.buffer.reset();
						}
						// Если - это не одиночное слово
						if(!onewrd && ((item.second.lev != var2.lev) || (var2.lev != 1))){
							// Если вес слов больше предыдущего
							if(item.second.wltf >= var2.wltf){
								// Снимаем рейтинг предыдущего варианта
								if(item.second.wltf > var2.wltf)
									// Устанавливаем рейтинг текущего варианта
									var2.buffer.reset(0);
								// Устанавливаем также рейтинг предыдущего варианта
								else var2.buffer.set(0);
								// Устанавливаем значение рейтинга
								var1.buffer.set(0);
								// Запоминаем вес слов
								var1.wltf = item.second.wltf;
							// Устанавливаем рейтинг предыдущего варианта
							} else var2.buffer.set(0);
							// Учитываем коэффициенты, только если они переданы переданы
							if((item.second.lev != 0) && (item.second.tmo != 0.0)){
								// Если значение Танимото больше предыдущего
								if(item.second.tmo >= var2.tmo){
									// Снимаем рейтинг предыдущего варианта
									if(item.second.tmo > var2.tmo){
										// Сбрасываем рейтинг предыдущего варианта
										var2.buffer.reset(1);
										// Запоминаем вес слов
										var1.wltf = item.second.wltf;
									// Запоминаем предыдущее значение рейтинга слова
									} else {
										// Устанавливаем также рейтинг предыдущего варианта
										var2.buffer.set(1);
										// Запоминаем предыдущее значение рейтинга слова
										var1.wltf = var2.wltf;
									}
									// Устанавливаем значение рейтинга
									var1.buffer.set(1);
									// Запоминаем значение Танимото
									var1.tmo = item.second.tmo;
								// Устанавливаем рейтинг предыдущего варианта
								} else var2.buffer.set(1);
							}
						// Если - это одиночное слово
						} else {
							// Учитываем коэффициенты, только если они переданы переданы
							if((item.second.lev != 0) && (item.second.tmo != 0.0)){
								// Если значение Танимото больше предыдущего
								if(item.second.tmo >= var2.tmo){
									// Снимаем рейтинг предыдущего варианта
									if(item.second.tmo > var2.tmo)
										// Устанавливаем рейтинг текущего варианта
										var2.buffer.reset(0);
									// Устанавливаем также рейтинг предыдущего варианта
									else var2.buffer.set(0);
									// Устанавливаем значение рейтинга
									var1.buffer.set(0);
									// Запоминаем значение Танимото
									var1.tmo = item.second.tmo;
								// Устанавливаем рейтинг предыдущего варианта
								} else var2.buffer.set(0);
							}
							// Если вес слов больше предыдущего
							if(item.second.wltf >= var2.wltf){
								// Снимаем рейтинг предыдущего варианта
								if(item.second.wltf > var2.wltf){
									// Сбрасываем рейтинг предыдущего варианта
									var2.buffer.reset(1);
									// Запоминаем значение Танимото
									var1.tmo = item.second.tmo;
								// Запоминаем предыдущее значение Танимото
								} else {
									// Устанавливаем также рейтинг предыдущего варианта
									var2.buffer.set(1);
									// Устанавливаем предыдущее значение Танимота
									var1.tmo = var2.tmo;
								}
								// Устанавливаем значение рейтинга
								var1.buffer.set(1);
								// Запоминаем вес слов
								var1.wltf = item.second.wltf;
							// Устанавливаем рейтинг предыдущего варианта
							} else var2.buffer.set(1);
						}
						// Учитываем коэффициенты, только если они переданы переданы
						if((item.second.lev != 0) && (item.second.tmo != 0.0)){
							// Если дистанция Левенштейна меньше предыдущей
							if(item.second.lev <= var2.lev){
								// Снимаем рейтинг предыдущего варианта
								if(item.second.lev < var2.lev){
									// Сбрасываем рейтинг предыдущего варианта
									var2.buffer.reset(2);
									// Запоминаем значение Танимото
									var1.tmo = item.second.tmo;
									// Запоминаем вес слов
									var1.wltf = item.second.wltf;
								// Если расстояние Левенштейна такое-же
								} else {
									// Устанавливаем также рейтинг предыдущего варианта
									var2.buffer.set(2);
									// Запоминаем предыдущее значение Танимото
									var1.tmo = var2.tmo;
									// Запоминаем предыдущий рейтинг слова
									var1.wltf = var2.wltf;
								}
								// Устанавливаем значение рейтинга
								var1.buffer.set(2);
								// Запоминаем дистанцию Левенштейна
								var1.lev = item.second.lev;
							// Устанавливаем рейтинг предыдущего варианта
							} else var2.buffer.set(2);
						}
						// Если последовательность существует
						if(exist && !onewrd){
							// Если предыдущий вариант не существовал, сбрасываем рейтинги
							if(!var2.buffer.test(4)){
								// Запоминаем значение Танимото
								var1.tmo = item.second.tmo;
								// Запоминаем дистанцию Левенштейна
								var1.lev = item.second.lev;
								// Запоминаем вес слов
								var1.wltf = item.second.wltf;
							}
							// Устанавливаем флаг
							var1.buffer.set(4);
							// Если количество слов больше 3-х
							if(count >= 3){
								// Добавляем конец предложения
								if((finish = (seq->back() != size_t(token_t::finish)))) seq->push_back(size_t(token_t::finish));
								// Добавляем начало предложения
								if((start = (seq->front() != size_t(token_t::start)))) seq->insert(seq->begin(), size_t(token_t::start));
								// Выполняем расчёт перплексии
								const auto & ppl = this->alm->perplexity(* seq);
								// Если добавлена конечная N-грамма
								if(finish) seq->pop_back();
								// Если добавлена начальная N-грамма
								if(start) seq->erase(seq->begin());
								// Получаем разницу по дистанции Левенштейна
								distLev = (max(item.second.lev, var2.lev) - min(item.second.lev, var2.lev));
								// Получаем разницу по значению Танимото
								distTmo = (max(item.second.tmo, var2.tmo) - min(item.second.tmo, var2.tmo));
								// Получаем разницу по частоте последовательности
								distWid = (max(ppl.logprob, var2.logprob) - min(ppl.logprob, var2.logprob));
								// Выполняем расчёт диапазона по Левенштейну
								mode1 = (item.second.lev > var2.lev ? distLev < 3 : item.second.lev == var2.lev);
								// Выполняем расчёт диапазона по Танимото
								mode2 = (item.second.tmo < var2.tmo ? distTmo > 0.2 : item.second.tmo == var2.tmo);
								// Выполняем првоерку на дистанцию значения Танимото
								distmo = ((item.second.tmo >= var2.tmo) && (distTmo > 0.05));
								// Если значение частоты выше лучшей, устанавливаем
								if(((ppl.logprob >= var2.logprob) && ((item.second.lev <= var2.lev) || ((size > 3) && (count > 3)) || distmo))
								|| ((ppl.logprob > var2.logprob) && ((distWid > 1.4) && (distLev < 2)))){
									// Снимаем рейтинг предыдущего варианта
									if(ppl.logprob > var2.logprob){
										// Сбрасываем рейтинг предыдущего варианта
										var2.buffer.reset(3);
										// Запоминаем предыдущее значение частоты
										logprob = var2.logprob;
									// Устанавливаем также рейтинг предыдущего варианта
									} else var2.buffer.set(3);
									// Устанавливаем значение рейтинга
									var1.buffer.set(3);
									// Запоминаем частоту последовательности
									var1.logprob = ppl.logprob;
								// Устанавливаем рейтинг предыдущего варианта
								} else var2.buffer.set(3);
								// Если частоты, псевдо-одинаковые, выполняем коррекцию
								if(((max(var1.logprob, logprob) - min(var1.logprob, logprob)) < 0.09) && mode1 && mode2){
									// Запоминаем значение второго буфера
									var.buffer.set(3);
									// Восстанавливаем у предыдущего буфера рейтинг
									var2.buffer.set(3);
									// Сбрасываем у текущего буфера рейтинг
									var1.buffer.reset(3);
									// Возвращаем предыдущее значение частоты
									var1.logprob = logprob;
								}
								// Если рейтинг текущего варианта, лучше предыдущего
								if(var1.buffer.test(3)){
									// Запоминаем значение Танимото
									var1.tmo = item.second.tmo;
									// Запоминаем дистанцию Левенштейна
									var1.lev = item.second.lev;
									// Запоминаем вес слов
									var1.wltf = item.second.wltf;
								// Устанавливаем предыдущие значения
								} else {
									// Запоминаем значение Танимото
									var1.tmo = var2.tmo;
									// Запоминаем дистанцию Левенштейна
									var1.lev = var2.lev;
									// Запоминаем вес слов
									var1.wltf = var2.wltf;
									// Запоминаем частоту последовательности
									var1.logprob = var2.logprob;
								}
								// Выводим отладочную информацию о текущем варианте
								printFn(item.second.wltf, ppl.logprob, item.second.tmo, item.second.lev, * seq);
							// Иначе выводим сообщение без перплексии
							} else printFn(item.second.wltf, 0.0, item.second.tmo, item.second.lev, * seq);
							// Очищаем список лучших кандидатов
							if(!var2.buffer.test(4) && (var1.buffer.to_ulong() > var2.buffer.to_ulong())) this->candidates.clear();
						}
						// Выводим отладочную информацию о текущем варианте
						if(!exist || onewrd) printFn(item.second.wltf, 0.0, item.second.tmo, item.second.lev, * seq);
						// Если рейтинг лучше предыдущего, запоминаем вариант
						if(var1.buffer.to_ulong() > var2.buffer.to_ulong()){
							// Запоминаем значение второго буфера
							var = var1;
							// Запоминаем рейтинг варианта
							var2 = var1;
							// Формируем результат
							resFn(item.first, &item.second, * seq);
						// Если рейтинги равны, считаем количество триграмм
						} else if(!onewrd && (var1.buffer.to_ulong() == var2.buffer.to_ulong())) {
							// Выполняем проверку количества триграмм
							count = this->alm->trigrams(* seq);
							// Если количество триграмм больше
							if(count > trigram){
								// Запоминаем значение второго буфера
								var = var1;
								// Запоминаем рейтинг варианта
								var2 = var1;
								// Запоминаем количество триграмм
								trigram = count;
								// Формируем результат
								resFn(item.first, &item.second, * seq);
							// Если количество триграмм одинаковое
							} else if(count == trigram) {
								// Выполняем проверку количества биграмм
								count = this->alm->bigrams(* seq);
								// Если количество биграмм больше, либо такоеже
								if(count >= bigram){
									// Запоминаем значение второго буфера
									var = var1;
									// Запоминаем рейтинг варианта
									var2 = var1;
									// Запоминаем количество биграмм
									bigram = count;
									// Формируем результат
									resFn(item.first, &item.second, * seq);
								}
							// Восстанавливаем значение 2-го буфера
							} else var2 = var;
						// Восстанавливаем значение 2-го буфера
						} else var2 = var;
					}
				}
			}
			// Если вывод отладочной информации разрешён
			if(this->isOption(options_t::debug)){
				// Выводим основное сообщение отладки
				this->alphabet->log("best candidate: [%s]\r\n", alphabet_t::log_t::info, this->logfile, bestCandidate.c_str());
			}
		}
	}
	// Выводим результат
	return result;
}
/**
 * bests Метод вывода списка лучших кандидатов
 * @return список лучших кандидатов
 */
const map <size_t, const anyks::Dumper::awrd_t *> & anyks::Dumper::bests() const noexcept {
	// Выполняем сборку лучших вариантов
	this->best();
	// Выводим результат
	return this->candidates;
}
/**
 * getSequence Метод извлечения собранной последовательности
 * @param ids идентификатор последовательности
 * @return    собранная последовательность
 */
const vector <size_t> & anyks::Dumper::getSequence(const size_t ids) const noexcept {
	// Результат работы функции
	static const vector <size_t> result;
	// Если последовательность получена
	if((ids > 0) && (ids != idw_t::NIDW) && !this->sequence.empty()){
		// Ищем последовательность
		auto it = this->sequence.find(ids);
		// Если последовательность найдена
		if(it != this->sequence.end()) return it->second;
	}
	// Выводим результат
	return result;
}
/**
 * get Метод вывода списка всех кандидатов
 * @param size размер последовательности
 * @return     список всех собранных кандидатов
 */
const map <size_t, anyks::Dumper::awrd_t> & anyks::Dumper::get(const size_t size) const noexcept {
	// Если промаркированные варианты существуют
	if(!this->marks.empty() && !this->hypotheses.empty()){
		// Очищаем список собранных вариантов
		this->variants.clear();
		// Переходим по всему списку промаркированных вариантов
		for(auto & item : this->marks){
			// Получаем список последовательности
			auto it = this->hypotheses.find(item.second);
			// Если список гипотез получен
			if(it != this->hypotheses.end()){
				// Ищем нужный нам вариант последовательности
				auto jt = it->second.find(item.first);
				// Если вариант найден
				if(jt != it->second.end()) this->variants.emplace(jt->first, jt->second);
			}
		}
		// Выводим результат
		return this->variants;
	// Если промаркированный список не заполнен
	} else if(!this->hypotheses.empty()) {
		// Если размер нулевой
		if(size == 0) return this->hypotheses.rbegin()->second;
		// Если размер указан
		else {
			// Ищем размер последовательности
			auto it = this->hypotheses.find(size);
			// Если последовательность найдена
			if(it != this->hypotheses.end()) return it->second;
			// Если последовательность не найдена
			else {
				// Формируем гипотезу для вывода
				static const map <size_t, awrd_t> hypothesis;
				// Выводим гипотезу
				return hypothesis;
			}
		}
	// Если список гипотез пустой
	} else {
		// Создаём статический пустой элемент
		static const map <size_t, awrd_t> result;
		// Выводим результат
		return result;
	}
}
/**
 * word Метод вывода слова из гипотезы
 * @param hyps гипотеза из которой нужно сформировать слово
 * @return     слово сформированное из гипотезы
 */
const wstring anyks::Dumper::word(const pair <size_t, const awrd_t *> & hyps) const noexcept {
	// Результат работы функции
	wstring result = L"";
	// Если данные заполнены
	if(!this->empty() && (this->alm != nullptr)){
		// Последовательность
		vector <size_t> seq;
		/**
		 * modifyFn Функция модификации гипотезы
		 * @param hypothesis гипотеза для модификации
		 */
		auto modifyFn = [&seq, this](const awrd_t * hypothesis) noexcept {
			// Пытаемся восстановить альтернативное слово
			this->alter(hypothesis, seq);
			// Снимаем с гипотезы константу
			awrd_t * hts = const_cast <awrd_t *> (hypothesis);
			// Если слово не пустое и не неизвестное
			if((hts->word.size() > 1) && (hts->word.compare(NOTFOUND) != 0)){
				// Если опция установки регистров активна
				if(this->isOption(options_t::uppers)){
					// Регистры слова
					size_t uppers = 0;
					// Если слово является римским числом или аббревиатурой
					if((this->alphabet->roman2Arabic(hts->word) > 0) || this->tokenizer->isAbbr(hts->idw)){
						// Получаем значение слова в верхнем регистре
						hts->word = this->alphabet->toUpper(hts->word.wstr());
					// Если слово, всегда начинается с заглавной буквы
					} else if((uppers = this->alt->getUWord(hts->idw)) > 0) hts->word.setUppers(uppers);
					// Если это нормальное слово
					else {
						// Выполняем проверку последовательности
						const auto & res = this->alm->check(seq);
						// Если регистры получены
						if(res.first && (res.second > 0)){
							// Устанавливаем регистр слова
							hts->word.setUppers(res.second);
						// Если такой последовательности не найдено, устанавливаем нижний регистр
						} else hts->word.setUppers(0);
					}
				// Иначе устанавливаем нижний регистр
				} else hts->word.setUppers(0);
			// Если слово не существует а эталонное слово существует
			} else if((hts->etalon.size() > 1) && this->isOption(options_t::uppers)) {
				// Регистры слова
				size_t uppers = 0;
				// Если слово является римским числом или аббревиатурой
				if((this->alphabet->roman2Arabic(hts->etalon) > 0) ||
				this->tokenizer->isAbbr(this->tokenizer->idw(hts->etalon))){
					// Получаем значение слова в верхнем регистре
					hts->word = this->alphabet->toUpper(hts->etalon);
				// Если слово, всегда начинается с заглавной буквы
				} else if((uppers = this->alt->getUWord(hts->etalon)) > 0) {
					// Запоминаем эталонное слово
					hts->word = hts->etalon;
					// Переводим первый символ в верхний регистр
					hts->word.setUppers(uppers);
				// Если это нормальное слово
				} else {
					// Выполняем проверку последовательности
					const auto & res = this->alm->check(seq);
					// Если регистры получены
					if(res.first && (res.second > 0)){
						// Запоминаем эталонное слово
						hts->word = hts->etalon;
						// Устанавливаем регистр слова
						hts->word.setUppers(res.second);
					}
				}
			}
		};
		// Если слово подобранно
		if(!hyps.second->word.empty()){
			// Выполняем поиск последовательности
			auto it = this->sequence.find(hyps.first);
			// Если последовательность получена
			if(it != this->sequence.end()) seq = it->second;
			// Добавляем слово в последовательность
			seq.push_back(hyps.second->idw);
			// Выполняем модификацию гипотезы
			modifyFn(hyps.second);
			// Формируем результат
			result = hyps.second->word.wreal();
		// Если эталонный вариант не указан
		} else if(hyps.second->split && !this->hypseseq.empty()){
			// Выполняем поиск последовательности
			auto it = this->sequence.find(hyps.first);
			// Если последовательность получена
			if(it != this->sequence.end()){
				// Получаем последовательность
				seq = it->second;
				// Выполняем проверку поиск нашей последовательности
				auto jt = this->hypseseq.find(hyps.first);
				// Если цепочка гипотез получена
				if(jt != this->hypseseq.end()){
					// Извлекаем последнее слово из списка
					auto it = jt->second.rbegin();
					// Выполняем модификацию гипотезы
					modifyFn(&(* it));
					// Формируем результат
					result = (* it).word.wreal();
				}
			}
		// Иначе выводим эталонное слово
		} else result = hyps.second->etalon;
	}
	// Выводим результат
	return result;
}
/**
 * ctx Метод сборки контекста из гипотезы
 * @param hyps  гипотеза для сборки текста
 * @param words список полученных слов
 * @param info  список слов для которых выполнена коррекция
 * @return      количество символов в обработанных словах и рейтинг варианта
 */
const anyks::Dumper::cstatus_t anyks::Dumper::ctx(const pair <size_t, const awrd_t *> & hyps, vector <wstring> & words, vector <pair <wstring, wstring>> * info) const noexcept {
	// Результат работы функции
	cstatus_t result;
	// Если данные заполнены
	if(!this->empty() && (hyps.second != nullptr)){
		// Очищаем список слов
		words.clear();
		// Проспличенный вариант слова
		wstring split = L"";
		// Флаг последовательного однобуквенного слова
		bool isOneLetter = false;
		// Выполняем сборку текста
		this->ctxIt(hyps, [&](const size_t count, const awrd_t * hypothesis) noexcept {
			// Получаем результат слова
			const wstring & word = (!hypothesis->word.empty() && (hypothesis->word.compare(NOTFOUND) != 0) ? hypothesis->word.wreal() : hypothesis->etalon);
			// Если слово получено
			if(!word.empty()){
				// Получаем длину слова
				const size_t length = word.length();
				// Если статистику нужно собирать
				if(info != nullptr){
					// Если вариант проспличен
					if(hypothesis->split){
						// Если эталонный вариант найден
						if(!hypothesis->etalon.empty()){
							// Добавляем в информационный список, полученное слово
							info->insert(info->begin(), make_pair(hypothesis->etalon, word + split));
						// Иначе собираем проспличенные варианты
						} else {
							// Добавляем само слово
							split.insert(split.begin(), word.begin(), word.end());
							// Добавляем разделитель слов
							split.insert(split.begin(), L' ');
						}
					// Если это не проспличенные варианты
					} else {
						// Очищаем проспличенные варианты
						split.clear();
						// Добавляем в информационный список, полученное слово
						info->insert(
							info->begin(),
							make_pair(
								hypothesis->etalon,
								(hypothesis->word.compare(NOTFOUND) == 0 ? NOTFOUND : word)
							)
						);
					}
				}
				// Добавляем слово в последовательность
				words.insert(words.begin(), word);
				// Увеличиваем количество полученных слов
				result.words++;
				// Увеличиваем количество обработанных символов
				result.letters += length;
				// Увеличиваем количество двухбуквенных слов
				result.bilet += (length < 3 ? 1 : 0);
				// Определяем максимальную длину слова
				result.maxwords += (length > 3 ? length : 0);
				// Определяем минимальную длину слова
				result.minwords += (length <= 3 ? length : 0);
				// Если слово состоит всего из одной бкувы
				if(word.length() == 1){
					// Увеличиваем количество однобуквенных последовательных слов
					if(isOneLetter) result.oclet++;
					// Запоминаем, что слово состоит из одной буквы
					isOneLetter = true;
				// Запоминаем, что слово состоит из нескольких букв
				} else isOneLetter = false;
			}
		});
		// Выполняем расчет рейтинга
		result.estimate = log(
			((max(result.maxwords, result.minwords) - min(result.maxwords, result.minwords)) * result.letters) /
			double(words.size() == 1 ? (round(log10(words.front().length())) + 3) : words.size())
		);
	}
	// Выводим результат
	return result;
}
/**
 * alter Метод восстановления альтернативного слова
 * @param hypothesis вариант гипотезы для обработки
 * @param seq        последовательность для проверки
 */
void anyks::Dumper::alter(const awrd_t * hypothesis, vector <size_t> & seq) const noexcept {
	// Если объект альтернативных вариантов не пустой
	if(this->isOption(options_t::alter) && (this->alt != nullptr) &&
	!this->alt->empty() && (!hypothesis->word.empty() || !hypothesis->etalon.empty()) &&
	(hypothesis->word.compare(NOTFOUND) != 0) && !seq.empty()){
		// Получаем идентификатор слова
		const size_t idw = seq.back();
		// Проверяем существует ли контекстно-зависимое альтернативное слово
		const auto & alt = this->alt->get(idw, true);
		// Если такое слово существует
		if(!alt.second.empty()){
			// Добавляем полученный идентификатор в список последовательности
			seq.back() = alt.first;
			// Проверяем существует ли такая N-грамма
			const auto & res = this->alm->check(seq);
			// Если такая последовательность существует
			if(res.first){
				// Заменяем идентификатор слова
				const_cast <awrd_t *> (hypothesis)->idw = alt.first;
				// Заменяем само, альтернативное слово
				const_cast <awrd_t *> (hypothesis)->word = alt.second;
			// Удаляем из последовательности вариант
			} else seq.back() = idw;
		// Если альтернативное слово не найдено
		} else {
			// Проверяем существует ли альтернативное слово
			const auto & alt = this->alt->get(idw);
			// Если вариант получен
			if(!alt.second.empty()){
				// Получаем регистры слова
				const size_t uppers = hypothesis->word.getUppers();
				// Заменяем идентификатор слова
				const_cast <awrd_t *> (hypothesis)->idw = alt.first;
				// Заменяем само, альтернативное слово
				const_cast <awrd_t *> (hypothesis)->word = alt.second;
				// Устанавливаем регистры обратно
				const_cast <awrd_t *> (hypothesis)->word.setUppers(uppers);
			}
		}
	}
}
/**
 * ctxIt Метод перебора последовательно в обратном направлении всех вариантов последовательности
 * @param hyps     гипотеза для переборки
 * @param callback функция обратного вызова
 */
void anyks::Dumper::ctxIt(const pair <size_t, const awrd_t *> & hyps, function <void (const size_t, const awrd_t *)> callback) const noexcept {
	// Если данные заполнены
	if(!this->empty() && (this->alm != nullptr) && (this->tokenizer != nullptr)){
		// Последовательность
		vector <size_t> seq;
		/**
		 * modifyFn Функция модификации гипотезы
		 * @param hypothesis гипотеза для модификации
		 */
		auto modifyFn = [&seq, this](const awrd_t * hypothesis) noexcept {
			// Пытаемся восстановить альтернативное слово
			this->alter(hypothesis, seq);
			// Снимаем с гипотезы константу
			awrd_t * hts = const_cast <awrd_t *> (hypothesis);
			// Если слово не пустое и не неизвестное
			if((hts->word.size() > 1) && (hts->word.compare(NOTFOUND) != 0)){
				// Если опция установки регистров активна
				if(this->isOption(options_t::uppers)){
					// Регистры слова
					size_t uppers = 0;
					// Если слово является римским числом или аббревиатурой
					if((this->alphabet->roman2Arabic(hts->word) > 0) || this->tokenizer->isAbbr(hts->idw)){
						// Получаем значение слова в верхнем регистре
						hts->word = this->alphabet->toUpper(hts->word.wstr());
					// Если слово, всегда начинается с заглавной буквы, устанавливаем это
					} else if((uppers = this->alt->getUWord(hts->idw)) > 0) hts->word.setUppers(uppers);
					// Если это нормальное слово
					else {
						// Выполняем проверку последовательности
						const auto & res = this->alm->check(seq);
						// Если регистры получены
						if(res.first && (res.second > 0)){
							// Устанавливаем регистр слова
							hts->word.setUppers(res.second);
						// Если такой последовательности не найдено, устанавливаем нижний регистр
						} else hts->word.setUppers(0);
					}
				// Иначе устанавливаем нижний регистр
				} else hts->word.setUppers(0);
			// Если слово не существует а эталонное слово существует
			} else if((hts->etalon.size() > 1) && this->isOption(options_t::uppers)) {
				// Регистры слова
				size_t uppers = 0;
				// Если слово является римским числом или аббревиатурой
				if((this->alphabet->roman2Arabic(hts->etalon) > 0) ||
				this->tokenizer->isAbbr(this->tokenizer->idw(hts->etalon))){
					// Получаем значение слова в верхнем регистре
					hts->word = this->alphabet->toUpper(hts->etalon);
				// Если слово, всегда начинается с заглавной буквы
				} else if((uppers = this->alt->getUWord(hts->etalon)) > 0) {
					// Запоминаем эталонное слово
					hts->word = hts->etalon;
					// Переводим первый символ в верхний регистр
					hts->word.setUppers(uppers);
				// Если это нормальное слово
				} else {
					// Выполняем проверку последовательности
					const auto & res = this->alm->check(seq);
					// Если регистры получены
					if(res.first && (res.second > 0)){
						// Запоминаем эталонное слово
						hts->word = hts->etalon;
						// Устанавливаем регистр слова
						hts->word.setUppers(res.second);
					}
				}
			}
		};
		/**
		 * getHypseseqFn Функция перебора цепочки гипотез
		 * @param ids идентификатор гипотезы
		 */
		auto getHypseseqFn = [&seq, &modifyFn, &callback, this](const size_t ids) noexcept {
			// Если цепочки последовательностей  существуют
			if(!this->hypseseq.empty()){
				// Выполняем проверку поиск нашей последовательности
				auto it = this->hypseseq.find(ids);
				// Если цепочка гипотез получена
				if(it != this->hypseseq.end()){
					// Переходим по всей цепочке
					for(auto jt = it->second.rbegin(); jt != it->second.rend(); ++jt){
						// Выполняем модификацию гипотезы
						modifyFn(&(* jt));
						// Если слово или эталонное слово не пустое
						if(!jt->etalon.empty() || !jt->word.empty())
							// Выводим каждый вариант
							callback(seq.size(), &(* jt));
						// Удаляем последний элемент из списка последовательности
						seq.pop_back();
					}
				}
			}
		};
		// Если слово подобранно
		if(!hyps.second->word.empty()){
			// Выполняем поиск последовательности
			auto it = this->sequence.find(hyps.first);
			// Если последовательность получена
			if(it != this->sequence.end()) seq = it->second;
			// Добавляем слово в последовательность
			else seq.push_back(hyps.second->idw);
			// Выполняем модификацию гипотезы
			modifyFn(hyps.second);
			// Удаляем последний элемент из списка последовательности
			seq.pop_back();
		// Если эталонный вариант не указан
		} else {
			// Выполняем поиск последовательности
			auto it = this->sequence.find(hyps.first);
			// Если последовательность получена
			if(it != this->sequence.end()){
				// Получаем последовательность
				seq = it->second;
				// Выполняем модификацию гипотезы
				modifyFn(hyps.second);
				// Выполняем перебор цепочки последовательности
				if(hyps.second->split && hyps.second->word.empty()) getHypseseqFn(hyps.first);
				// Удаляем последний элемент из списка последовательности
				else seq.pop_back();
			}
		}
		// Если слово или эталонное слово не пустое
		if(!hyps.second->etalon.empty() || !hyps.second->word.empty())
			// Выводим результат
			callback((!seq.empty() ? seq.size() : 1), hyps.second);
		// Если последовательность существует
		if(!seq.empty()){
			// Проверяем есть ли дочерние последовательности
			auto it = this->links.find(hyps.first);
			// Если последовательность существует
			if(it != this->links.end()){
				// Получаем количество значений в последовательности
				size_t count = it->second.size();
				// Переходим по всей последовательности
				for(auto jt = it->second.rbegin(); jt != it->second.rend(); ++jt){
					// Если эталонная фраза не указана
					if((* jt)->split && (* jt)->word.empty()) getHypseseqFn(this->tokenizer->ids(seq));
					// Если слово существует
					else {
						// Выполняем модификацию гипотезы
						modifyFn(* jt);
						// Удаляем последний элемент из списка последовательности
						seq.pop_back();
						// Выводим каждый вариант
						callback(count, * jt);
					}
					// Уменьшаем количество элементов
					count--;
				}
			}
		}
	}
}
/**
 * setAlm Метод установки языковой модели
 * @param alm объект языковой модели
 */
void anyks::Dumper::setAlm(const alm_t * alm) noexcept {
	// Устанавливаем языковую модель
	if(alm != nullptr) this->alm = alm;
}
/**
 * setAlt Метод установки объекта альтернативных вариантов
 * @param alt объект альтернативных вариантов
 */
void anyks::Dumper::setAlt(const alt_t * alt) noexcept {
	// Устанавливаем объект альтернативных вариантов
	if(alt != nullptr) this->alt = alt;
}
/**
 * setLogfile Метод установки файла для вывода логов
 * @param logifle адрес файла для вывода отладочной информации
 */
void anyks::Dumper::setLogfile(const char * logfile) noexcept {
	// Устанавливаем адрес log файла
	this->logfile = logfile;
}
/**
 * setOption Метод установки опций модуля
 * @param option опция для установки
 */
void anyks::Dumper::setOption(const options_t option) noexcept {
	// Устанавливаем опции
	this->options.set((u_short) option);
}
/**
 * setAlphabet Метод установки алфавита
 * @param alphabet объект алфавита
 */
void anyks::Dumper::setAlphabet(const alphabet_t * alphabet) noexcept {
	// Устанавливаем объект алфавита
	if(alphabet != nullptr) this->alphabet = alphabet;
}
/**
 * setAdCw Метод установки характеристик словаря
 * @param cw количество обработанных слов
 * @param ad количество обработанных документов
 */
void anyks::Dumper::setAdCw(const size_t cw, const size_t ad) noexcept {
	// Если статистика словаря передана, устанавливаем
	if(cw > 0) this->cw = cw;
	if(ad > 0) this->ad = ad;
}
/**
 * setTokenizer Метод установки токенизатора
 * @param tokenizer объект токенизатора
 */
void anyks::Dumper::setTokenizer(const tokenizer_t * tokenizer) noexcept {
	// Устанавливаем токенизатор
	if(tokenizer != nullptr) this->tokenizer = tokenizer;
}
/**
 * mark Метод маркировки варианта для последующей работы
 * @param seq  собранная последовательность вариантов
 * @param size размер последовательности, куда нужно добавить слово
 */
void anyks::Dumper::mark(const vector <size_t> & seq, const size_t size) noexcept {
	// Если последовательность передана
	if(!seq.empty()){
		// Результирующий размер N-граммы
		size_t result = 0;
		// Если размер последовательности не указан
		if(size == 0){
			// Если список гипотез пустой
			if(this->hypotheses.empty()) result = 2;
			// Получаем размер последовательности
			else result = (this->hypotheses.rbegin()->first + 1);
		// Запоминаем результат
		} else result = (size + 1);
		// Выполняем маркировку варианта
		this->marks.emplace((seq.size() == 1 ? seq.front() : this->tokenizer->ids(seq)), result);
	}
}
/**
 * add Метод добавления слова
 * @param word    параметры слова
 * @param size    размер последовательности, куда нужно добавить слово
 * @param replace разрешить заменять похожую последовательность
 * @return        размер полученной последовательности
 */
const size_t anyks::Dumper::add(const awrd_t & word, const size_t size, const bool replace) noexcept {
	// Результат работы функции
	size_t result = 0;
	// Если объект токенизатора существует
	if(this->tokenizer != nullptr){
		// Получаем значение Танимото
		const double tmo = word.tmo;
		// Получаем дистанцию Дамерау-Левенштейна
		const u_short lev = (word.lev == MAX_LENGTH_TEXT ? 0 : word.lev);
		// Получаем рейтинг слова
		const double wltf = (word.etalon.empty() ? word.word.calc(this->ad, this->cw).wltf : MIN_WORD_WEIGHT);
		// Если вывод отладочной информации разрешён
		if(this->isOption(options_t::debug)){
			// Выводим основное сообщение отладки
			this->alphabet->log(
				"add candidate: [%s]\r\n"
				"* WLTF = %4.8f\r\n",
				alphabet_t::log_t::info,
				this->logfile,
				this->alphabet->convert(word.etalon).c_str(), wltf
			);
		}
		// Если размер последовательности не указан
		if(size == 0){
			// Если список гипотез пустой
			if(this->hypotheses.empty()) result = 2;
			// Получаем размер последовательности
			else result = (this->hypotheses.rbegin()->first + 1);
		// Запоминаем результат
		} else result = (size + 1);
		// Если нужно в нулевой размер последовательности
		if((result - 1) == 1){
			// Создаём список связей
			vector <const awrd_t *> lnk;
			// Список последовательности
			vector <size_t> seq = {word.idw};
			// Устанавливаем дистанцию Дамерау-Левенштейна, предыдущего слова
			const_cast <awrd_t *> (&word)->lev = lev;
			// Устанавливаем рейтинг слова
			const_cast <awrd_t *> (&word)->wltf = wltf;
			// Добавляем новую связь
			this->links.emplace(word.idw, move(lnk));
			// Добавляем последовательность в список последовательностей
			this->sequence.emplace(word.idw, move(seq));
			// Ищем гипотезы для такого размера
			auto it = this->hypotheses.find(result);
			// Если гипотезы такого размера существуют
			if(it != this->hypotheses.end()){
				// Если такая запись уже есть
				if(replace && (it->second.count(word.idw) > 0))
					// Удаляем текущую запись
					it->second.erase(word.idw);
				// Добавляем в список новую гипотезу
				it->second.emplace(word.idw, word);
			// Если гипотез для такого размера ещё нет
			} else {
				// Создаём список гипотез
				map <size_t, awrd_t> hypothesis = {{word.idw, word}};
				// Добавляем в список гипотез
				this->hypotheses.emplace(result, move(hypothesis));
			}
		// Иначе добавляем в указанный размер последовательности
		} else {
			// Получаем идентификатор последовательности
			size_t ids = idw_t::NIDW;
			// Список последовательности
			vector <size_t> * seq = nullptr;
			// Список всех собранных связей
			vector <const awrd_t *> * lnk = nullptr;
			// Указатель на текущий список гипотез
			map <size_t, awrd_t> * hypotheses = nullptr;
			// Получаем список гипотез для текущего размера последовательности
			auto it = this->hypotheses.find(result - 1);
			// Если список последовательности найден
			if(it != this->hypotheses.end()) hypotheses = &it->second;
			// Получаем максимально-возможный размер последовательности
			else hypotheses = &this->hypotheses.rbegin()->second;
			/*
			// Если количество подобранных вариантов больше 1000
			if(hypotheses->size() > 1000){
				// Переходим по всему списку гипотез
				for(auto it = hypotheses->begin(); it != hypotheses->end();){
					// Получаем последовательность
					seq = &this->sequence.at(it->first);
					// Если вариант не существует и это не сплит, тогда выполняем удаление
					if(!this->alm->check(* seq, (u_short) 2) && !it->second.split){
						// Получаем идентификатор последовательности
						ids = this->tokenizer->ids(* seq);
						// Удаляем ссылки
						this->links.erase(ids);
						// Удаляем полученную последовательность
						this->sequence.erase(ids);
						// Удаляем текущий вариант
						it = hypotheses->erase(it);
					// Иначе продолжаем обход
					} else it++;
				}
			}
			*/
			{
				// Контейнер куда, нужно добавлять варианты
				map <size_t, awrd_t> * dest = nullptr;
				// Выполняем извлечение текущего контейнера
				auto it = this->hypotheses.find(result);
				// Если контейнер существует
				if(it != this->hypotheses.end()) dest = &it->second;
				// Если такой контейнер еще не существует
				else {
					// Создаём новый контейнер
					auto ret = this->hypotheses.emplace(result, map <size_t, awrd_t> ());
					// Получаем блок структуры
					dest = &ret.first->second;
				}
				// Переходим по всему списку гипотез
				for(auto & item : * hypotheses){
					// Получаем связи
					lnk = &this->links.at(item.first);
					// Получаем последовательность
					seq = &this->sequence.at(item.first);
					// Устанавливаем значение Танимото
					const_cast <awrd_t *> (&word)->tmo = (tmo + item.second.tmo);
					// Устанавливаем дистанцию Дамерау-Левенштейна, предыдущего слова
					const_cast <awrd_t *> (&word)->lev = ((lev == MAX_LENGTH_TEXT ? 0 : lev) + item.second.lev);
					// Устанавливаем рейтинг слова
					const_cast <awrd_t *> (&word)->wltf = ((wltf == MIN_WORD_WEIGHT ? 0 : wltf) + (item.second.wltf == MIN_WORD_WEIGHT ? 0 : item.second.wltf));
					// Если вес оказался нулевым, сбрасываем вес
					if(word.wltf == 0) const_cast <awrd_t *> (&word)->wltf = MIN_WORD_WEIGHT;
					// Добавляем в последовательность наш идентификатор
					seq->push_back(word.idw);
					// Добавляем новую связь
					lnk->push_back(&item.second);
					// Получаем идентификатор последовательности
					ids = this->tokenizer->ids(* seq);
					// Добавляем в список связей новую связь
					this->links.emplace(ids, * lnk);
					// Добавляем последовательность в список последовательностей
					this->sequence.emplace(ids, * seq);
					// Если такая запись уже есть, удаляем запись
					if(replace && (dest->count(ids) > 0)) dest->erase(ids);
					// Добавляем в список новую гипотезу
					dest->emplace(ids, word);
					// Удаляем добавленную ранее связь из списка связей
					lnk->pop_back();
					// Удаляем добавленное значение из последовательности
					seq->pop_back();
				}
			}
		}
	}
	// Выводим результат
	return result;
}
/**
 * once Метод добавления слова один раз, в младшие секции
 * @param word    параметры слова
 * @param count   количество слов в тексте
 * @param size    размер последовательности, куда нужно добавить слово
 * @param replace разрешить заменять похожую последовательность
 * @return        размер полученной последовательности
 */
const size_t anyks::Dumper::once(const awrd_t & word, const size_t count, const size_t size, const bool replace) noexcept {
	// Результат работы функции
	size_t result = 0;
	// Если объект токенизатора существует
	if(this->tokenizer != nullptr){
		// Позиция для добавления варианта
		size_t pos = 0;
		// Устанавливаем значение результата
		result = (size + count - 1);
		// Получаем значение Танимото
		const double tmo = word.tmo;
		// Запоминаем что слово проспличено
		const_cast <awrd_t *> (&word)->split = true;
		// Получаем рейтинг слова
		const double wltf = word.word.calc(this->ad, this->cw).wltf;
		// Получаем дистанцию Дамерау-Левенштейна
		const u_short lev = (word.lev == MAX_LENGTH_TEXT ? 0 : word.lev);
		// Если вывод отладочной информации разрешён
		if(this->isOption(options_t::debug)){
			// Выводим основное сообщение отладки
			this->alphabet->log(
				"once candidate: [%s]\r\n"
				"* WLTF = %4.8f\r\n"
				"* Tanimoto = %4.8f\r\n"
				"* Levenshtein = %u\r\n",
				alphabet_t::log_t::info,
				this->logfile,
				word.word.real().c_str(),
				wltf, word.tmo, word.lev
			);
		}
		// Если размер последовательности не указан
		if(size == 0){
			// Если список гипотез пустой
			if(this->hypotheses.empty()) pos = 2;
			// Получаем размер последовательности
			else pos = (this->hypotheses.rbegin()->first + 1);
		// Запоминаем результат
		} else pos = (size + 1);
		// Если нужно в нулевой размер последовательности
		if((pos - 1) == 1){
			// Создаём список связей
			vector <const awrd_t *> lnk;
			// Список последовательности
			vector <size_t> seq = {word.idw};
			// Устанавливаем дистанцию Дамерау-Левенштейна, предыдущего слова
			const_cast <awrd_t *> (&word)->lev = lev;
			// Устанавливаем рейтинг слова
			const_cast <awrd_t *> (&word)->wltf = wltf;
			// Добавляем новую связь
			this->links.emplace(word.idw, lnk);
			// Добавляем последовательность в список последовательностей
			this->sequence.emplace(word.idw, seq);
			// Ищем гипотезы для такого размера
			auto it = this->hypotheses.find(size + count);
			// Если гипотезы такого размера существуют
			if(it != this->hypotheses.end()){
				// Если такая запись уже есть
				if(replace && (it->second.count(word.idw) > 0))
					// Удаляем текущую запись
					it->second.erase(word.idw);
				// Добавляем в список новую гипотезу
				it->second.emplace(word.idw, word);
			// Если гипотез для такого размера ещё нет
			} else {
				// Создаём список гипотез
				map <size_t, awrd_t> hypothesis = {{word.idw, word}};
				// Добавляем в список гипотез
				this->hypotheses.emplace(size + count, move(hypothesis));
			}
		// Иначе добавляем в указанный размер последовательности
		} else {
			// Получаем идентификатор последовательности
			size_t ids = idw_t::NIDW;
			// Список последовательности
			vector <size_t> * seq = nullptr;
			// Список всех собранных связей
			vector <const awrd_t *> * lnk = nullptr;
			// Указатель на текущий список гипотез
			map <size_t, awrd_t> * hypotheses = nullptr;
			// Получаем список гипотез для текущего размера последовательности
			auto it = this->hypotheses.find(pos - 1);
			// Если список последовательности найден
			if(it != this->hypotheses.end()) hypotheses = &it->second;
			// Получаем максимально-возможный размер последовательности
			else hypotheses = &this->hypotheses.rbegin()->second;
			// Переходим по всему списку гипотез
			for(auto & item : * hypotheses){
				// Получаем связи
				lnk = &this->links.at(item.first);
				// Получаем последовательность
				seq = &this->sequence.at(item.first);
				// Устанавливаем значение Танимото
				const_cast <awrd_t *> (&word)->tmo = (tmo + item.second.tmo);
				// Устанавливаем дистанцию Дамерау-Левенштейна, предыдущего слова
				const_cast <awrd_t *> (&word)->lev = ((lev == MAX_LENGTH_TEXT ? 0 : lev) + item.second.lev);
				// Устанавливаем рейтинг слова
				const_cast <awrd_t *> (&word)->wltf = ((wltf == MIN_WORD_WEIGHT ? 0 : wltf) + (item.second.wltf == MIN_WORD_WEIGHT ? 0 : item.second.wltf));
				// Если вес оказался нулевым, сбрасываем вес
				if(word.wltf == 0) const_cast <awrd_t *> (&word)->wltf = MIN_WORD_WEIGHT;
				// Добавляем в последовательность наш идентификатор
				seq->push_back(word.idw);
				// Добавляем новую связь
				lnk->push_back(&item.second);
				// Получаем идентификатор последовательности
				ids = this->tokenizer->ids(* seq);
				// Добавляем в список связей новую связь
				this->links.emplace(ids, * lnk);
				// Добавляем последовательность в список последовательностей
				this->sequence.emplace(ids, * seq);
				// Получаем список последовательности
				auto it = this->hypotheses.find(size + count);
				// Если список гипотез получен
				if(it != this->hypotheses.end()){
					// Если такая запись уже есть
					if(replace && (it->second.count(ids) > 0))
						// Удаляем текущую запись
						it->second.erase(ids);
					// Добавляем в список новую гипотезу
					it->second.emplace(ids, word);
				// Если такая последовательность не найдена
				} else {
					// Создаём список гипотез
					map <size_t, awrd_t> hypothesis = {{ids, word}};
					// Добавляем новую гипотезу
					this->hypotheses.emplace(size + count, move(hypothesis));
				}
				// Удаляем добавленную ранее связь из списка связей
				lnk->pop_back();
				// Удаляем добавленное значение из последовательности
				seq->pop_back();
			}
		}
	}
	// Выводим результат
	return result;
}
/**
 * add Метод добавления последовательности слов
 * @param words   параметры списка слов
 * @param lev     дистанция Дамерау-Левенштейна
 * @param tmo     значение Танимото
 * @param size    размер последовательности, куда нужно добавить слово
 * @param replace разрешить заменять похожую последовательность
 * @return        размер полученной последовательности
 */
const size_t anyks::Dumper::add(const vector <awrd_t> & words, const u_short lev, const double tmo, const size_t size, const bool replace) noexcept {
	// Результат работы функции
	size_t result = 0;
	// Если объект токенизатора существует
	if(!words.empty() && (this->tokenizer != nullptr)){
		// Создаем гипотезу
		awrd_t word;
		// Запоминаем что слово проспличено
		word.split = true;
		// Устанавливаем идентификатор гипотезы
		word.idw = words.front().idw;
		// Рейтинг всех слов последовательности
		double wltf = MIN_WORD_WEIGHT, wltf2 = MIN_WORD_WEIGHT;
		// Переходим по всему списку слов и считаем их вес
		for(auto & item : words){
			// Выполняем расчёт веса слова
			wltf2 = item.word.calc(this->ad, this->cw).wltf;
			// Если вес слова инициализирован
			wltf = ((wltf == MIN_WORD_WEIGHT ? 0 : wltf) + (wltf2 == MIN_WORD_WEIGHT ? 0 : wltf2));
		}
		// Если вес нулевой, убираем инициализацию
		if(wltf == 0) wltf = MIN_WORD_WEIGHT;
		// Если вывод отладочной информации разрешён
		if(this->isOption(options_t::debug)){
			// Вариант слова для добавления
			string word = "";
			// Переходим по всему списку слов
			for(auto & item : words){
				// Добавляем слово в список
				word.append(item.word.real());
				// Добавляем разделитель
				word.append(1, ' ');
			}
			// Удаляем последний элемент
			word.pop_back();
			// Выводим основное сообщение отладки
			this->alphabet->log(
				"add candidates: [%s]\r\n"
				"* Tanimoto = %4.8f\r\n"
				"* Levenshtein = %u\r\n",
				alphabet_t::log_t::info,
				this->logfile, word.c_str(), tmo, lev
			);
		}
		// Если размер последовательности не указан
		if(size == 0){
			// Если список гипотез пустой
			if(this->hypotheses.empty()) result = 2;
			// Получаем размер последовательности
			else result = (this->hypotheses.rbegin()->first + 1);
		// Запоминаем результат
		} else result = (size + 1);
		// Если нужно в нулевой размер последовательности
		if((result - 1) == 1){
			// Создаём список связей
			vector <const awrd_t *> lnk;
			// Список последовательности
			vector <size_t> seq(words.size());
			// Формируем последовательность
			for(size_t i = 0; i < words.size(); i++) seq[i] = words[i].idw;
			// Устанавливаем значение Танимото
			const_cast <awrd_t *> (&word)->tmo = tmo;
			// Устанавливаем дистанцию Дамерау-Левенштейна, предыдущего слова
			const_cast <awrd_t *> (&word)->lev = lev;
			// Устанавливаем рейтинг слова
			const_cast <awrd_t *> (&word)->wltf = wltf;
			// Получаем идентификатор последовательности
			const size_t ids = this->tokenizer->ids(seq);
			// Добавляем последовательность в список
			this->hypseseq.emplace(ids, words);
			// Добавляем новую связь
			this->links.emplace(ids, move(lnk));
			// Добавляем последовательность в список последовательностей
			this->sequence.emplace(ids, move(seq));
			// Ищем гипотезы для такого размера
			auto it = this->hypotheses.find(result);
			// Если гипотезы такого размера существуют
			if(it != this->hypotheses.end()){
				// Если такая запись уже есть
				if(replace && (it->second.count(ids) > 0))
					// Удаляем текущую запись
					it->second.erase(ids);
				// Добавляем в список новую гипотезу
				it->second.emplace(ids, move(word));
			// Если гипотез для такого размера ещё нет
			} else {
				// Создаём список гипотез
				map <size_t, awrd_t> hypothesis = {{ids, move(word)}};
				// Добавляем в список гипотез
				this->hypotheses.emplace(result, move(hypothesis));
			}
		// Иначе добавляем в указанный размер последовательности
		} else {
			// Получаем идентификатор последовательности
			size_t ids = idw_t::NIDW;
			// Список последовательности
			vector <size_t> * seq = nullptr;
			// Список текущей последовательности
			vector <size_t> tmp(words.size());
			// Список всех собранных связей
			vector <const awrd_t *> * lnk = nullptr;
			// Указатель на текущий список гипотез
			map <size_t, awrd_t> * hypotheses = nullptr;
			// Получаем список гипотез для текущего размера последовательности
			auto it = this->hypotheses.find(result - 1);
			// Если список последовательности найден
			if(it != this->hypotheses.end()) hypotheses = &it->second;
			// Получаем максимально-возможный размер последовательности
			else hypotheses = &this->hypotheses.rbegin()->second;
			/*
			// Если количество подобранных вариантов больше 1000
			if(hypotheses->size() > 1000){
				// Переходим по всему списку гипотез
				for(auto it = hypotheses->begin(); it != hypotheses->end();){
					// Получаем последовательность
					seq = &this->sequence.at(it->first);
					// Если вариант не существует и это не сплит, тогда выполняем удаление
					if(!this->alm->check(* seq, (u_short) 2) && !it->second.split){
						// Получаем идентификатор последовательности
						ids = this->tokenizer->ids(* seq);
						// Удаляем ссылки
						this->links.erase(ids);
						// Удаляем полученную последовательность
						this->sequence.erase(ids);
						// Удаляем текущий вариант
						it = hypotheses->erase(it);
					// Иначе продолжаем обход
					} else it++;
				}
			}
			*/
			// Формируем последовательность
			for(size_t i = 0; i < words.size(); i++) tmp[i] = words[i].idw;
			{
				// Контейнер куда, нужно добавлять варианты
				map <size_t, awrd_t> * dest = nullptr;
				// Выполняем извлечение текущего контейнера
				auto it = this->hypotheses.find(result);
				// Если контейнер существует
				if(it != this->hypotheses.end()) dest = &it->second;
				// Если такой контейнер еще не существует
				else {
					// Создаём новый контейнер
					auto ret = this->hypotheses.emplace(result, map <size_t, awrd_t> ());
					// Получаем блок структуры
					dest = &ret.first->second;
				}
				// Переходим по всему списку гипотез
				for(auto & item : * hypotheses){
					// Получаем связи
					lnk = &this->links.at(item.first);
					// Получаем последовательность
					seq = &this->sequence.at(item.first);
					// Устанавливаем значение Танимото
					const_cast <awrd_t *> (&word)->tmo = (tmo + item.second.tmo);
					// Устанавливаем дистанцию Дамерау-Левенштейна, предыдущего слова
					const_cast <awrd_t *> (&word)->lev = ((lev == MAX_LENGTH_TEXT ? 0 : lev) + item.second.lev);
					// Устанавливаем рейтинг слова
					const_cast <awrd_t *> (&word)->wltf = ((wltf == MIN_WORD_WEIGHT ? 0 : wltf) + (item.second.wltf == MIN_WORD_WEIGHT ? 0 : item.second.wltf));
					// Если вес оказался нулевым, сбрасываем вес
					if(word.wltf == 0) const_cast <awrd_t *> (&word)->wltf = MIN_WORD_WEIGHT;
					// Добавляем в последовательность наши идентификаторы
					seq->insert(seq->end(), tmp.begin(), tmp.end());
					// Добавляем новую связь
					lnk->push_back(&item.second);
					// Получаем идентификатор последовательности
					ids = this->tokenizer->ids(* seq);
					// Добавляем в список связей новую связь
					this->links.emplace(ids, * lnk);
					// Добавляем последовательность в список последовательностей
					this->sequence.emplace(ids, * seq);
					// Добавляем последовательность в список
					this->hypseseq.emplace(ids, words);
					// Если такая запись уже есть, удаляем запись
					if(replace && (dest->count(ids) > 0)) dest->erase(ids);
					// Добавляем в список новую гипотезу
					dest->emplace(ids, word);
					// Удаляем добавленную ранее связь из списка связей
					lnk->pop_back();
					// Удаляем добавленное значение из последовательности
					seq->erase(seq->begin() + tmp.size(), seq->end());
				}
			}
		}
	}
	// Выводим результат
	return result;
}
/**
 * smart Метод умного добавления слова
 * @param word параметры слова
 * @param size размер последовательности, куда нужно добавить слово
 * @return     размер полученной последовательности
 */
const pair <size_t, size_t> anyks::Dumper::smart(const awrd_t & word, const size_t size) noexcept {
	// Результат работы функции
	pair <size_t, size_t> result = {0, 0};
	// Если объект токенизатора существует
	if((this->alm != nullptr) && (this->tokenizer != nullptr)){
		// Получаем значение Танимото
		const double tmo = word.tmo;
		// Получаем рейтинг слова
		const double wltf = word.word.calc(this->ad, this->cw).wltf;
		// Получаем дистанцию Дамерау-Левенштейна
		const u_short lev = (word.lev == MAX_LENGTH_TEXT ? 0 : word.lev);
		// Если размер последовательности не указан
		if(size == 0){
			// Если список гипотез пустой
			if(this->hypotheses.empty()) result.first = 2;
			// Получаем размер последовательности
			else result.first = (this->hypotheses.rbegin()->first + 1);
		// Запоминаем результат
		} else result.first = (size + 1);
		// Устанавливаем лучшее слово
		const_cast <awrd_t *> (&word)->best = (lev == 1 ? word.word.length() > 6 : false);
		// Если нужно в нулевой размер последовательности
		if((result.first - 1) == 1){
			// Увеличиваем количество добавленных вариантов
			result.second++;
			// Создаём список связей
			vector <const awrd_t *> lnk;
			// Список последовательности
			vector <size_t> seq = {word.idw};
			// Устанавливаем дистанцию Дамерау-Левенштейна, предыдущего слова
			const_cast <awrd_t *> (&word)->lev = lev;
			// Устанавливаем рейтинг слова
			const_cast <awrd_t *> (&word)->wltf = wltf;
			// Добавляем новую связь
			this->links.emplace(word.idw, move(lnk));
			// Добавляем последовательность в список последовательностей
			this->sequence.emplace(word.idw, move(seq));
			// Ищем гипотезы для такого размера
			auto it = this->hypotheses.find(result.first);
			// Если гипотезы такого размера существуют
			if(it != this->hypotheses.end()){
				// Добавляем в список новую гипотезу
				it->second.emplace(word.idw, word);
			// Если гипотез для такого размера ещё нет
			} else {
				// Создаём список гипотез
				map <size_t, awrd_t> hypothesis = {{word.idw, word}};
				// Добавляем в список гипотез
				this->hypotheses.emplace(result.first, move(hypothesis));
			}
			// Если вывод отладочной информации разрешён
			if(this->isOption(options_t::debug)){
				// Выводим основное сообщение отладки
				this->alphabet->log(
					"smart candidate: [%s]\r\n"
					"* WLTF = %4.8f\r\n"
					"* Tanimoto = %4.8f\r\n"
					"* Levenshtein = %u\r\n",
					alphabet_t::log_t::info,
					this->logfile,
					word.word.real().c_str(),
					wltf, word.tmo, word.lev
				);
			}
		// Иначе добавляем в указанный размер последовательности
		} else {
			// Получаем идентификатор последовательности
			size_t ids = idw_t::NIDW;
			// Список последовательности
			vector <size_t> * seq = nullptr;
			// Список всех собранных связей
			vector <const awrd_t *> * lnk = nullptr;
			// Указатель на текущий список гипотез
			map <size_t, awrd_t> * hypotheses = nullptr;
			// Получаем список гипотез для текущего размера последовательности
			auto it = this->hypotheses.find(result.first - 1);
			// Если список последовательности найден
			if(it != this->hypotheses.end()) hypotheses = &it->second;
			// Получаем максимально-возможный размер последовательности
			else hypotheses = &this->hypotheses.rbegin()->second;
			/*
			// Если количество подобранных вариантов больше 1000
			if(hypotheses->size() > 1000){
				// Переходим по всему списку гипотез
				for(auto it = hypotheses->begin(); it != hypotheses->end();){
					// Получаем последовательность
					seq = &this->sequence.at(it->first);
					// Если вариант не существует и это не сплит, тогда выполняем удаление
					if(!this->alm->check(* seq, (u_short) 2) && !it->second.split){
						// Получаем идентификатор последовательности
						ids = this->tokenizer->ids(* seq);
						// Удаляем ссылки
						this->links.erase(ids);
						// Удаляем полученную последовательность
						this->sequence.erase(ids);
						// Удаляем текущий вариант
						it = hypotheses->erase(it);
					// Иначе продолжаем обход
					} else it++;
				}
			}
			*/
			{
				// Контейнер куда, нужно добавлять варианты
				map <size_t, awrd_t> * dest = nullptr;
				// Выполняем извлечение текущего контейнера
				auto it = this->hypotheses.find(result.first);
				// Если контейнер существует
				if(it != this->hypotheses.end()) dest = &it->second;
				// Если такой контейнер еще не существует
				else {
					// Создаём новый контейнер
					auto ret = this->hypotheses.emplace(result.first, map <size_t, awrd_t> ());
					// Получаем блок структуры
					dest = &ret.first->second;
				}
				// Переходим по всему списку гипотез
				for(auto & item : * hypotheses){
					// Получаем последовательность
					seq = &this->sequence.at(item.first);
					// Добавляем в последовательность наш идентификатор
					seq->push_back(word.idw);
					// Проверяем на существование N-граммы
					const_cast <awrd_t *> (&word)->ngram = this->alm->check(* seq, (u_short) 2);
					// Если добавление разрешено
					if(word.ngram || word.best){
						// Увеличиваем количество добавленных вариантов
						result.second++;
						// Получаем связи
						lnk = &this->links.at(item.first);
						// Устанавливаем дистанцию Дамерау-Левенштейна, предыдущего слова
						const_cast <awrd_t *> (&word)->lev = (lev + item.second.lev);
						// Устанавливаем значение Танимото
						const_cast <awrd_t *> (&word)->tmo = (tmo + item.second.tmo);
						// Устанавливаем рейтинг слова
						const_cast <awrd_t *> (&word)->wltf = ((wltf == MIN_WORD_WEIGHT ? 0 : wltf) + (item.second.wltf == MIN_WORD_WEIGHT ? 0 : item.second.wltf));
						// Если вес оказался нулевым, сбрасываем вес
						if(word.wltf == 0) const_cast <awrd_t *> (&word)->wltf = MIN_WORD_WEIGHT;
						// Добавляем новую связь
						lnk->push_back(&item.second);
						// Получаем идентификатор последовательности
						ids = this->tokenizer->ids(* seq);
						// Добавляем новую гипотезу
						dest->emplace(ids, word);
						// Добавляем в список связей новую связь
						this->links.emplace(ids, * lnk);
						// Добавляем последовательность в список последовательностей
						this->sequence.emplace(ids, * seq);
						// Удаляем добавленную ранее связь из списка связей
						lnk->pop_back();
						// Если вывод отладочной информации разрешён
						if(this->isOption(options_t::debug)){
							// Выводим основное сообщение отладки
							this->alphabet->log(
								"smart candidate: [%s]\r\n"
								"* WLTF = %4.8f\r\n"
								"* Tanimoto = %4.8f\r\n"
								"* Levenshtein = %u\r\n",
								alphabet_t::log_t::info,
								this->logfile,
								word.word.real().c_str(),
								wltf, word.tmo, word.lev
							);
						}
					}
					// Удаляем добавленное значение из последовательности
					seq->pop_back();
				}
			}
		}
	}
	// Выводим результат
	return result;
}
/**
 * set Метод установки нового слова в последовательность собираемых вариантов
 * @param word    параметры слова
 * @param ids     идентификатор предыдущей последовательности
 * @param size    размер последовательности, куда нужно добавить слово
 * @param replace разрешить заменять похожую последовательность
 * @return        размер полученной последовательности
 */
const pair <size_t, size_t> anyks::Dumper::set(const awrd_t & word, const size_t ids, const size_t size, const bool replace) noexcept {
	// Получаем идентификатор слова по-умолчанию
	const size_t nidw = idw_t::NIDW;
	// Результат работы функции
	pair <size_t, size_t> result = make_pair(nidw, 0);
	// Если объект токенизатора существует
	if(this->tokenizer != nullptr){
		// Запоминаем идентификатор последовательности
		result.first = word.idw;
		// Получаем значение Танимото
		const double tmo = word.tmo;
		// Получаем рейтинг слова
		const double wltf = word.word.calc(this->ad, this->cw).wltf;
		// Получаем дистанцию Дамерау-Левенштейна
		const u_short lev = (word.lev == MAX_LENGTH_TEXT ? 0 : word.lev);
		// Если вывод отладочной информации разрешён
		if(this->isOption(options_t::debug)){
			// Выводим основное сообщение отладки
			this->alphabet->log(
				"set candidate: [%s]\r\n"
				"* WLTF = %4.8f\r\n",
				alphabet_t::log_t::info,
				this->logfile,
				this->alphabet->convert(word.etalon).c_str(), wltf
			);
		}
		// Если размер последовательности не указан
		if(size == 0){
			// Если список гипотез пустой
			if(this->hypotheses.empty()) result.second = 2;
			// Получаем размер последовательности
			else result.second = (this->hypotheses.rbegin()->first + 1);
		// Запоминаем результат
		} else result.second = (size + 1);
		// Если нужно в нулевой размер последовательности
		if((result.second - 1) == 1){
			// Создаём список связей
			vector <const awrd_t *> lnk;
			// Список последовательности
			vector <size_t> seq = {result.first};
			// Устанавливаем дистанцию Дамерау-Левенштейна, предыдущего слова
			const_cast <awrd_t *> (&word)->lev = lev;
			// Устанавливаем рейтинг слова
			const_cast <awrd_t *> (&word)->wltf = wltf;
			// Добавляем новую связь
			this->links.emplace(result.first, move(lnk));
			// Добавляем последовательность в список последовательностей
			this->sequence.emplace(result.first, move(seq));
			// Ищем гипотезы для такого размера
			auto it = this->hypotheses.find(result.second);
			// Если гипотезы такого размера существуют
			if(it != this->hypotheses.end()){
				// Если такая запись уже есть
				if(replace && (it->second.count(result.first) > 0))
					// Удаляем текущую запись
					it->second.erase(result.first);
				// Добавляем в список новую гипотезу
				it->second.emplace(result.first, word);
			// Если гипотез для такого размера ещё нет
			} else {
				// Создаём список гипотез
				map <size_t, awrd_t> hypothesis = {{result.first, word}};
				// Добавляем в список гипотез
				this->hypotheses.emplace(result.second, move(hypothesis));
			}
		// Иначе добавляем в указанный размер последовательности
		} else {
			// Указатель на текущий список гипотез
			map <size_t, awrd_t> * hypotheses = nullptr;
			// Получаем список гипотез для текущего размера последовательности
			auto it = this->hypotheses.find(result.second - 1);
			// Если список последовательности найден
			if(it != this->hypotheses.end()) hypotheses = &it->second;
			// Получаем максимально-возможный размер последовательности
			else hypotheses = &this->hypotheses.rbegin()->second;
			// Выполняем поиск нужного нам идентификатора
			auto jt = hypotheses->find(ids);
			// Если идентификатор найден
			if(jt != hypotheses->end()){
				// Получаем последовательность
				vector <size_t> * seq = &this->sequence.at(jt->first);
				// Получаем связи последовательности
				vector <const awrd_t *> * lnk = &this->links.at(jt->first);
				// Устанавливаем дистанцию Дамерау-Левенштейна, предыдущего слова
				const_cast <awrd_t *> (&word)->lev = (lev + jt->second.lev);
				// Устанавливаем значение Танимото
				const_cast <awrd_t *> (&word)->tmo = (tmo + jt->second.tmo);
				// Устанавливаем рейтинг слова
				const_cast <awrd_t *> (&word)->wltf = ((wltf == MIN_WORD_WEIGHT ? 0 : wltf) + (jt->second.wltf == MIN_WORD_WEIGHT ? 0 : jt->second.wltf));
				// Если вес оказался нулевым, сбрасываем вес
				if(word.wltf == 0) const_cast <awrd_t *> (&word)->wltf = MIN_WORD_WEIGHT;
				// Добавляем в последовательность наш идентификатор
				seq->push_back(word.idw);
				// Добавляем новую связь
				lnk->push_back(&jt->second);
				// Получаем идентификатор последовательности
				result.first = this->tokenizer->ids(* seq);
				// Добавляем в список связей новую связь
				this->links.emplace(result.first, * lnk);
				// Добавляем последовательность в список последовательностей
				this->sequence.emplace(result.first, * seq);
				// Если такого размера последовательности ещё нет
				if(this->hypotheses.count(result.second) < 1){
					// Создаём список гипотез
					map <size_t, awrd_t> hypothesis = {{result.first, word}};
					// Добавляем новую гипотезу
					this->hypotheses.emplace(result.second, move(hypothesis));
				// Если такой размер последовательности уже есть
				} else {
					// Получаем список последовательности
					auto it = this->hypotheses.find(result.second);
					// Если список гипотез получен
					if(it != this->hypotheses.end()){
						// Если такая запись уже есть
						if(replace && (it->second.count(result.first) > 0))
							// Удаляем текущую запись
							it->second.erase(result.first);
						// Добавляем в список новую гипотезу
						it->second.emplace(result.first, word);
					}
				}
				// Удаляем добавленную ранее связь из списка связей
				lnk->pop_back();
				// Удаляем добавленное значение из последовательности
				seq->pop_back();
			}
		}
	}
	// Выводим результат
	return result;
}
