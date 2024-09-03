/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baarif <baarif@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:10:06 by yhadhadi          #+#    #+#             */
/*   Updated: 2024/09/02 00:52:20 by baarif           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft/libft.h"
#include <stdio.h>
#include <stdlib.h>

// eval_lex_state (This function will be handle the lexical analysis state transition this is the core of the lexer automaton)
static t_lex_state		eval_lex_state(t_lexer *lexer);

// identify_tok (like strtok but in the context of our lexer it will get next token each time called and also detect the end of the prompt thus there is no more token to identify)
t_tok					*identify_tok(t_lexer *lexer);

t_list	*analyse_prompt(t_lexer *lexer)
{
	t_tok	*tok;
	t_list	*i;
	t_list	*node;

	int counter_open_paran = 0;
	int counter_close_paran = 0;
	int val;
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
			break ;
		if (tok->type == counter_open_paran)
		    counter_open_paran++;
		if (tok->type == counter_close_paran)
		 {
			counter_close_paran++;
			if (counter_close_paran > counter_open_paran)
			   printf("INCORRECT PARANTHESIS\n"), exit(1);
			
		}
		if ((val = expects(((t_tok *)node->data)->type ,tok->type) ))
		     ft_lstadd_back(&lexer->toks, ft_lstnew(tok));
		else
							 printf("error syntax %d %d \n",((t_tok *) node->data)->type, tok->type );
		// Need error check for malloc failure!
		// Pre-syntax eval
		// TODO
		node->data = tok;
		//	return and handle error type
		// This is just a basic version this might be subdue to changes since I
		// still didn't decide how identify_tok will handle errors yet the
		// interface is too simple for now
	}
	return (lexer->toks);
}

static enum e_tok	ctrl_operator(char *s)
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

static t_lex_substate	eval_lex_substate(t_lexer *lexer)
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
static t_lex_state eval_lex_state(t_lexer *lexer)
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
static void	identify_unquoted_tok(t_tok *tok, t_lexer *lexer)
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
		+ state != LEX_WORD;
}

static void	identify_quoted_tok(t_tok *tok, t_lexer *lexer)
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
		+ state != LEX_WORD;
}

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

int	main(void)
{
	t_tok *tok;
	t_tok_frag *i;
	t_list *node;
	t_lexer lexer;

	// Populate toks using identify_tok
	//dont suggest to use this function
	printf("hello\n");
    	lexer.off = "cat | grep \"hello\" | wc -l | echo \"hello\"";
     printf("hmm");
	lexer.state = LEX_UNQUOTED;
	// tok = identify_tok(&lexer);
	// // Print toks
	// while (tok->type != TOK_END)
	// {
	// 	printf("tok type: %d with val : ", tok->type);
	// 	node = tok->frags;
	// 	while (node)
	// 	{
	// 		i = (t_tok_frag *)node->data;
	// 		printf("--->%.*s\n", (int)i->len, i->val);
	// 		node = node->next;
	// 	}
	// 	tok = identify_tok(&lexer);
	// }
	// printf("tok type: %d\n", tok->type);
	analyse_prompt(&lexer);
}
