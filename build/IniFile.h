#pragma once

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <memory>

#include <string>
#include <vector>
#include <map>


using KeysMap = std::map<std::string, std::string>;
using SectionsMap = std::map<std::string, KeysMap>;

const std::vector<std::string> trueValues = { "true", "yes", "y", "1" };

/// Класс для работы с файлами конфигурации формата .ini
class IniFile 
{
public:
    /// Конструктор
    /// @param fileName Путь до файла конфигурации
    explicit IniFile(const std::string& path);
    /// Деструктор
    /// @note Должен перезаписывать файл кешированой конифгурацией
    ~IniFile();
 
    /// Конструктор копирования
    /// @note При сохранении вносит изменения не в файл, а 
    //IniFile(const IniFile& other);

    /// Запись кешированной конфигурации в файл
    void save();
    void save_as(const std::string& filename);

    /// Чтение значения типа int
    /// @param section Секция ini-файла 
    /// @param key Ключ в секции
    /// @param def Значение по умолчанию, возвращаемое в случае отсутсвия ключа/секции
    int readInt(const std::string& section, const std::string& key, int def = 0);
    /// Чтение значения типа double 
    double readDouble(const std::string& section, const std::string& key, double def = 0);
    /// Чтение значения типа std::string 
    std::string readString(const std::string& section, const std::string& key, const std::string& def = "");
    /// Чтение значения типа bool 
    /// @note При чтении строка должна сопоставляться со всеми значениям из trueValues, независимо от регистра
    /// @note В остальные случаях - вовзращается false
    bool readBool(const std::string& section, const std::string& key, bool def = false);

    /// Запись значения типа int
    void writeInt(const std::string& section, const std::string& key, int value);
    /// Запись значения типа double
    void writeDouble(const std::string& section, const std::string& key, double value);
    /// Запись значения типа std::string
    void writeString(const std::string& section, const std::string& key, const std::string& value);
    /// Запись значения типа bool 
    void writeBool(const std::string& section, const std::string& key, bool value);

    /// Проверка существования секции 
    bool isSectionExist(const std::string& section);
    /// Проверка существования ключа в секции 
    bool isKeysExist(const std::string& section, const std::string& key);

    /// Получение количества секций 
    size_t getSectionsCount();
    /// Получение количества ключей в секции
    size_t getKeysCount(const std::string& section);

    /// Получение значений всех секций
    SectionsMap getSections() const;
    KeysMap getKeys(const std::string &section);

    /// Удаление секции
    bool deleteSection(const std::string& section);
    /// Удаление ключа из секции 
    bool deleteKey(const std::string& section, const std::string& key);

    /// @brief Clears all the keys and sections
    void clear();

    /// Добавление новой секции
    bool addNewSection(const std::string& section);

    /// @brief Merges two IniFile's into one
    /// @param primary More privilaged IniFile
    /// @param secondary Less privilaged IniFile
    /// @param filePath - Filepath for the new IniFile (may be already existing file, excluding ones merging from)
    /// @note When key collision, value from file is taken. If there is no value in file, value from the primary IniFile is taken.
    /// @return New IniFile merged
    static IniFile merge(const IniFile& primary, const IniFile& secondary, const std::string& filePath);

    std::string get_path() const;
private:
    const std::string filePath;     ///< Путь до файла конфигурации
    SectionsMap data;               ///< Значения
};

/*
Задача: реализовать класс для работы с файлами конфигурации формата .ini по предложенному интерфейсу
Файл имеет следующий формат:

[Window]
fullscreen=true
width=1920
height=1080
[Graphics]
; настройки графики
quality=high ; качество графики
filtering=x16

Файл может содержать:
    - комментарии - от символа «;» до конца строки;
    - секции - строки, содержащие название секции в квадратных скобках;
    - значения - строки вида «ключ=значение»;
    - пустые строки;
    - ошибочные строки - секции без имени, значения без ключа, ключ без значения, множественные '[', ']', '=' и т.д.

Лишние пробелы по краям ключей/значений должны отбрасываться
Ошибочные строки не должны быть обработаны и включены в конфигурацию
Содержимое одинаковых секций объединяется в одну
При наличии одинаковых ключей - берется значение последнего ключа

Пример использования:
IniFile ini("config.ini");
bool fs = ini.readBool("Window", "fullscreen", false); // true from ini
int uiScale = ini.readInt("Window", "UIScale", 1);    // 1 from default
*/
