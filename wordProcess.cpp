#include "wordProcess.h"

WP::WP()
{
}

WP::~WP()
{
    result.clear();
    data.clear();
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
    if(data.empty())
        return;
    for (auto iter = data.begin(); iter != data.end();iter++)
    {
        std::cout << *iter;
    }
    std::cout << std::endl;
}

void WP::printResult()
{
    if(result.empty())
        return;
    for (auto iter = result.begin(); iter != result.end(); iter++)
    {
        std::cout << "<" << iter->syn << ", " << iter->token << ">" << std::endl;
        
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

bool WP::setKeyword(std::vector<char>::iterator &iter,
                   std::string keyword, unsigned int length, int define)
{
    serial temp;
    int step = 0;
    bool match = true;
    while(step < length - 1)
    {
        step++;
        iter++;
        if (*iter != keyword.at(step))
            match = false;
        /* if is not a keyword, return pointer back */
        if (!match)
        {
            while (step > 0)
            {
                iter--;
                step--;
            }
            break;
        }
    }
    if (match)
    {
        temp.syn = define;
        temp.token = keyword;
        result.push_back(temp);
    }
    return match;
}

    void WP::process()
{
    for (auto iter = data.begin(); iter != data.end(); iter++)
    {
        /* deal with digit */
        if (isDigit(*iter))
        {
            int a, b, i = 0;
            std::string token;
            //int i = 0;
            a = (int)*iter - '0';
            for (iter++; iter != data.end(); iter++)
            {
                if (isDigit(*iter))
                {
                    b = (int)*iter - '0';
                    a = a * 10 + b;
                }
                else
                    break;
            }
            token = std::to_string(a);
            serial temp;
            temp.syn = _digit;
            temp.token = token;
            result.push_back(temp);
        }

        /* deal with letter */
        if(isLetter(*iter))
        {
            int counter = 0;
            serial temp;
            bool isKeyword = true;
            /* find keywords */
            switch(*iter)
            {
                /* if, include, int */
                case 'i':
                    /*iter++;
                    counter++;
                    if(*iter == 'f')
                    {
                        iter--;
                        counter = 0;
                        isKeyword = setKeyword(iter, "if", 2, _if);
                    }
                    else if(*iter=='n')
                    {
                        iter++;
                        counter++;
                        if(*iter=='c')
                        {
                            iter -= 2;
                            counter = 0;
                            isKeyword = setKeyword(iter, "include", 7, _include);
                        }
                        else if(*iter=='t')
                        {
                            iter -= 2;
                            counter = 0;
                            isKeyword = setKeyword(iter, "int", 3, _int);
                        }
                    }*/
                    if (!(setKeyword(iter, "if", 2, _if) ||
                        setKeyword(iter, "include", 7, _include) ||
                        setKeyword(iter, "int", 3, _int)))
                        isKeyword = false;
                    break;
                /* char */
                case 'c':
                    isKeyword = setKeyword(iter, "char", 4, _char);
                    break;
                /* else */
                case 'e':
                    isKeyword = setKeyword(iter, "else", 4, _else);
                    break;
                /* main */
                case 'm':
                    isKeyword = setKeyword(iter, "main", 4, _main);
                    break;
                /* for */
                case 'f':
                    isKeyword = setKeyword(iter, "for", 3, _for);
                    break;
                /* while */
                case 'w':
                    isKeyword = setKeyword(iter, "while", 5, _while);
                    break;
                /* return */
                case 'r':
                    isKeyword = setKeyword(iter, "return", 6, _return);
                    break;
                default:
                    break;
            }
            
            /* TODO: letter(letter|digit)* */
            if (!isKeyword)
            {
                temp.syn = _letter;
                temp.token = *iter;
                result.push_back(temp);
            }
        }
    }
}