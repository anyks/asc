/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef __ANYKS_BLOOM__
#define __ANYKS_BLOOM__

/**
 * Стандартная библиотека
 */
#include <memory>
#include <string>
#include <cassert>
#include <iostream>
/**
 * Наши модули
 */
#include <bloom/bloom.hpp>
#include <handypack/handypack.hpp>

// Активируем стандартное пространство имён
using namespace std;

/**
 * anyks пространство имён
 */
namespace anyks {
	/**
	 * Класс Блум фильтра
	 */
	typedef class Bloom {
		private:
			/**
			 * Структура исходного фильтра Блума
			 */
			typedef struct Impl: public bloom_filter {
				/**
				 * load Метод загрузки дампа
				 * @param in поток для чтения
				 */
				void load(istream & in) noexcept;
				/**
				 * dump Метод выгрузки дампа
				 * @param out поток для записи
				 */
				void dump(ostream & out) const noexcept;
				/**
				 * Impl Конструктор
				 */
				Impl(): bloom_filter() {}
				/**
				 * Impl Конструктор
				 * @param filter фильтр данных
				 */
				Impl(const Impl & filter): bloom_filter(filter) {}
				/**
				 * Impl Конструктор
				 * @param params параметры фильтра Блума
				 */
				Impl(const bloom_parameters & params): bloom_filter(params) {}
				/**
				 * ~Impl Деструктор
				 */
				~Impl(){}
			} impl_t;
		private:
			// Фильтр данных
			unique_ptr <Impl> filter;
		public:
			/**
			 * load Метод загрузки дампа
			 * @param in поток для чтения
			 */
			void load(istream & in) noexcept;
			/**
			 * dump Метод выгрузки дампа
			 * @param out поток для записи
			 */
			void dump(ostream & out) const noexcept;
			/**
			 * insert Метод добавления элемента
			 * @param element элемент для добавления
			 */
			void insert(const string & element) noexcept;
			/**
			 * contains Метод проверки наличия элемента
			 * @param element элемент для проверки
			 * @return        результат проверки
			 */
			const bool contains(const string & element) const noexcept;
		public:
			/**
			 * Bloom Конструктор
			 */
			Bloom() noexcept;
			/**
			 * Bloom Конструктор
			 * @param elements          количество элементов в фильтре
			 * @param falsePositiveRate размер ложно-положительной ставки
			 */
			Bloom(const size_t elements, const double falsePositiveRate) noexcept;
			/**
			 * ~Bloom Деструктор
			 */
			~Bloom() noexcept {};
	} bloom_t;
};

#endif // __ANYKS_BLOOM__
