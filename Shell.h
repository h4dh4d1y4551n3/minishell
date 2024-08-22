#ifndef MINI_SHELL_H
    #define MINI_SHELL_H
    #include <ctype.h>
    #include <string.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/wait.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <stdbool.h>
    typedef enum e_token {
        TOKEN_AND = 1,
        TOKEN_OR = 2,
        TOKEN_PIPE = 4,
        TOKEN_PAR_OPEN = 8,
        TOKEN_PAR_CLOSE = 16,
        TOKEN_REDIR_IN = 32,
        TOKEN_REDIR_OUT = 64,
        TOKEN_REDIR_APPEND = 128,
        TOKEN_HEREDOC = 256,
        TOKEN_WORD = 512,
        TOKEN_EOF = 1024,
        TOKEN_START = 2048,
        TOKEN_ASSIGN = 4096,
        TOKEN_EXPAND = 8192,
    } t_token;
    enum {
        IN_SINGLE_QUOTE,
        IN_DQUOTE,
        OUT_QUOTE
    };
    typedef struct s_ttk_token {
        t_token type;
        char *value;
        struct s_ttk_token *next;
        int size;
    } t_ttk_token;
    
    typedef struct s_stream {
        t_ttk_token *token;
        int position;
        char *line;
    } t_stream;
    
    bool expects(t_ttk_token *current, t_ttk_token *next);
    

#endif