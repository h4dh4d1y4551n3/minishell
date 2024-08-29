/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhadhadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:10:06 by yhadhadi          #+#    #+#             */
/*   Updated: 2024/08/30 00:14:34 by yhadhadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

// eval_lex_state (This function will be handle the lexical analysis state transition this is the core of the lexer automaton)
t_lex_state	eval_lex_state(t_lexer *lexer);

// identify_tok (like strtok but in the context of our lexer it will get next token each time called and also detect the end of the prompt thus there is no more token to identify)
t_tok		*identify_tok(t_lexer *lexer);

t_list	*analyse_prompt(t_lexer *lexer, t_logger *logger)
{
	t_tok	*tok;
	t_list	*i;
	t_list	*node;

	i = lexer->toks;
	while (*lexer->off)
	{
		// Populate toks using identify_tok
		lexer->state = eval_lex_state(lexer);
		tok = identify_tok(lexer);
		if (!tok)
			break ;
		// Need error check for malloc failure!
		// Pre-syntax eval
		// TODO
		node = ft_lstnew(tok);
		// if (!node)
		//	return and handle error type
		if (i)
			i->next = node;
		i = node;
		// This is just a basic version this might be subdue to changes since I
		// still didn't decide how identify_tok will handle errors yet the
		// interface is too simple for now
	}
	return (lexer->toks);
}

static t_lex_state	eval_lex_state(t_lexer *lexer)
{
	// LEX_QUOTED OR LEX_PARTLY_QUOTED
	if ((lexer->state == LEX_QUOTED && *lexer->off == '\'')
		|| (lexer->state == LEX_PARTLY_QUOTED && *lexer->off == '"'))
		return (++lexer->off, LEX_UNQUOTED);
	// LEX_UNQUOTED
	if (*lexer->off == '\'')
		return (++lexer->off, LEX_QUOTED);
	if (*lexer->off == '"')
		return (++lexer->off, LEX_PARTLY_QUOTED);
	return (lexer->state);
}

t_tok	*identify_tok(t_lexer *lexer)
{
	t_tok		*tok;

	tok = (t_tok *)malloc(sizeof(t_tok));
	if (!tok)
		return (NULL);
	*tok = (t_tok){};
	if (lexer->state & (LEX_QUOTED | LEX_PARTLY_QUOTED))
		identify_quoted_tok(tok, lexer);
	else
		identify_unquoted_tok(tok, lexer);
	return (tok);
}

static void	identify_unquoted_tok(t_tok *tok, t_lexer *lexer)
{
	const char		*off = lexer->off;
	const char		*bounds[2] = {off, NULL}; // Refactor to inner functions
	t_lex_substate	state;

	while (*lexer->off)
	{
		state = eval_lex_substate(lexer);
		if (state == LEX_WHITESPACE)
			// Skip
		if (state == LEX_WORD)
			// Accumulate and mark word fragment
		if (state == LEX_PARAM)
			// Skip indicator '$' accumulate identifier fragment
		if (state == LEX_ASGNMT)
			// Mark assignement fragment
		if (state == LEX_REDIR_OPRTR)
			// Mark redirection operator
		if (state == LEX_CTRL_OPRTR)
			// Retrieve valid monographs ignore invalid one look ahead in case
			// of digraphs then retrieve them if valid or ignore them if not.
			// When I say ignore I mean switch the state back to whatever
			// following consumable char is without updating the bounds[0] wich
			// is the token start and everything will flow back to the automaton
	}
	// All the above checks should return out of the function since if we are
	// out of the loop by mean of !*lexer->off then tok->type should be TOK_END
	tok->type = TOK_END;
}

static void	identify_quoted_tok(t_tok *tok, t_lexer *lexer)
{
	// Check state and handle quoted part differently from partly quoted parts The logic is similar to unquoted but less stricted since fewer state are acknowledged 
}

static t_lex_substate	eval_lex_substate(t_lexer *lexer)
{
	const char	*off = lexer->off;

	if (!*off)
		return (LEX_BOUND);
	if (lexer->state == LEX_UNQUOTED)
	{
		if (ft_isspace(*off))
			return (LEX_WHITESPACE);
		if (*off == '=')
			return (LEX_ASGNMT);
		if (ft_strchr("<>", *off))
			return (LEX_REDIR_OPRTR);
		if ((*off == '&' && *(off + 1) == '&')
			|| ft_strchr("()|", *off))
			return (LEX_CTRL_OPRTR);
	}
	if (lexer->state != LEX_QUOTED
		&& (*off == '$' && (ft_isalpha(*(off + 1)) || *(off + 1) == '?'
				|| *(off + 1) == '_')))
		return (LEX_PARAM);
	return (LEX_WORD);
}
