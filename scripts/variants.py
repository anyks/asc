#!/usr/bin/env python3

# -*- coding: utf-8 -*-

#
#   автор:              Юрий Николаевич Лобарев
#   icq:                222-976-291
#   телефон:            +7(920)672-33-22
#   авторские права:    Все права принадлежат автору © Юрий Лобарев, 2014
#

import pymorphy2
import json
import sys

# Морфологический анализатор
morphRu = None

# Функция парсинга данных
def parserMorph(p):
    # Определяем часть речи
    POST = p.tag.POS            # Part of Speech, часть речи
    ANim = p.tag.animacy        # одушевленность
    GNdr = p.tag.gender         # род (мужской, женский, средний)
    NMbr = p.tag.number         # число (единственное, множественное)
    CAse = p.tag.case           # падеж
    ASpc = p.tag.aspect         # вид: совершенный или несовершенный
    INvl = p.tag.involvement    # включенность говорящего в действие
    MOod = p.tag.mood           # наклонение (повелительное, изъявительное)
    PErs = p.tag.person         # лицо (1, 2, 3)
    TEns = p.tag.tense          # время (настоящее, прошедшее, будущее)
    TRns = p.tag.transitivity   # переходность (переходный, непереходный)
    VOic = p.tag.voice          # залог (действительный, страдательный)
    # общий род
    if ('Ms-f' in p.tag):
        Msf = True
    else:
        Msf = False
    # singularia tantum
    if ('Sgtm' in p.tag):
        Sgtm = True
    else:
        Sgtm = False
    # pluralia tantum
    if ('Pltm' in p.tag):
        Pltm = True
    else:
        Pltm = False
    # неизменяемое
    if ('Fixd' in p.tag):
        Fixd = True
    else:
        Fixd = False
    # аббревиатура
    if ('Abbr' in p.tag):
        Abbr = True
    else:
        Abbr = False
    # имя
    if ('Name' in p.tag):
        Name = True
    else:
        Name = False
    # имя
    if ('Surn' in p.tag):
        Surn = True
    else:
        Surn = False
    # отчество
    if ('Patr' in p.tag):
        Patr = True
    else:
        Patr = False
    # топоним
    if ('Geox' in p.tag):
        Geox = True
    else:
        Geox = False
    # организация
    if ('Orgn' in p.tag):
        Orgn = True
    else:
        Orgn = False
    # торговая марка
    if ('Trad' in p.tag):
        Trad = True
    else:
        Trad = False
    # возможна субстантивация
    if ('Subx' in p.tag):
        Subx = True
    else:
        Subx = False
    # превосходная степень
    if ('Supr' in p.tag):
        Supr = True
    else:
        Supr = False
    # качественное
    if ('Qual' in p.tag):
        Qual = True
    else:
        Qual = False
    # местоименное
    if ('Apro' in p.tag):
        Apro = True
    else:
        Apro = False
    # порядковое
    if ('Anum' in p.tag):
        Anum = True
    else:
        Anum = False
    # притяжательное
    if ('Poss' in p.tag):
        Poss = True
    else:
        Poss = False
    # притяжательное
    if ('V-ey' in p.tag):
        Vey = True
    else:
        Vey = False
    # форма на -ою
    if ('V-oy' in p.tag):
        Voy = True
    else:
        Voy = False
    # сравнительная степень на по-
    if ('Cmp2' in p.tag):
        Cmp2 = True
    else:
        Cmp2 = False
    # форма компаратива на -ей
    if ('V-ej' in p.tag):
        Vej = True
    else:
        Vej = False
    # безличный
    if ('Impe' in p.tag):
        Impe = True
    else:
        Impe = False
    # анафорическое (местоимение)
    if ('Anph' in p.tag):
        Anph = True
    else:
        Anph = False
    # инициал
    if ('Init' in p.tag):
        Init = True
    else:
        Init = False
    # инициал
    if ('Adjx' in p.tag):
        Adjx = True
    else:
        Adjx = False
    # многократный
    if ('Mult' in p.tag):
        Mult = True
    else:
        Mult = False
    # возвратный
    if ('Refl' in p.tag):
        Refl = True
    else:
        Refl = False
    # разговорное
    if ('Infr' in p.tag):
        Infr = True
    else:
        Infr = False
    # жаргонное
    if ('Slng' in p.tag):
        Slng = True
    else:
        Slng = False
    # устаревшее
    if ('Arch' in p.tag):
        Arch = True
    else:
        Arch = False
    # литературный вариант
    if ('Litr' in p.tag):
        Litr = True
    else:
        Litr = False
    # опечатка
    if ('Erro' in p.tag):
        Erro = True
    else:
        Erro = False
    # искажение
    if ('Dist' in p.tag):
        Dist = True
    else:
        Dist = False
    # вопросительное
    if ('Ques' in p.tag):
        Ques = True
    else:
        Ques = False
    # указательное
    if ('Dmns' in p.tag):
        Dmns = True
    else:
        Dmns = False
    # вводное слово
    if ('Prnt' in p.tag):
        Prnt = True
    else:
        Prnt = False
    # форма на -ье
    if ('V-be' in p.tag):
        Vbe = True
    else:
        Vbe = False
    # форма на -енен
    if ('V-en' in p.tag):
        Ven = True
    else:
        Ven = False
    # отчество через -ие-
    if ('V-ie' in p.tag):
        Vie = True
    else:
        Vie = False
    # форма на -ьи
    if ('V-bi' in p.tag):
        Vbi = True
    else:
        Vbi = False
    # деепричастие от глагола несовершенного вида
    if ('Fimp' in p.tag):
        Fimp = True
    else:
        Fimp = False
    # может выступать в роли предикатива
    if ('Prdx' in p.tag):
        Prdx = True
    else:
        Prdx = False
    # счётная форма
    if ('Coun' in p.tag):
        Coun = True
    else:
        Coun = False
    # собирательное числительное
    if ('Coll' in p.tag):
        Coll = True
    else:
        Coll = False
    # деепричастие на -ши
    if ('V-sh' in p.tag):
        Vsh = True
    else:
        Vsh = False
    # форма после предлога
    if ('Af-p' in p.tag):
        Afp = True
    else:
        Afp = False
    # может использоваться как одуш. / неодуш.
    if ('Inmx' in p.tag):
        Inmx = True
    else:
        Inmx = False
    # Вариант предлога ( со, подо, ...)
    if ('Vpre' in p.tag):
        Vpre = True
    else:
        Vpre = False
    # Помещаем данные в массив
    return {
        "Normal": p.normal_form,
        "WORD": p.word,
        "POST": POST,
        "ANim": ANim,
        "GNdr": GNdr,
        "NMbr": NMbr,
        "CAse": CAse,
        "ASpc": ASpc,
        "INvl": INvl,
        "MOod": MOod,
        "PErs": PErs,
        "TEns": TEns,
        "TRns": TRns,
        "VOic": VOic,
        "Msf": Msf,
        "Sgtm": Sgtm,
        "Pltm": Pltm,
        "Fixd": Fixd,
        "Abbr": Abbr,
        "Name": Name,
        "Surn": Surn,
        "Patr": Patr,
        "Geox": Geox,
        "Orgn": Orgn,
        "Trad": Trad,
        "Subx": Subx,
        "Supr": Supr,
        "Qual": Qual,
        "Apro": Apro,
        "Anum": Anum,
        "Poss": Poss,
        "Vey": Vey,
        "Voy": Voy,
        "Cmp2": Cmp2,
        "Vej": Vej,
        "Impe": Impe,
        "Anph": Anph,
        "Mult": Mult,
        "Refl": Refl,
        "Infr": Infr,
        "Slng": Slng,
        "Arch": Arch,
        "Litr": Litr,
        "Erro": Erro,
        "Dist": Dist,
        "Ques": Ques,
        "Dmns": Dmns,
        "Prnt": Prnt,
        "Init": Init,
        "Adjx": Adjx,
        "Vbe": Vbe,
        "Ven": Ven,
        "Vie": Vie,
        "Vbi": Vbi,
        "Fimp": Fimp,
        "Prdx": Prdx,
        "Coun": Coun,
        "Coll": Coll,
        "Vsh": Vsh,
        "Afp": Afp,
        "Inmx": Inmx,
        "Vpre": Vpre
    }

