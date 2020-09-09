/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <hnsw.hpp>

/**
 * normalize Функция нормализации вектора
 * @param data данные для нормализации
 * @param res  список нормализованных значений
 */
void anyks::Hnsw::normalize(const float * data, float * res) const noexcept {
	// Нормализованное значение
	float norm = 0.0f;
	// Выполняем перемножение данных
	for(u_short i = 0; i < this->dim; i++) norm += (data[i] * data[i]);
	// Высчитываем нормализованное число
	norm = (1.0f / (sqrtf(norm) + 1e-30f));
	// Заполняем список нормализации полученными данными
	for(u_short i = 0; i < this->dim; i++) res[i] = (data[i] * norm);
}
/**
 * threadPool Тредпул для параллельной обработки
 * @param start      начало диапазона
 * @param end        конец диапазона
 * @param count      количество доступных ядер
 * @param callbackFn функция обратного вызова
 */
void anyks::Hnsw::threadPool(const u_short start, const size_t end, u_short count, function <void (size_t, size_t)> callbackFn) const noexcept {
	// Если общее количество ядер не установлено, устанавливаем все доступные
	if(count <= 0) count = thread::hardware_concurrency();
	// Если общее количество ядер равно 1
	if(count == 1)
		// Обрабатываем весь диапазон ID в одном ядре
		for(size_t id = start; id < end; id++) callbackFn(id, 0);
	// Если количество ядер больше 1
	else {
		// Мютекс для блокировки потока
		mutex lastExceptMutex;
		// Список потоков
		vector <thread> threads;
		// Создаём список атамарных операций
		atomic <size_t> current(start);
		// Обработчик ошибок в потоке
		exception_ptr lastException = nullptr;
		// Создаем указанное количество потоков
		for(u_short num = 0; num < count; ++num){
			// Формируем поток
			threads.push_back(thread([&, num]{
				// Создаем бесконечный цикл
				while(true){
					// Получаем номер (идентификатор) потока
					size_t id = current.fetch_add(1);
					// Если количество потоков достигло максимума, выходим
					if((id >= end)) break;
					try {
						// Выполняем функцию обратного вызова
						callbackFn(id, num);
					} catch (...) {
						// Выполняем блокировку потока
						unique_lock <mutex> lastExcepLock(lastExceptMutex);
						// Извлекаем данные исключения
						lastException = current_exception();
						// Запоминаем номер отработанного потока
						current = end;
						// Выходим
						break;
					}
				}
			}));
		}
		// Ожидаем завершения каждого потока
		for(auto & thread : threads) thread.join();
		// Если происходит ошибка, выводим её
		if(lastException != nullptr) rethrow_exception(lastException);
	}
}
/**
 * empty Метод проверки на существование индекса
 * @return существование индекса
 */
const bool anyks::Hnsw::empty() const noexcept {
	// Выводим состояние индекса
	return !this->inited;
}
/**
 * getM Метод получения максимального количества исходящих соединений
 * @return максимальное количество исходящих соединений
 */
const size_t anyks::Hnsw::getM() const noexcept {
	// Выводим максимальное количество исходящих соединений
	return this->appralg->M_;
}
/**
 * getMaxElements Метод получения максимального количества элементов
 * @return количество установленного максимального количества элементов
 */
const size_t anyks::Hnsw::getMaxElements() const noexcept {
	// Выводим установленное максимальное количество элементов
	return this->appralg->max_elements_;
}
/**
 * getCurrentCount Метод получения количество текущих элементов
 * @return количество текущих элементов
 */
const size_t anyks::Hnsw::getCurrentCount() const noexcept {
	// Выводим количество текущих элементов
	return this->appralg->cur_element_count;
}
/**
 * getEfConstruction Метод получения компромиса между временем и точностью строительства индекса
 * @return компромис между временем и точностью строительства индекса
 */
const size_t anyks::Hnsw::getEfConstruction() const noexcept {
	// Выводим компромис между временем и точностью строительства индекса
	return this->appralg->ef_construction_;
}
/**
 * getIds Метод получения списка идентификаторов
 * @return список идентификаторов индекса
 */
const vector <labeltype> anyks::Hnsw::getIds() const noexcept {
	// Список собранных лейблов
	vector <labeltype> ids;
	// Переходим по всему список идентификаторов
	for(auto kv : this->appralg->label_lookup_){
		// Собираем список идентификаторов
		ids.push_back(kv.first);
	}
	// Выводим список идентификаторов
	return ids;
}
/**
 * getData Метод извлечения списка дистанций для указанных идентификаторов
 * @param ids список идентификаторов для получения дистанций
 * @return    список дистанций для указанных идентификаторов
 */
