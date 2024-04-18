/* Authors: Nathan Hoffman (nh662219@ohio.edu)              */
/*          Kara Nemet     (kn485120@ohio.edu)              */
/* Brief: This lex program parses a file and tokenizes it.  */
/*        It then outputs the tokens to a file.             */
/* Date: 04-16-2024                                         */

%{
    #include <iostream>
    using namespace std;

    #define TK_VAR



%}
/*------------MACROS------------*/
/* Number */
NUM [0-9]
/* Letter */
L [a-zA-Z]
/* Variable */
VAR ({L}|{NUM})({L}|{NUM}| _ )*