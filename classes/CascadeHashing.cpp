#include "CascadeHashing.h"


/*
 ****************************************************
 ***********     Вспомогательные ф-ии     ***********
 ****************************************************
*/
/**
 * Перегружаем ф-ию натурального логарифма
 */
double log(double a, double b) {
    double result = 0;
    double log_b = log(b);
    
    if(log_b != 0)
        result = log(a) / log_b;

    return result;
}


/*
 ****************************************************
 ******************     Методы     ******************
 ****************************************************
*/





/**
 * Конструктор
 */
CascadeHashing::CascadeHashing() {
    this->s = 1;
    this->t = 2;
    this->M = ceil(this->g()) - 1;
    this->b = Bucket::size;

    for(int i=1; i <= this->M; i++) {
        this->addBucket(i);
    }
}



/**
 * Деструктор
 */
CascadeHashing::~CascadeHashing() {
    print("Запущен деструктор ~CascadeHashing");
    for(int i=0; i <= this->M; i++) {
        delete[] this->buckets[i];

        this->buckets[i] = NULL;
    }
}



/**
 * Создает новую корзину
 * @param int $BucketNum -- концептуальный номер корзины
 * @return Bucket
 */
Bucket *CascadeHashing::newBucket(int num) {
    Bucket *bucket = new Bucket(num);

    return bucket;
}


/**
 * Добавляет новую корзину в конец вектора корзин
 */
void CascadeHashing::addBucket(int num) {
    this->buckets.push_back(this->newBucket(num));
}



/**
 * Удаляет корзину из вектора корзин
 */
void CascadeHashing::delBucket(int num) {
    int page = this->page(num);

    this->buckets.erase(this->buckets.begin() + page);
}



/**
 * Возвращает номера корзин
 */
std::vector<int> CascadeHashing::getBucketNums() {
    std::vector<int> nums(this->M);

    for(int i=0; i < this->M; i++) {
        nums[i] = this->buckets[i]->getBucketNumber();
    }

    return nums;
}



/**
 * Возвращает номера корзин отсортированные по возрастанию
 */
std::vector<int> CascadeHashing::getSortBucketNums() {
    std::vector<int> nums = this->getBucketNums();

    int length = nums.size();

    if(length >= 2) {
        for(int i=1; i < length; i++) { // Проходим по массиву
            int current_int = nums[i];
            
            for(int j=(i-1); j >= 0; j--) {
                if(current_int < nums[j]) {
                    nums[j+1] = nums[j];
                    if(j-1 <= 0)
                        nums[j] = current_int;
                    else
                        nums[j] = 0; // Для отслеживания
                }
                else {
                    nums[j+1] = current_int;
                    break;
                }

            }
        }
    }

    return nums;
}



/**
 * Возвращает минимальный номер корзины
 */
int CascadeHashing::getMinBucketNum() {
    std::vector<int> nums = this->getBucketNums();
    int min = nums[0];

    for(int i=0; i < nums.size(); i++) {
        if(nums[i] < min)
            min = nums[i];
    }

    return min;
}



/**
 * Возвращает максимальный номер корзины
 */
int CascadeHashing::getMaxBucketNum() {
    std::vector<int> nums = this->getBucketNums();
    int max = nums[0];

    for(int i=0; i < nums.size(); i++) {
        if(nums[i] > max)
            max = nums[i];
    }

    return max;
}



/**
 * Возвращает значение g
 * @return double
 */
double CascadeHashing::g() {
    return (this->t / this->s);
}



/**
 * Функция преобразующая ключ в число
 * @param  string $key -- ключ
 * @return int 
 */
int CascadeHashing::key_to_int(std::string key) {
    int length  = key.length();
    int integer = 0;
    
    for(int i=0; i < length; i++) {
        integer += key[i];
    }

    if(integer < 0)
        integer *= -1;
    
    return integer;
}



/**
 * Ф-ия равномерно распределяющая все записи на отрезке [0,1[
 * @return double
 */
double CascadeHashing::d(std::string key) {
    int k_int = this->key_to_int(key);
    double residue = k_int % this->M;
    double result = residue / this->M;

    return result;
}



/**
 * Функция возвращающая номер разделяемой корзины
 * @return double
 */
double CascadeHashing::p() {
    return ( this->M/(this->g() - 1) );
}





/**
 * Функция возвращающая значение файлового уровня
 * @return int
 */
int CascadeHashing::L() {
    return floor( log(this->p(), this->g()) );
}



/**
 * Хеш-функция Каскадного хеширования
 * @param  string $k -- ключ
 * @return int
 */
int CascadeHashing::h(std::string key) {
    double g   = this->g();
    double p   = this->p();
    double d_k = this->d(key);

    double exp = ceil(log(p, g) - d_k) + d_k;

    int result = pow(g, exp);

    return result;
}



