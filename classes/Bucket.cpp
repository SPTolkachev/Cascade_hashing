#include "Bucket.h"
#include "Record.cpp"


/**
 * Конструктор
 */
Bucket::Bucket(int number) {
    this->number = number;
    this->firstOverflowRecord = NULL;

    this->records = new Record*[Bucket::size];

    for(int i=0; i < Bucket::size; i++) {
        this->records[i] = NULL;
    }
}



/**
 * Деструктор
 */
Bucket::~Bucket() {
    print("Запущен деструктор ~Bucket");
    for(int i=0; i < Bucket::size; i++) {
        delete[] this->records[i];

        this->records[i] = NULL;
    }
}



/**
 * Возвращает кол-во записей в корзине
 */
int Bucket::countRecords() {
    int count = 0;

    for(int i=0; i < Bucket::size; i++) {
        if(this->records[i] != NULL)
            count++;
    }

    return count;
}



/**
 * Возвращает кол-во собственных записей в корзине
 */
int Bucket::countSelfRecords() {
    int count = 0;

    for(int i=0; i < Bucket::size; i++) {
        if(this->records[i] != NULL && !this->records[i]->isOverflowRecord())
            count++;
    }

    return count;
}



/**
 * Возвращает кол-во пустого пространства
 */
int Bucket::returnEmptySpace() {
    return (Bucket::size - this->countRecords());
}



/**
 * Возвращает номер корзины
 */
int Bucket::getBucketNumber() {
    return this->number;
}



/**
 * Устанавливает номер корзины
 */
void Bucket::setBucketNumber(int number) {
    this->number = number;
}



/**
 * Считает количество внешних записей переполнения в корзине 
 */
int Bucket::countOuterOverflow() {
    int result = 0;

    for(int i=0; i < Bucket::size; i++) {
        if((this->records[i] != NULL) && this->records[i]->isOverflowRecord())
            result++;
    }

    return result;
}



/**
 * Проверка наличия в корзине внешних записей переполнения
 */
bool Bucket::issetOuterOverflow() {
    bool result = false;

    for(int i=0; i < Bucket::size; i++) {
        if((this->records[i] != NULL) && this->records[i]->isOverflowRecord()) {
            result = true;
            break;
        }
    }

    return result;
}



/**
 * Возвращает первую найденную внешнюю запись переполнения, 
 * хранимую в текущей корзине
 */
Record *Bucket::_getOuterOverflowRec() {
    Record *rec = NULL;

    for(int i=0; i < Bucket::size; i++) {
        if(this->records[i] != NULL && this->records[i]->isOverflowRecord()) {
            rec = this->records[i];
            break;
        }
    }

    return rec;
}



/**
 * Возвращает первую внешнюю запись переполнения, 
 * хранимую в текущей корзине
 */
Record *Bucket::getFirstOuterOverflowRec() {
    Record *rec = this->_getOuterOverflowRec();

    if(rec != NULL) {
        std::string key;
        std::vector <std::string> recKeys; // Защита от зацикливания
        Record *prevRec = rec->getPrevRecord();

        while(prevRec != NULL) { // Цикл до первой записи в цепочке ... 
            key        = rec->getKey();
            auto it    = std::find(recKeys.begin(), recKeys.end(), key);
            auto index = std::distance(recKeys.begin(), it);
            if(index == recKeys.size()) // Совпадений ключей в цепочке не найдено (цепочка не зациклина)
                recKeys.push_back(key);
            else {
                rec = NULL;
                die("Bucket::getFirstOuterOverflowRec(): внешняя цепочка переполнения для корзины "+ std::to_string(this->getBucketNumber()) + " оказалась зацикленной");
                break;
            }

            if(this->getBucketNumber() == prevRec->getOverflowBucket()->getBucketNumber()) {
                rec     = prevRec;
                prevRec = rec->getPrevRecord();
            }
            else // ... или до первой внешней записи в данной корзине 
                break;
        }
    }

    return rec;
}



/**
 * Возвращает последнюю внешнюю запись переполнения, 
 * хранимую в текущей корзине
 */
Record *Bucket::getLastOuterOverflowRec() {
    Record *rec = this->_getOuterOverflowRec();

    if(rec != NULL) {
        std::string key;
        std::vector <std::string> recKeys; // Защита от зацикливания
        Record *nextRec = rec->getNextRecord();

        while(nextRec != NULL) { // Цикл до последней записи в цепочке ... 
            key        = rec->getKey();
            auto it    = std::find(recKeys.begin(), recKeys.end(), key);
            auto index = std::distance(recKeys.begin(), it);
            if(index == recKeys.size()) // Совпадений ключей в цепочке не найдено (цепочка не зациклина)
                recKeys.push_back(key);
            else {
                rec = NULL;
                die("Bucket::getLastOuterOverflowRec(): внешняя цепочка переполнения для корзины "+ std::to_string(this->getBucketNumber()) +" оказалась зацикленной");
                break;
            }

            if(this->getBucketNumber() == nextRec->getOverflowBucket()->getBucketNumber()) {
                rec     = nextRec;
                nextRec = rec->getNextRecord();
            }
            else // ... или до первой внешней записи в данной корзине 
                break;
        }
    }

    return rec;
}



