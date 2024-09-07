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
#include "lexer.h"


enum t_type  get_token_type(t_tok *token)
{
    if (token->type == TOK_WORD)
        return (NODE_COMMAND);
    if (token->type == TOK_LOGIC_OPRTR && *(short *)((t_tok_frag *)token->frags->data)->val == *(short *)"&&")
        return (NODE_AND);
    if (token->type == TOK_LOGIC_OPRTR && *(short *)((t_tok_frag *)token->frags->data)->val == *(short *)"||")
        return (NODE_OR);
    if (token->type == TOK_PIPE)
        return (NODE_PIPE);
    if (token->type == TOK_REDIR_OPRTR)
        {
            if (*(short *)((t_tok_frag *)token->frags->data)->val == *(short *)">>")
                return (NODE_APPEND);
            if (*(short *)((t_tok_frag *)token->frags->data)->val == *(short *)">")
                return (NODE_REDIR);
            if (*(short *)((t_tok_frag *)token->frags->data)->val == *(short *)"<")
                return (NODE_REDIR_IN);
            if (*(short *)((t_tok_frag *)token->frags->data)->val == *(short *)"<<")
                return (NODE_HEREDOC);
        }
    return (NODE_END);
}
t_tree_node *create_node(t_tok *token)
{
    t_tree_node *node = (t_tree_node *)malloc(sizeof( t_tree_node));
    node->type = get_token_type(token);
    return (node);
}

t_tree_node *parse_words(t_list **tokens)
{
    size_t arguments = 0;
    t_list *current = *tokens;
    t_list **args;
    int i = 0;
    if (((t_tok *)(current->data))->type != TOK_WORD)
        return NULL;
    t_tree_node *node = create_node(current->data);
    while (current && ((t_tok *)current->data)->type == TOK_WORD)
    {
        arguments++;
        current = current->next;
    } 
    args = (t_list **)malloc((sizeof(t_list *)) * ( 1 + arguments));
    while (i < arguments)
    {
        args[i] = ((t_tok *)((*tokens)->data))->frags;
        *tokens = (*tokens)->next;
        i++;
    }
    *tokens = current;
    args[i] = NULL;
    node->command.args = args;  
    return (node); 
}

t_tree_node *parse_command(t_list **tokens)
{
 
   // int i = 0;
   //  while (command && command->command.args[i])
   //  {
   //      t_list *lst_frags = command->command.args[i];
   //      while (lst_frags)
   //      {
   //         printf("%.*s\n",  ((t_tok_frag *) (lst_frags->data))->len, ((t_tok_frag *) (lst_frags->data))->val);
   //          lst_frags = lst_frags->next;
   //      }
   //      i++;
        
   //  }
        t_tree_node *command = parse_words(tokens);
        if (!*tokens)
            return (command);
        if (((t_tok *)(*tokens)->data)->type == TOK_REDIR_OPRTR)
        {
            t_tree_node *node = create_node((*tokens)->data);
            node->redir.command = command;
            *tokens = (*tokens)->next;
            node->redir.file = ((t_tok *) (*tokens)->data)->frags;
            *tokens = (*tokens)->next;
            command = node;
        }
        return command;
}

t_tree_node *parse_pipe(t_list **tokens)
{
    t_tree_node *command = parse_command(tokens);
    if (!*tokens)
        return (command);
    if (((t_tok *)(*tokens)->data)->type == TOK_PIPE)
    {
        t_tree_node *node = create_node((*tokens)->data);
        node->op.left = command;
        *tokens = (*tokens)->next;
        node->op.right = parse_pipe(tokens);
        return (node);
    }
    return (command);
}

t_tree_node *parse_and(t_list **tokens)
{
    t_tree_node *command = parse_pipe(tokens);
    if (!*tokens)
        return (command);
    if (((t_tok *)(*tokens)->data)->type == TOK_LOGIC_OPRTR && ((t_tok_frag *)(*tokens)->data)->val[0] == '&')
    {
        t_tree_node *node = create_node((*tokens)->data);
        node->op.left = command;
        *tokens = (*tokens)->next;
        node->op.right = parse_and(tokens);
        return (node);
    }
    return (command);
}

t_tree_node *parse_or(t_list **tokens)
{
    t_tree_node *command = parse_and(tokens);
    if (!*tokens)
        return (command);
    if (((t_tok *)(*tokens)->data)->type == TOK_LOGIC_OPRTR && ((t_tok_frag *)(*tokens)->data)->val[0] == '|')
    {
        t_tree_node *node = create_node((*tokens)->data);
        node->op.left = command;
        *tokens = (*tokens)->next;
        node->op.right = parse_or(tokens);
        return (node);
    }
    return (command);
}

t_tree_node *parse_tree(t_list **tokens)
{
    return (parse_or(tokens));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baarif <baarif@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:10:06 by yhadhadi          #+#    #+#             */
/*   Updated: 2024/09/02 00:52:20 by baarif           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft/libft.h"

int	main(void)
{
	t_tok *tok;
	t_tok_frag *i;
	t_list *node;
	t_lexer lexer;

	// Populate toks using identify_tok
	//dont suggest to use this function
    	lexer.off = "echo hello world";
	lexer.state = LEX_UNQUOTED;
	// tok = identify_tok(&lexer);
	// // Print toks
	// while (tok->type != TOK_END)
	// {
	// 	printf("tok type: %d with val : ", tok->type);
	// 	node = tok->frags;
	// 	while (node)
	// 	{
	// 		i = (t_tok_frag *)node->data;
	// 		printf("--->%.*s\n", (int)i->len, i->val);
	// 		node = node->next;
	// 	}
	// 	tok = identify_tok(&lexer);
	// }
	// printf("tok type: %d\n", tok->type);
	analyse_prompt(&lexer);
    parse_tree(&lexer.toks); 	
    
//     while (lexer.toks)
//     {
//         tok = (t_tok *)lexer.toks->data;
//         printf("tok type: %d with val : ", tok->type);
//         node = tok->frags;
//         while (node)
//         {
//             i = (t_tok_frag *)node->data;
//             printf("--->%.*s\n", (int)i->len, i->val);
//             node = node->next;
//         }
//         lexer.toks = lexer.toks->next;
//     }
// }
