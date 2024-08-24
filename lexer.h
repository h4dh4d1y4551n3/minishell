/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhadhadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 03:07:15 by yhadhadi          #+#    #+#             */
/*   Updated: 2024/08/24 15:11:50 by yhadhadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "libft.h"
# include "logger.h"

enum e_tok
{
	TOK_END			= 0x00,
	TOK_WORD		= 0x01,
	TOK_ASGNMT		= 0x02,
	TOK_REDIR_OPRTR	= 0x04,
	TOK_PIPE		= 0x08,
	TOK_CHAIN_OPRTR	= 0x10,
	TOK_SUBSH_BOUND	= 0x20
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
	size_t		frags_cnt;
	void		*cntx;
	enum e_tok	type;
}	t_tok;

typedef enum e_lex_state
{
	LEX_UNQUOTED		= 0x00,
	LEX_PARTLY_QUOTED	= 0x01,
	LEX_QUOTED			= 0x02
}	t_lex_state;

typedef struct s_lexer
{
	const char	*off;
	t_list		*toks;
	t_lex_state	state;
}	t_lexer;

typedef enum e_lex_substate
{
	LEX_BOUND		= 0x00,
	LEX_WHITESPACE	= 0x01,
	LEX_WORD		= 0x02,
	LEX_VAR			= 0x04,
	LEX_ASGNMT		= 0x08,
	LEX_OPRTR		= 0x10,
	LEX_SUBSH_BOUND = 0X20
}	t_lex_substate;

// analyse_prompt (This will be a function that will encapsulate the lexer
// automaton and will use both eval_lex_state and identify_tok to fully identify
// all tokens of a given prompt)
t_list				*analyse_prompt(t_lexer *lexer, t_logger *logger)
					__attribute__((nonnull(1)));

// conclude_lex_analysis (Clean up function for a lexer)
void				conclude_lex_analysis(t_lexer *lexer)
					__attribute__((nonnull(1)));

#endif