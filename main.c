#include "Shell.h"

t_ttk_token *token_class(t_token type, char *value, int size)
{
    t_ttk_token *token = malloc(sizeof(t_ttk_token));
    if (!token) return NULL;
    token->type = type;
    token->value = value ? value : NULL;
    token->next = NULL;
    token->size = size;
    return token;
}

char *substr(const char *start, const char *end)
{
    if (start >= end) return NULL;
    int size = end - start;
    char *result = malloc(size + 1);
    if (!result) return NULL;
    strncpy(result, start, size);
    result[size] = '\0';
    return result;
}

// t_ttk_token *get_next_token(t_stream *Token_stream)
// {
//     char *input = Token_stream->line + Token_stream->position;
//     int flag = OUT_QUOTE;
//     while (*input == ' ') {
//         input++;
//         Token_stream->position++;
//      }
//     if (*input == '\0')
//         return token_class(TOKEN_EOF, NULL);

//     if (*input == '|' && *(input+1) == '|') {
//         Token_stream->position += 2;
//         return token_class(TOKEN_OR, "||");
//     }
//     if (*input == '&' && *(input+1) == '&') {
//         Token_stream->position += 2;
//         return token_class(TOKEN_AND, "&&");
//     }
//     if (*input == '|') {
//         Token_stream->position++;
//         return token_class(TOKEN_PIPE, "|");
//     }
//     if (*input == '(') {
//         Token_stream->position++;
//         return token_class(TOKEN_PAR_OPEN, "(");
//     }
//     if (*input == ')') {
//         Token_stream->position++;
//         return token_class(TOKEN_PAR_CLOSE, ")");
//     }
//     if (*input == '>'){
//         Token_stream->position++;
//         return (token_class(TOKEN_REDIR_IN, ">"));
//     }
//     if (*input == '<'){
//         Token_stream->position++;
//         return (token_class(TOKEN_REDIR_OUT, "<"));
//     }
//     if (*input == '>' && *(input+1) == '>'){
//         Token_stream->position += 2;
//         return (token_class(TOKEN_REDIR_APPEND, ">>"));
//     }
//     if (*input == '<' && *(input+1) == '<'){
//         Token_stream->position += 2;
//         return (token_class(TOKEN_HEREDOC, "<<"));
//     }
//     char *word_end = input;
//     if (*word_end == '"' && word_end++)
//         flag = IN_DQUOTE;
//     else if (*word_end == '\'' && word_end++)
//         flag = IN_SINGLE_QUOTE; 
//     while (*word_end) {
//         if ((*word_end == ' ' || *word_end == '|' ||(*word_end == '&' && *(word_end + 1) == '&')
//                || *word_end == '(' || *word_end == ')' || *word_end == '>' || *word_end == '<'))
//                 if (flag != IN_DQUOTE && flag != IN_SINGLE_QUOTE)
//                         break;
//         if (flag == IN_DQUOTE && (*word_end == '"' || *word_end == '$') )
//             {
//                     word_end++;
//                     flag = OUT_QUOTE;
//                     break;
//                 }
//         if (flag == IN_SINGLE_QUOTE && *word_end == '\'')
//             {
//                             word_end++;
//                             flag = OUT_QUOTE;
//                             break;
//                         }
//         word_end++;
//     }
//     char *word = substr(input, word_end);
//     Token_stream->position += word_end - input;
//     return token_class(TOKEN_WORD, word);
// }

// t_ttk_token *parse(t_stream *Token_stream)
// {
//     t_ttk_token *head = malloc(sizeof (t_token));
//     if (!head) return NULL;
//     t_ttk_token *holder;
//     head = token_class(TOKEN_START, NULL);
//     t_ttk_token *current = head;
//     while (current->type != TOKEN_EOF) {
//         holder = get_next_token(Token_stream);
//         if (expects(current, holder))
//             current->next = holder;
//         else {
//             printf("%s %s" , current->value, holder->value);
//             printf("Syntax error\n");
//             return NULL;
//         }
//         current = current->next;
//     }
//     return head;
// }

char *handle_dquote(char *input)
{
    char *word_end = input;
    while (*word_end) {
        if (*word_end == '"' || *word_end == '$')
            break;
        word_end++;
    }
    return word_end;
}
t_ttk_token *get_token(t_stream *Token_Stream)
{
    static int state = OUT_QUOTE;
    char *s = Token_Stream->line + Token_Stream->position;
    if (*s == '\0')
        return token_class(TOKEN_EOF, NULL, 0);
    char *tok = strpbrk(s, "\n\t&|()<> $\"'");
    printf("tok: %c\n", *tok);
    if (*tok == ' ' || *tok == '\t' || *tok == '\0')
    {    
        while (*tok == ' ')
            tok++;
        Token_Stream->position = tok - Token_Stream->line;
        return token_class(TOKEN_WORD,s, tok - s);
    }
    if (*tok == '|')
    {
        Token_Stream->position++;
        return token_class(TOKEN_PIPE, "|", 1);
    }
    if (*tok == '"' && state == OUT_QUOTE)
    {
        state = IN_DQUOTE;
        tok++;
        char *dummy = handle_dquote(tok);
        if (*dummy == '"')
        {
            state = OUT_QUOTE;
            dummy++;
        }
        Token_Stream->position = dummy - Token_Stream->line;
        return token_class(TOKEN_WORD, tok, dummy - tok);
    }
    if (*tok == '\'')
    {
        state = IN_SINGLE_QUOTE;
        tok++;
        char *dummy = strpbrk(tok, "'");
        if (*dummy == '\'')
            state = OUT_QUOTE;
        Token_Stream->position = dummy - Token_Stream->line;
        return token_class(TOKEN_WORD, s, dummy - s);
    }
    if (*tok == '&' && *(tok + 1) == '&')
    {
        Token_Stream->position += 2;
        return token_class(TOKEN_AND, tok, 2);
    }
    return token_class(TOKEN_EOF, NULL,1);
}
void print_all_token_values(t_ttk_token *token)
{
    while (token) {
        if (token->value)
            printf("%s\n", token->value);
        token = token->next;
    }
}
int main()
{
    char *example = "echo hello \"|\" cat>file.txt&&echo 'haha'\"ls\"world\"prettysex\"||echo bye\"''\"\"\"\"''\"''''''''''$ls";
    char simple[50] = "echo \"hello\" world";
    t_stream tokenStream;
    tokenStream.line = simple;
    tokenStream.position = 0;
    t_ttk_token *token = get_token(&tokenStream);
    printf("%.*s\n", token->size, token->value);
    token = get_token(&tokenStream);
    printf("%.*s\n", token->size, token->value);
    token = get_token(&tokenStream);
    printf("%.*s\n", token->size, token->value);
    return 0; 
    
}