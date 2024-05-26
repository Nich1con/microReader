/*
    Замена EEPROM для ESP8266/32 для хранения любых данных в файлах
    Документация:
    GitHub: https://github.com/GyverLibs/FileData
    Возможности:
    - Механизм автоматического "флага" первой записи
    - Поддержка всех файловых систем (LittleFS, SPIFFS, SDFS) 
    - Поддержка любых типов статических данных
    - Отложенная запись по таймауту
    - "Обновление" данных - файл не перезапишется, если данные не изменились

    AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

    Версии:
    v1.0 - релиз
*/

#ifndef _FileData_h
#define _FileData_h

#include <Arduino.h>
#include <FS.h>

// статусы
enum FDstat_t {
    FD_IDLE,      // 0 - холостая работа
    FD_WAIT,      // 1 - ожидание таймаута
    FD_FS_ERR,    // 2 - ошибка файловой системы
    FD_FILE_ERR,  // 3 - ошибка открытия файла
    FD_WRITE,     // 4 - запись данных в файл
    FD_READ,      // 5 - чтение данных из файла
    FD_ADD,       // 6 - добавление данных в файл
    FD_NO_DIF,    // 7 - данные не отличаются (не записаны)
    FD_RESET,     // 8 - произведён сброс ключа
};

class FileData {
   public:
    FileData(fs::FS* nfs = nullptr, const char* path = nullptr, uint8_t key = 'A', void* data = nullptr, uint16_t size = 0, uint16_t tout = 5000) {
        setFS(nfs, path);
        setKey(key);
        setData(data, size);
        setTimeout(tout);
    }

    // установить файловую систему
    void setFS(fs::FS* nfs, const char* path) {
        _fs = nfs;
        _path = path;
    }

    // установить ключ
    void setKey(uint8_t key) {
        _key = key;
    }

    // подключить данные (переменную)
    void setData(void* data, uint16_t size) {
        _data = data;
        _size = size;
    }

    // установить таймаут записи
    void setTimeout(uint16_t tout) {
        _tout = tout;
    }

    // прочитать файл в переменную
    // возврат: FD_FS_ERR/FD_FILE_ERR/FD_WRITE/FD_ADD/FD_READ
    FDstat_t read() {
        if (!_fs || !_data) return FD_FS_ERR;
        if (!_fs->exists(_path)) return write();
        File file = _fs->open(_path, "r+");
        if (!file) return FD_FILE_ERR;
        uint16_t size = file.size();
        uint8_t key = file.read();

        if (key == _key) {
            if (size > _size + 1) {
                file.close();
                return write();
            } else if (size < _size + 1) {
                if (_addw) {
                    file.read((uint8_t*)_data, size - 1);
                    file.write((uint8_t*)_data + size - 1, _size + 1 - size);
                    return FD_ADD;
                } else {
                    file.close();
                    return write();
                }
            } else {
                file.read((uint8_t*)_data, size - 1);
                return FD_READ;
            }
        } else {
            file.close();
            return write();
        }
    }

    // обновить сейчас
    // возврат: FD_FS_ERR/FD_FILE_ERR/FD_WRITE/FD_NO_DIF
    FDstat_t updateNow() {
        if (!_fs || !_data) return FD_FS_ERR;
        if (!_fs->exists(_path)) return FD_FILE_ERR;
        File file = _fs->open(_path, "r+");
        if (!file || file.size() <= 1) return FD_FILE_ERR;
        file.read();  // skip key
        uint16_t len = file.size() - 1;
        for (uint16_t i = 0; i < len; i++) {
            if (((uint8_t*)_data)[i] != file.read()) {
                file.close();
                return write();
            }
        }
        return FD_NO_DIF;
    }

    // отложить обновление на заданный таймаут
    void update() {
        _tmr = millis();
        _updf = true;
    }

    // тикер, обновит данные по таймауту
    // возврат: FD_FS_ERR/FD_FILE_ERR/FD_WRITE/FD_NO_DIF/FD_WAIT/FD_IDLE
    FDstat_t tick() {
        if (_updf && (uint16_t)((uint16_t)millis() - _tmr) >= _tout) {
            _updf = false;
            return updateNow();
        }
        return _updf ? FD_WAIT : FD_IDLE;
    }

    // записать данные в файл
    // возврат: FD_FS_ERR/FD_FILE_ERR/FD_WRITE
    FDstat_t write() {
        if (!_fs || !_data) return FD_FS_ERR;
        File file = _fs->open(_path, "w");
        if (!file) return FD_FILE_ERR;
        file.write(_key);
        file.write((uint8_t*)_data, _size);
        return FD_WRITE;
    }

    // сбросить ключ
    // возврат: FD_FS_ERR/FD_FILE_ERR/FD_RESET
    FDstat_t reset() {
        if (!_fs || !_data) return FD_FS_ERR;
        if (!_fs->exists(_path)) return FD_FILE_ERR;
        File file = _fs->open(_path, "r+");
        if (!file) return FD_FILE_ERR;
        uint8_t key = file.read();
        file.seek(0);
        file.write(key + 1);
        return FD_RESET;
    }

    // включить режим увеличения данных с добавлением в файл без очистки
    void addWithoutWipe(bool addw) {
        _addw = addw;
    }

   private:
    fs::FS* _fs;
    const char* _path;
    uint8_t _key;
    void* _data;
    uint16_t _size;
    uint16_t _tout;
    bool _addw = false;

    bool _updf = false;
    uint16_t _tmr = 0;
};

#endif