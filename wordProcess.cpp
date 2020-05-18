#include "wordProcess.h"

WP::WP()
{
}

WP::~WP()
{
    result = nullptr;
}

/* skip waste line breaks ,tabs, annotations and whitespaces */
void WP::preProcess(std::fstream &stream)
{
    bool inAnnotation = false;
    bool inString = false;
    std::string line;
    char ch;
    while (stream.peek() != EOF)
    {
        stream.get(ch);
        switch (ch)
        {
        /* skip annotations */
        case '/':
            if(!inAnnotation)
            {
                if (stream.peek() == '*')
                {
                    inAnnotation = true;
                    //printf("\ninAnnotation: %d\n", inAnnotation);
                    // skip '/'
                    stream.get();
                    // skip '*'
                    stream.get(ch);
                }
                else if (stream.peek() == '/')  // skip whole line
                {
                    while (ch != '\n')
                        stream.get(ch);
                    // skip the last '\n' to save tiny time
                    stream.get(ch);
                }
            }
            break;
        case '*':
            if (inAnnotation && stream.peek() == '/')
            {
                inAnnotation = false;
                //printf("\ninAnnotation: %d\n", inAnnotation);
                // skip '*'
                stream.get();
                // skip '/'
                stream.get(ch);
            }
            break;
        /* mark in string */
        /* WARN: the ' only be detected once */
        case '\'':
        case '\"':
            if(!inAnnotation)
            {
                inString = !inString;
                /*if (inString)
                    inString = false;
                else
                    inString = true;*/
                //printf("\ninString: %d\n", inString);
            }
            break;
        /* skip white spaces */
        case ' ':
            if (!inAnnotation && !inString && ch == ' ')
            {
                /* WARN: wrong whitespace skip cause weird result */
                /* skip to last whitespace */
                while (stream.peek() == ' ')
                    stream.get();
                continue;
            }
            break;
        default:
            break;
        }

        /*if(ch=='/' && !inAnnotation)
        {
            
            if(stream.peek()=='*')
            {
                inAnnotation = true;
                printf("\ninAnnotation: %d\n", inAnnotation);
                stream.get();
                stream.get(ch);
                printf("---/* detected---");
            }
            else if (stream.peek() == '/')
            {
                while(ch!='\n')
                    stream.get(ch);
                // skip the last \n to save tiny time
                stream.get(ch);
            }
        }
        if(ch=='*' && inAnnotation)
        {
            if(stream.peek()=='/')
            {
                inAnnotation = false;
                printf("\ninAnnotation: %d\n", inAnnotation);
                stream.get();
                stream.get(ch);
            }
        }
        
        if(!inAnnotation && (ch=='\"' || ch=='\''))
        {
            //inString = !inString;
            if(inString)
                inString = false;
            else
                inString = true;
            printf("\ninString: %d\n", inString);
            //printf("\ninString: %d\n", inString);
        }
        
        if(!inAnnotation && !inString && ch==' ')
        {
            while(ch==' ')
                stream.get(ch);
        }
        */

        /* skip line breaks and tabs */
        if (ch != '\n' && ch != '\t' && !inAnnotation)
            //std::cout << ch;
            data.push_back(ch);
    }
}

void WP::printData()
{
    for (auto iter = data.begin(); iter != data.end();iter++)
    {
        std::cout << *iter;
    }
    std::cout << std::endl;
}

bool WP::isDigit(char ch)
{
    if (ch >= '0' && ch <= '9')
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

void WP::process()
{
}