const vector <vector <float>> anyks::Hnsw::getData(const vector <labeltype> & ids) const noexcept {
	// Результат работы функции
	vector <vector <float>> result;
	// Если список идентификаторв не пустой
	if(!ids.empty()){
		// Переходим по всему списку идентификаторов
		for(auto & id : ids) result.push_back(this->appralg->template getDataByLabel <float> (id));
	}
	// Выводим список дистанций для идентификаторов
	return result;
}
/**
 * query Метод поиска подходящего варианта
 * @param input   эталонный вариант для сравнения
 * @param k       количество вариантов к выдачи
 * @param threads количество используемых ядер процессора
 */
const pair <vector <labeltype>, vector <float>> anyks::Hnsw::query(const vector <vector <float>> & input, const size_t k, short threads) const noexcept {
	// Результат работы функции
	pair <vector <labeltype>, vector <float>> result;
	// Если эталонный вариант для сравнения передан
	if(!input.empty()){
		// Количество строк
		const size_t rows = input.size();
		// Количество фич
		const size_t features = input[0].size();
		// Если количество ядер не установлено, устанавливаем по дефолту
		if(threads <= 0) threads = this->threads;
		// Если количество строк меньше количества ядер * 4, тогда переводим всё на одно ядро
		if(rows <= size_t(threads * 4)) threads = 1;
		// Создаем массив дистанций
		result.second = vector <float> (rows * k);
		// Создаем массив данных
		result.first = vector <labeltype> (rows * k);
		// Если нормализация не включена
		if(!this->norm){
			// Выполняем обработку
			this->threadPool(0, rows, threads, [&](const size_t row, const u_short threadId){
				// Выполняем поиск ближнего варианта
				priority_queue <pair <float, labeltype >> res = this->appralg->searchKnn(input.at(row).data(), k);
				// Получаем количество найденных элементов
				const size_t count = res.size();
				// Если количество вариантов не соответствует заданному
				if(res.size() != k){
					// Выполняем изменение размера массива данных
					result.first.resize(rows * count);
					// Выполняем изменение размера списка дистанций
					result.second.resize(rows * count);
				}
				// Переходим по всему списку
				for(int i = count - 1; i >= 0; i--){
					// Получаем первый вариант
					auto & tuple = res.top();
					// Создаём список лейблов
					result.first[row * count + i] = move(tuple.second);
					// Создаём список дистанций
					result.second[row * count + i] = move(tuple.first);
					// Удаляем последний вариант
					res.pop();
				}
			});
		// Если нормализация включена
		} else {
			// Формируем список нормализованных данных
			vector <float> norm(threads * features);
			// Выполняем обработку
			this->threadPool(0, rows, threads, [&](const size_t row, const u_short threadId){
				// Получаем количество искомых вариантов
				const u_short startIdx = (threadId * this->dim);
				// Выполняем нормализацию вектора
				this->normalize((const float *) input.at(row).data(), norm.data() + startIdx);
				// Выполняем поиск ближнего варианта
				priority_queue <pair <float, labeltype >> res = this->appralg->searchKnn(norm.data() + startIdx, k);
				// Получаем количество найденных элементов
				const size_t count = res.size();
				// Если количество вариантов не соответствует заданному
				if(res.size() != k){
					// Выполняем изменение размера массива данных
					result.first.resize(rows * count);
					// Выполняем изменение размера списка дистанций
					result.second.resize(rows * count);
				}
				// Переходим по всему списку
				for(int i = count - 1; i >= 0; i--){
					// Получаем первый вариант
					auto & tuple = res.top();
					// Создаём список лейблов
					result.first[row * count + i] = move(tuple.second);
					// Создаём список дистанций
					result.second[row * count + i] = move(tuple.first);
					// Удаляем последний вариант
					res.pop();
				}
			});
		}
	}
	// Выводим результат
	return result;
}
/**
 * clear Метод очистки
 */
void anyks::Hnsw::clear() noexcept {
	// Размерность вектора
	this->dim = 0;
	// Количество добавленных элементов
	this->count = 0;
	// Название векторного пространства
	this->name = "";
	// Флаг включённой нормализации
	this->norm = false;
	// Флаг инициализации индекса
	this->inited = false;
	// Флаг заполненности индекса
	this->epadded = false;
	// Количество доступных ядер в системе
	this->threads = thread::hardware_concurrency();
	// Очищаем память если она выделенна
	if(this->l2space != nullptr) delete this->l2space;
	if(this->appralg != nullptr) delete this->appralg;
	// Зануляем объекты
	this->l2space = nullptr;
	this->appralg = nullptr;
}
/**
 * ef Метод установки точности
 * @param ef точность работы алгоритма
 */
void anyks::Hnsw::ef(const size_t ef) noexcept {
	// Устанавливаем точность работы алгоритма
	this->appralg->ef_ = ef;
}
/**
 * resize Метод перераспределения индекса
 * @param size размер нового индекса
 */