/**
 * Возвращает текущее кол-во записей (x)
 * @return int
 */
int CascadeHashing::countAllRecords() {
    int count = 0;

    for(int i=0; i < this->M; i++) {
        count += this->buckets[i]->countRecords();
    }

    return count;
}



/**
 * Возвращает значение коэффицента загрузки (x/bM)
 * Т.к. переполнение хранится в основном файле, 
 * то значение используемого хранилища равно 
 * коэффициенту загрузки
 * @return double
 */
double CascadeHashing::f() {
    double x = this->countAllRecords();
    double b = this->b;
    double M = this->M;

    return (x / (b * M));
}



/**
 * Возвращает значение максимального коэффицента загрузки
 * @return double
 */
double CascadeHashing::f_max() {
    double g = this->g();

    return (log(g) / (g - 1));
}



/**
 * Возвращает значение минимального коэффицента загрузки
 * т.к. в тексте минимального коэффициента нет, то возьмем
 * значение загруженности в конце файла из спирального хранения
 * @return double
 */
double CascadeHashing::f_min() {
    int g = this->g();

    return (log(g) / (g - 0));
}



/**
 * Необходима ли группировка
 * @return bool
 */
bool CascadeHashing::needGrouping() {
    return (this->f() < this->f_min() && this->buckets.size() > 1);
}



/**
 * Необходимо ли разделение
 * @return bool
 */
bool CascadeHashing::needSplit() {
    return (this->f() > this->f_max());
}



/**
 * Функция преобразования концептуального номера корзины 
 * в номер страницы (фактический номер корзины, т.е.
 * новый номер текущей корзины)
 * @param  int $n -- концептуальный номер корзины
 * @return int    -- номер страницы
 */
int CascadeHashing::page(int n) { // Алгоритм без рекурсии
    int m = n;
    int t = this->t;
    int s = this->s;

    if(n <= 0)
        this->die("CascadeHashing::page: Ошибка! n = " + std::to_string(n) + " (n должна быть > 0)");

    while((m % t) < s) {
        m = (floor(m / t) * s + (m % t));
    }
    
    return (floor(m / t) * (t - s) + ((m - s) % t));
}



// =================================================


/**
 * Возвращает первую свободную для переполнения корзину
 */


Bucket *CascadeHashing::getFreeOverflowBucket() {
    std::vector<int> not_in;

    return this->getFreeOverflowBucket(not_in);
}



Bucket *CascadeHashing::getFreeOverflowBucket(std::vector<int> not_in) {
    Bucket * b = NULL;
    int maxEmpty = 0;
    int maxEmptyBucketNum = -1;

    for(int i=0, bucketEmpty=0, bucketNum=-1; i < this->buckets.size(); i++) {
        bucketNum = this->buckets[i]->getBucketNumber();

        if(not_in.empty() || !(std::find( not_in.begin(), not_in.end(), bucketNum) != not_in.end())) {
            bucketEmpty = this->buckets[i]->returnEmptySpace();

            if(bucketEmpty > maxEmpty && !this->buckets[i]->countOuterOverflow()) {
                maxEmptyBucketNum = i;
                maxEmpty = bucketEmpty;
            }
        }
        // if(not_in.empty() || !(std::find( not_in.begin(), not_in.end(), num) != not_in.end())) {
        //     if(this->buckets[i]->returnEmptySpace() && !this->buckets[i]->countOuterOverflow()) {
        //         b = this->buckets[i];
        //         break;
        //     }
        // }
    }

    if(maxEmptyBucketNum >= 0)
        b = this->buckets[maxEmptyBucketNum];

    return b;
}


/*
 ***************************************************
 *************** МЕТОДЫ ПО АЛГОРИТМАМ **************
 ***************************************************
*/



/**
 * Публичный метод вставка записи по ключу и значению
 */
void CascadeHashing::insert(std::string key, std::string value) {
    Record *rec = new Record (key, value); // newRecord

    this->_insert(rec);
}



/**
 * Публичный метод вставка записи по записи
 */
void CascadeHashing::insert(Record *rec) {
    this->_insert(rec);
}



/**
 * Защищенный метод вставки записи
 */
