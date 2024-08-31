/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhadhadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 03:07:15 by yhadhadi          #+#    #+#             */
/*   Updated: 2024/08/31 18:52:47 by yhadhadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "libft.h"
# include "logger.h"

enum e_tok
{
	TOK_END				= 0x00,
	TOK_WORD			= 0x01,
	TOK_IN_REDIR		= 0x02,
	TOK_OUT_REDIR		= 0x04,
	TOK_APPEND			= 0x08,
	TOK_HEREDOC			= 0x10,
	TOK_PIPE			= 0x20,
	TOK_AND				= 0x40,
	TOK_OR				= 0x80,
	TOK_SUBSH_INBOUND	= 0x100,
	TOK_SUBSH_OUTBOUND	= 0x200

};

typedef struct s_tok_frag
{
	char	*val;
	size_t	len;
	bool	xpandabl;
}	t_tok_frag;

typedef struct s_tok
{
	t_list		*frags;
	enum e_tok	type;
}	t_tok;

typedef enum e_lex_stt
{
	LEX_UNQUOTED		= 0x00,
	LEX_PARTLY_QUOTED	= 0x01,
	LEX_QUOTED			= 0x02
}	t_lex_stt;

typedef struct s_lexer
{
	const char	*off;
	t_list		*toks;
	t_lex_stt	stt;
	bool		tok_occ;
}	t_lexer;

typedef enum e_lex_substt
{
	LEX_BOUND		= 0x00,
	LEX_WHITESPACE	= 0x01,
	LEX_WORD		= 0x02,
	LEX_PARAM		= 0x04,
	LEX_ASGNMT		= 0x08,
	LEX_REDIR_OPRTR = 0x10,
	LEX_CTRL_OPRTR	= 0x20
}	t_lex_substt;

t_list	*analyse_prompt(t_lexer *lexer, t_logger *logger)
		__attribute__((nonnull(1)));

void	conclude_lex_analysis(t_lexer *lexer)
		__attribute__((nonnull(1)));

#endif