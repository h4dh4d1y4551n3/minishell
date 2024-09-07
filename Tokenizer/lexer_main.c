#include "lexer.h"






t_tok	*identify_tok(t_lexer *lexer)
{
	t_tok	*tok;

	tok = (t_tok *)malloc(sizeof(t_tok));
	if (!tok)
		return (NULL);
	*tok = (t_tok){};
	tok->eot = 0;
	while (ft_isspace(*lexer->off))
		lexer->off++;
	if (*lexer->off == 0)
		tok->type = TOK_END;
	lexer->state = eval_lex_state(lexer);
	while (*lexer->off && !tok->eot)
	{
		lexer->state = eval_lex_state(lexer);
		if (lexer->state == LEX_UNQUOTED)
			identify_unquoted_tok(tok, lexer);
		if (lexer->state & (LEX_QUOTED | LEX_PARTLY_QUOTED) && !tok->eot)
			identify_quoted_tok(tok, lexer);
	}
	return (tok);
}

t_list	*analyse_prompt(t_lexer *lexer)
{
	t_tok	*tok;
	t_list	   *node;
	int counter_paran = 0;

	lexer->toks = ft_lstnew(NULL);
	lexer->toks->data = (t_tok*) malloc(sizeof(t_tok));
	if (!lexer->toks->data)
	    return (NULL);
	((t_tok *) lexer->toks->data)->type = TOK_START;
	node = lexer->toks;
	while (*lexer->off)
	{
		tok = identify_tok(lexer);
		if (!tok)
			break;
		if (tok->type == TOK_OPEN_PARAN)
		    counter_paran++;
		if (tok->type == TOK_CLOSE_PARAN)
		    if ( --counter_paran < 0)
			    printf("INCORRECT PARANTHESIS\n"), exit(1);
		if (expects(((t_tok *)node->data)->type ,tok->type))
		    ft_lstadd_back(&lexer->toks, ft_lstnew(tok));
		else
			printf("error syntax %d %d \n",((t_tok *) node->data)->type, tok->type ), exit(1);
		node = node->next;
	}
	if (tok->type == TOK_END)
	   if (counter_paran != 0)
				printf("CLOSE INCORRECT PARANTHESIS\n"), exit(1);
	return (lexer->toks);
}