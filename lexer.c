/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhadhadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:10:06 by yhadhadi          #+#    #+#             */
/*   Updated: 2024/09/02 01:27:52 by yhadhadi         ###   ########.fr       */
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
	const char	**off = &lexer->off;

	if ((lexer->stt & LEX_QUOTED && **off == '\'')
		|| (lexer->stt & LEX_PARTLY_QUOTED && **off == '"'))
		return (++*off, LEX_UNQUOTED);
	if (**off == '\'')
		return (++*off, LEX_QUOTED);
	if (**off == '"')
		return (++*off, LEX_PARTLY_QUOTED);
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
		lexer->stt = (lexer->stt & ~LEX_STT_MASK) | eval_lex_stt(lexer);
		if (lexer->stt & (LEX_QUOTED | LEX_PARTLY_QUOTED))
			identify_quoted_tok(tok, lexer);
		else
			identify_unquoted_tok(tok, lexer);
	}
	return (tok);
}

static void	identify_unquoted_tok(t_tok *tok, t_lexer *lexer)
{
	const char		**off = &lexer->off;
	const char		*bounds[2] = {*off, NULL};

	while (**off)
	{
		lexer->stt = (lexer->stt & LEX_STT_MASK) | eval_lex_substt(lexer);
		if (lexer->stt & LEX_WHITESPACE_OCC)
		{
			while (ft_isspace(**off))
				++*off;
			continue ;
		}
		if (lexer->stt & LEX_WORD_OCC)
			// TO-DO
		if (lexer->stt & LEX_PARAM_OCC)
			// TO-DO
		if (lexer->stt & LEX_REDIR_OPRTR_OCC)
			// TO-DO
		if (lexer->stt & LEX_CTRL_OPRTR_OCC)
			// TO-DO
		lexer->ref_stt = lexer->stt;
	}
	lexer->stt = (lexer->stt & LEX_STT_MASK) | eval_lex_substt(lexer);
	// TO-DO
}

static void	identify_quoted_tok(t_tok *tok, t_lexer *lexer)
{
}

static void	identify_word_tok(t_tok *tok, t_lexer *lexer)
{
}

static t_lex_stt	eval_lex_substt(t_lexer *lexer)
{
	const char	**off = &lexer->off;

	if (lexer->stt & LEX_UNQUOTED)
	{
		if (ft_isspace(**off))
			return (LEX_WHITESPACE_OCC);
		if (ft_strchr("<>", **off))
			return (LEX_REDIR_OPRTR_OCC);
		if (**off == '|')
			return (LEX_CTRL_OPRTR_OCC);
	}
	if (!(lexer->stt & LEX_QUOTED)
		&& (**off == '$' && (ft_isalpha(*(*off + 1)) || *(*off + 1) == '?'
				|| *(*off + 1) == '_')))
		return (++*off, LEX_PARAM_OCC);
	if (!**off)
		return (LEX_ENDBOUND);
	return (LEX_WORD_OCC);
}
