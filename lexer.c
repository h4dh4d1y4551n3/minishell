/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhadhadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:10:06 by yhadhadi          #+#    #+#             */
/*   Updated: 2024/09/02 20:11:06 by yhadhadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "string.h"

enum e_lex_stt	eval_lex_stt(t_lexer *lexer);

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

t_tok	*identify_tok(t_lexer *lexer)
{
	struct s_lex_cntx	cntx;

	cntx = (struct s_lex_cntx){(t_tok *)malloc(sizeof(t_tok)), lexer->off};
	if (!cntx.tok)
		return (NULL);
	*cntx.tok = (t_tok){};
	while (!lexer->tok_occ)
	{
		lexer->stt = (lexer->stt & LEX_STT_MASK) | eval_lex_stt(lexer);
		if (lexer->stt & (LEX_WHITESPACE_OCC | LEX_WORD_OCC | LEX_PARAM_OCC))
			identify_word_tok(&cntx, lexer);
		if (lexer->stt & (LEX_REDIR_OPRTR_OCC | LEX_CTRL_OPRTR_OCC))
			identify_oprtr_tok(&cntx, lexer);
		lexer->ref_stt = lexer->stt;
	}
	return (cntx.tok);
}

static void	identify_word_tok(struct s_lex_cntx *cntx, t_lexer *lexer)
{
	t_tok_frag	*frag;
	t_list		*node;

	if (lexer->stt & (LEX_UNQUOTED & LEX_WHITESPACE_OCC)
		&& lexer->ref_stt & (LEX_WORD_OCC | LEX_PARAM_OCC))
	{
		frag = (t_tok_frag *)malloc(sizeof(t_tok_frag));
		*frag = (t_tok_frag){cntx->bnds[0], cntx->bnds[1] - cntx->bnds[0]};
		if (lexer->ref_stt & LEX_WORD_OCC)
			frag->type = FRAG_LTRL;
		if (lexer->ref_stt & LEX_PARAM_OCC)
			frag->type = FRAG_XPNDBL;
		node = ft_lstnew(frag);
		// if (!node)
		//	Return and handle error type
		cntx->tok->type = TOK_WORD;
		if (cntx->i)
			cntx->i->next = node;
		else
			cntx->i = cntx->tok->frags;
		cntx->i = node;
	}
	if (lexer->stt &  (LEX_UNQUOTED))

	if (lexer->stt & (LEX_UNQUOTED | LEX_WHITESPACE_OCC))
	{
		while (ft_isspace(*lexer->off))
			++lexer->off;
		return ;
	}
	
}

static void	identify_oprtr_tok(struct s_lex_cntx *cntx, t_lexer *lexer)
{

}

// delimit_tok_frag

static enum e_lex_stt	eval_lex_stt(t_lexer *lexer)
{
	lexer->stt = (lexer->stt & ~LEX_STT_MASK) | eval_lex_refstt(lexer);
	if (!*lexer->off)
		return (LEX_ENDBOUND);
	if (lexer->stt & LEX_UNQUOTED)
	{
		if (ft_isspace(*lexer->off))
			return (LEX_WHITESPACE_OCC);
		if (ft_strchr("<>", *lexer->off))
			return (LEX_REDIR_OPRTR_OCC);
		if (*lexer->off == '|')
			return (LEX_CTRL_OPRTR_OCC);
	}
	if (!(lexer->stt & LEX_QUOTED) && (*lexer->off == '$'
			&& (ft_isalpha(*(lexer->off + 1))
				|| *(lexer->off + 1) == '?' || *(lexer->off + 1) == '_')))
		return (++lexer->off, LEX_PARAM_OCC);
	return (LEX_WORD_OCC);
}

static enum e_lex_stt	eval_lex_refstt(t_lexer *lexer)
{
	if ((lexer->stt & LEX_QUOTED && *lexer->off == '\'')
		|| (lexer->stt & LEX_PARTLY_QUOTED && *lexer->off == '"'))
		return (++lexer->off, LEX_UNQUOTED);
	if (*lexer->off == '\'')
		return (++lexer->off, LEX_QUOTED);
	if (*lexer->off == '"')
		return (++lexer->off, LEX_PARTLY_QUOTED);
	return (lexer->stt);
}