# Функция обрезки данных
def Morph(p):
    # Парсим данные
    d = parserMorph(p)
    m = {}
    # Очищаем все пустые данные
    for k in d:
        if d[k]:
            m[k] = d[k]
    return m

# Метод лемматизации русского языка
def rus(word):
    # Получаем морфологический разбор
    global morphRu
    # Формируем массив данных
    words = []
    # Формируем результат
    result = []
    # Если морфологический разбор активирован
    if morphRu != None:
        # Переводим слово к нижнему регистру
        word = word.lower()
        # Переходим по всему массиву данных
        for i in reversed(range(0, len(morphRu.parse(word)))):
            p = morphRu.parse(word)[i]
            # Записываем результирующие данные
            d = Morph(p)
            k = []
            # Получаем лексему слова
            for j in reversed(range(0, len(p.lexeme))):
                m = Morph(p.lexeme[j])
                k.append(m)

            d['lexeme'] = k
            # Добавляем в массив полный список
            words.append(d)

        # Если варианты набраны
        if len(words) > 0:
            # Переходим по всему списку
            for i in range(len(words)):
                # Переходим по всему списку полученных вариантов
                for val in words[i]['lexeme']:
                    # Формируем результат
                    result.append(val['WORD'])

        # Выводим результат
        return '|'.join(list(set(result)))
    else:
        # Выводим пустоту
        return ""

# Метод инициализации
def init():
    # Получаем морфологический разбор
    global morphRu
    # Активируем морфологический разбор для русского языка
    morphRu = pymorphy2.MorphAnalyzer()

# Метод запуска морфологического разбора
def run(word, lang):
    # Если это русский язык
    if lang == "ru":
        # Выводим лемму для русского языка
        return rus(word.lower())
    else:
        # Сообщаем что ничего не обработано
        return ""

# Тест работы скрипта
if __name__ == "__main__":
    # Выполняем инициализацию
    init()
    # Выводим тестовый запуск для русского языка
    print(run("Лошади", "ru"))