void CascadeHashing::_insert(Record *k) {
    std::string key = k->getKey();
    print("=================== insert("+ key +") ===================");
    // this->print_table();
    // 1. [Разделить] Если максимальный коэффициент загруженности превышен, 
    //    выполните разделение.
    if(this->needSplit()) {
        logs->upLevel();
        this->split();
        logs->downLevel();
    }

    to_i2:
    // 2. [Поиск] Поиск ключа k. Если ключ найден, вставка завершается неудачей. 
    //    (Поиск заканчивается последней корзиной в цепочке переполнения для k 
    //    -- возможно, основной корзиной -- как текущей). Пусть B <-- текущая 
    //    корзина. (B может быть любого типа, но c1 и c2.) 
    int n = this->h(key);
    int P = this->page(n);

    Bucket  *findBucket = this->buckets[P]; // this->buckets
    Record *findRecord = findBucket->findRecord(key);

    if(findRecord == NULL) {
        int bucketType = -1;
        Record *lastOverflowRecord = findBucket->getLastOverflowRecord();
        Record *B_lastOverflowRecord = NULL;
        Bucket *lastBucketInChan = NULL;
        
        //Bucket *B = NULL;

        if(lastOverflowRecord != NULL) {
            lastBucketInChan = lastOverflowRecord->getOverflowBucket();
            bucketType = lastBucketInChan->getType(true);
            //B = lastBucketInChan;
        }
        else {
            bucketType = findBucket->getType();
            //B = &this->buckets[P]; // primaryBucket
        }

        Bucket *B = this->buckets[P];
        Record *_k; // k'
        Bucket *_B; // B'

        print("Вставка (п.2): добавляется запись типа '"+ Bucket::getStringType(bucketType) +"' -- "+ key +" (Primary bucket page: "+ std::to_string(P) +")");

        to_switch:
        switch(bucketType) {
            // 3. [Корзины типа a или d, вставка] Если B имеет тип a или d (в основной 
            //    корзине есть место для k), тогда вставьте k в B и завершите.
            case Bucket::type_a:
            case Bucket::type_d:
                // primaryBucket = &this->buckets[P];
                B->add(k);
                k->setPrimaryBucket(B);
                goto to_iend;
                break;

            // 4. [Корзина типа e1 или e2] Если B имеет тип e1 или e2 (основная корзина 
            //    заполнена, но содержит переполнение из другой корзины)
            case Bucket::type_e1:
            case Bucket::type_e2:
                // 4.1 [Выбор записи переполнения] Выберите одну из 'внешних' записей переполнения в B, 
                //     скажем k', и удалите её (временно!).
                _k = B->getLastOuterOverflowRec();

                if(_k == NULL)
                    this->die("Вставка (п.4.1): Не найдено ни одной подходящей 'внешней' записи переполнения");
                
                // Временно удаляем внешнюю запись переполнения
                //_k->removeFromChain();
                B->extraction(_k->getKey());
                _k->delOverflowBucket(); // Удаляем связь с корзиной

                // 4.2 [Вставка] Вставьте k в B.
                // Добавляем запись в основную корзину вместо удалённой
                B->add(k);
                k->setPrimaryBucket(B);

                // 4.3 [Реорганизация и поиск конца цепочки для k'].
                if(bucketType == Bucket::type_e1) {
                    // 4.3.1 Если B имеет тип e1, а k' была единственной записью переполнения в B, 
                    //       то пусть B <-- предыдущая корзина в цепочке для k'.
                    if(!B->countOuterOverflow()) {
                        _B = _k->getPrimaryBucket();

                        if(_B->getLastOverflowRecord() == NULL) // была единственная запись переполнения
                            B = _B;//this->getFreeOverflowBucket();
                        else
                            B = _B->getLastOverflowRecord()->getOverflowBucket();
                    }
                }
                else { // bucketType == Bucket::type_e2
                    // 4.3.2 Если B имеет тип e2, то 
                    // 4.3.2.1 Если k' была единственной записью переполнения в B, тогда удалите связь B 
                    //         с цепочкой k'.
                    if(!B->countOuterOverflow()) {
                        _k->delOverflowBucket();
                    }
                    // 4.3.2.2 Следуйте за цепочкой k' до конца и пусть B <-- последняя корзина в цепочке.
                    _B = _k->getPrimaryBucket();
                    B = _B->getLastOverflowRecord()->getOverflowBucket();
                }

                // _B = _k->getPrimaryBucket();
                // B = _B->getLastOverflowRecord()->getOverflowBucket();
                
                // 4.4 [Переключение на вставку k'] Пусть k <-- k'. (Теперь известно, что B имеет тип b, c3 или c4.)
                k  = _k;
                _k = NULL;
                _B = NULL;
                bucketType = B->getType();
                goto to_switch;
                break;
            
            // 5. [Корзина типа c3, вставка] Если B имеет тип c3 (B имеет пространство необходимое для вставки k), тогда 
            //    вставьте k и завершите.
            case Bucket::type_c3:
                if(lastBucketInChan->returnEmptySpace()) {
                    lastBucketInChan->add(k);
                    k->setPrimaryBucket(B);
                    k->setOverflowBucket(lastBucketInChan);
                    
                    Record *prev = k->getPrimaryBucket()->getLastOverflowRecord();
                    prev->setNextRecord(k);
                    k->setPrevRecord(prev);
                    k->delNextRecord();
                }
                else
                    this->die("Вставка (п.5): У коризины " + std::to_string(B->getBucketNumber()) + " типа 'c3' не оказалось достаточно пустого пространтства!");
                break;
            
            // 6. [Выбор новой корзины переполнения] (теперь B имеет тип b или c4.) Выберите новую 
            //    корзину переполнения B'. Если ничего не найдено, выполните разделение, напишите 
            //    предупреждение обслуживающему персоналу и вернитесь к I2.
            case Bucket::type_b:
            case Bucket::type_c4:
                _B = this->getFreeOverflowBucket();
                if(_B != NULL) {
                    // 7. [Вставка] Вставьте k в B'.
                    _B->add(k);
                    k->setPrimaryBucket(B);
                    k->setOverflowBucket(_B);

                    if(bucketType == Bucket::type_b) {
                        B->setFirstOverflowRecord(k);
                    }
                    else { // c4
                        _k = B->getLastOverflowRecord();
                        _k->setNextRecord(k);
                        k->setPrevRecord(_k);

                        // 8. [Реорганизация цепочки] Если B -- корзина переполнения, а B' может содержать 
                        //    все записи переполнения в B, то переместите записи переполнения из B в B' и 
                        //    пусть B <-- предшественник B' в цепочке. Повторите I8 как можно больше.

                        // 9. [Связывание] Связать B' с B.
                        logs->upLevel();
                        this->reorganizationChain(B->getBucketNumber());
                        logs->downLevel();
                    }
                }
                else {
                    print("Вставка (п.6): Не найдено ни одной свободной корзины для переполнения!");
                    this->split();
                    goto to_i2;
                }
                break;

            default: 
                this->die("Вставка (п.2): Попытка вставки записи '"+ key +"' непредусмотренного типа '"+ Bucket::getStringType(bucketType) +"'\n");
        } 
    }
    else
        this->die("Вставка (п.2): Попытка вставки записи с существующим ключем '"+ key +"'!");
    

    to_iend:
    this->print_table();
    print("=================== \\insert("+ key +") ===================\n\n");
}



