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
	TOK_OPEN_PARAN = 16,
	TOK_CLOSE_PARAN = 32,
	TOK_LOGIC_OPRTR = 64,
	TOK_START = 128
};
typedef struct s_tok_frag
{
	char		*val;
	size_t		len;
	bool		xpandabl;
	bool        quoted;
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
 enum t_type {NODE_AND, NODE_OR, NODE_PIPE, NODE_COMMAND, NODE_APPEND, NODE_REDIR_IN , NODE_REDIR,NODE_HEREDOC, NODE_END} ;
typedef struct s_tree_node {
    enum t_type type; 
    union {
        struct {
            struct s_tree_node *left;
            struct s_tree_node *right;
        } op;
        struct {
            t_list **args;
        } command;
        struct {
            t_list *file;
            struct s_tree_node *command;
        } redir;
        };
} t_tree_node;
// analyse_prompt (This will be a function that will encapsulate the lexer
// automaton and will use both eval_lex_state and identify_tok to fully identify
// all tokens of a given prompt)
t_list			*analyse_prompt(t_lexer *lexer);
__attribute__((nonnull(1)));

// conclude_lex_analysis (Clean up function for a lexer)
void			conclude_lex_analysis(t_lexer *lexer) __attribute__((nonnull(1)));
bool expects(enum e_tok current, enum e_tok next);
enum e_tok	ctrl_operator(char *s);
t_lex_state eval_lex_state(t_lexer *lexer);
t_tok_frag	*frag_class(const char *s, size_t size, bool can_expand, bool is_quote);
t_lex_substate	eval_lex_substate(t_lexer *lexer);
void	identify_quoted_tok(t_tok *tok, t_lexer *lexer);
void	identify_unquoted_tok(t_tok *tok, t_lexer *lexer);
t_list	*analyse_prompt(t_lexer *lexer);
t_tree_node *parse_tree(t_list **tokens);
t_tok	*identify_tok(t_lexer *lexer);
#endif
