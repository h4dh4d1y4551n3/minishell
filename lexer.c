/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhadhadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:10:06 by yhadhadi          #+#    #+#             */
/*   Updated: 2024/08/31 19:46:42 by yhadhadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "string.h"

#define ft_strcspn strcspn

t_lex_stt	eval_lex_stt(t_lexer *lexer);

t_tok		*identify_tok(t_lexer *lexer);

t_list	*analyse_prompt(t_lexer *lexer, t_logger *logger)
{
	t_tok	*tok;
	t_list	*i;
	t_list	*node;

	i = lexer->toks;
	while (*lexer->off)
	{
		tok = identify_tok(lexer);
		// if (!tok)
		//	Return and handle malloc failure error
		
		// Pre-syntax eval
		// TO-DO
		node = ft_lstnew(tok);
		// if (!node)
		//	Return and handle error type
		if (i)
			i->next = node;
		i = node;
	}
	return (lexer->toks);
}

static t_lex_stt	eval_lex_stt(t_lexer *lexer)
{
	if ((lexer->stt == LEX_QUOTED && *lexer->off == '\'')
		|| (lexer->stt == LEX_PARTLY_QUOTED && *lexer->off == '"'))
		return (++lexer->off, LEX_UNQUOTED);
	if (*lexer->off == '\'')
		return (++lexer->off, LEX_QUOTED);
	if (*lexer->off == '"')
		return (++lexer->off, LEX_PARTLY_QUOTED);
	return (lexer->stt);
}

t_tok	*identify_tok(t_lexer *lexer)
{
	t_tok	*tok;

	tok = (t_tok *)malloc(sizeof(t_tok));
	if (!tok)
		return (NULL);
	*tok = (t_tok){};
	while (!lexer->tok_occ)
	{
		lexer->stt = eval_lex_stt(lexer);
		if (lexer->stt & (LEX_QUOTED | LEX_PARTLY_QUOTED))
			identify_quoted_tok(tok, lexer);
		else
			identify_unquoted_tok(tok, lexer);
	}
	return (tok);
}

static void	identify_unquoted_tok(t_tok *tok, t_lexer *lexer)
{
	const char		**off = lexer->off;
	t_lex_substt	sub_stt;

	while (**off)
	{
		sub_stt = eval_lex_substt(lexer);
		if (sub_stt == LEX_WHITESPACE)
		{
			while (ft_isspace(**off))
				++*off;
			continue ;
		}
		if (sub_stt == LEX_WORD)
			identify_word_tok(tok, sub_stt, lexer);
		if (sub_stt == LEX_PARAM)
			// Skip indicator '$' accumulate identifier fragment
		if (sub_stt == LEX_REDIR_OPRTR)
			// Mark redirection operator
		if (sub_stt == LEX_CTRL_OPRTR)
			// Mark operator
	}
	
}

static void	identify_quoted_tok(t_tok *tok, t_lexer *lexer)
{
}

static void	identify_word_tok(t_tok *tok, t_lex_substt *sub_stt, t_lexer *lexer)
{
	t_tok_frag	*frag;
	t_list		*node;
	const char	**off = lexer->off;

	frag = (t_tok_frag *)malloc(sizeof(t_tok_frag));
	frag->val = *off;
	frag->len = ft_strcspn(*off, " \t\n\"\'");
	node = ft_lstnew(frag);
}

static t_lex_substt	eval_lex_substt(t_lexer *lexer)
{
	const char	**off = &lexer->off;

	if (!**off)
		return (LEX_BOUND);
	if (lexer->stt == LEX_UNQUOTED)
	{
		if (ft_isspace(**off))
			return (LEX_WHITESPACE);
		if (ft_strchr("<>", **off))
			return (LEX_REDIR_OPRTR);
		if (**off == '|')
			return (LEX_CTRL_OPRTR);
	}
	if (lexer->stt != LEX_QUOTED
		&& (**off == '$' && (ft_isalpha(*(*off + 1)) || *(*off + 1) == '?'
				|| *(*off + 1) == '_')))
		return (++*off, LEX_PARAM);
	return (LEX_WORD);
}