/**
 * Реорганизация цепочки (I8)
 * 8. [Реорганизация цепочки] Если B -- корзина переполнения, а B' может содержать 
 *    все записи переполнения в B, то переместите записи переполнения из B в B' и 
 *    пусть B <-- предшественник B' в цепочке. Повторите I8 как можно больше. 
 */
void CascadeHashing::reorganizationChain(int bNum) {
    print("=================== reorganizationChain(" + std::to_string(bNum) + ") ===================");
    // this->print_table();
    int P = this->page(bNum);
    Bucket *B = this->buckets[P]; // primaryBucket

    Record *currRecord = B->getFirstOverflowRecord();
    //Record *nextRecord = NULL;
    std::vector<Record*> chainRecs;

    while(currRecord!= NULL) {
        //nextRecord = currRecord->getNextRecord();
        currRecord->getOverflowBucket()->extraction(currRecord);
        currRecord->delNextRecord();
        currRecord->delPrevRecord();
        currRecord->delOverflowBucket();
        chainRecs.push_back(currRecord);

        currRecord = B->getFirstOverflowRecord();
    }

    B->setFirstOverflowRecord(chainRecs[0]);
    Bucket *B0 = this->getFreeOverflowBucket();

    for(int i = 0; i < chainRecs.size(); i++) {
        B0->add(chainRecs[i]);

        if(i) {
            chainRecs[i-1]->setNextRecord(chainRecs[i]);
            chainRecs[i]->setPrevRecord(chainRecs[i-1]);
        }

        chainRecs[i]->setOverflowBucket(B0);

        if(!B0->returnEmptySpace())
            B0 = this->getFreeOverflowBucket();
    }

    this->print_table();
    print("=================== \\reorganizationChain(" + std::to_string(bNum) + ") ===================\n\n");
}



/**
 * Удаление записи
 */
