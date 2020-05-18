#include <unistd.h>

#include "wordProcess.h"

int main(int argc, char* argv[])
{
    if(argc==1)
    {
        printf("filePhrase: A source file needs to be read\n");
        printf("Try 'filePhrase -?'\n");
        return 0;
    }
    WP *wp = new WP();
    std::ifstream file;
    std::ofstream outfile;
    int o;
    const char *optstring = "r:o:";
    while ((o = getopt(argc, argv, optstring))!=-1)
    {
        switch (o)
        {
        case 'r':
            //printf("infile %s opened\n", optarg);
            file.open(optarg, std::ios::in);
            break;
        case 'o':
            //printf("outfile %s opened\n", optarg);
            outfile.open(optarg, std::ios::out|std::ios::trunc);
            break;
        case '?':
            printf("Usage: filePhrase [-r][-o]\n");
            printf("  -r file that needs to be process\n");
            printf("  -o file that records result\n");
            break;
        default:
            printf("Wrong arguments");
            return -1;
            break;
        }
    }
    if(file.is_open())
    {
        wp->preProcess(file);
        wp->printData();
        wp->process();
        if(!outfile.is_open())
            wp->printResult();
        else
        {
            wp->outputData(outfile);
            outfile.close();
        }
        file.close();
    }
    return 0;
}