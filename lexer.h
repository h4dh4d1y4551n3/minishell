/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baarif <baarif@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 03:07:15 by yhadhadi          #+#    #+#             */
/*   Updated: 2024/09/02 00:52:29 by baarif           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "libft/libft.h"

enum			e_tok
{
	TOK_END = 0x00,
	TOK_WORD = 0x01,
	TOK_PIPE = 0x02,
	TOK_REDIR_OPRTR = 0x04,
	TOK_LEFT_PARAN = 16,
	TOK_RIGHT_PARAN = 32,
	TOK_LOGIC_OPRTR = 64
};
typedef struct s_tok_frag
{
	char		*val;
	size_t		len;
	bool		xpandabl;
}				t_tok_frag;

typedef struct s_tok
{
	t_list		*frags;
	size_t		frags_cnt;
	enum e_tok	type;
	int			eot;
}				t_tok;

// Context structs
struct			s_asgnmt
{
	char		*assignee_id;
	size_t		id_len;
};

// What about redirection context do we need to extract infor prior?
struct			s_redir
{
};

typedef enum e_lex_state
{
	LEX_UNQUOTED = 0x00,
	LEX_PARTLY_QUOTED = 0x01,
	LEX_QUOTED = 0x02
}				t_lex_state;

typedef struct s_lexer
{
	const char	*off;
	t_list		*toks;
	t_lex_state	state;
}				t_lexer;

typedef enum e_lex_substate
{
	LEX_BOUND = 0x00,
	LEX_WHITESPACE = 0x01,
	LEX_WORD = 0x02,
	LEX_PARAM = 0x04,
	LEX_ASGNMT = 0x08,
	LEX_REDIR_OPRTR = 16,
	LEX_CTRL_OPRTR = 32
}				t_lex_substate;

// analyse_prompt (This will be a function that will encapsulate the lexer
// automaton and will use both eval_lex_state and identify_tok to fully identify
// all tokens of a given prompt)
t_list			*analyse_prompt(t_lexer *lexer);
__attribute__((nonnull(1)));

// conclude_lex_analysis (Clean up function for a lexer)
void			conclude_lex_analysis(t_lexer *lexer) __attribute__((nonnull(1)));

#endif