#include <iostream>
#include <fstream>
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
        void preProcess(FILE *file);
        void process();

    private:
        serial result[1024];
        char *data;
        
};