void CascadeHashing::deleteRecord(std::string key) {
    print("=================== deleteRecord(" + key + ") ===================");
    // this->print_table();

    int n = this->h(key);
    int P = this->page(n);

    // 1. [Поиск] Поиск ключа k. Если ключ не найден, удаление завершается неудачно. 
    //    (поиск заканчивается корзиной, содержащей k в качестве текущего.) Пусть B <-- текущая корзина.
    Bucket *B  = NULL;
    Bucket *_B = NULL;
    Bucket *findBucket = this->buckets[P];
    Record *k  = findBucket->findRecord(key);
    Record *_k = NULL;

    if(k != NULL) {
        int bucketType       = -1;
        int outerRecInBucket = 0;
        bool isOverflowRecord = k->isOverflowRecord();
        if(isOverflowRecord) {
            B = k->getOverflowBucket();
            bucketType = B->getType(true);
            k->removeFromChain();
        }
        else {
            B = findBucket;
            bucketType = B->getType();
            _k = B->getFirstOuterOverflowRec();
        }

        // 2. [Удаление] Удаление k из B.
        B->del(key);

        print("Удаление записи (п.2): удаляется запись типа '"+ Bucket::getStringType(bucketType) +"' -- "+ key +" (Primary bucket page: "+ std::to_string(n) +")\n");
        switch(bucketType) {
            // 3. [Простое удаление] Если B имеет тип a, b, c3, c4, d или e1 (цепочки не нуждаются 
            //    в реорганизации после удаления k), тогда перейдите к D7. (В случае c3, c4, d и e1 
            //    может быть выполнено действие, аналогичное I8).
            case Bucket::type_a:
            case Bucket::type_b:
                // Переход к D7
                break;
            
            case Bucket::type_c3:
            case Bucket::type_c4:
            case Bucket::type_d:
            case Bucket::type_e1:
                    // Можно выполнить действие аналогичное I8
                    logs->upLevel();
                    if(isOverflowRecord)
                        this->reorganizationChain(B->getBucketNumber());
                    else
                        this->reorganizationChain(_k->getPrimaryBucket()->getBucketNumber());
                    logs->downLevel();
                    
                    // Переход к D7
                break;

            case Bucket::type_c1:
            case Bucket::type_c2:
            case Bucket::type_e2:
                // 4. [Найти конец цепочки] (B теперь имеет тип c1, c2 или e2. Цепочка переполнения 
                //    нуждается в реорганизации.) Следуйте за цепочкой, исходящей от B до конца; пусть 
                //    B' <-- последняя корзина в цепочке.
                if(bucketType == Bucket::type_c1)
                    _k = B->getLastOverflowRecord();
                else if(bucketType == Bucket::type_c2) {
                    _k = this->buckets[P]->getLastOverflowRecord();
                    outerRecInBucket = this->buckets[P]->countOuterOverflow();
                }
                else {// bucketType == Bucket::type_e2 
                    _k = B->getLastOuterOverflowRec()->getPrimaryBucket()->getLastOverflowRecord();
                    outerRecInBucket = B->countOuterOverflow();
                }

                _B = _k->getOverflowBucket();
                

                // 5. [Выбор записи] Выберите запись переполнения в B', скажем k'. Удалите k' из B'. 
                //    Если  k' была единственной записью переполнения в B', тогда удалите связь B' из цепочки.

                // 6. [Восстановить k'] Вставить k' в B. («Пустота» после заполнения k).
                if(bucketType == Bucket::type_c1) {
                    _B->extraction(_k);

                    B->add(_k);
                    _k->delOverflowBucket();
                    _k->delPrevRecord();
                    _k->delNextRecord();
                }
                else if(bucketType == Bucket::type_c2) {
                    if(outerRecInBucket > 1) {
                        _B->extraction(_k);

                        Record *prev = B->getLastOuterOverflowRec();
                        Record *next = prev->getNextRecord();

                        B->add(_k);
                        _k->setOverflowBucket(B);
                        _k->setPrevRecord(prev);
                        prev->setNextRecord(_k);
                        _k->setNextRecord(next);
                        next->setPrevRecord(_k);
                    }
                }
                else { // bucketType == Bucket::type_e2 
                    if(outerRecInBucket > 1) {
                        _B->extraction(_k);

                        Record *prev = B->getLastOuterOverflowRec();
                        Record *next = prev->getNextRecord();

                        B->add(_k);
                        _k->setOverflowBucket(B);
                        _k->setPrevRecord(prev);
                        prev->setNextRecord(_k);
                        _k->setNextRecord(next);
                        next->setPrevRecord(_k);
                    }
                }
                break;

            default: 
                this->die("Удаление записи (п.2): попытка удаления записи '"+ key +"' непредусмотренного типа '"+ Bucket::getStringType(bucketType) +"'\n");
        }

        // 7. [Группировка] Если теперь коэффициент загруженности ниже минимального, 
        //    то выполните группировку.
        if(this->needGrouping())
            this->grouping();
    }
    else
        this->die("Удаление записи (п.1): запись '"+ key +"' не найдена в корзине "+ std::to_string(n) +"!");

    this->print_table();
    print("=================== \\deleteRecord(" + key + ") ===================\n\n");
}



/**
 * Разделение корзин(ы)
 */
