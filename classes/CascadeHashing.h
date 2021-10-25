#ifndef  CASCADEHASHING_H
#define  CASCADEHASHING_H

#include<stdlib.h>
#include<iostream>
#include <string>
#include <cstdio>
#include <map>
#include <vector>
#include <math.h>
#include <limits>
#include "Log.cpp"
#include "Bucket.cpp"


class CascadeHashing {
    private:
        int s; // Кол-во разделяемых корзин
        int t; // Кол-во создаваемых корзин при разделении
        int M; // Размер таблицы (Кол-во корзин)
        int b; // Размер корзины (Блок коэффициент -- b)

        std::vector<Bucket*> buckets; // Индексом корзины вляется её фактический номер (т.е. номер страницы)

        Bucket * newBucket(int num);
        void     addBucket(int num);
        void     delBucket(int num);
        std::vector<int> getBucketNums();
        std::vector<int> getSortBucketNums();
        int      getMinBucketNum();
        int      getMaxBucketNum();
        double   g();
        int      key_to_int(std::string key);
        double   d(std::string key);
        double   p();
        int      L();
        int      h(std::string);
        int      countAllRecords();
        double   f();
        double   f_max();
        double   f_min();
        bool     needSplit();
        bool     needGrouping();
        int      page(int n);

        Bucket * getFreeOverflowBucket(std::vector<int> not_in);
        Bucket * getFreeOverflowBucket();
        void     _insert(Record *Rec);

    public:
        CascadeHashing();
        ~CascadeHashing();
        
        void     insert(std::string key, std::string value);
        void     insert(Record *Rec);
        void     reorganizationChain(int bNum);
        void     deleteRecord(std::string key);
        void     split();
        void     grouping();

        void     die(std::string msg);
        void     print_table();
};
#endif 