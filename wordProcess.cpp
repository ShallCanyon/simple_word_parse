#include "wordProcess.h"

WP::WP()
{
}

WP::~WP()
{
    result.clear();
    data.clear();
}

/*	
	peek for next data in vector 
	while not move the pointer of iterator,
	return true if next iterator don't reach the end,
	vice versa
*/
bool WP::iterPeek(std::vector<char>::iterator &iter, const std::vector<char>::iterator end, char &ret)
{
	if(++iter != end)
	{
		ret = *iter;
		iter--;
		return true;
	}
	else
	{
		iter--;
		return false;
	}
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
        std::cout << "< " << iter->syn << ",  " << iter->token << " >\n";
        
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
                   std::string keyword, int define)
{
    serial temp;
    int step = 0;
    bool match = true;
	int length = keyword.length();
	if(length>0)
	{
		while (step < length - 1)
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
    bool inQuote = false;
    for (auto iter = data.begin(); iter != data.end(); iter++)
    {
        serial temp;
        /* deal with digit */
        if (isDigit(*iter))
        {
            int a;
            std::string token;
            for (; iter != data.end(); iter++)
            {
                if (isDigit(*iter))
                {
                    a = *iter - '0';
                    token.append(std::to_string(a));
                }
                else
                    break;
            }
            temp.syn = _INT;
            temp.token = token;
            result.push_back(temp);
        }

        /* deal with letter */
        if(isLetter(*iter))
        {
            int counter = 0;
            bool isKeyword = true;
            /* find keywords */
            switch(*iter)
            {
                /* if, include, int */
                case 'i':
                    if (!(setKeyword(iter, "if", _if) ||
                        setKeyword(iter, "include", _include) ||
                        setKeyword(iter, "int", _int)))
                        isKeyword = false;
                    break;
                /* char */
                case 'c':
                    isKeyword = setKeyword(iter, "char", _char);
                    break;
                /* else */
                case 'e':
                    isKeyword = setKeyword(iter, "else", _else);
                    break;
                /* main */
                case 'm':
                    isKeyword = setKeyword(iter, "main", _main);
                    break;
                /* for */
                case 'f':
                    isKeyword = setKeyword(iter, "for", _for);
                    break;
                /* while */
                case 'w':
                    isKeyword = setKeyword(iter, "while", _while);
                    break;
                /* return */
                case 'r':
                    isKeyword = setKeyword(iter, "return", _return);
                    break;
                default:
                    isKeyword = false;
                    break;
            }
            
            /* TODO: letter(letter|digit)* */
            if (!isKeyword)
            {
                std::string ID;
                while(isLetter(*iter) || isDigit(*iter))
                {
                    ID.push_back(*iter);
                    iter++;
                }
                iter--;
                temp.syn = _ID;
                temp.token = ID;
                result.push_back(temp);
            }
        }

        /* deal with symbols */
        else
		{
			std::string token;
			char ch;
			switch (*iter)
			{
			/* doubleQuote */
			case '\"':
				/* first doubleQuote */
				temp.syn = doubleQuote;
				temp.token = "\"";
				result.push_back(temp);
				/* quoted string */
				for (iter++; *iter != '\"'; iter++)
				{
					token.push_back(*iter);
				}
				temp.syn = _STRING;
				temp.token = token;
				result.push_back(temp);
				/* second doubleQuote */
				temp.syn = doubleQuote;
				temp.token = "\"";
				result.push_back(temp);
				break;
			/* singleQuote */
			case '\'':
				/* first singleQuote */
				temp.syn = singleQuote;
				temp.token = "\'";
				result.push_back(temp);
				/* quoted string */
				for (iter++; *iter != '\''; iter++)
				{
					token.push_back(*iter);
				}
				temp.syn = _ID;
				temp.token = token;
				result.push_back(temp);
				/* second singleQuote */
				temp.syn = singleQuote;
				temp.token = "\'";
				result.push_back(temp);
				break;
			/* assign or equal */
			case '=':
				/*if(*(++iter)=='=')
				{
					temp.syn = equal;
					temp.token = "==";
					result.push_back(temp);
				}
				else
				{
					iter--;
					temp.syn = assign;
					temp.token = "=";
					result.push_back(temp);
				}*/
				if(iterPeek(iter, data.end(), ch) && ch == '=')
				{
					temp.syn = equal;
					temp.token = "==";
					result.push_back(temp);
					iter++;
				}
				else
				{
					temp.syn = assign;
					temp.token = "=";
					result.push_back(temp);
				}
				
				break;
			/* plus or selfPlus */
			case '+':
				if (iterPeek(iter, data.end(), ch) && ch == '+')
				{
					temp.syn = selfPlus;
					temp.token = "++";
					result.push_back(temp);
					iter++;
				}
				else
				{
					temp.syn = plus;
					temp.token = "+";
					result.push_back(temp);
				}
				
				break;
			/* minus or selfMinus */
			case '-':
				if (iterPeek(iter, data.end(), ch) && ch == '+')
				{
					temp.syn = selfMinus;
					temp.token = "--";
					result.push_back(temp);
					iter++;
				}
				else
				{
					temp.syn = minus;
					temp.token = "-";
					result.push_back(temp);
				}
				break;
			/* math multiply or pointer */
			case '*':
				if (iterPeek(iter, data.end(), ch) && isDigit(ch))
				{
					temp.syn = multiply;
					temp.token = "*";
					result.push_back(temp);
				}
				else
				{
					temp.syn = pointer;
					temp.token = "*";
					result.push_back(temp);
				}
				break;
			case '/':
				temp.syn = divide;
				temp.token = "/";
				result.push_back(temp);
				break;
			case '(':
				temp.syn = lParenth;
				temp.token = "(";
				result.push_back(temp);
				break;
			case ')':
				temp.syn = rParenth;
				temp.token = ")";
				result.push_back(temp);
				break;
			case '[':
				temp.syn = lSqBracket;
				temp.token = "[";
				result.push_back(temp);
				break;
			case ']':
				temp.syn = rSqBracket;
				temp.token = "]";
				result.push_back(temp);
				break;
			case '{':
				temp.syn = lCuBracket;
				temp.token = "{";
				result.push_back(temp);
				break;
			case '}':
				temp.syn = rCubracket;
				temp.token = "}";
				result.push_back(temp);
				break;
			case ',':
				temp.syn = comma;
				temp.token = ",";
				result.push_back(temp);
				break;
			case ':':
				temp.syn = colon;
				temp.token = ":";
				result.push_back(temp);
				break;
			case ';':
				temp.syn = semicolon;
				temp.token = ";";
				result.push_back(temp);
				break;
			/* _and ,refer or logicAnd */
			case '&':
				if (iterPeek(iter, data.end(), ch) && ch=='&')
				{
					temp.syn = logicAnd;
					temp.token = "&&";
					result.push_back(temp);
					iter++;
				}
				else if(isDigit(ch))
				{
					temp.syn = _and;
					temp.token = "&";
					result.push_back(temp);
				}
				else
				{
					temp.syn = refer;
					temp.token = "&";
					result.push_back(temp);
				}
				break;
			/* _or or logicOr */
			case '|':
				if (iterPeek(iter, data.end(), ch) && ch == '|')
				{
					temp.syn = logicOr;
					temp.token = "||";
					result.push_back(temp);
					iter++;
				}
				else
				{
					temp.syn = _or;
					temp.token = "|";
					result.push_back(temp);
				}
				break;
			/* xor */
			case '^':
				temp.syn = _xor;
				temp.token = "^";
				result.push_back(temp);
				break;
			/* not or neq */
			case '!':
				if (iterPeek(iter, data.end(), ch) && ch == '=')
				{
					temp.syn = notEqual;
					temp.token = "!=";
					result.push_back(temp);
					iter++;
				}
				else
				{
					temp.syn = _not;
					temp.token = "!";
					result.push_back(temp);
				}
				break;
			/* bigger or boe */
			case '>':
				if (iterPeek(iter, data.end(), ch) && ch == '=')
				{
					temp.syn = boe;
					temp.token = ">=";
					result.push_back(temp);
					iter++;
				}
				else
				{
					temp.syn = bigger;
					temp.token = ">";
					result.push_back(temp);
				}
				break;
			/* smaller or soe */
			case '<':
				if (iterPeek(iter, data.end(), ch) && ch == '=')
				{
					temp.syn = soe;
					temp.token = "<=";
					result.push_back(temp);
					iter++;
				}
				else
				{
					temp.syn = smaller;
					temp.token = "<";
					result.push_back(temp);
				}
				break;
			case '#':
				temp.syn = sign;
				temp.token = "#";
				result.push_back(temp);
				break;
			default:
				temp.syn = -1;
				temp.token = "NULL";
				result.push_back(temp);
				break;
			}
		}
	}
}