void CascadeHashing::split() {
    double p = this->p();
    int splitPointer = this->page(p);

    print("=================== split("+ std::to_string(p) +") ===================");
    // this->print_table();

    // 1. [Сканирование корзин для разделения]. Пусть B - это корзина с номером p, p+1, ..., p+s-1 раз. 
    //    Для каждой из этих корзин сделайте 
    Bucket *B = this->buckets[splitPointer];
    int bucketType = B->getType();

    //    1.1 [Чтение] Чтение B. (Это может быть любой тип основной корзины, т.е. все, кроме 
    //        c2, c3 и c4.)
    print("Разделение (п.2): разделяется корзина типа '"+ Bucket::getStringType(bucketType) +"'");

    Record *currRecord;
    std::vector<Record*> self_overflows;
    std::vector<Record*> outer_overflows;
    switch(bucketType) {
        case Bucket::type_a:
        case Bucket::type_b:
            // Не нуждаются в доп. действиях (т.к. не имеют записей переполнения)
            break;

        case Bucket::type_c1:
            // 1.2 [Чтение собственного переполнения] Если B имеет тип c1, то в его цепочке 
            //     считываются корзины переполнения, записи переполнения удаляются (но позже 
            //     сохраняются!), а корзины переписываются с переполнением пространства, помеченного 
            //     как пустое.
            currRecord = B->getFirstOverflowRecord();
            
            do {
                currRecord->getOverflowBucket()->extraction(currRecord);
                self_overflows.push_back(currRecord);
                currRecord = currRecord->getNextRecord();
            } while(currRecord != NULL);

            B->delFirstOverflowRecord();
            break;

        case Bucket::type_d:
        case Bucket::type_e1:
        case Bucket::type_e2:
            // 1.3 [Реорг. ’Внешней’ цепочки переполнения] Если B имеет тип d, e1 или e2 (эта ситуация 
            //     должна быть очень редкой),  связан с цепочкой, а ’внешние’ записи переполнения 
            //     откладываются на некоторое время.
            currRecord = B->getFirstOuterOverflowRec();

            do {
                B->extraction(currRecord);
                outer_overflows.push_back(currRecord);
                currRecord = currRecord->getNextRecord();
            } while(currRecord != NULL && p == currRecord->getOverflowBucket()->getBucketNumber());
            break;

        default: 
            this->die("Разделение (п.1.1): Неправильный тип корзины для разделения -- '"+ Bucket::getStringType(bucketType) +"'");
    }

    double g = this->g();

    // Извлекаем собственные записи
    std::vector<Record*> self_records;
    currRecord = B->getFirstSelfRecord();
    while(currRecord != NULL) {
        B->extraction(currRecord);
        self_records.push_back(currRecord);
        currRecord = B->getFirstSelfRecord();
    }

    // 2. [Приращение] Пусть p <-- p + s.
    if(B->countRecords())
        this->die("Разделение (п.2): попытка разделения корзины ("+ std::to_string(splitPointer) +"), у которой есть записи!");

    int addBuckets = this->t - this->s;
    int numLastBucket = this->getMaxBucketNum();
    B->setBucketNumber(++numLastBucket); // Переназначет номер в старой корзине

    numLastBucket++;

    for(int i=0; i < addBuckets; i++) {
        this->addBucket(i + numLastBucket);
        this->M++;
    }

    // 3. [Перехеширование] Перехешируйте все записи, включая собственные записи переполнения из S1.2, 
    //    но исключая ’внешние’ записи переполнения из S1.3. (Проверьте записи в новых корзинах 
    //    gp-t, ..., gp-1.) Если некоторые из новых корзин должны переполняться (это должно быть редкой 
    //    ситуацией), то переполнение откладывается на некоторое время.
    logs->upLevel();

    std::vector<Record*> all_records;
    Record *r = NULL;
    for(int i=0; i < this->M; i++) {
        for(int j=0; j < Bucket::size; j++) {
            r = this->buckets[i]->extraction(j);

            if(r != NULL) {
                r->delOverflowBucket();
                r->delNextRecord();
                r->delPrevRecord();

                all_records.push_back(r);
            }
        }

        this->buckets[i]->delFirstOverflowRecord();
    }

    for(int i=0; i < all_records.size(); i++) {
        this->insert(all_records[i]);
    }

    // 4. [Перезапись] Перепишите новые корзины.
    for(int i=0; i < self_records.size(); i++) {
        this->insert(self_records[i]);
    }

    // 5. [Вставка переполнения] Используйте алгоритм I для вставки записей переполнения, 
    // отложенных в S1.2 или S3 -- если они есть.
    for(int i=0; i < self_overflows.size(); i++) {
        self_overflows[i]->delOverflowBucket();
        self_overflows[i]->delNextRecord();
        self_overflows[i]->delPrevRecord();
        this->insert(self_overflows[i]);
    }

    for(int i=0; i < outer_overflows.size(); i++) {
        outer_overflows[i]->delOverflowBucket();
        outer_overflows[i]->delNextRecord();
        outer_overflows[i]->delPrevRecord();
        this->insert(outer_overflows[i]);
    }

    logs->downLevel();

    this->print_table();
    print("=================== \\split("+ std::to_string(splitPointer) +") ===================\n\n");
}



/**
 * Группировка
 */
