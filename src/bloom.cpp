/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <bloom.hpp>

/**
 * load Метод загрузки дампа
 * @param in поток для чтения
 */
void anyks::Bloom::Impl::load(istream & in) noexcept {
	// Выполняем загрузку основных параметров
	NHandyPack::Load(in, salt_, bit_table_, salt_count_, table_size_, projected_element_count_, inserted_element_count_, random_seed_, desired_false_positive_probability_);
}
/**
 * dump Метод выгрузки дампа
 * @param out поток для записи
 */
void anyks::Bloom::Impl::dump(ostream & out) const noexcept {
	// Выполняем дамп основных параметров
	NHandyPack::Dump(out, salt_, bit_table_, salt_count_, table_size_, projected_element_count_, inserted_element_count_, random_seed_, desired_false_positive_probability_);
}
/**
 * load Метод загрузки дампа
 * @param in поток для чтения
 */
void anyks::Bloom::load(istream & in) noexcept {
	// Выполняем загрузку дампа фильтра
	this->filter->load(in);
}
/**
 * dump Метод выгрузки дампа
 * @param out поток для записи
 */
void anyks::Bloom::dump(ostream & out) const noexcept {
	// Выполняем дамп фильтра
	this->filter->dump(out);
}
/**
 * insert Метод добавления элемента
 * @param element элемент для добавления
 */
void anyks::Bloom::insert(const string & element) noexcept {
	// Устанавливаем элемент в фильтр
	this->filter->insert(element);
}
/**
 * contains Метод проверки наличия элемента
 * @param element элемент для проверки
 * @return        результат проверки
 */
const bool anyks::Bloom::contains(const string & element) const noexcept {
	// Выполняем проверку элемента в фильтре
	return this->filter->contains(element);
}
/**
 * Bloom Конструктор
 */
anyks::Bloom::Bloom() noexcept {
	// Создаём фильтр данных
	this->filter.reset(new impl_t());
}
/**
 * Bloom Конструктор
 * @param elements          количество элементов в фильтре
 * @param falsePositiveRate размер ложно-положительной ставки
 */
anyks::Bloom::Bloom(const size_t elements, const double falsePositiveRate) noexcept {
	// Создаём параметры фильтра
	bloom_parameters parameters;
	// Устанавливаем количество произвольных сидов
	parameters.random_seed = 42;
	// Устанавливаем количество элементов в фильтре
	parameters.projected_element_count = elements;
	// Устанавливаем размер ложно-положительной ставки
	parameters.false_positive_probability = falsePositiveRate;
	// Выполняем расчёт оптимальных параметров
	parameters.compute_optimal_parameters();
	// Проверяем на корректность параметров
	assert(!(!parameters));
	// Создаём фильтр данных
	this->filter.reset(new impl_t(parameters));
}
