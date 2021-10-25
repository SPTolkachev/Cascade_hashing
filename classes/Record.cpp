#include "Record.h"


/**
 * Конструктор
 */
Record::Record(std::string key, std::string value) {
    if(key.empty()) 
        die("при создании записи не был указан ключ!");
    else
        this->key = key;
    
    if(value.empty())
        die("при создании записи не было указано значение!");
    else
        this->value = value;

    this->primaryBucket  = NULL;
    this->overflowBucket = NULL;
    this->preview = NULL;
    this->next    = NULL;
}



/**
 * Деструктор
 */
Record::~Record() {
    print("Запущен деструктор ~Record");
    this->primaryBucket = NULL;
    this->overflowBucket = NULL;
    this->preview = NULL;
    this->next = NULL;
}



/**
 * Возвращает ключ
 * @return std::string 
 */
std::string Record::getKey() {
    return this->key;
}



/**
 * Возвращает значение
 * @return std::string 
 */
std::string  Record::getValue() {
    return this->value;
}



/**
 * Возвращает номер корзины к которой относится запись
 * @return string
 */
int Record::getPrimaryBucketNumber() {
    return (this->primaryBucket != NULL ? this->primaryBucket->getBucketNumber() : -1);
}



/**
 * Является ли запись записью переполнения
 */
bool Record::isOverflowRecord() {
    return (this->overflowBucket != NULL ? true : false);
}



/**
 * Назначение ссылки на корзину переполнения
 */
void Record::setOverflowBucket(Bucket *bucket) {
    this->overflowBucket = bucket;
}



/**
 * Удаление ссылки на корзину переполнения
 */
void Record::delOverflowBucket() {
    this->overflowBucket = NULL;
}



/**
 * Получение ссылки на корзину переполнения
 */
Bucket* Record::getOverflowBucket() {
    return this->overflowBucket;
}



/**
 * Возвращает концептуальный номер корзины переполнения
 */
int Record::getOverflowBucketNumber() {
    int result = -1;

    if(this->overflowBucket != NULL)
        result = this->overflowBucket->getBucketNumber();

    return result;
}



/**
 * Назначение ссылки на основную корзину 
 */
void Record::setPrimaryBucket(Bucket *bucket) {
    this->primaryBucket = bucket;
}



/**
 * Получение ссылки на основную корзину 
 */
Bucket* Record::getPrimaryBucket() {
    return this->primaryBucket;
}



/**
 * Назначение ссылки на предыдущую запись 
 */
void Record::setPrevRecord(Record *record) {
    this->preview = record;
}



/**
 * Получение ссылки на предыдущую запись 
 */
Record* Record::getPrevRecord() {
    return this->preview;
}



/**
 * Назначение null на предыдущую запись 
 */
void Record::delPrevRecord() {
    this->preview = NULL;
}



/**
 * Назначение ссылки на следущую запись 
 */
void Record::setNextRecord(Record *record) {
    this->next = record;
}



/**
 * Назначение null на следущую запись 
 */
void Record::delNextRecord() {
    this->next = NULL;
}



/**
 * Возвращает ссылку на следущую запись 
 * @return Record -- запись 
 */
Record* Record::getNextRecord() {
    return this->next;
}



void Record::removeFromChain() {
    if(this->overflowBucket != NULL) {
        Record *prev = this->getPrevRecord();
        Record *next = this->getNextRecord();

        if(next != NULL) {
            if(prev != NULL) {
                prev->setNextRecord(next);
                next->setPrevRecord(prev);
            }
            else {
                next->delPrevRecord();
                this->primaryBucket->setFirstOverflowRecord(next);
            }
        }
        else {
            if(prev != NULL)
                prev->delNextRecord();
            else
                this->primaryBucket->delFirstOverflowRecord();
        }
    }
    else
        die("Попытка удалить запись '" + this->getKey() + "', которая не находится в цепочке.");
    
}