void CascadeHashing::grouping() {
    double g = this->g();
    double p = this->p();
    int    t = this->t;
    int    start = (g*p-t);
    int    end   = (g*p-1);
    print("=================== grouping("+std::to_string(start)+" - "+std::to_string(end)+") ===================");
    if(this->buckets.size() > 1) {
        std::vector<Record*> self_records;
        std::vector<Record*> self_overflows;
        std::vector<Record*> outer_overflows;

        // 1. [Сканировать корзины, которые нужно сгруппировать). Пусть B -- 
        //    номер корзины gp-t, ..., gp-1. Для каждой корзины сделайте 
        for(int i=start; i <= end; i++) {
            int page   = this->page(i);
            Bucket *B  = this->buckets[ page ];
            Bucket *_B = NULL;
            Bucket *B0 = NULL;
            Bucket *outerPrimaryBucket = NULL;
            Record *k  = NULL;
            Record *_k = NULL;
            Record * _B_lastOverflowRecord  = NULL;

            // Извлекаем собственные записи
            Record *currRecord = B->getFirstSelfRecord();
            if(currRecord != NULL) {
                do {
                    B->extraction(currRecord);
                    self_records.push_back(currRecord);
                    currRecord = B->getFirstSelfRecord();
                } while(currRecord != NULL);
            }

            // 1.1 [Чтение] Прочтите корзину B. (Она может быть любого типа, кроме c2, c3 и c4.)
            std::vector <int> delBucks;
            int bucketType = B->getType();

            print("Группировка (п.1.1): удяляется корзина типа '"+ Bucket::getStringType(bucketType) +"'");

            switch(bucketType) {
                case Bucket::type_a:
                case Bucket::type_b:
                    // не требует доп. действий
                    break;
                
                // 1.2 [Чтение собственного переполнения] Если B имеет тип c1, то все корзины переполнения 
                //     в цепочке читаются, записи переполнения удаляются (но сохраняются позже!), а корзины 
                //     переписываются с заполнением пространства, помеченным как пустое. (Такая ситуация 
                //     должна быть редкой.) 
                case Bucket::type_c1:
                    currRecord = B->getFirstOverflowRecord();
                
                    do {
                        currRecord->getOverflowBucket()->extraction(currRecord);
                        self_overflows.push_back(currRecord);
                        currRecord = currRecord->getNextRecord();
                    } while(currRecord != NULL);

                    B->delFirstOverflowRecord();
                    break;

                case Bucket::type_c2:
                case Bucket::type_c3:
                case Bucket::type_c4:
                    this->die("Группировка (п.1.1): Удяляемая корзина не может иметь тип 'c2', 'c3', 'c4'");
                    break;

                // 1.3 [Перемещение ’внешнего’ переполнения] Если B имеет тип d, e1 или e2, тогда
                case Bucket::type_e1:
                case Bucket::type_e2:
                case Bucket::type_d:
                    // 1.3.1 Если B имеет тип e2, тогда удалите связь B из цепочки, следуйте за цепочкой 
                    //       до конца и пусть B' <-- последняя корзина в цепочке. Если B имеет тип d или e1, 
                    //       то пусть B' <-- предшественник B в цепочке.
                    currRecord = B->getFirstOuterOverflowRec();
                    outerPrimaryBucket = currRecord->getPrimaryBucket();

                    if(bucketType == Bucket::type_e2) {
                        _B = currRecord->getPrimaryBucket()->getLastOverflowRecord()->getOverflowBucket();

                        if(_B == NULL)
                            this->die("Группировка (п.1.3): невозможно найти последнюю корзину 'внешнего' переполнения в цепочке.");
                    }
                    else { // d, e1
                        _k = currRecord->getPrevRecord();
                        if(_k != NULL) // Первая запись в цепочке
                            _B = _k->getOverflowBucket();
                        else
                            _B = NULL;
                    }

                    // Извлечем все 'внешние' переполнения из корзины
                    do {
                        // Record *next = currRecord->getNextRecord();
                        B->extraction(currRecord);
                        outer_overflows.push_back(currRecord);
                        currRecord = currRecord->getNextRecord();
                    } while(currRecord != NULL && i == currRecord->getOverflowBucket()->getBucketNumber());

                    if(_B != NULL)
                        _B_lastOverflowRecord = _B->getLastOverflowRecord();
                    else
                        _B_lastOverflowRecord = NULL;

                    delBucks = {i};
                    while(!outer_overflows.empty()) {
                        // 1.3.2 Выберите новую корзину переполнения B0. Если ничего не найдено, то воссоздайте 
                        //       первоначальную ситуацию, напишите предупреждение обслуживающему персоналу и 
                        //       завершите работу.
                        B0 = this->getFreeOverflowBucket(delBucks);

                        if(B0 != NULL) {
                            // 1.3.3 Вставьте как можно больше записей переполнения из B в B0, свяжите B0 с B' и 
                            //       пусть B' <-- B0. Повторяйте G1.3.2--G1.3.3 до тех пор, пока все записи 
                            //       переполнения в B не будут восстановлены.

                            while(B0->returnEmptySpace() && !outer_overflows.empty()) {
                                Record *outer_rec = outer_overflows.back();
                                B0->add(outer_rec);
                                outer_rec->setOverflowBucket(B0);
                                if(_B_lastOverflowRecord == NULL) {
                                    outer_rec->delPrevRecord();
                                    outerPrimaryBucket->setFirstOverflowRecord(outer_rec);
                                    // _B_lastOverflowRecord = outer_rec;
                                }
                                else {
                                    outer_rec->setPrevRecord(_B_lastOverflowRecord);
                                    _B_lastOverflowRecord = outer_rec;
                                }
                                outer_rec->delNextRecord();
                                outer_overflows.pop_back();
                            }
                        }
                        else 
                            this->die("Группировка (п.1.3.2): не удалось найти свободной корзины переполнения для группировки.");
                    }
                    break;
                
                default:
                    this->die("Группировка (п.1.1): неправильный тип корзины для группировки -- "+ std::to_string(bucketType));
            }

            // 2 [Уменьшение] Пусть p <-- p-1. (Удаление корзины)
            if(i != start) {
                this->delBucket(i);
                this->M--;
            }
            else {
                int start_pg = this->page(i);
                this->buckets[start_pg]->setBucketNumber(this->getMinBucketNum() - 1);
            }
            
        }

        // 3 [Перехеширование] Перехешируйте все записи, включая собственные записи переполнения 
        //   из G1.2. (Записи хешируются в новых корзинах p, ..., p+s-1.)
        std::vector<Record*> all_records;
        Record *r = NULL;
        for(int i=0; i < this->M; i++) {
            for(int j=0; j < Bucket::size; j++) {
                r = this->buckets[i]->extraction(j);

                if(r != NULL) {
                    r->delOverflowBucket();
                    r->delNextRecord();
                    r->delPrevRecord();

                    all_records.push_back(r);
                }
            }

            this->buckets[i]->delFirstOverflowRecord();
        }

        for(int i=0; i < all_records.size(); i++) {
            this->insert(all_records[i]);
        }

        // 4 [Перезапись] Для каждой из новых корзин, пусть это будет B', сделайте
        // 4.1 [Перезапись основной корзины] Перезапишите B'.
        // 4.2 [Перезапись переполнения] Сохраните записи переполнения из B', используя ту же процедуру, 
        //     что и G1.3.2--G1.3.3.
        for(int i=0; i < self_records.size(); i++) {
            logs->upLevel();
            this->insert(self_records[i]);
            logs->downLevel();
        }

        for(int i=0; i < self_overflows.size(); i++) {
            logs->upLevel();
            self_overflows[i]->delOverflowBucket();
            self_overflows[i]->delNextRecord();
            self_overflows[i]->delPrevRecord();
            this->insert(self_overflows[i]);
            logs->downLevel();
        }

        this->print_table();
    }
    else
        print("Группировка: хеш таблица (= "+std::to_string(this->buckets.size())+") не нуждается в группировке");

    print("=================== \\grouping() ===================\n\n");
    // die("1140");
}





