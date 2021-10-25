#ifndef  LOG_H
#define  LOG_H

#include<stdlib.h>
//#include<time.h>
#include<iostream>
#include <fstream>
#include <string>



class Log {
    private:
        std::string file;
        int level;
        void _print(std::string msg, bool onlyMsg=false);
    
    public:
        std::string getTabs(int correct=0);
        Log(std::string dir="");
        void print(std::string msg);
        void upLevel();
        void downLevel();

        std::string getCurrentTime();
};

#endif 