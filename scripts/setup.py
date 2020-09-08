#!/usr/bin/env python3

# -*- coding: utf-8 -*-

import os
import sys
import subprocess

# Тест работы скрипта
if __name__ == "__main__":
    # import nltk
    # nltk.download('wordnet')

    # Список модулей для установки
    modules = [
        "pip install nltk",
        "pip install spacy",
        "spacy download en",
        "pip install pymorphy2",
        "pip install -U pymorphy2-dicts-ru",
        "pip install pymystem3"
    ]

    # Переходим по всему списку модулей
    for module in modules:
        # Изменяем модуль
        module = "%s -m %s" % (sys.executable, module)

        # Выполняем установку приложения
        p = subprocess.Popen(module.split(" "))
        try:
            # Ожидаем выполнение процесса
            p.wait(timeout=3000)
        # Если произошел таймаут
        except subprocess.TimeoutExpired:
            # Останавливаем процесс
            p.terminate()
            # Выводим сообщение о таймауте
            print("Stop install module: [%s]" % module)
