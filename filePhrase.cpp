#include "wordProcess.h"

int main()
{
    WP *wp = new WP();
    std::fstream file;
    file.open("data.dat", std::ios::in);
    
    if(file.is_open())
    {
        wp->preProcess(file);
        /*std::string line;
        while (!file.eof())
        {
            //file.getline(buf, 256);
            getline(file, line);
            //if(line.at(0)!='\n')
            std::cout << line;
        }*/
        printf("\n");
        file.close();
    }
    
    return 0;
}