#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <vector>
#include "wordDefine.h"

struct serial
{
    int syn;
    char token[64];
};

class WP{
    public:
        WP();
        ~WP();
        bool isDigit(char ch);
        bool isLetter(char ch);
        void preProcess(std::fstream &stream);
        void process();

    private:
        serial *result;
        std::vector<char> data;
};

