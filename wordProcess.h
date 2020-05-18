#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <vector>
#include "wordDefine.h"

struct serial
{
    int syn;
    //char token[64];
    std::string token;
};

class WP{
    public:
        WP();
        ~WP();
        void preProcess(std::fstream &stream);
        void process();
        void printData();
        void printResult();

    protected:
        bool isDigit(char ch);
        bool isLetter(char ch);
        bool setKeyword(std::vector<char>::iterator &iter,
                std::string keyword, unsigned int length, int define);

    private:
        std::vector<serial> result;
        std::vector<char> data;
};

