#ifndef  RECORD_H
#define  RECORD_H

#include<stdlib.h>
#include<iostream>
#include <fstream>
#include <string>
#include "Bucket.h"


class Record {
    private:
        std::string key;
        std::string value;
        
        Bucket *primaryBucket;
        Bucket *overflowBucket;
        Record *preview;
        Record *next;

    
    public:
        Record(std::string key, std::string value);
        ~Record();

        std::string getKey();
        std::string getValue();

        int         getPrimaryBucketNumber();
        bool        isOverflowRecord();
        void        setOverflowBucket(Bucket *bucket);
        Bucket*     getOverflowBucket();
        int         getOverflowBucketNumber();
        void        delOverflowBucket();
        void        setPrimaryBucket(Bucket *bucket);
        Bucket*     getPrimaryBucket();
        void        setPrevRecord(Record *record);
        Record*     getPrevRecord();
        void        delPrevRecord();
        void        setNextRecord(Record *record);
        Record*     getNextRecord();
        void        delNextRecord();

        void        removeFromChain();
};

#endif 