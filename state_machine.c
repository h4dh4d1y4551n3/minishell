#include "Shell.h"
bool handle_word(t_ttk_token *next)
{
    static const t_token valid_tokens[] = { TOKEN_WORD, TOKEN_AND, TOKEN_OR, TOKEN_PIPE, TOKEN_PAR_CLOSE, TOKEN_EOF};
    for (int i = 0; valid_tokens[i] != TOKEN_EOF; i++) {
          if (next->type == valid_tokens[i])
              return true;
      }
    if (next->type == TOKEN_EOF)
        return true;
      return false;
}

bool handle_operator(t_ttk_token *next)
{
    t_token valid_tokens[] = {TOKEN_WORD, TOKEN_PAR_OPEN, TOKEN_REDIR_APPEND, TOKEN_REDIR_IN, TOKEN_REDIR_OUT, TOKEN_HEREDOC, TOKEN_EOF};
    for (int i = 0; valid_tokens[i] != TOKEN_EOF; i++) {
        if (next->type == valid_tokens[i])
            return true;
    }
    return false;
}

bool expects(t_ttk_token *current, t_ttk_token *next)
{
    if (current->type == TOKEN_START)
    {
        return (next->type == TOKEN_WORD || next->type == TOKEN_PAR_OPEN || 
                next->type == TOKEN_EOF || next->type == TOKEN_REDIR_IN || 
                next->type == TOKEN_REDIR_OUT || next->type == TOKEN_REDIR_APPEND || 
                next->type == TOKEN_HEREDOC);
    }
    if (current->type == TOKEN_WORD)
        return handle_word(next);
    if (current->type == TOKEN_AND || current->type == TOKEN_OR || current->type == TOKEN_PIPE)
        return handle_operator(next);
    if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_OUT || 
        current->type == TOKEN_REDIR_APPEND || current->type == TOKEN_HEREDOC)
        return next->type == TOKEN_WORD;
    if (current->type == TOKEN_PAR_OPEN)
    { 
        t_ttk_token start_token = {.type = TOKEN_START};
        return expects(&start_token, next);
    }
    if (current->type == TOKEN_PAR_CLOSE)
            return next->type == TOKEN_AND || next->type == TOKEN_OR || next->type == TOKEN_PIPE || next->type == TOKEN_EOF;
    return false;
}