/**
 * Возвращает последнюю запись переполнения
 */
Record* Bucket::getLastOverflowRecord() {
    Record* rec = this->firstOverflowRecord;

    if(rec) {
        Record* currentRec = NULL;

        while((currentRec = rec->getNextRecord())) {
            rec = currentRec;
        }
    }

    return rec;
}



/**
 * Возвращает первую собственную запись в корзине
 */
Record* Bucket::getFirstSelfRecord() {
    Record *rec = NULL;

    for(int i=0; i < Bucket::size; i++) {
        if(this->records[i] != NULL && !this->records[i]->isOverflowRecord()) {
            rec = this->records[i];
            break;
        }
    }

    return rec;
}



/**
 * Поиск записи в корзине
 */
Record* Bucket::findRecord(std::string key) {
    Record* rec = NULL;

    // Поиск записи в корзине
    rec = this->findRecordInBucket(key);

    // поиск записи в цепочке переполнения
    if(rec == NULL)
        rec = this->findRecordInOverflow(key);

    return rec;
}



/**
 * Поиск записи в самой корзине
 */
Record* Bucket::findRecordInBucket(std::string key) {
    Record* rec = NULL;

    for(int i=0; i < Bucket::size; i++) {
        if(this->records[i] != NULL && (this->records[i]->getKey() == key)) {
            rec = this->records[i];
            break;
        }
    }

    return rec;
}



/**
 * Поиск записи в цепочке переполнения
 */
Record* Bucket::findRecordInOverflow(std::string key) {
    Record* rec        = NULL;
    Record* currentRec = this->firstOverflowRecord;

    if(currentRec != NULL) {
        do {
            if(currentRec->getKey() == key) {
                rec = currentRec;
                break;
            }

            currentRec = currentRec->getNextRecord();
        } while(currentRec != NULL);
    }

    return rec;
}



/**
 * Назначение ссылки на первую запись переполнения 
 */
void Bucket::setFirstOverflowRecord(Record *r) {
    this->firstOverflowRecord = r;
}


/**
 * Возвращает ссылку на первую запись переполнения 
 */
Record* Bucket::getFirstOverflowRecord() {
    return this->firstOverflowRecord;
}



/**
 * Удаление ссылки на первую запись переполнения 
 */
void Bucket::delFirstOverflowRecord() {
    this->firstOverflowRecord = NULL;
}



/**
 * Возвращает номер пустой ячейки массива
 */
int Bucket::getAmountEmptySpace() {
    int num = -1;

    for(int i=0; i < Bucket::size; i++) {
        if(this->records[i] == NULL) {
            num = i;
            break;
        }
    }

    return num;
}



/**
 * Добавление записи в саму корзину
 */
bool Bucket::add(Record *record) {
    bool result = false;

    int c_records = this->countRecords();
    if(c_records < Bucket::size) {
        int empty_num = this->getAmountEmptySpace();

        if(empty_num != -1) {
            this->records[empty_num] = record;
            result = true;
        }
        else
            die("Bucket::add(): счетчик показал наличие пустого пространства в корзине, но на деле такого не оказалось.");
    }
    else
        die("Bucket["+ std::to_string(this->getBucketNumber()) +"]::add(): корзина заполнена!");

    return result;
}



/**
 * Удаление записи
 */
bool Bucket::del(std::string key) {
    bool result = false;

    int i;
    for(i=0; i < Bucket::size; i++) {
        if(this->records[i] != NULL && this->records[i]->getKey() == key) {
            break;
        }
    }

    if(i >= 0) {
        result = true;
        delete this->records[i];
        this->records[i] = NULL;
    }
    else
        die("Bucket::del(): запись '" + key + "' в корзине не найдена!");

    return result;
}



/**
 * Извлечение записи из корзины
 */
Record *Bucket::extraction(std::string key) {
    Record * rec = NULL;

    int i;
    int ir = -1;
    for(i=0; i < Bucket::size; i++) {
        if(this->records[i] != NULL && this->records[i]->getKey() == key) {
            ir = i;
            break;
        }
    }

    if(i >= 0) {
        rec = this->records[i];

        if(rec->isOverflowRecord())
            rec->removeFromChain(); // Вырезаем из цепочки
        
        this->records[i] = NULL;
    }
    else
        die("Bucket::extraction(std::string key): запись '" + key + "' в корзине не найдена!");

    return rec;
}



/**
 * Извлечение записи из корзины (перегрузка)
 */
