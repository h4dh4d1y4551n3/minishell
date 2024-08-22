/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhadhadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 03:07:15 by yhadhadi          #+#    #+#             */
/*   Updated: 2024/08/22 06:13:01 by yhadhadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"

enum e_tok
{
	TOK_EOF				= 0x01,
	TOK_EOL				= 0x02,
	TOK_WORD			= 0x04,
	TOK_ASGNMT			= 0x08,
	TOK_REDIR_OP		= 0x10,
	TOK_PIPE			= 0x20,
	TOK_CHAIN_OP		= 0x40,
	TOK_SUBSHELL_BOUND	= 0x80
};

union u_symbol
{
	char	unigram;
	char	*bigram;
};

// Contexts
typedef struct s_redir
{
	union u_symbol	type;
	// We will add fields as needed
}	t_redir;

typedef struct s_chain
{
	union u_symbol	type;
	// We will add fields as needed
}	t_chain;

typedef struct s_subshell
{
	union u_symbol	type;
	// We will add fields as needed
}	t_subshell;

typedef struct s_tok
{
	enum e_tok	type;
	char		*str;
	ssize_t		len;
	void		*cntx;
}	t_tok;

typedef struct s_lex
{
	const char	*input;
	int			off;
	// Other lexer state needed
}	t_lex;

#endif