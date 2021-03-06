#include<stdlib.h>
#include<iostream>
#include <string>
#include <cstdio>
#include <map>
#include "./classes/CascadeHashing.cpp"


int random(int from,int to) {
    int result = 0;

    srand(time(NULL));

    if (from > 0) 
        result = from + rand() % (to - from);
    else 
        result = from + rand() % (abs(from) + to);

    return result;
}


int main () {
    setlocale(LC_ALL, "rus_rus.866");
    std::map<std::string,std::string> countries = {
        { "Австралия", "{'Столица':'Канберра',      'Часть света':'Австралия',    'Код':'AUS'}" },
        { "Австрия",   "{'Столица':'Вена',          'Часть света':'Европа',       'Код':'AUT'}" },
        { "Барбадос",  "{'Столица':'Бриджтаун',     'Часть света':'Сев. Америка', 'Код':'BRB'}" },
        { "Бельгия",   "{'Столица':'Брюссель',      'Часть света':'Европа',       'Код':'BEL'}" },

        {"Болгария",       "{'Столица':'София',         'Часть света':'Европа',       'Код':'BGR'}"},
        {"Бурунди",        "{'Столица':'Бужумбура',     'Часть света':'Африка',       'Код':'BDI'}"},
        {"Ватикан",        "{'Столица':'Ватикан',       'Часть света':'Европа',       'Код':'VAT'}"},
        {"Великобритания", "{'Столица':'Лондон',        'Часть света':'Европа',       'Код':'GBR'}"},

        {"Вьетнам",   "{'Столица':'Ханой',         'Часть света':'Азия',         'Код':'VNM'}"},
        {"Гаити",     "{'Столица':'Порт-о-Пренс',  'Часть света':'Сев. Америка', 'Код':'GMB'}"},
        {"Гватемала", "{'Столица':'Гватемала',     'Часть света':'Сев. Америка', 'Код':'GTM'}"},
        {"Германия",  "{'Столица':'Берлин',        'Часть света':'Европа',       'Код':'DEU'}"},

        {"Греция",     "{'Столица':'Афины',         'Часть света':'Европа',       'Код':'GRC'}"},
        {"Дания",      "{'Столица':'Копенгаген',    'Часть света':'Европа',       'Код':'DNK'}"},
        {"Доминикана", "{'Столица':'Санто-Доминго', 'Часть света':'Сев. Америка', 'Код':'DOM'}"},
        {"Египет",     "{'Столица':'Каир',          'Часть света':'Африка',       'Код':'EGY'}"},

        {"Зимбабве", "{'Столица':'Хараре',        'Часть света':'Африка',       'Код':'ZWE'}"},
        {"Израиль",  "{'Столица':'Иерусалим',     'Часть света':'Азия',         'Код':'ISR'}"},
        {"Индия",    "{'Столица':'Нью-Дели',      'Часть света':'Азия',         'Код':'IND'}"},
        {"Ирландия", "{'Столица':'Дублин',        'Часть света':'Европа',       'Код':'IRL'}"},

        {"Исландия",  "{'Столица':'Рейкьявик',     'Часть света':'Европа',       'Код':'ISL'}"},
        {"Испания",   "{'Столица':'Мадрид',        'Часть света':'Европа',       'Код':'ESP'}"},
        {"Казахстан", "{'Столица':'Астана',        'Часть света':'Азия',         'Код':'KAZ'}"},
        {"Канада",    "{'Столица':'Оттава',        'Часть света':'Сев. Америка', 'Код':'CAN'}"},

        {"Кипр",  "{'Столица':'Никосия',       'Часть света':'Европа',       'Код':'CYP'}"},
        {"Куба",  "{'Столица':'Гавана',        'Часть света':'Сев. Америка', 'Код':'CUB'}"},
        {"Литва", "{'Столица':'Вильнюс',       'Часть света':'Европа',       'Код':'LTU'}"},
        {"Мали",  "{'Столица':'Бамако',        'Часть света':'Африка',       'Код':'MLI'}"},

        {"Монголия",   "{'Столица':'Улан-Батор',    'Часть света':'Азия',         'Код':'MNG'}"},
        {"Нидерланды", "{'Столица':'Амстердам',     'Часть света':'Европа',       'Код':'NLD'}"},
        {"Норвегия",   "{'Столица':'Осло',          'Часть света':'Европа',       'Код':'NOR'}"},
        {"ОАЭ",        "{'Столица':'Абу-Даби',      'Часть света':'Азия',         'Код':'ARE'}"},

        {"Панама",  "{'Столица':'Панама',        'Часть света':'Сев. Америка', 'Код':'PAN'}"},
        {"Польша",  "{'Столица':'Варшава',       'Часть света':'Европа',       'Код':'POL'}"},
        {"Россия",  "{'Столица':'Москва',        'Часть света':'Европа, Азия', 'Код':'RUS'}"},
        {"Румыния", "{'Столица':'Бухарест',      'Часть света':'Европа',       'Код':'ROU'}"},

        {"Сербия",  "{'Столица':'Белград',       'Часть света':'Европа',       'Код':'SRB'}"},
        {"США",     "{'Столица':'Вашингтон',     'Часть света':'Сев. Америка', 'Код':'USA'}"},
        {"Тайвань", "{'Столица':'Тайбэй',        'Часть света':'Азия',         'Код':'TWN'}"},
        {"Турция",  "{'Столица':'Анкара',        'Часть света':'Азия',         'Код':'TUR'}"},

        {"Украина",   "{'Столица':'Киев',          'Часть света':'Европа',       'Код':'UKR'}"},
        {"Уругвай",   "{'Столица':'Монтевидео',    'Часть света':'Южн. Америка', 'Код':'URY'}"},
        {"Финляндия", "{'Столица':'Хельсинки',     'Часть света':'Европа',       'Код':'FIN'}"},
        {"Франция",   "{'Столица':'Париж',         'Часть света':'Европа',       'Код':'FRA'}"},

        {"Чехия",  "{'Столица':'Прага',         'Часть света':'Европа',       'Код':'CZE'}"},
        {"Швеция", "{'Столица':'Стокгольм',     'Часть света':'Европа',       'Код':'SWE'}"},
        {"Япония", "{'Столица':'Токио',         'Часть света':'Азия',         'Код':'JPN'}"},
    };

    print("КАСКАДНОЕ ХЕШИРОВАНИЕ\n\n\n");
    CascadeHashing *hasher = new CascadeHashing();

    if(true) { // Прямой перебор
        for(auto& item : countries) {
            hasher->insert(item.first, item.second);
        }

        std::cout << "\n\n\n\n\n\n\n\n---------------------------------------------------------\n\n\n\n\n\n\n" << std::endl;

        for(auto& item : countries) {
            hasher->deleteRecord(item.first);
        }
    }
    else { // Случайный перебор
        std::vector<std::string> keys;

        for(auto& item : countries) {
            keys.push_back(item.first);
        }

        int j = 100;
        int i = -1;
        while(keys.size() && (j-- > 0)) {
            i = random(0, keys.size());

            hasher->insert(keys[i], countries[keys[i]]);

            keys.erase(keys.begin() + i);
        }
    }
    
    hasher->print_table();

    std::cout << "Нажмите любую клавишу для завершения работы программы...";
    getchar();
}