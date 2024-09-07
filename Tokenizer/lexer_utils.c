#include "../lexer.h"

enum e_tok	ctrl_operator(char *s)
{
    //first check is to elimate buffer overflow caused by type punning
    if (*s != '\0' )
        if (*(short *)"&&" == *(short *)s || *(short *)"||" == *(short *)s)
            return (TOK_LOGIC_OPRTR);
	if (*s == '(')
		return (TOK_OPEN_PARAN);
	if (*s == ')')
		return (TOK_CLOSE_PARAN);
	if (*s == '|')
		return (TOK_PIPE);
	if (ft_strchr("<>", *s))
		return (TOK_REDIR_OPRTR);
	return (TOK_END);
}

t_lex_substate	eval_lex_substate(t_lexer *lexer)
{
	if (!*lexer->off)
		return (LEX_BOUND);
	if (ft_isspace(*lexer->off))
		return (LEX_WHITESPACE);
	if (*lexer->off == '$')
		return (LEX_PARAM);
	if (ft_strchr("<>", *lexer->off))
		return (LEX_REDIR_OPRTR);
	if (ft_strchr("&|()", *lexer->off))
		return (LEX_CTRL_OPRTR);
	return (LEX_WORD);
}
t_lex_state eval_lex_state(t_lexer *lexer)
{
    if ((lexer->state == LEX_QUOTED && *lexer->off == '\'') ||
        (lexer->state == LEX_PARTLY_QUOTED && *lexer->off == '"'))
        return (++lexer->off, lexer->state = LEX_UNQUOTED);
    if (*lexer->off == '\'')
        return (++lexer->off, lexer->state = LEX_QUOTED);
    if (*lexer->off == '"')
        return (++lexer->off, lexer->state = LEX_PARTLY_QUOTED);
    return lexer->state;
}

t_tok_frag	*frag_class(const char *s, size_t size, bool can_expand, bool is_quote)
{
	t_tok_frag	*token;

	token = malloc(sizeof(t_tok_frag));
	if (!token)
		return (NULL);
	token->val = (char *)s;
	token->len = size;
	token->xpandabl = can_expand;
	token->quoted = is_quote;
	return (token);
}