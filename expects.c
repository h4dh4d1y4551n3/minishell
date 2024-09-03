#include "lexer.h"



bool expects(enum e_tok current, enum e_tok next)
{
    if (current == TOK_START)
        return (next & (TOK_WORD | TOK_END  | TOK_REDIR_OPRTR ) || next == TOK_END);
    if (current == TOK_WORD)
        return (next & (TOK_WORD | TOK_REDIR_OPRTR |TOK_LOGIC_OPRTR |TOK_PIPE |  TOK_END  | TOK_CLOSE_PARAN) || next == TOK_END);
    if (current == TOK_REDIR_OPRTR)
        return (next & (TOK_WORD));
    if (current & (TOK_LOGIC_OPRTR |TOK_PIPE))
        return (next & (TOK_WORD | TOK_OPEN_PARAN ));
    if (current == TOK_OPEN_PARAN)
        return (next & (TOK_WORD | TOK_OPEN_PARAN | TOK_CLOSE_PARAN));
    if (current == TOK_CLOSE_PARAN)
        return (next & (TOK_LOGIC_OPRTR | TOK_PIPE | TOK_CLOSE_PARAN) || next == TOK_END);
    return (false);
}
