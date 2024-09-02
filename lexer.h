/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhadhadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 03:07:15 by yhadhadi          #+#    #+#             */
/*   Updated: 2024/09/02 19:08:14 by yhadhadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "libft.h"
# include "logger.h"

enum e_tok
{
	TOK_END				= 0x0,
	TOK_WORD			= 0x1,
	TOK_IN_REDIR		= 0x2,
	TOK_OUT_REDIR		= 0x4,
	TOK_APPEND			= 0x8,
	TOK_HEREDOC			= 0x10,
	TOK_PIPE			= 0x20,
	TOK_AND				= 0x40,
	TOK_OR				= 0x80,
	TOK_SUBSH_INBOUND	= 0x100,
	TOK_SUBSH_OUTBOUND	= 0x200
};

enum e_tok_frag
{
	LTRL,
	XPNDBL,
	SPLTBL
};

typedef struct s_tok_frag
{
	char			*val;
	size_t			len;
	enum e_tok_frag	type;
}	t_tok_frag;

typedef struct s_tok
{
	t_list		*frags;
	enum e_tok	type;
}	t_tok;

enum e_lex_stt
{
	LEX_UNQUOTED		= 0x1,
	LEX_PARTLY_QUOTED	= 0x2,
	LEX_QUOTED			= 0x4,
	LEX_ENDBOUND		= 0x8,
	LEX_WHITESPACE_OCC	= 0x10,
	LEX_WORD_OCC		= 0x20,
	LEX_PARAM_OCC		= 0x40,
	LEX_ASGNMT_OCC		= 0x80,
	LEX_REDIR_OPRTR_OCC = 0x100,
	LEX_CTRL_OPRTR_OCC	= 0x200
};

# define LEX_STT_MASK 0x7

typedef struct s_lexer
{
	const char		*off;
	t_list			*toks;
	bool			tok_occ;
	unsigned short	stt;
	unsigned short	ref_stt;
}	t_lexer;

struct s_lex_cntx
{
	t_tok		*tok;
	const char	*bounds[2];
	t_list		*i;
};

t_list	*analyse_prompt(t_lexer *lexer, t_logger *logger)
		__attribute__((nonnull(1)));

void	conclude_lex_analysis(t_lexer *lexer)
		__attribute__((nonnull(1)));

#endif