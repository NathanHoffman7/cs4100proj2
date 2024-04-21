/* Authors: Nathan Hoffman (nh662219@ohio.edu)              */
/*          Kara Nemet     (kn485120@ohio.edu)              */
/* Brief: This lex program parses a file and tokenizes it.  */
/*        It then outputs the tokens to a file.             */
/* Date: 04-16-2024                                         */

%{
    #define TK_VAR 1
    #define TK_WS 2 /* White space */

    #include <iostream>
    #include <vector>
    #include <list>
    #include <set>
    #include <map>
    #include <algorithm>
    #include <cstdlib>
    using namespace std;

    int linecount=1;
    int indent;
    int n;

    void ACC(char c)
    {
      if (c == '\n')
        {
          linecount++;
        }
    }
    int line_num()
    {
      return linecount;
    } 

%}

/*------------MACROS------------*/
/* Number */
NUM [0-9]
/* Letter */
L [a-zA-Z]
/* Variable */
VAR ({L}|{NUM})({L}|{NUM}| _ )*



%%
\/\/                     {char c; while ((c= yyinput())!='\n') {
                              if (c==EOF) {
                                  break;
                               }
                              ACC(c);
                             }
                                }
[ \t\n\f]                       {ACC(yytext[0]); /* Remove tabs/spaces/newlines */} 
\/\*                            {char c;
                                 int line_cur;
                                 line_cur = linecount; 
                                 while (1)  
                                   {if ((c = yyinput()) == EOF) 
                                       { 
                                        /* If this is the case, there is
                                           an error, an unterminated 
                                           comment. */
                               cout << "Detected unterminated comment starting on line" << line_cur << endl;
                                        return(0); 
                                       }   
                                    ACC(c);   
                                    if (c == '*') 
                                       {       
                                        if ((c = yyinput()) == '/')
                                        {
                                           break;
                                        }
                                        else {unput(c);} 
                                       }
                                   } 
                                }  
%%