// /**
//  * Печать логов
//  */
// void CascadeHashing::print(std::string msg) {
//     this->logs->print(msg);
// }



/**
 * Остановка выполнения программы
 */
void CascadeHashing::die(std::string msg) {
    print(msg);
    this->print_table();
    exit(1);
}



/**
 * Распечатка корзин
 */
void CascadeHashing::print_table() {
    std::vector<int> bucketNums = this->getSortBucketNums();
    std::string fTabs = getTabs(-1);
    std::string tabs = getTabs();
    std::string msg = "";
    std::string ovBucket;

    for(int i=0; i < bucketNums.size(); i++) {
        int bNum = bucketNums[i];
        int page = this->page( bNum );
        Record *frstOverflowRec = this->buckets[page]->getFirstOverflowRecord();
        
        msg += (i==0 ? fTabs : tabs) + "Основная корзина [page => " + std::to_string(page) + ", num => " + std::to_string(bNum) + "]:  -->  '" + (frstOverflowRec != NULL ? frstOverflowRec->getKey() : "-----") + "'\n";
        std::vector<RecordInfo> Records =  this->buckets[page]->getRecords();
        for(int j=0; j < Records.size(); j++) {
            msg += tabs + "\t" + std::to_string(Records[j].num) + ": " + Records[j].key + " => " + Records[j].value;

            if(Records[j].isEmpty)
                msg += "\n";
            else {
                ovBucket = (Records[j].oBucket != -1 ? std::to_string(Records[j].oBucket) : "--");
                msg += " (Primary bucket: " + std::to_string(Records[j].pBucket) + "; Overflow bucket: " + ovBucket + ")\t(" + Records[j].prev + " <-- * --> " + Records[j].next + ")\n";
            }
        }
    }

    print(msg);
}