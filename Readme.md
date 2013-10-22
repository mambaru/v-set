# Установка и настройка

## Зависимости

### faslib-0.2.1
http://faslib.com
Краткая инструкция по установке faslib:

```bash
git clone git@github.com:migashko/faslib.git
cd faslib
git checkout release-0.2.1
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
```

### Компиляция

Компиляция не требуется, достаточно:

```bash
git clone git@gitlab.lan:cpp/v-set.git
```

Для компиляции тестов и примеров:

```bash
cd v-set
mkdir build
cd build
cmake -DFASLIB_DIR=path_to_faslib -DCMAKE_BUILD_TYPE=Release ..
make -j4
cd libs/tests
ctest
```

# Введение

Пакет vset содержит следующие классы:
* vset::multiset
* vset::memory::manager
* vset::buffer::buffer, vset::buffer::persistent_buffer


## vset::multiset

Структура данных, представляет собой дерево сортированных массивов фиксированного размера, по структуре и характеристикам близка к B+дереву.

Свойства:
* Поддержка итераторов произвольного доступа:
** После вставки/удаление итераторы теряют валидность
** Быстрое вычисление std::distance(): O(log 4N/K)
* Меньшей оверхед по памяти для небольших значений ключей sizeof(key)*4K + M
* Возможность быстрого дампа на диск или прямого отображения в пямять

Область применения:
* Требуется персистентность
* Построение множества индексов (ссылка)
* Нужно быстрое вычисление расстояние между узлами

Доступные аллокаторы :
* std::allocator аналог vset::allocator<512>
* vset::allocator<int K>, где K степень дерева. Самый быстрый вариант, но без персистентности.
* vset::buffer_allocator<int K> данные размещаются в непрерывном буфере, который можно быстро сдампить на диск
* vset::filesync_allocator<int K>, то же что buffer_allocator, с предостовлением функционала сохранения и фостановления из файла
* vset::mmap_allocator<int K> данные отображаются в файла


```bash
--------------------------------------------------
|              |  c++03  |  c++11  |             |
|--------------|---------|---------|-------------|
| ctor         |    +    |    +    |             |
| copy ctor    |    -    |    -    |             |
| move ctor    |         |    -    |             |
| op =         |    -    |    -    |             |
| key_comp     |    +    |    +    |             |
| value_comp   |    +    |    +    |             |
| get_allocator|    +    |    +    |             |
| begin        |    +    |    +    |             |
| end          |    +    |    +    |             |
| rbegin       |    +    |    +    |             |
| rend         |    +    |    +    |             |
| cbegin       |         |    +    |             |
| cend         |         |    +    |             |
| crbegin      |         |    +    |             |
| crend        |         |    +    |             |
| empty        |    +    |    +    |             |
| size         |    +    |    +    |             |
| max_size     |    +    |    +    |             |
| swap         |    -    |    -    |             |
| insert       |    +/-  |    +/-  |             |
| erase        |    +    |    +    |             |
| count        |    -    |    -    |             |
| find         |    -    |    +    |             |
| lower_bound  |    +    |    +    |             |
| upper_bound  |    +    |    +    |             |
| equal_range  |    +    |    +    |             |
| ==           |    -    |    -    |             |
| <            |    -    |    -    |             |
| <=           |    -    |    -    |             |
| !=           |    -    |    -    |             |
| >            |    -    |    -    |             |
| >=           |    -    |    -    |             |
--------------------------------------------------
```

get_container
capacity


## vset::memory::manager

Менеджер памяти используеться vset::multiset с аллокаторами buffer_allocator, filesync_allocator и mmap_allocator.

На данный момент реализована толко одна стратегия размещения объектов - fixed size block allocation.
Поддерживает три стратегии работы с буффером:

```bash
-----------------------------------------------------------------------------------
| fsb_mmap    |                                 | mmap_allocator                  |
| fsb_filesync|                                 | filesync_allocator              |
| fsb_inmem   |                                 | buffer_allocator                |
-----------------------------------------------------------------------------------
```

```bash
--------------------------------------------------
|              |                                 |
|--------------|---------------------------------|
| ctor         |                                 |
| begin        |                                 |
| end          |                                 |
| allocate     |                                 |
| deallocate   |                                 |
| count        |                                 |
| capacity     |                                 |
| buffer       |                                 |
--------------------------------------------------
```

## vset::buffer::buffer, vset::buffer::persistent_buffer

```bash
---------------------------------------------------
| mmap    |                                       |
| filesync|                                       |
| inmem   |                                       |
---------------------------------------------------
```

```bash
--------------------------------------------------
|              |                                 |
|--------------|---------------------------------|
| size         |                                 |
| capacity     |                                 |
| data         |                                 |
| clear        |                                 |
| resize       |                                 |
| truncate     |                                 |
| reserve      |                                 |
--------------------------------------------------
| close        |                                 |
| open         |                                 |
| sync         |                                 |
| sync         |                                 |
--------------------------------------------------
```

# Стратегии построения индексов


# Персистентый multiset


# Конструктор компараторов


# Менеджер памяти