void anyks::Hnsw::resize(const size_t size) noexcept {
	// Выполняем перераспределение индекса
	this->appralg->resizeIndex(size);
}
/**
 * markDeleted Метод пометки идентификатора для удаления из индекса
 * @param id идентификатор для удаления из индекса
 */
void anyks::Hnsw::markDeleted(const labeltype id) noexcept {
	// Помечаем лейбл для удаления
	this->appralg->markDelete(id);
}
/**
 * setThreads Метод установки количества используемых ядер
 * @param threads количество используемых ядер для установки
 */
void anyks::Hnsw::setThreads(const u_short threads) noexcept {
	// Выполняем установку количества используемых ядер
	this->threads = threads;
}
/**
 * save Метод сохранения индекса в буфер
 * @param buffer буфер данных для извлечения байт индекса
 */
void anyks::Hnsw::save(vector <char> & buffer) const noexcept {
	// Выполняем сохранение индекса на диск
	this->appralg->saveIndex(buffer);
}
/**
 * save Метод сохранения индекса на диск
 * @param filename адрес файла индекса
 */
void anyks::Hnsw::save(const string & filename) const noexcept {
	// Выполняем сохранение индекса на диск
	this->appralg->saveIndex(filename);
}
/**
 * init Метод инициализации
 * @param name название меода поиска
 * @param dim  размерность индекса
 */
void anyks::Hnsw::init(const string & name, const u_short dim){
	// Если имя и размерность переданы
	if(!name.empty() && (dim > 0)){
		try {
			// Устанавливаем размерность вектора
			this->dim = dim;
			// Устанавливаем название векторного пространства
			this->name = move(name);
			// Определяем тип векторного пространства
			if(this->name.compare("l2") == 0) this->l2space = new L2Space(this->dim);
			else if(this->name.compare("ip") == 0) this->l2space = new InnerProductSpace(this->dim);
			else if(this->name.compare("cosine") == 0){
				this->norm = true;
				this->l2space = new InnerProductSpace(this->dim);
			}
		// Если происходит ошибка то игнорируем её
		} catch(const bad_alloc &) {
			// Выходим из приложения
			exit(EXIT_FAILURE);
		}
	}
}
/**
 * load Метод загрузки индекса из буфера в память
 * @param buffer   буфер данных индекса
 * @param elements количество элементов в индексе
 */
void anyks::Hnsw::load(const char * buffer, const size_t elements){
	try {
		// Если индекс уже загружен
		if(this->appralg != nullptr){
			// Выводим сообщение об ошибке, что индекс уже загружен
			cerr << "Warning: Calling load_index for an already inited index. Old index is being deallocated." << endl;
			// Удаляем выделенную ранее память
			delete this->appralg;
		}
		// Создаем новый индекс
		this->appralg = new HierarchicalNSW <float> (this->l2space, buffer, false, elements);
		// Запоминаем количество добавленных элементов
		this->count = this->appralg->cur_element_count;
		// Запоминаем что инициализация индекса выполнена
		this->inited = true;
	// Если происходит ошибка то игнорируем её
	} catch(const bad_alloc &) {
		// Выходим из приложения
		exit(EXIT_FAILURE);
	}
}
/**
 * load Метод загрузки индекса с диска в память
 * @param filename адрес файла индекса
 * @param elements количество элементов в индексе
 */
void anyks::Hnsw::load(const string & filename, const size_t elements){
	try {
		// Если индекс уже загружен
		if(this->appralg != nullptr){
			// Выводим сообщение об ошибке, что индекс уже загружен
			cerr << "Warning: Calling load_index for an already inited index. Old index is being deallocated." << endl;
			// Удаляем выделенную ранее память
			delete this->appralg;
		}
		// Создаем новый индекс
		this->appralg = new HierarchicalNSW <float> (this->l2space, filename, false, elements);
		// Запоминаем количество добавленных элементов
		this->count = this->appralg->cur_element_count;
		// Запоминаем что инициализация индекса выполнена
		this->inited = true;
	// Если происходит ошибка то игнорируем её
	} catch(const bad_alloc &) {
		// Выходим из приложения
		exit(EXIT_FAILURE);
	}
}
/**
 * load Метод загрузки индекса из буфера в память
 * @param buffer   буфер данных индекса
 * @param elements количество элементов в индексе
 */
void anyks::Hnsw::load(const vector <char> & buffer, const size_t elements){
	try {
		// Если индекс уже загружен
		if(this->appralg != nullptr){
			// Выводим сообщение об ошибке, что индекс уже загружен
			cerr << "Warning: Calling load_index for an already inited index. Old index is being deallocated." << endl;
			// Удаляем выделенную ранее память
			delete this->appralg;
		}
		// Создаем новый индекс
		this->appralg = new HierarchicalNSW <float> (this->l2space, buffer, false, elements);
		// Запоминаем количество добавленных элементов
		this->count = this->appralg->cur_element_count;
		// Запоминаем что инициализация индекса выполнена
		this->inited = true;
	// Если происходит ошибка то игнорируем её
	} catch(const bad_alloc &) {
		// Выходим из приложения
		exit(EXIT_FAILURE);
	}
}
/**
 * add Метод добавления в индекс новых данных
 * @param input   эталонный вариант для сравнения
 * @param ids     список идентификаторов дистанций
 * @param threads количество используемых ядер процессора
 */
