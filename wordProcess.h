#include <iostream>
#include <fstream>
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
        void preProcess(std::ifstream &stream);
        void process();
        void printData();
        void printResult();
        void outputData(std::ofstream &ostream);

    protected:
        bool isDigit(const char ch);
        bool isLetter(const char ch);
        bool setKeyword(std::vector<char>::iterator &iter,
                const std::string keyword, const int define);
        bool iterPeek(std::vector<char>::iterator &iter, 
            const std::vector<char>::iterator end, char &ret);

    private:
        std::vector<serial> result;
        std::vector<char> data;
};

