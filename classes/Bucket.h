#ifndef  BUCKET_H
#define  BUCKET_H

#include <stdlib.h> /* malloc, free, rand */
#include <stdio.h> /* printf, scanf, NULL */
#include <iostream>
#include <fstream>
#include <string>

#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include <locale> 
#include "Log.cpp"

class Record; // Объявляем класс записи
struct RecordInfo {
        bool         isEmpty;      // Является ли данная запись пустой
        std::string  key;   // Ключ записи
        std::string  value; // Значение записи
        int          num;
        int          pBucket;       // Концептуальный номер основной корзины
        int          oBucket;       // Концептуальный номер корзины переполнения (если запись содержится в корзине переполнения)
        std::string  prev;  // Ключ предыдущей записи
        std::string  next;  // Ключ следующей  записи
};

class Bucket {
    private:
        int number; // Номер корзины
        Record** records; // Массив записей
        Record*  firstOverflowRecord; // Ссылка на первую запись из корзины переполнения
        Record*  _getOuterOverflowRec();

    public:
        static int const size    = 4; // b - блок-коэффициент (кол-во записей, которое может храниться в корзине)
        static int const type_a  = 0;  // Константа коризны типа 'a'
        static int const type_b  = 1;  // Константа коризны типа 'b'
        static int const type_c1 = 2; // Константа коризны типа 'c1'
        static int const type_c2 = 3; // Константа коризны типа 'c2'
        static int const type_c3 = 4; // Константа коризны типа 'c3'
        static int const type_c4 = 5; // Константа коризны типа 'c4'
        static int const type_d  = 6;  // Константа коризны типа 'd'
        static int const type_e1 = 7; // Константа коризны типа 'e1'
        static int const type_e2 = 8; // Константа коризны типа 'e2'

        static std::string getStringType(int iType) {
            std::string type = "undefined ("+ std::to_string(iType) +")";

            switch (iType) {
                case Bucket::type_a:
                    type = "a";
                    break;
                case Bucket::type_b:
                    type = "b";
                    break;
                case Bucket::type_c1:
                    type = "c1";
                    break;
                case Bucket::type_c2:
                    type = "c2";
                    break;
                case Bucket::type_c3:
                    type = "c3";
                    break;
                case Bucket::type_c4:
                    type = "c4";
                    break;
                case Bucket::type_d:
                    type = "d";
                    break;
                case Bucket::type_e1:
                    type = "e1";
                    break;
                case Bucket::type_e2:
                    type = "e2";
                    break;
            }

            return type;
        }


        Bucket(int number);
        ~Bucket();

        int       countRecords();
        int       countSelfRecords();
        int       returnEmptySpace();
        int       getBucketNumber();
        void      setBucketNumber(int number);
        int       countOuterOverflow();
        bool      issetOuterOverflow();
        Record*   getFirstOuterOverflowRec();
        Record*   getLastOuterOverflowRec();
        Record*   getLastOverflowRecord();
        Record*   getFirstSelfRecord();
        Record*   findRecord(std::string key);
        Record*   findRecordInBucket(std::string key);
        Record*   findRecordInOverflow(std::string key);
        void      setFirstOverflowRecord(Record *r);
        Record  * getFirstOverflowRecord();
        void      delFirstOverflowRecord();
        int       getAmountEmptySpace(); //getEmptyNumber
        bool      add(Record *record);
        bool      del(std::string key);
        Record  * extraction(int num);
        Record  * extraction(std::string key);
        Record  * extraction(Record * rec);

        int       getType(bool isOverflow=false);

        std::vector<RecordInfo>  getRecords();
};

#endif 