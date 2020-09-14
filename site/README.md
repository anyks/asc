[![ANYKS Smart language model](https://raw.githubusercontent.com/anyks/asc/master/site/img/banner.jpg)](https://anyks.com)

# ANYKS Spell-checker (ASC) C++11

- [Описание проекта](https://github.com/anyks/asc/tree/master/site#%D0%BE%D0%BF%D0%B8%D1%81%D0%B0%D0%BD%D0%B8%D0%B5-%D0%BF%D1%80%D0%BE%D0%B5%D0%BA%D1%82%D0%B0)
- [Список возможностей](https://github.com/anyks/asc/tree/master/site#%D1%81%D0%BF%D0%B8%D1%81%D0%BE%D0%BA-%D0%B2%D0%BE%D0%B7%D0%BC%D0%BE%D0%B6%D0%BD%D0%BE%D1%81%D1%82%D0%B5%D0%B9)
- [Зависимости](https://github.com/anyks/asc/tree/master/site#%D0%B7%D0%B0%D0%B2%D0%B8%D1%81%D0%B8%D0%BC%D0%BE%D1%81%D1%82%D0%B8)
- [Сборка проекта](https://github.com/anyks/asc/tree/master/site#%D1%81%D0%B1%D0%BE%D1%80%D0%BA%D0%B0-%D0%BF%D1%80%D0%BE%D0%B5%D0%BA%D1%82%D0%B0)
    - [Python версия ASC](https://github.com/anyks/asc/tree/master/site#python-%D0%B2%D0%B5%D1%80%D1%81%D0%B8%D1%8F-asc)
    - [Клонирование проекта](https://github.com/anyks/asc/tree/master/site#%D0%BA%D0%BB%D0%BE%D0%BD%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5-%D0%BF%D1%80%D0%BE%D0%B5%D0%BA%D1%82%D0%B0)
    - [Linux и FreeBSD](https://github.com/anyks/asc/tree/master/site#linux-%D0%B8-freebsd)
    - [MacOS X](https://github.com/anyks/asc/tree/master/site#macos-x)
- [Готовые словари](https://github.com/anyks/asc/tree/master/site#%D0%B3%D0%BE%D1%82%D0%BE%D0%B2%D1%8B%D0%B5-%D1%81%D0%BB%D0%BE%D0%B2%D0%B0%D1%80%D0%B8)
- [Тестирование](https://github.com/anyks/asc/tree/master/site#%D1%82%D0%B5%D1%81%D1%82%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5)
    - [Материалы использовавшиеся в тестировании](https://github.com/anyks/asc/tree/master/site#%D0%BC%D0%B0%D1%82%D0%B5%D1%80%D0%B8%D0%B0%D0%BB%D1%8B-%D0%B8%D1%81%D0%BF%D0%BE%D0%BB%D1%8C%D0%B7%D0%BE%D0%B2%D0%B0%D0%B2%D1%88%D0%B8%D0%B5%D1%81%D1%8F-%D0%B2-%D1%82%D0%B5%D1%81%D1%82%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B8)
- [Форматы файлов](https://github.com/anyks/asc/tree/master/site#%D1%84%D0%BE%D1%80%D0%BC%D0%B0%D1%82%D1%8B-%D1%84%D0%B0%D0%B9%D0%BB%D0%BE%D0%B2)
    - [Arpa](https://github.com/anyks/asc/tree/master/site#arpa)
    - [Vocab](https://github.com/anyks/asc/tree/master/site#vocab)
        - [Описание](https://github.com/anyks/asc/tree/master/site#%D0%BE%D0%BF%D0%B8%D1%81%D0%B0%D0%BD%D0%B8%D0%B5)
    - [Файл похожих букв в разных словарях](https://github.com/anyks/asc/tree/master/site#%D1%84%D0%B0%D0%B9%D0%BB-%D0%BF%D0%BE%D1%85%D0%BE%D0%B6%D0%B8%D1%85-%D0%B1%D1%83%D0%BA%D0%B2-%D0%B2-%D1%80%D0%B0%D0%B7%D0%BD%D1%8B%D1%85-%D1%81%D0%BB%D0%BE%D0%B2%D0%B0%D1%80%D1%8F%D1%85)
    - [Файл списка аббревиатур](https://github.com/anyks/asc/tree/master/site#%D1%84%D0%B0%D0%B9%D0%BB-%D1%81%D0%BF%D0%B8%D1%81%D0%BA%D0%B0-%D0%B0%D0%B1%D0%B1%D1%80%D0%B5%D0%B2%D0%B8%D0%B0%D1%82%D1%83%D1%80)
    - [Файл списка доменных зон](https://github.com/anyks/asc/tree/master/site#%D1%84%D0%B0%D0%B9%D0%BB-%D1%81%D0%BF%D0%B8%D1%81%D0%BA%D0%B0-%D0%B4%D0%BE%D0%BC%D0%B5%D0%BD%D0%BD%D1%8B%D1%85-%D0%B7%D0%BE%D0%BD)
    - [Формат скрипта python предобработки полученных слов](https://github.com/anyks/asc/tree/master/site#%D1%84%D0%BE%D1%80%D0%BC%D0%B0%D1%82-%D1%81%D0%BA%D1%80%D0%B8%D0%BF%D1%82%D0%B0-python-%D0%BF%D1%80%D0%B5%D0%B4%D0%BE%D0%B1%D1%80%D0%B0%D0%B1%D0%BE%D1%82%D0%BA%D0%B8-%D0%BF%D0%BE%D0%BB%D1%83%D1%87%D0%B5%D0%BD%D0%BD%D1%8B%D1%85-%D1%81%D0%BB%D0%BE%D0%B2)
    - [Формат скрипта python определения собственных токенов слов](https://github.com/anyks/asc/tree/master/site#%D1%84%D0%BE%D1%80%D0%BC%D0%B0%D1%82-%D1%81%D0%BA%D1%80%D0%B8%D0%BF%D1%82%D0%B0-python-%D0%BE%D0%BF%D1%80%D0%B5%D0%B4%D0%B5%D0%BB%D0%B5%D0%BD%D0%B8%D1%8F-%D1%81%D0%BE%D0%B1%D1%81%D1%82%D0%B2%D0%B5%D0%BD%D0%BD%D1%8B%D1%85-%D1%82%D0%BE%D0%BA%D0%B5%D0%BD%D0%BE%D0%B2-%D1%81%D0%BB%D0%BE%D0%B2)
    - [Пример скрипта python стемминга](https://github.com/anyks/asc/tree/master/site#%D0%BF%D1%80%D0%B8%D0%BC%D0%B5%D1%80-%D1%81%D0%BA%D1%80%D0%B8%D0%BF%D1%82%D0%B0-python-%D1%81%D1%82%D0%B5%D0%BC%D0%BC%D0%B8%D0%BD%D0%B3%D0%B0)
    - [Переменные окружения](https://github.com/anyks/asc/tree/master/site#%D0%BF%D0%B5%D1%80%D0%B5%D0%BC%D0%B5%D0%BD%D0%BD%D1%8B%D0%B5-%D0%BE%D0%BA%D1%80%D1%83%D0%B6%D0%B5%D0%BD%D0%B8%D1%8F)
- [Примеры](https://github.com/anyks/asc/tree/master/site#%D0%BF%D1%80%D0%B8%D0%BC%D0%B5%D1%80%D1%8B)
    - [Информация о бинарном словаре](https://github.com/anyks/asc/tree/master/site#%D0%B8%D0%BD%D1%84%D0%BE%D1%80%D0%BC%D0%B0%D1%86%D0%B8%D1%8F-%D0%BE-%D0%B1%D0%B8%D0%BD%D0%B0%D1%80%D0%BD%D0%BE%D0%BC-%D1%81%D0%BB%D0%BE%D0%B2%D0%B0%D1%80%D0%B5)
    - [Обучение](https://github.com/anyks/asc/tree/master/site#%D0%BE%D0%B1%D1%83%D1%87%D0%B5%D0%BD%D0%B8%D0%B5)
    - [Исправление ошибок](https://github.com/anyks/asc/tree/master/site#%D0%B8%D1%81%D0%BF%D1%80%D0%B0%D0%B2%D0%BB%D0%B5%D0%BD%D0%B8%D0%B5-%D0%BE%D1%88%D0%B8%D0%B1%D0%BE%D0%BA)
- [Лицензия](https://github.com/anyks/asc/tree/master/site#%D0%BB%D0%B8%D1%86%D0%B5%D0%BD%D0%B7%D0%B8%D1%8F)
- [Контакты](https://github.com/anyks/asc/tree/master/site#%D0%BA%D0%BE%D0%BD%D1%82%D0%B0%D0%BA%D1%82%D1%8B)

## Описание проекта

Систем для исправления ошибок и опечаток в тексте, существует большое количество. У каждой такой системы есть свои плюсы и минусы, каждая система имеет право на жизнь и найдет своего пользователя. Я хочу представить, ещё одну версию системы исправления опечаток, то - как я вижу такую систему и какой она должна быть.

## Список возможностей

- Исправление ошибок в словах с разницей до 4-х дистанций по **Левенштейну**.
- Исправление опечаток в словах (вставка, удаление, замещение, перестановка) символов.
- **Ё**фикация с учётом контекста.
- Простановка регистра первой буквы слова, для (имён собственных и названий) с учётом контекста.
- Разбиение объединённых слов на отдельные слова, с учётом контекста.
- Выполнение анализа текста без корректировки исходного текста.
- Поиск в тексте наличия (ошибок, опечаток, неверного контекста).

## Зависимости

- [Zlib](http://www.zlib.net)
- [Bloom](http://www.partow.net/programming/bloomfilter/index.html)
- [OpenSSL](https://www.openssl.org)
- [hnswlib](https://github.com/nmslib/hnswlib)
- [HandyPack](https://github.com/bakwc/HandyPack)
- [GperfTools](https://github.com/gperftools/gperftools)
- [Python3](https://www.python.org/download/releases/3.0)
- [NLohmann::json](https://github.com/nlohmann/json)
- [BigInteger](http://mattmccutchen.net/bigint)
- [ALM](https://github.com/anyks/alm)

## Сборка проекта

### Python версия ASC
```bash
$ python3 -m pip install pybind11
$ python3 -m pip install anyks-sc
```

[Документация pip](https://pypi.org/project/anyks-sc)

### Клонирование проекта

```bash
$ git clone --recursive https://github.com/anyks/asc.git
```

### Linux и FreeBSD

```bash
$ mkdir ./build
$ cd ./build

$ cmake ..
$ make
```

### MacOS X

```bash
$ mkdir ./build
$ cd ./build

$ cmake -DOPENSSL_ROOT_DIR=/usr/local/Cellar/openssl@1.1/1.1.1g -DOPENSSL_LIBRARIES=/usr/local/Cellar/openssl@1.1/1.1.1g/include/lib ..
$ make
```

## Готовые словари

| Название                                                               | Размер (Гб) | Оперативная память (Гб) | Размер N-грамм | Язык  |
|------------------------------------------------------------------------|-------------|-------------------------|----------------|-------|
| [wittenbell-3-big.asc](https://cloud.mail.ru/public/2b9E/sz9b8YGJ1)    | 1.97        | 15.6                    | 3              | RU    |
| [wittenbell-3-middle.asc](https://cloud.mail.ru/public/5zo5/2F6uav8fz) | 1.24        | 9.7                     | 3              | RU    |
| [mkneserney-3-middle.asc](https://cloud.mail.ru/public/3SBP/35WJFKFyd) | 1.33        | 9.7                     | 3              | RU    |
| [wittenbell-3-single.asc](https://cloud.mail.ru/public/21jt/YEW493XQa) | 0.772       | 5.14                    | 3              | RU    |
| [wittenbell-5-single.asc](https://cloud.mail.ru/public/5Adc/5x5Ea1eA6) | 1.37        | 10.7                    | 5              | RU    |

## Тестирование

Для проверки работы системы, использовались данные соревнования [«исправления опечаток» 2016 года от Dialog21](http://www.dialog-21.ru/evaluation/2016/spelling_correction).
Для тестирования использовался обученный бинарный словарь: [wittenbell-3-middle.asc](https://cloud.mail.ru/public/5zo5/2F6uav8fz)

| №                              | Precision | Recall | FMeasure |
|--------------------------------|-----------|--------|----------|
| **Режим исправления опечаток** | 76.97     | 62.71  | 69.11    |
| **Режим исправления ошибок**   | 73.72     | 60.53  | 66.48    |

Думаю, излишне добавлять другие данные, при желании каждый может повторить тест, все используемые материалы в тестировании прикладываю ниже.

### Материалы использовавшиеся в тестировании
- [test.txt](https://cloud.mail.ru/public/3rzx/2HwqDU9j5) - Текст для тестирования
- [correct.txt](https://cloud.mail.ru/public/3dAN/J4EzV4U3W) - Текст корректных вариантов
- [evaluate.py](https://cloud.mail.ru/public/ZTRf/4sUS1Sc2p) - Скрипт Python3 для расчёта результатов коррекции

---

## Форматы файлов

### Arpa
```
\data\
ngram 1=52
ngram 2=68
ngram 3=15

\1-grams:
-1.807052	1-й	-0.30103
-1.807052	2	-0.30103
-1.807052	3~4	-0.30103
-2.332414	как	-0.394770
-3.185530	после	-0.311249
-3.055896	того	-0.441649
-1.150508	</s>
-99	<s>	-0.3309932
-2.112406	<unk>
-1.807052	T358	-0.30103
-1.807052	VII	-0.30103
-1.503878	Грека	-0.39794
-1.807052	Греку	-0.30103
-1.62953	Ехал	-0.30103
...

\2-grams:
-0.29431	1-й передал
-0.29431	2 ложки
-0.29431	3~4 дня
-0.8407791	<s> Ехал
-1.328447	после того	-0.477121
...

\3-grams:
-0.09521468	рак на руке
-0.166590	после того как
...

\end\
```

| N-грамма              | Встречаемость в корпусе      | Встречаемость в документах |
|-----------------------|------------------------------|----------------------------|
| только в одном        | 2                            | 1                          |

### Vocab
```
\data\
ad=1
cw=23832
unq=9390

\words:
33	а	244 | 1 | 0.010238 | 0.000000 | -3.581616
34	б	11 | 1 | 0.000462 | 0.000000 | -6.680889
35	в	762 | 1 | 0.031974 | 0.000000 | -2.442838
40	ж	12 | 1 | 0.000504 | 0.000000 | -6.593878
330344	был	47 | 1 | 0.001972 | 0.000000 | -5.228637
335190	вам	17 | 1 | 0.000713 | 0.000000 | -6.245571
335192	дам	1 | 1 | 0.000042 | 0.000000 | -9.078785
335202	нам	22 | 1 | 0.000923 | 0.000000 | -5.987742
335206	сам	7 | 1 | 0.000294 | 0.000000 | -7.132874
335207	там	29 | 1 | 0.001217 | 0.000000 | -5.711489
2282019644	похожесть	1 | 1 | 0.000042 | 0.000000 | -9.078785
2282345502	новый	10 | 1 | 0.000420 | 0.000000 | -6.776199
2282416889	белый	2 | 1 | 0.000084 | 0.000000 | -8.385637
3009239976	гражданский	1 | 1 | 0.000042 | 0.000000 | -9.078785
3009763109	банкиры	1 | 1 | 0.000042 | 0.000000 | -9.078785
3013240091	геныч	1 | 1 | 0.000042 | 0.000000 | -9.078785
3014009989	преступлениях	1 | 1 | 0.000042 | 0.000000 | -9.078785
3015727462	тысяч	2 | 1 | 0.000084 | 0.000000 | -8.385637
3025113549	позаботьтесь	1 | 1 | 0.000042 | 0.000000 | -9.078785
3049820849	комментарием	1 | 1 | 0.000042 | 0.000000 | -9.078785
3061388599	компьютерная	1 | 1 | 0.000042 | 0.000000 | -9.078785
3063804798	шаблонов	1 | 1 | 0.000042 | 0.000000 | -9.078785
3071212736	завидной	1 | 1 | 0.000042 | 0.000000 | -9.078785
3074971025	холодной	1 | 1 | 0.000042 | 0.000000 | -9.078785
3075044360	выходной	1 | 1 | 0.000042 | 0.000000 | -9.078785
3123271427	делаешь	1 | 1 | 0.000042 | 0.000000 | -9.078785
3123322362	читаешь	1 | 1 | 0.000042 | 0.000000 | -9.078785
3126399411	готовится	1 | 1 | 0.000042 | 0.000000 | -9.078785
...
```

| Идентификатор слова   | Слово     | Встречаемость в корпусе    | Встречаемость в документах | tf       | tf-idf   | wltf      |
|-----------------------|-----------|----------------------------|----------------------------|----------|----------|-----------|
| 2282345502            | новый     | 10                         | 1                          | 0.000420 | 0.000000 | -6.776199 |

#### Описание:

- **ad** - Общее количество документов в обучающем корпусе
- **cw** - Общее количество слов в обучающем корпусе
- **oc** - Встречаемость в корпусе
- **dc** - Встречаемость в документах
- **tf** - (term frequency — частота слова) — отношение числа вхождений некоторого слова к общему числу слов документа. Таким образом, оценивается важность слова в пределах отдельного документа, расчитывается как: [tf = oc / cw]
- **idf** - (inverse document frequency — обратная частота документа) — инверсия частоты, с которой некоторое слово встречается в документах коллекции, расчитывается как: [idf = log(ad / dc)]
- **tf-idf** - Расчитывается как: [tf-idf = tf * idf]
- **wltf** - Рейтинг слова, расчитывается как: [wltf = 1 + log(tf * dc)]

### Файл похожих букв в разных словарях
```
p  р
c  с
o  о
t  т
k  к
e  е
a  а
h  н
x  х
b  в
m  м
...
```

| Искомая буква | Разделитель | Буква на замену   |
|---------------|-------------|-------------------|
| t             | \t          | т                 |

### Файл списка аббревиатур
```
г
р
США
ул
руб
рус
чел
...
```

> Все слова из данного списка будут идентифицированны как неизвестное слово **〈abbr〉**.

### Файл списка доменных зон
```
ru
su
cc
net
com
org
info
...
```

> Для более точного идентифицирования токена **〈url〉**, следует добавить свои доменные зоны (все доменные зоны в примере, уже предустановлены).

---

### Формат скрипта python предобработки полученных слов
```python
# -*- coding: utf-8 -*-

def init():
    """
    Метод инициализации: выполняется только один раз при запуске приложения
    """

def run(word, context):
    """
    Метод запуска обработки: запускается при извлечении слова из текста
    @word    обрабатываемое слово
    @context последовательность предыдущих слов в виде массива
    """
    return word
```

### Формат скрипта python определения собственных токенов слов
```python
# -*- coding: utf-8 -*-

def init():
    """
    Метод инициализации: выполняется только один раз при запуске приложения
    """

def run(token, word):
    """
    Метод запуска обработки: запускается при извлечении слова из текста
    @token название токена слова
    @word  обрабатываемое слово
    """
    if token and (token == "<usa>"):
        if word and (word.lower() == "сша"): return "ok"
    elif token and (token == "<russia>"):
        if word and (word.lower() == "россия"): return "ok"
    return "no"
```

### Пример скрипта python стемминга
```python
import spacy
import pymorphy2

# Морфологический анализатор
morphRu = None
morphEn = None

def init():
    """
    Метод инициализации: выполняется только один раз при запуске приложения
    """
    # Получаем морфологический разбор
    global morphRu
    global morphEn
    # Активируем морфологический разбор для русского языка
    morphRu = pymorphy2.MorphAnalyzer()
    # Активируем морфологический разбор для английского языка
    morphEn = spacy.load('en', disable=['parser', 'ner'])

def eng(word):
    """
    Метод лемматизации английского языка
    @word слово для получения леммы
    """
    # Получаем морфологический разбор
    global morphEn
    # Получаем результат
    words = morphEn(word)
    # Получаем результат
    word = ''.join([token.lemma_ for token in words]).strip()
    # Если слово получено верное
    if word[0] != '-' and word[len(word) - 1] != '-':
        # Выводим результат
        return word
    else:
        # Выводим пустоту
        return ""

def rus(word):
    """
    Метод лемматизации русского языка
    @word слово для получения леммы
    """
    # Получаем морфологический разбор
    global morphRu
    # Если морфологический разбор активирован
    if morphRu != None:
        # Извлекаем результат
        word = morphRu.parse(word)[0].normal_form
        # Выводим результат
        return word
    else:
        # Выводим пустоту
        return ""

def run(word, lang):
    """
    Метод запуска морфологического разбора
    @word слово для получения леммы
    @lang язык алфавита передаваемого слова
    """
    # Если это русский язык
    if lang == "ru":
        # Выводим лемму для русского языка
        return rus(word.lower())
    # Если это английский язык
    elif lang == "en":
        # Выводим лемму для английского языка
        return eng(word.lower())
```

---

### Переменные окружения

- Все параметры можно передавать через переменные окружения. Переменные начинаются с префикса **ASC_** и должны записываться в верхнем регистре, в остальном названия их соответствуют параметрам приложения.
- Если одновременно указаны и параметры приложения и переменные окружения, приоритет будут иметь параметры приложения.

```bash
$ export ASC_R-ARPA=./lm.arpa

$ export ASC_R-BIN=./wittenbell-3-single.asc
```

- Пример параметров в JSON формате

```json
{
  "debug": 1,
  "method": "spell",
  "spell-verbose": true,
  "asc-split": true,
  "asc-alter": true,
  "asc-esplit": true,
  "asc-rsplit": true,
  "asc-uppers": true,
  "asc-hyphen": true,
  "asc-wordrep": true,
  "r-text": "./texts/input.txt",
  "w-text": "./texts/output.txt",
  "r-bin": "./dict/wittenbell-3-middle.asc"
}
```

---

## Примеры

![Пример работы программы](https://raw.githubusercontent.com/anyks/asc/master/site/img/screen1.png "Пример работы программы")

### Информация о бинарном словаре
```bash
$ ./asc -method info -r-bin ./dict/wittenbell-3-middle.asc
```

---

### Обучение
```json
{
  "ext": "txt",
  "size": 3,
  "alter": {"е":"ё"},
  "debug": 1,
  "threads": 0,
  "method": "train",
  "vprune": true,
  "allow-unk": true,
  "reset-unk": true,
  "confidence": true,
  "interpolate": true,
  "mixed-dicts": true,
  "only-token-words": true,
  "kneserney-modified": true,
  "kneserney-prepares": true,
  "vprune-wltf": -15.0,
  "locale": "en_US.UTF-8",
  "smoothing": "mkneserney",
  "pilots": ["а","у","в","о","с","к","б","и","я","э","a","i","o","e","g"],
  "corpus": "./texts/corpus",
  "w-bin": "./dictionary/3-middle.asc",
  "w-abbr": "./dict/release/lm.abbr",
  "w-vocab": "./dict/release/lm.vocab",
  "w-arpa": "./dict/release/lm.arpa",
  "abbrs": "./texts/abbrs/abbrs.txt",
  "goodwords": "./texts/whitelist/words.txt",
  "badwords": "./texts/blacklist/words.txt",
  "alters": "./texts/alters/yoficator.txt",
  "upwords": "./texts/words/names/words",
  "mix-restwords": "./texts/similars/letters.txt",
  "alphabet": "абвгдеёжзийклмнопрстуфхцчшщъыьэюяabcdefghijklmnopqrstuvwxyz",
  "bin-code": "ru",
  "bin-name": "Russian",
  "bin-author": "You name",
  "bin-copyright": "You company LLC",
  "bin-contacts": "site: https://example.com, e-mail: info@example.com",
  "bin-lictype": "MIT",
  "bin-lictext": "... License text ...",
  "embedding-size": 28,
  "embedding": {
      "а": 0, "б": 1, "в": 2, "г": 3, "д": 4, "е": 5,
      "ё": 5, "ж": 6, "з": 7, "и": 8, "й": 8, "к": 9,
      "л": 10, "м": 11, "н": 12, "о": 0, "п": 13, "р": 14,
      "с": 15, "т": 16, "у": 17, "ф": 18, "х": 19, "ц": 20,
      "ч": 21, "ш": 21, "щ": 21, "ъ": 22, "ы": 23, "ь": 22,
      "э": 5, "ю": 24, "я": 25, "<": 26, ">": 26, "~": 26,
      "-": 26, "+": 26, "=": 26, "*": 26, "/": 26, ":": 26,
      "%": 26, "|": 26, "^": 26, "&": 26, "#": 26, "'": 26,
      "\\": 26, "0": 27, "1": 27, "2": 27, "3": 27, "4": 27,
      "5": 27, "6": 27, "7": 27, "8": 27, "9": 27, "a": 0,
      "b": 2, "c": 15, "d": 4, "e": 5, "f": 18, "g": 3,
      "h": 12, "i": 8, "j": 6, "k": 9, "l": 10, "m": 11,
      "n": 12, "o": 0, "p": 14, "q": 13, "r": 14, "s": 15,
      "t": 16, "u": 24, "v": 21, "w": 22, "x": 19, "y": 17, "z": 7
  }
}
```

```bash
$ ./asc -r-json ./train.json
```
---

### Исправление ошибок

**Чтение из файла текста -> исправление -> запись в новый файл исправленный текст**
```bash
$ ./asc -debug 1 -method spell -spell-verbose -asc-split -asc-alter -asc-esplit -asc-rsplit -asc-uppers -asc-hyphen -asc-wordrep -r-text ./texts/input.txt -w-text ./texts/output.txt -r-bin ./dict/wittenbell-3-middle.asc
```

**Чтение из потока -> исправление -> вывод в поток**
```bash
$ echo "слзы теут на мрозе" | ./asc -debug 1 -method spell -spell-verbose -asc-split -asc-alter -asc-esplit -asc-rsplit -asc-uppers -asc-hyphen -asc-wordrep -r-bin ./dict/wittenbell-3-middle.asc
```

**Работа в интерактивном режиме**
```bash
$ ./asc -debug 1 -method spell -spell-verbose -asc-split -asc-alter -asc-esplit -asc-rsplit -asc-uppers -asc-hyphen -asc-wordrep -interactive -r-bin ./dict/wittenbell-3-middle.asc
```

**Тоже работа с файлами, но через шаблон в формате JSON**
```json
{
  "debug": 1,
  "method": "spell",
  "spell-verbose": true,
  "asc-split": true,
  "asc-alter": true,
  "asc-esplit": true,
  "asc-rsplit": true,
  "asc-uppers": true,
  "asc-hyphen": true,
  "asc-wordrep": true,
  "r-text": "./texts/input.txt",
  "w-text": "./texts/output.txt",
  "r-bin": "./dict/wittenbell-3-middle.asc"
}
```

```bash
$ ./asc -r-json ./spell.json
```

* * *

## Лицензия

![MIT License](http://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png "MIT License")

Приложение лицензируется под [MIT License](http://opensource.org/licenses/MIT):

Авторские права © 2020 [Лобарев Юрий](https://anyks.com)

Данная лицензия разрешает лицам, получившим копию данного программного обеспечения и сопутствующей документации (в дальнейшем именуемыми «Программное Обеспечение»), безвозмездно использовать Программное Обеспечение без ограничений, включая неограниченное право на использование, копирование, изменение, слияние, публикацию, распространение, сублицензирование и/или продажу копий Программного Обеспечения, а также лицам, которым предоставляется данное Программное Обеспечение, при соблюдении следующих условий:

Указанное выше уведомление об авторском праве и данные условия должны быть включены во все копии или значимые части данного Программного Обеспечения.

ДАННОЕ ПРОГРАММНОЕ ОБЕСПЕЧЕНИЕ ПРЕДОСТАВЛЯЕТСЯ «КАК ЕСТЬ», БЕЗ КАКИХ-ЛИБО ГАРАНТИЙ, ЯВНО ВЫРАЖЕННЫХ ИЛИ ПОДРАЗУМЕВАЕМЫХ, ВКЛЮЧАЯ ГАРАНТИИ ТОВАРНОЙ ПРИГОДНОСТИ, СООТВЕТСТВИЯ ПО ЕГО КОНКРЕТНОМУ НАЗНАЧЕНИЮ И ОТСУТСТВИЯ НАРУШЕНИЙ, НО НЕ ОГРАНИЧИВАЯСЬ ИМИ. НИ В КАКОМ СЛУЧАЕ АВТОРЫ ИЛИ ПРАВООБЛАДАТЕЛИ НЕ НЕСУТ ОТВЕТСТВЕННОСТИ ПО КАКИМ-ЛИБО ИСКАМ, ЗА УЩЕРБ ИЛИ ПО ИНЫМ ТРЕБОВАНИЯМ, В ТОМ ЧИСЛЕ, ПРИ ДЕЙСТВИИ КОНТРАКТА, ДЕЛИКТЕ ИЛИ ИНОЙ СИТУАЦИИ, ВОЗНИКШИМ ИЗ-ЗА ИСПОЛЬЗОВАНИЯ ПРОГРАММНОГО ОБЕСПЕЧЕНИЯ ИЛИ ИНЫХ ДЕЙСТВИЙ С ПРОГРАММНЫМ ОБЕСПЕЧЕНИЕМ.

* * *

## Контакты

Если у вас есть вопросы относительно приложения, я бы хотел пригласить вас [на GitHub](https://github.com/anyks/asc/issues/new/choose). Пожалуйста, опишите ваш запрос, проблему или вопрос как можно более подробно, а также укажите версию библиотеки, которую вы используете, а также версию вашего компилятора и операционной системы. Открытие проблемы на GitHub позволяет другим пользователям и участникам этой библиотеки сотрудничать.

---

[Лобарев Юрий](https://anyks.com) <forman@anyks.com>
