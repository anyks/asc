/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef __ANYKS_HNSW__
#define __ANYKS_HNSW__

/**
 * Стандартная библиотека
 */
#include <map>
#include <thread>
#include <atomic>
#include <vector>
#include <iostream>
#include <functional>
/**
 * Наши модули
 */
#include <hnswlib/hnswlib.h>

using namespace std;
using namespace hnswlib;

/**
 * anyks пространство имён
 */
namespace anyks {
	/**
	 * Hnsw Класс системы нахождения ближайшего вектора
	 */
	typedef class Hnsw {
		private:
			// Размерность вектора
			u_short dim = 0;
			// Название векторного пространства
			string name = "";
			// Флаг включённой нормализации
			bool norm = false;
			// Флаг инициализации индекса
			bool inited = false;
			// Флаг заполненности индекса
			bool epadded = false;
			// Количество добавленных элементов
			labeltype count = 0;
			// Количество доступных ядер в системе
			u_short threads = thread::hardware_concurrency();
		private:
			// Интерфейс векторного пространства
			SpaceInterface <float> * l2space = nullptr;
			// Список эмбеддингов для поиска
			HierarchicalNSW <float> * appralg = nullptr;
		private:
			/**
			 * normalize Функция нормализации вектора
			 * @param data данные для нормализации
			 * @param res  список нормализованных значений
			 */
			void normalize(const float * data, float * res) const noexcept;
			/**
			 * threadPool Тредпул для параллельной обработки
			 * @param start      начало диапазона
			 * @param end        конец диапазона
			 * @param count      количество доступных ядер
			 * @param callbackFn функция обратного вызова
			 */
			inline void threadPool(const u_short start, const size_t end, u_short count, function <void (size_t, size_t)> callbackFn) const noexcept;
		public:
			/**
			 * empty Метод проверки на существование индекса
			 * @return существование индекса
			 */
			const bool empty() const noexcept;
			/**
			 * getM Метод получения максимального количества исходящих соединений
			 * @return максимальное количество исходящих соединений
			 */
			const size_t getM() const noexcept;
			/**
			 * getMaxElements Метод получения максимального количества элементов
			 * @return количество установленного максимального количества элементов
			 */
			const size_t getMaxElements() const noexcept;
			/**
			 * getCurrentCount Метод получения количество текущих элементов
			 * @return количество текущих элементов
			 */
			const size_t getCurrentCount() const noexcept;
			/**
			 * getEfConstruction Метод получения компромиса между временем и точностью строительства индекса
			 * @return компромис между временем и точностью строительства индекса
			 */
			const size_t getEfConstruction() const noexcept;
		public:
			/**
			 * getIds Метод получения списка идентификаторов
			 * @return список идентификаторов индекса
			 */
			const vector <labeltype> getIds() const noexcept;
			/**
			 * getData Метод извлечения списка дистанций для указанных идентификаторов
			 * @param ids список идентификаторов для получения дистанций
			 * @return    список дистанций для указанных идентификаторов
			 */
			const vector <vector <float>> getData(const vector <labeltype> & ids) const noexcept;
			/**
			 * query Метод поиска подходящего варианта
			 * @param input   эталонный вариант для сравнения
			 * @param k       количество вариантов к выдачи
			 * @param threads количество используемых ядер процессора
			 */
			const pair <vector <labeltype>, vector <float>> query(const vector <vector <float>> & input, const size_t k = 1, short threads = -1) const noexcept;
		public:
			/**
			 * clear Метод очистки
			 */
			void clear() noexcept;
			/**
			 * ef Метод установки точности
			 * @param ef точность работы алгоритма
			 */
			void ef(const size_t ef) noexcept;
			/**
			 * resize Метод перераспределения индекса
			 * @param size размер нового индекса
			 */
			void resize(const size_t size) noexcept;
			/**
			 * markDeleted Метод пометки идентификатора для удаления из индекса
			 * @param id идентификатор для удаления из индекса
			 */
			void markDeleted(const labeltype id) noexcept;
			/**
			 * setThreads Метод установки количества используемых ядер
			 * @param threads количество используемых ядер для установки
			 */
			void setThreads(const u_short threads) noexcept;
			/**
			 * save Метод сохранения индекса в буфер
			 * @param buffer буфер данных для извлечения байт индекса
			 */
			void save(vector <char> & buffer) const noexcept;
			/**
			 * save Метод сохранения индекса на диск
			 * @param filename адрес файла индекса
			 */
			void save(const string & filename) const noexcept;
		public:
			/**
			 * init Метод инициализации
			 * @param name название меода поиска
			 * @param dim  размерность индекса
			 */
			void init(const string & name, const u_short dim);
			/**
			 * load Метод загрузки индекса из буфера в память
			 * @param buffer   буфер данных индекса
			 * @param elements количество элементов в индексе
			 */
			void load(const char * buffer, const size_t elements);
			/**
			 * load Метод загрузки индекса с диска в память
			 * @param filename адрес файла индекса
			 * @param elements количество элементов в индексе
			 */
			void load(const string & filename, const size_t elements);
			/**
			 * load Метод загрузки индекса из буфера в память
			 * @param buffer   буфер данных индекса
			 * @param elements количество элементов в индексе
			 */
			void load(const vector <char> & buffer, const size_t elements);
			/**
			 * add Метод добавления в индекс новых данных
			 * @param input   эталонный вариант для сравнения
			 * @param ids     список идентификаторов дистанций
			 * @param threads количество используемых ядер процессора
			 */
			void add(const vector <vector <float>> & input, const vector <size_t> & ids, short threads = -1);
			/**
			 * index Метод инициализации индекса
			 * @param elements       количество элементов в индексе
			 * @param M              максимальное количество исходящих соединений
			 * @param efConstruction компромис между временем и точностью строительства индекса
			 * @param randomSeed     размер случайного заполнения
			 */
			void index(const size_t elements, const size_t M = 16, const size_t efConstruction = 200, const size_t randomSeed = 100);
		public:
			/**
			 * Hnsw Конструктор
			 */
			Hnsw() noexcept {};
			/**
			 * Hnsw Конструктор
			 * @param name название меода поиска
			 * @param dim  размерность индекса
			 */
			Hnsw(const string & name, const u_short dim);
			/**
			 * ~Hnsw Деструктор
			 */
			~Hnsw();
	} hnsw_t;
};

#endif // __ANYKS_HNSW__