void anyks::Hnsw::add(const vector <vector <float>> & input, const vector <size_t> & ids, short threads){
	// Количество строк
	const size_t rows = input.size();
	// Количество фич
	const size_t features = input[0].size();
	// Если количество ядер не установлено, устанавливаем по дефолту
	if(threads <= 0) threads = this->threads;
	// Если количество фич не равно размерности вектора
	if(features != this->dim) throw runtime_error("Wrong dimensionality of the vectors.");
	// Если количество строк меньше количества ядер * 4, тогда переводим всё на одно ядро
	if(rows <= size_t(threads * 4)) threads = 1;
	{
		// Начало работы
		u_short start = 0;
		// Если индекс еще не заполнен
		if(!this->epadded){
			// Получаем первый идентификатор
			const size_t id = (!ids.empty() ? ids.at(0) : this->count);
			// Получаем данные первого вектора
			float * data = (float *) input.at(0).data();
			// Создаем список нормализатора
			vector <float> norm(this->dim);
			// Если необходима нормализация
			if(this->norm){
				// Выполняем нормализацию вектора
				this->normalize((const float *) data, norm.data());
				// Получаем сами данные
				data = norm.data();
			}
			// Добавляем первый идентификатор
			this->appralg->addPoint(data, id);
			// Запоминаем начало работы
			start = 1;
			// Запоминаем что индекс уже заполнен
			this->epadded = true;
		}
		// Если нормализация не требуется
		if(!this->norm){
			// Выполняем обработку
			this->threadPool(start, rows, threads, [&](const size_t row, const u_short threadId){
				// Получаем идентификатор
				const size_t id = (!ids.empty() ? ids.at(row) : this->count + row);
				// Добавляем идентификатор в список
				this->appralg->addPoint(input.at(row).data(), id);
			});
		// Если требуется нормализация
		} else {
			// Формируем список нормализованных данных
			vector <float> norm(threads * this->dim);
			// Выполняем обработку
			this->threadPool(start, rows, threads, [&](const size_t row, const u_short threadId){
				// Получаем количество искомых вариантов
				const u_short startIdx = (threadId * this->dim);
				// Выполняем нормализацию вектора
				this->normalize((const float *) input.at(row).data(), norm.data() + startIdx);
				// Получаем идентификатор
				const size_t id = (!ids.empty() ? ids.at(row) : this->count + row);
				// Добавляем идентификатор в список
				this->appralg->addPoint(norm.data() + startIdx, id);
			});
		}
		// Запоминаем количество добавленных вариантов
		this->count += rows;
	}
}
/**
 * index Метод инициализации индекса
 * @param elements       количество элементов в индексе
 * @param M              максимальное количество исходящих соединений
 * @param efConstruction компромис между временем и точностью строительства индекса
 * @param randomSeed     размер случайного заполнения
 */
void anyks::Hnsw::index(const size_t elements, const size_t M, const size_t efConstruction, const size_t randomSeed){
	try {
		// Если данные уже инициализованы
		if(this->inited) cerr << "Warning: The index is already create." << endl;
		// Если еще не создан
		else {
			// Если индекс уже инициализован, генерируем ошибку
			if(this->appralg != nullptr) throw runtime_error("The index is already initiated.");
			// Сбрасываем количество добавленных элементов
			this->count = 0;
			// Выполняем инициализацию индекса
			this->appralg = new HierarchicalNSW <float> (this->l2space, elements, M, efConstruction, randomSeed);
			// Запоминаем, что в индекс ничего не добавлено
			this->epadded = false;
			// Запоминаем что инициализация индекса выполнена
			this->inited = true;
		}
	// Если происходит ошибка то игнорируем её
	} catch(const bad_alloc &) {
		// Выходим из приложения
		exit(EXIT_FAILURE);
	}
}
/**
 * Hnsw Конструктор
 * @param name название меода поиска
 * @param dim  размерность индекса
 */
anyks::Hnsw::Hnsw(const string & name, const u_short dim){
	// Выполняем инициализацию векторного пространства
	this->init(name, dim);
}
/**
 * ~Hnsw Деструктор
 */
anyks::Hnsw::~Hnsw(){
	// Очищаем память если она выделенна
	if(this->l2space != nullptr) delete this->l2space;
	if(this->appralg != nullptr) delete this->appralg;
}
