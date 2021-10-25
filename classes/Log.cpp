#include "Log.h"
#ifndef  LOG_CPP 
#define  LOG_CPP

/**
 * Конструктор
 * @param std::string file -- путь к файлу логов
 */
Log::Log(std::string dir) {
    // std::string carrent_time = std::string(this->getCurrentTime());
    // this->file = dir + carrent_time + "_log";
    this->file = dir + "_log";
    this->level = 0;

    if(!std::ifstream(file.c_str()).good()) { // Проверка файла на существование
        this->_print("Создан файл логгирования");
        this->_print("\n- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \n", true);
    }
    else {
        this->_print("\n- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ", true);
        this->_print("Инициализация логгирования");
    }
}



/**
 * Повышение уровня вложенности
 */
void Log::upLevel() {
    this->level++;
}



/**
 * Понижение уровня вложенности
 */
void Log::downLevel() {
    if(this->level > 0)
        this->level--;
}



/**
 * Возвращает кол-во отступов для текущего уровня
 */
std::string Log::getTabs(int correct) {
    std::string tabs = "";

    for(int i=0; i < (this->level + correct); i++)
        tabs += "   ";

    return tabs;
}



/**
 * Внутренний метод печати
 * @param std::string msg -- сообщение для печати
 */
void Log::_print(std::string msg, bool onlyMsg) {
        std::string time = std::string(this->getCurrentTime());
        std::string tabs = this->getTabs();
        
        std::ofstream newFile;
        newFile.open(this->file.c_str(), std::ios::app);

        if(onlyMsg)
            newFile << msg << "\n";
        else
            newFile << time << tabs << msg << "\n";
            
        newFile.close();

        std::cout << tabs << msg << "\n";
}



/**
 * Метод-обертка для печати 
 * @param std::string msg -- сообщение для печати
 */
void Log::print(std::string msg) {
    this->_print(msg);
}



/**
 * Возвращает текущее время
 * @return char* текущее время в виде строки
 */
std::string Log::getCurrentTime() {
    setlocale(LC_ALL, "rus_rus.866"); //Rus
    
    char buffer[20] = "0000-00-00 00:00:00";
    time_t seconds = time(NULL);
    tm* timeinfo = localtime(&seconds);
    const char* format = "%Y-%m-%d %H:%M:%S";
    strftime(buffer, 20, format, timeinfo);

    return std::string(buffer);
}



/*
 *************************************************
 ************  Инициализация логов   *************
 *************************************************
*/

Log *logs = new Log("./logs/");

/**
 * Печать логов
 */
void print(std::string msg) {
    logs->print(msg);
}



/**
 * Остановка выполнения программы
 */
void die(std::string msg) {
    logs->print(msg);
    exit(1);
}


std::string getTabs(int correct=0) {
    return logs->getTabs(correct);
}

#endif