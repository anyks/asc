#!/usr/bin/env python3

# -*- coding: utf-8 -*-

import spacy
import pymorphy2
# from pymystem3 import Mystem

# Морфологический анализатор
morphRu = None
morphEn = None

# Метод инициализации
def init():
    # Получаем морфологический разбор
    global morphRu
    global morphEn
    # Активируем морфологический разбор для русского языка
    morphRu = pymorphy2.MorphAnalyzer()
    # morphRu = Mystem()
    # Активируем морфологический разбор для английского языка
    morphEn = spacy.load('en', disable=['parser', 'ner'])

# Метод лемматизации английского языка
def eng(word):
    # Получаем морфологический разбор
    global morphEn
    # Получаем результат
    words = morphEn(word)
    # Получаем результат
    word = ''.join([token.lemma_ for token in words]).strip()
    # Если слово получено верное
    if word[0] != '-' and word[len(word) - 1] != '-':
        # Запоминаем результат в файле
        # with open("./eng.log", "a") as file:
        #    # Выполняем запись леммы в файл
        #    file.write("%s\r\n" % word)
        # Выводим результат
        return word
    else:
        # Выводим пустоту
        return ""

# Метод лемматизации русского языка
def rus(word):
    # Получаем морфологический разбор
    global morphRu
    # Если морфологический разбор активирован
    if morphRu != None:
        # Получаем результат
        # lemmas = morphRu.lemmatize(word.lower())
        # Извлекаем результат
        # word = ''.join(lemmas).strip()
        word = morphRu.parse(word)[0].normal_form
        # Запоминаем результат в файле
        # with open("./rus.log", "a") as file:
        #    # Выполняем запись леммы в файл
        #    file.write("%s\r\n" % word)
        # Выводим результат
        return word
    else:
        # Выводим пустоту
        return ""

# Метод запуска морфологического разбора
def run(word, lang):
    # Если это русский язык
    if lang == "ru":
        # Выводим лемму для русского языка
        return rus(word.lower())
    # Если это английский язык
    elif lang == "en":
        # Выводим лемму для английского языка
        return eng(word.lower())

# Тест работы скрипта
if __name__ == "__main__":
    # Выполняем инициализацию
    init()
    # Выводим тестовый запуск для русского языка
    print(run("Лошади", "ru"))
    # Выводим тестовый запуск для английского языка
    print(run("Horses", "en"))
