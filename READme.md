## Описание
Программа иммулирует работу QAM модулятора сигнала, QAM демодулятора и канала с Гауссовским шумом. Работает в системе Unix. Для Windows требуется исправить **makefile**.

## Если хотите увидеть только график зависимости вероятности ошибки на бит от отношения сигнал/шум , то

1) Перейти в терминале в папку, в которой будет храниться локальный репозиторий с необходимыми файлами. Набрать в терминале:
```bash
git init
git clone https://github.com/YasnoDelo/QAM_modulation
```
В корневой папке появится папка с локальным репозиторием. 
2) Перейдите в терминале в этот локальный репозиторий:
```bash
cd QAM_modulation
```
3) Наберите:
```bash
python3 output.py
```

## Как запустить пересчёт значений для графика?
1) Перейти в терминале в папку, в которой будет храниться локальный репозиторий с необходимыми файлами. Набрать в терминале:
```bash
git init
git clone https://github.com/YasnoDelo/QAM_modulation
```
В корневой папке появится папка с локальным репозиторием. 
2) Перейдите в терминале в этот локальный репозиторий:
```bash
cd QAM_modulation
```
3) Скомпилировать и запустить программу командой:
```bash
make
```
Далее начнётся выполнение программы и компьютерное вычисление. Необходимо подождать 5-10 минут до завершения подсчёта данных.

## Особенности
1) Если Вы хотите изменить **время выполнения** программы, то можете заменить значение в файле $/include/help.h$ на строке $39$
```C++
#define ACCUR  10000;    //Than more this number - than mor will be value on the graph
```
Оно отвечает за зладкость получаемого графика. Чем больше это число - тем больше будет **точность точек на графике**. Чем больше это значение - тем дольше будет исполняться программа.
2) Если Вы хотите изменить количесво точек на графике, то измените значение в файле $/include/help.h$ на строке $40$
```C++
#define POINTS     90;   //Amount of points on the graph
```
Это значение - количесво точек на графике

## Результат выполнения
В конце выполнения программы сгенерируется файл output.py, который сразу же запустится и выведет график **зависимости вероятности ошибки на бит от отношения сигнал/шум в QAM канале**:
![График](https://github.com/YasnoDelo/QAM_modulation/blob/main/img/Graph.png)