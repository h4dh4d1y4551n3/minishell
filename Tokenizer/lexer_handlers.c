#include "lexer.h"

static void	handle_control_operator(t_tok *tok, t_lexer *lexer,
		const char *start)
{
	int	length;

	tok->type = ctrl_operator((char *)lexer->off);
	length = (tok->type == TOK_LOGIC_OPRTR) + 1;
	lexer->off += length;
	ft_lstadd_back(&tok->frags, ft_lstnew(frag_class(start, length, 0, 0)));
	tok->frags_cnt++;
}

static void	handle_redirection_operator(t_tok *tok, t_lexer *lexer)
{
	int	length;

	tok->type = TOK_REDIR_OPRTR;
	length = (lexer->off[0] == lexer->off[1]) + 1;
	ft_lstadd_back(&tok->frags, ft_lstnew(frag_class(lexer->off, length, 0, 0)));
	lexer->off += length;
	tok->frags_cnt++;
}
static void	handle_word_or_param(t_tok *tok, t_lexer *lexer,
		t_lex_substate state, const char *start)
{
	int	length;

	tok->type = TOK_WORD;
	if (state == LEX_PARAM)
		lexer->off++;
	while ((state = eval_lex_substate(lexer)) == LEX_WORD
		&& eval_lex_state(lexer) == LEX_UNQUOTED)
		lexer->off++;
	length = lexer->off - start - (state == LEX_WORD);
	ft_lstadd_back(&tok->frags, ft_lstnew(frag_class(start, length, 1, 0)));
	tok->frags_cnt++;
}
void	identify_unquoted_tok(t_tok *tok, t_lexer *lexer)
{
	const char		*start;
	t_lex_substate	state;

	start = lexer->off;
	state = eval_lex_substate(lexer);
	if (state & (LEX_WORD | LEX_PARAM))
		handle_word_or_param(tok, lexer, state, start);
	else if (state == LEX_CTRL_OPRTR)
		handle_control_operator(tok, lexer, start);
	else if (state == LEX_REDIR_OPRTR)
		handle_redirection_operator(tok, lexer);
	tok->eot = ft_isspace(*lexer->off) + ctrl_operator((char *)lexer->off)
		+ (state != LEX_WORD);
}

void	identify_quoted_tok(t_tok *tok, t_lexer *lexer)
{
	const char		*bounds[2] = {lexer->off, NULL};
	bool			canExpand;
	t_lex_substate	state;

	canExpand = lexer->state == LEX_PARTLY_QUOTED;
	bounds[0] = lexer->off;
	while (*lexer->off && eval_lex_state(lexer) != LEX_UNQUOTED)
		lexer->off++;
	tok->type = TOK_WORD;
	ft_lstadd_back(&tok->frags, ft_lstnew(frag_class(bounds[0], lexer->off
				- bounds[0] - 1, canExpand, canExpand)));
	state = eval_lex_substate(lexer);
	tok->eot = ft_isspace(*lexer->off) + ctrl_operator((char *)lexer->off)
		+ (state != LEX_WORD);
}
