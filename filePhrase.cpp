#include "wordProcess.h"

int main()
{
    WP *wp = new WP();
    std::fstream file;
    file.open("data.dat", std::ios::in);
    
    if(file.is_open())
    {
        wp->preProcess(file);
        wp->printData();
        wp->process();
        wp->printResult();
    }
    
    return 0;
}