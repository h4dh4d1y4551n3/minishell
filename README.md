<ALPHA> ::= a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|
            A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z

<DIGIT> ::= 0|1|2|3|4|5|6|7|8|9

<NUMBER> ::= <DIGIT>
           | <NUMBER> <DIGIT>

<WORD> ::= <ALPHA>
         | <WORD> <ALPHA>
         | '_' <WORD>
         | <WORD> '_'
         | <WORD> <NUMBER>
         | <WORD> '_' <NUMBER>
         | <WORD> <NUMBER> '_'

<WORD-LIST> ::= <WORD>
              | <WORD-LIST> <WORD>

<ASSIGNMENT-WORD> ::= <WORD> '=' <WORD>

<REDIRECTION> ::= '>' <WORD>
                | '<' <WORD>
                | <NUMBER> '>' <WORD>
                | <NUMBER> '<' <WORD>
                | '>>' <WORD>
                | <NUMBER> '>>' <WORD>
                | '<<' <WORD>
                | <NUMBER> '<<' <WORD>

<REDIRECTION-LIST> ::= <REDIRECTION>
                     | <REDIRECTION-LIST> <REDIRECTION>

<COMMAND> ::= <WORD>
            | <ASSIGNMENT-WORD>
            | <REDIRECTION>

<COMPOUND-COMMAND> ::= <COMMAND>
                     | <COMPOUND-COMMAND> <COMMAND>

<PIPELINE-COMMAND> ::= <COMPOUND-COMMAND>
                     | <PIPELINE-COMMAND> '|' <PIPELINE-COMMAND>

<COMMAND-LIST> ::= <PIPELINE-COMMAND>
                 | <COMMAND-LIST> '&&' <COMMAND-LIST>
                 | <COMMAND-LIST> '||' <COMMAND-LIST>

<SUBSHELL> ::=  '(' <COMMAND-LIST> ')'