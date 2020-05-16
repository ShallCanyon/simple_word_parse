#include <fstream>
#include <iostream>
#include "wordProcess.h"

int main()
{
    std::fstream file;
    file.open("data.dat", std::ios::in);
    char buf;
    while(!file.eof())
    {
        //file.getline(buf, 256, '\n');
        file.get(buf);
        if(buf!='\n' && buf!='\t')
            std::cout << buf;
    }
    file.close();
    return 0;
}