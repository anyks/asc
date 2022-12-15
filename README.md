[![ANYKS Smart language model](https://raw.githubusercontent.com/anyks/asc/master/site/img/banner.jpg)](https://anyks.com)

# ANYKS Spell-checker (ASC) C++11

- [Project description](https://github.com/anyks/asc#project-description)
- [List of features](https://github.com/anyks/asc#list-of-features)
- [Dependencies](https://github.com/anyks/asc#dependencies)
- [Building the project](https://github.com/anyks/asc#building-the-project)
    - [Python version ASC](https://github.com/anyks/asc#python-version-asc)
    - [Cloning the project](https://github.com/anyks/asc#cloning-the-project)
    - [Linux and FreeBSD](https://github.com/anyks/asc#linux-and-freebsd)
    - [MacOS X](https://github.com/anyks/asc#macos-x)
- [Ready-to-use dictionaries](https://github.com/anyks/asc#ready-to-use-dictionaries)
- [Testing](https://github.com/anyks/asc#testing)
    - [Files used for testing](https://github.com/anyks/asc#files-used-for-testing)
- [File formats](https://github.com/anyks/asc#file-formats)
    - [ARPA](https://github.com/anyks/asc#arpa)
    - [Vocab](https://github.com/anyks/asc#vocab)
        - [Description](https://github.com/anyks/asc#description)
    - [File containing similar letters encountered in different dictionaries](https://github.com/anyks/asc#file-containing-similar-letters-encountered-in-different-dictionaries)
    - [File containing a list of abbreviations](https://github.com/anyks/asc#file-containing-a-list-of-abbreviations)
    - [File containing a list of domain zones](https://github.com/anyks/asc#file-containing-a-list-of-domain-zones)
    - [Python word preprocessing script template](https://github.com/anyks/asc#python-word-preprocessing-script-template)
    - [Python custom word token definition script template](https://github.com/anyks/asc#python-custom-word-token-definition-script-template)
    - [Python stemming script example](https://github.com/anyks/asc#python-stemming-script-example)
    - [Environment variables](https://github.com/anyks/asc#environment-variables)
- [Examples](https://github.com/anyks/asc#examples)
    - [Information about the binary dictionary](https://github.com/anyks/asc#information-about-the-binary-dictionary)
    - [Training process](https://github.com/anyks/asc#training-process)
    - [Error correction](https://github.com/anyks/asc#error-correction)
- [License](https://github.com/anyks/asc#license)
- [Contact info](https://github.com/anyks/asc#contact-info)

## Project description

There are a lot of typo and text error correction systems out there. Each one of those systems has its pros and cons, and each system has the right to live and will find its own user base. I would like to present my own version of the typo correction system with its own unique features.

## List of features

- Correction of mistakes in words with a **Levenshtein distance** of up to 4;
- Correction of different types of typos in words: insertion, deletion, substitution, rearrangement of character;
- **Ё**-fication of a word given the context (letter 'ё' is commonly replaced by letter 'е' in russian typed text);
- Context-based word capitalization for proper names and titles;
- Context-based splitting for words that are missing the separating space character;
- Text analysis without correcting the original text;
- Searching the text for errors, typos, incorrect context.

## Dependencies

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

## Building the project

### Python version ASC
```bash
$ python3 -m pip install pybind11
$ python3 -m pip install anyks-sc
```

[Documentation pip](https://pypi.org/project/anyks-sc)

### Cloning the project

```bash
$ git clone --recursive https://github.com/anyks/asc.git
```

### Build third party
```bash
$ ./build_third_party.sh
```

### Linux/MacOS X and FreeBSD

```bash
$ mkdir ./build
$ cd ./build

$ cmake ..
$ make
```

## Ready-to-use dictionaries

| Dictionary name                                                        | Size (GB)   | RAM (GB)                | N-gram order   | Language |
|------------------------------------------------------------------------|-------------|-------------------------|----------------|----------|
| [wittenbell-3-big.asc](https://cloud.mail.ru/public/2b9E/sz9b8YGJ1)    | 1.97        | 15.6                    | 3              | RU       |
| [wittenbell-3-middle.asc](https://cloud.mail.ru/public/5zo5/2F6uav8fz) | 1.24        | 9.7                     | 3              | RU       |
| [mkneserney-3-middle.asc](https://cloud.mail.ru/public/3SBP/35WJFKFyd) | 1.33        | 9.7                     | 3              | RU       |
| [wittenbell-3-single.asc](https://cloud.mail.ru/public/21jt/YEW493XQa) | 0.772       | 5.14                    | 3              | RU       |
| [wittenbell-5-single.asc](https://cloud.mail.ru/public/5Adc/5x5Ea1eA6) | 1.37        | 10.7                    | 5              | RU       |
| [wittenbell-5-big.asc](https://cloud.mail.ru/public/ukpb/NTVQgZ9pR)    | 9.76        | 22.1                    | 5              | RU       |
| [wittenbell-3-single.asc](https://cloud.mail.ru/public/6frp/x6PoNDpay) | 2.33        | 6.15                    | 3              | EN       |

## Testing

To test the system, we used data from the [2016 "spelling correction" competition organized by Dialog21](http://www.dialog-21.ru/evaluation/2016/spelling_correction).<br>
The trained binary dictionary that was used for testing: [wittenbell-3-middle.asc](https://cloud.mail.ru/public/5zo5/2F6uav8fz)

| Mode                              | Precision | Recall | FMeasure |
|-----------------------------------|-----------|--------|----------|
| **Typo correction**               | 76.97     | 62.71  | 69.11    |
| **Error correction**              | 73.72     | 60.53  | 66.48    |

I think it is unnecessary to add any other data. Anyone can repeat the test if they wish (all files used for testing are attached below).

### Files used for testing

- [test.txt](https://cloud.mail.ru/public/3rzx/2HwqDU9j5) - Text used for testing;
- [correct.txt](https://cloud.mail.ru/public/3dAN/J4EzV4U3W) - File with correct text;
- [evaluate.py](https://cloud.mail.ru/public/ZTRf/4sUS1Sc2p) - Python3 script for correction result evaluation.

---

## File formats

### ARPA
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

| N-gramm               | Occurance in the corpus      | Occurrence in documents    |
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

| Word ID               | Word      | Occurance in the corpus    | Occurance in documents     | tf       | tf-idf   | wltf      |
|-----------------------|-----------|----------------------------|----------------------------|----------|----------|-----------|
| 2282345502            | новый     | 10                         | 1                          | 0.000420 | 0.000000 | -6.776199 |

#### Description:

- **ad** - The total number of documents in the training corpus;
- **cw** - The total number of words in the training corpus;
- **oc** - Occurance in the corpus;
- **dc** - Occurance in documents;
- **tf** - (term frequency) — the ratio of the number of occurrences of a certain word to the total number of words in the document. Thus, the importance of a word within a separate document is estimated, calculated as: [tf = oc / cw];
- **idf** - (inverse document frequency) — the inversion of the frequency with which a word occurs in the collection of documents, calculated as: [idf = log(ad / dc)]
- **tf-idf** - calculated as: [tf-idf = tf * idf]
- **wltf** - Word rating, calculated as: [wltf = 1 + log(tf * dc)]

### File containing similar letters encountered in different dictionaries
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

| Original letter | Separator   | Replacement letter   |
|-----------------|-------------|----------------------|
| t               | \t          | т                    |

### File containing a list of abbreviations
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

> All words from this list will be marked as an unknown word **〈abbr〉**.

### File containing a list of domain zones
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

> For a more accurate definition of the ** 〈url〉 ** token, we recommend adding your own domain zones (all domain zones in the example are already pre-installed).

---

### Python word preprocessing script template
```python
# -*- coding: utf-8 -*-

def init():
    """
    Initialization Method: Executed once at application startup
    """

def run(word, context):
    """
    Processing method: called during word extraction from text
    @word    proccesed word
    @context sequence of previous words in form of an array
    """
    return word
```

### Python custom word token definition script template
```python
# -*- coding: utf-8 -*-

def init():
    """
    Initialization Method: Executed once at application startup
    """

def run(token, word):
    """
    Processing method: called during word extraction from text
    @token word token name
    @word processed word
    """
    if token and (token == "<usa>"):
        if word and (word.lower() == "сша"): return "ok"
    elif token and (token == "<russia>"):
        if word and (word.lower() == "россия"): return "ok"
    return "no"
```

### Python stemming script example
```python
import spacy
import pymorphy2

# Morphological analyzer
morphRu = None
morphEn = None

def init():
    """
    Initialization Method: Executed once at application startup
    """
    # Get morphological analyzer
    global morphRu
    global morphEn
    # Activate morphological analyzer for Russian language
    morphRu = pymorphy2.MorphAnalyzer()
    # Activate morphological analyzer for English language
    morphEn = spacy.load('en', disable=['parser', 'ner'])

def eng(word):
    """
    English lemmatization method
    @word word to lemmatize
    """
    # Get morphological analyzer
    global morphEn
    # Get morphological analyzer result
    words = morphEn(word)
    # Get lemmatization result
    word = ''.join([token.lemma_ for token in words]).strip()
    # If the resulting word is a correct word
    if word[0] != '-' and word[len(word) - 1] != '-':
        # Return result
        return word
    else:
        # Return empty string
        return ""

def rus(word):
    """
    Russian lemmatization method
    @word word to lemmatize
    """
    # Get morphological analyzer
    global morphRu
    # If morphological analyzer exists
    if morphRu != None:
        # Get morphological analyzer result
        word = morphRu.parse(word)[0].normal_form
        # Return analyzer result
        return word
    else:
        # Return empty string
        return ""

def run(word, lang):
    """
    Method that runs morphological processing
    @word word to lemmatize
    @lang alphabet name for @word
    """
    # If the word is in Russian
    if lang == "ru":
        # Return russian lemmatization result
        return rus(word.lower())
    # If the word is in English
    elif lang == "en":
        # Return english lemmatization result
        return eng(word.lower())
```

---

### Environment variables

- All arguments can be passed via environment variables. Variables start with the ** ASC _ ** prefix and must be written in uppercase,  other than that, all the variable names correspond to their application arguments.
- If both application parameters and environment variables were passed, application parameters will have priority.

```bash
$ export ASC_R-ARPA=./lm.arpa

$ export ASC_R-BIN=./wittenbell-3-single.asc
```

- Example of parameters in JSON format

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

## Examples

![Example of running the program](https://raw.githubusercontent.com/anyks/asc/master/site/img/screen1.png "Example of running the program")

### Information about the binary dictionary
```bash
$ ./asc -method info -r-bin ./dict/wittenbell-3-middle.asc
```

---

### Training process
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

### Error correction

**Reading text from file -> correction -> writing corrected text to a new file**
```bash
$ ./asc -debug 1 -method spell -spell-verbose -asc-split -asc-alter -asc-esplit -asc-rsplit -asc-uppers -asc-hyphen -asc-wordrep -r-text ./texts/input.txt -w-text ./texts/output.txt -r-bin ./dict/wittenbell-3-middle.asc
```

**Reading from stream -> correction -> output to stream**
```bash
$ echo "слзы теут на мрозе" | ./asc -debug 1 -method spell -spell-verbose -asc-split -asc-alter -asc-esplit -asc-rsplit -asc-uppers -asc-hyphen -asc-wordrep -r-bin ./dict/wittenbell-3-middle.asc
```

**Running in the interactive mode**
```bash
$ ./asc -debug 1 -method spell -spell-verbose -asc-split -asc-alter -asc-esplit -asc-rsplit -asc-uppers -asc-hyphen -asc-wordrep -interactive -r-bin ./dict/wittenbell-3-middle.asc
```

**Working with files using JSON template**
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

## License

![MIT License](http://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png "MIT License")

The class is licensed under the [MIT License](http://opensource.org/licenses/MIT):

Copyright © 2020 [Yuriy Lobarev](https://anyks.com)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

* * *

## Contact Info

If you have questions regarding the library, I would like to invite you to [open an issue at GitHub](https://github.com/anyks/asc/issues/new/choose). Please describe your request, problem, or question as detailed as possible, and also mention the version of the library you are using as well as the version of your compiler and operating system. Opening an issue at GitHub allows other users and contributors to this library to collaborate.

---

[Yuriy Lobarev](https://anyks.com) <forman@anyks.com>
