/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhadhadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:10:06 by yhadhadi          #+#    #+#             */
/*   Updated: 2024/08/25 10:26:39 by yhadhadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft/libft.h"
#include <stdlib.h>
#include <stdio.h>
// eval_lex_state (This function will be handle the lexical analysis state transition this is the core of the lexer automaton)
static t_lex_state	eval_lex_state(t_lexer *lexer);

// identify_tok (like strtok but in the context of our lexer it will get next token each time called and also detect the end of the prompt thus there is no more token to identify)
t_tok		*identify_tok(t_lexer *lexer);

// t_list	*analyse_prompt(t_lexer *lexer, t_logger *logger)
// {
// 	t_tok	*tok;
// 	t_list	*i;
// 	t_list	*node;

// 	i = lexer->toks;
// 	while (*lexer->off)
// 	{
// 		// Populate toks using identify_tok
// 		lexer->state = eval_lex_state(lexer);
// 		tok = identify_tok(lexer);
// 		if (!tok)
// 			break ;
// 		// Need error check for malloc failure!
// 		// Pre-syntax eval
// 		// TODO
// 		node = ft_lstnew(tok);
// 		// if (!node)
// 		//	return and handle error type
// 		if (i)
// 			i->next = node;
// 		i = node;
// 		// This is just a basic version this might be subdue to changes since I
// 		// still didn't decide how identify_tok will handle errors yet the
// 		// interface is too simple for now
// 	}
// 	return (lexer->toks);
// }
static enum e_tok ctrl_operator(char *s)
{
    if (ft_strncmp(s, "&&", 2) == 0 || ft_strncmp(s, "||", 2) == 0)
        return (TOK_LOGIC_OPRTR);
    if (ft_strncmp(s, "(", 1) == 0)
        return (TOK_LEFT_PARAN);
    if (ft_strncmp(s, ")", 1) == 0)
        return (TOK_RIGHT_PARAN);
}
static t_lex_substate	eval_lex_substate(t_lexer *lexer)
{
	if (!*lexer->off)
		return (LEX_BOUND);
	if (ft_isspace(*lexer->off))
		return (LEX_WHITESPACE);
	if (*lexer->off == '$')
		return (LEX_PARAM);
	if (ft_strchr("<>", *lexer->off))
		return (LEX_REDIR_OPRTR);
	if (ft_strchr("&|()", *lexer->off))
		return (LEX_CTRL_OPRTR);
	return (LEX_WORD);
}
static t_lex_state	eval_lex_state(t_lexer *lexer)
{
	// LEX_QUOTED OR LEX_PARTLY_QUOTED
	if ((lexer->state == LEX_QUOTED && *lexer->off == '\'')
		|| (lexer->state == LEX_PARTLY_QUOTED && *lexer->off == '"'))
		return (++lexer->off, LEX_UNQUOTED);
	// LEX_UNQUOTED
	if (*lexer->off == '\'')
		return (++lexer->off, LEX_QUOTED);
	if (*lexer->off == '"')
		return (++lexer->off, LEX_PARTLY_QUOTED);
	return (lexer->state);
}
t_tok_frag *frag_class(const char *s, size_t size, bool can_expand)
{
    t_tok_frag *token = malloc(sizeof (t_tok_frag));
    if (!token) return NULL;
    token->val = (char *) s;
    token->len = size;
    token->xpandabl = can_expand;
    return token;
}
static void	identify_unquoted_tok(t_tok *tok, t_lexer *lexer)
{
	const char		*bounds[2] = {lexer->off, NULL};
	t_lex_substate	state;
	//skips initial spaces
	while (eval_lex_substate(lexer) == LEX_WHITESPACE)
        lexer->off++;
	while (*lexer->off)
	{
		state = eval_lex_substate(lexer);
		//if it finds a whitespace after a word it will break
		if (state == LEX_WHITESPACE)
			return ;
		else if (state == LEX_WORD)
		{
		  bounds[0] = lexer->off;
				tok->frags_cnt++;
				tok->type = TOK_WORD;
		  while ((state = eval_lex_substate(lexer)) == LEX_WORD)
				lexer->off++;
			ft_lstadd_back(&tok->frags, ft_lstnew(frag_class(bounds[0], lexer->off - bounds[0], 1)));	
		}
		else if (state == LEX_CTRL_OPRTR )
	    {
					tok->type = ctrl_operator((char *) lexer->off);
					lexer->off += tok->type == TOK_LOGIC_OPRTR ? 2 : 1;
					return;
		}
		else if (state == LEX_PARAM) 
		{
		  lexer->off++;
		  bounds[0] = lexer->off;
				            while ((state = eval_lex_substate(lexer)) == LEX_WORD)
								lexer->off++;
		             ft_lstadd_back(&tok->frags, ft_lstnew(frag_class(bounds[0], lexer->off - bounds[0], 1)));
		}
		else if (state == LEX_REDIR_OPRTR)
		{}	// Not sure how this should be dealth with in relation to
			// information extraction
		else if (state == LEX_CTRL_OPRTR)
		{}	// Retrieve valid monographs ignore invalid one look ahead in case
			// of digraphs then retrieve them if valid or ignore them if not.
			// When I say ignore I mean switch the state back to whatever
			// following consumable char is without updating the bounds[0] wich
			// is the token start and everything will flow back to the automaton
	}
	
	// All the above checks should return out of the function since if we are
	// out of the loop by mean of !*lexer->off then tok->type should be TOK_END
	tok->type = TOK_END;
}

t_tok	*identify_tok(t_lexer *lexer)
{
	t_tok		*tok;

	tok = (t_tok *)malloc(sizeof(t_tok));
	if (!tok)
		return (NULL);
	*tok = (t_tok){};
	if (lexer->state & (LEX_QUOTED | LEX_PARTLY_QUOTED));
	else
		identify_unquoted_tok(tok, lexer);
	return (tok);
}

static void	identify_quoted_tok(t_tok *tok, t_lexer *lexer)
{
	// Check state and handle quoted part differently from partly quoted parts The logic is similar to unquoted but less stricted since fewer state are acknowledged 
}

int main()
{
    t_tok	*tok;
    	t_tok_frag	*i;
    	t_list	*node;
        	t_lexer	*lexer;
        
    
 
    		// Populate toks using identify_tok
        		lexer->off = "ls$USER hello";
    		lexer->state = eval_lex_state(lexer);
    		tok = identify_tok(lexer);
                  		i = tok->frags;
                            		while (i)
                                		{
                                			printf("%.*s\n", ((t_tok_frag *)i->val)->len, ((t_tok_frag *)i->)->val);
                                			i = i->next;
                                		}
}