Record *Bucket::extraction(Record *rec) {
    std::string key = rec->getKey();

    int i;
    int ir = -1;
    for(i=0; i < Bucket::size; i++) {
        if(this->records[i] != NULL && this->records[i]->getKey() == key) {
            ir = i;
            break;
        }
    }

    if(i >= 0) {
        if(rec->isOverflowRecord())
            rec->removeFromChain(); // Вырезаем из цепочки
        
        this->records[i] = NULL;
    }
    else
        die("Bucket::extraction(Record *rec): запись '" + key + "' в корзине не найдена!");

    return rec;
}



/**
 * Извлечение записи из корзины (перегрузка) по номеру в корзине
 */
Record * Bucket::extraction(int i) {
    Record * rec = NULL;

    if(i >= 0 && this->records[i] != NULL) {
        rec = this->records[i];

        if(rec->isOverflowRecord())
            rec->removeFromChain(); // Вырезаем из цепочки
        
        this->records[i] = NULL;
    }

    return rec;
}



/**
 * Возвращает тип корзины
 */
int Bucket::getType(bool isOverflow) {
    int type = -1;
    int countEmpty = this->returnEmptySpace();

    if(isOverflow) { // Корзина как корзина переполнения для значения
        if(countEmpty) // c3
            type = Bucket::type_c3;
        else { // c2, c4 
            Record *lastOuterOverflowRec = getLastOuterOverflowRec();
            if(lastOuterOverflowRec->getNextRecord() == NULL) 
                type = Bucket::type_c4;
            else
                type = Bucket::type_c2;
        }
    }
    else { // Корзина как собственная корзина для значения
        if(!this->issetOuterOverflow()) { // a, b, c1
            if(countEmpty)
                type = Bucket::type_a;
            else
                if(this->firstOverflowRecord != NULL)
                    type = Bucket::type_c1;
                else
                    type = Bucket::type_b;
        }
        else { // d, e1, e2
            if(countEmpty)
                type = Bucket::type_d;
            else { // e1, e2 
                Record *lastOuterOverflowRec = getLastOuterOverflowRec();
                if(lastOuterOverflowRec->getNextRecord() == NULL) 
                    type = Bucket::type_e1;
                else
                    type = Bucket::type_e2;
            }
        }
    }

    return type;
}



/**
 * Возвращает вектор записей
 */
std::vector<RecordInfo>  Bucket::getRecords() {
    std::vector<RecordInfo> recs;

    // Заполняем вектор записей и находим максимальный размер строк
    int maxKey   = 0;
    int maxValue = 0;
    for(int i=0; i < Bucket::size; i++) {
        RecordInfo rec;

        if(this->records[i] != NULL) {
            rec.isEmpty = false;
            rec.key     = this->records[i]->getKey();
            rec.value   = this->records[i]->getValue();
            rec.pBucket = this->records[i]->getPrimaryBucketNumber();

            if(this->records[i]->isOverflowRecord()) {
                rec.oBucket = this->records[i]->getOverflowBucketNumber();

                Record* prev = this->records[i]->getPrevRecord();
                if(prev != NULL)
                    rec.prev = prev->getKey();
                else 
                    rec.prev = "-----";

                Record* next = this->records[i]->getNextRecord();
                if(next != NULL)
                    rec.next = next->getKey();
                else 
                    rec.next = "-----";
            }
            else {
                rec.oBucket = -1;
                rec.prev    = "-----";
                rec.next    = "-----";
            }
        }
        else {
            rec.isEmpty = true;
            rec.key     = "-----";
            rec.value   = "----------";
            rec.pBucket = -1;
            rec.oBucket = -1;
            rec.prev    = "-----";
            rec.next    = "-----";
        }

        rec.num = i;
        recs.push_back(rec);

        int key_len = rec.key.length();
            if(key_len > maxKey)
                maxKey = key_len;

        int val_len = rec.value.length();
            if(val_len > maxValue)
                maxValue = val_len;
    }

    // Приводим строки в векторе записей к одной длинне
    // for(int i=0; i < recs.size(); i++) {
    //     int  addToKey = 0,
    //          addToVal = 0;
    //     if(recs[i].isEmpty) { // Костыль для подсчета кол-ва символов
    //         addToKey = (maxKey   - recs[i].key.length() * 2) / 2;
    //         addToVal = (maxValue - recs[i].value.length() * 2) / 2;
    //     }
    //     else {
    //         addToKey = (maxKey   - recs[i].key.length()) / 2;
    //         addToVal = (maxValue - recs[i].value.length()) / 2;
    //     }


    //     if(addToKey > 0) {
    //         std::string addEmptySpaceKey (addToKey, ' ');
    //         recs[i].key   += addEmptySpaceKey;
    //     }
        
    //     if(addToVal > 0) {
    //         std::string addEmptySpaceVal (addToVal, ' ');
    //         recs[i].key   += addEmptySpaceVal;
    //     }
    // }

    return recs;
}