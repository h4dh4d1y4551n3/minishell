/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhadhadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 03:07:15 by yhadhadi          #+#    #+#             */
/*   Updated: 2024/08/23 04:00:16 by yhadhadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "libft/libft.h"
    
typedef enum e_tok
{
	TOK_EOF			= 0x01,
	TOK_EOL			= 0x02,
	TOK_WORD		= 0x04,
	TOK_ASGNMT		= 0x08,
	TOK_REDIR_OP	= 0x10,
	TOK_PIPE		= 0x20,
	TOK_CHAIN_OP	= 0x40,
	TOK_SUBSH_BOUND	= 0x80
} t_type;

// This structure is for 
typedef struct s_tok_frag
{
	char	*val;
	size_t	len;
	bool	xpandabl;
}	t_tok_frag;

typedef struct s_tok
{
	t_list		*strm;
	size_t		frag_cnt;
	t_type	type;
}	t_tok;

typedef enum e_lex_state
{
	LEX_UNQUOTED,
	LEX_PARTLY_QUOTED,
	LEX_QUOTED
}	t_lex_state;

typedef struct s_lexer
{
	const char	*off;
	t_list		*toks;
	t_list		*recent_tok;
	t_lex_state	state;
}	t_lexer;

// analyse_prompt (This will be a function that will encapsulate the lexer
// automaton and will use both eval_lex_state and identify_tok to fully identify
// all tokens of a given prompt)
t_list				*analyse_prompt(t_lexer *lexer)
					__attribute__((nonnull(1)));

// conclude_lex_analysis (Clean up function for a lexer)
void				conclude_lex_analysis(t_lexer *lexer)
					__attribute__((nonnull(1)));


#endif