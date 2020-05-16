#include "wordProcess.h"


WP::WP()
{

}

WP::~WP()
{

}

/* skip waste line breaks ,tabs and whitespaces */
void WP::preProcess(FILE *file)
{
    bool inQuote = false;
}

bool WP::isDigit(char ch)
{
    if(ch>='0' && ch<='9')
        return true;
    else
        return false;
}

bool WP::isLetter(char ch)
{
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        return true;
    else
        return false;
}

void process()
{
    
}