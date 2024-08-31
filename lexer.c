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

t_list	*analyse_prompt(t_lexer *lexer)
{
	t_tok	*tok;
	t_list	*i;
	t_list	*node;

	i = lexer->toks;
	while (*lexer->off)
	{
		lexer->state = eval_lex_state(lexer);
		tok = identify_tok(lexer);
		if (!tok)
			break ;
		// Need error check for malloc failure!
		// Pre-syntax eval
		// TODO
		node = ft_lstnew(tok);
		// if (!node)
		//	return and handle error type
		if (i)
			i->next = node;
		i = node;
		// This is just a basic version this might be subdue to changes since I
		// still didn't decide how identify_tok will handle errors yet the
		// interface is too simple for now
	}
	return (lexer->toks);
}
static enum e_tok ctrl_operator(char *s)
{
    if (ft_strncmp(s, "&&", 2) == 0 || ft_strncmp(s, "||", 2) == 0)
        return (TOK_LOGIC_OPRTR);
    if (ft_strncmp(s, "(", 1) == 0)
        return (TOK_LEFT_PARAN);
    if (ft_strncmp(s, ")", 1) == 0)
        return (TOK_RIGHT_PARAN);
    printf("--->%d\n" , TOK_LOGIC_OPRTR);
    return (TOK_PIPE);
    
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
		{
		  lexer->state = LEX_UNQUOTED;
		return (++lexer->off, LEX_UNQUOTED);
		}
	// LEX_UNQUOTED
	if (*lexer->off == '\'')
	{  
	   lexer->state = LEX_QUOTED;
		return (++lexer->off, LEX_QUOTED);
	}
	if (*lexer->off == '"')
	{
	   lexer->state = LEX_PARTLY_QUOTED;
		return (++lexer->off, LEX_PARTLY_QUOTED);
	}
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
static void identify_unquoted_tok(t_tok *tok, t_lexer *lexer)
{
    const char *start = lexer->off;
    t_lex_substate state;
    state =  eval_lex_substate(lexer);
    if (state & (LEX_WORD | LEX_PARAM))
    {
        tok->type = TOK_WORD;
        if (state == LEX_PARAM)
            lexer->off++;
        start = lexer->off;
        while ((state = eval_lex_substate(lexer)) == LEX_WORD && eval_lex_state(lexer) == LEX_UNQUOTED)
            lexer->off++;
        ft_lstadd_back(&tok->frags, ft_lstnew(frag_class(start, lexer->off - start - (state == LEX_WORD), 1)));
        tok->frags_cnt++;
    }
    else if (state == LEX_CTRL_OPRTR)
    {
        start = lexer->off;
        tok->type = ctrl_operator((char *)lexer->off);
        lexer->off += (tok->type == TOK_LOGIC_OPRTR) ? 2 : 1;
        ft_lstadd_back(&tok->frags, ft_lstnew(frag_class(start, lexer->off - start, 0)));
        tok->frags_cnt++;
    }
    else if (state == LEX_REDIR_OPRTR)
       {
           tok->type = TOK_REDIR_OPRTR;
           tok->frags_cnt++;
   
           int len = (lexer->off[0] == lexer->off[1]) ? 2 : 1;
           ft_lstadd_back(&tok->frags, ft_lstnew(frag_class(lexer->off, len, 0)));
           lexer->off += len;
       }
    tok->eot = ft_isspace(*lexer->off);
    lexer->state = eval_lex_state(lexer);
}

static void	identify_quoted_tok(t_tok *tok, t_lexer *lexer)
{
    const char		*bounds[2] = {lexer->off, NULL};
    bool canExpand;
    canExpand = lexer->state == LEX_PARTLY_QUOTED;
    bounds[0] = lexer->off;
    while (*lexer->off && eval_lex_state(lexer) != LEX_UNQUOTED)
        lexer->off++;
    tok->type = TOK_WORD;
    ft_lstadd_back(&tok->frags, ft_lstnew(frag_class(bounds[0], lexer->off - bounds[0]-1, canExpand )));
    tok->eot = ft_isspace(*lexer->off);
}

t_tok	*identify_tok(t_lexer *lexer)
{
	t_tok		*tok;

	tok = (t_tok *)malloc(sizeof(t_tok));
	if (!tok)
		return (NULL);
	*tok = (t_tok){};
	tok->eot = 0;
	while (ft_isspace(*lexer->off))
        lexer->off++;
	if (*lexer->off == 0)
           tok->type = TOK_END;
	lexer->state = eval_lex_state(lexer);
	while (*lexer->off && !tok->eot)
	{
   	    if (lexer->state == LEX_UNQUOTED)
                identify_unquoted_tok(tok, lexer);
        if (lexer->state & (LEX_QUOTED | LEX_PARTLY_QUOTED) && !tok->eot)
                      identify_quoted_tok(tok, lexer);
	}
	return (tok);
}


int main()
{
    t_tok	*tok;
    	t_tok_frag	*i;
    	t_list	*node;
        	t_lexer	lexer;
        
    
 
    		// Populate toks using identify_tok
        		lexer.off = "echo \"hello world\"$USER-'pepe' > file.txt && ls -l | cat -e && echo $HOME && echo $USER && echo $PATH && echo $PWD";
    		lexer.state = LEX_UNQUOTED;
                    		tok = identify_tok(&lexer);
                              		// Print toks
                while (tok->type != TOK_END)
                {
                    printf("tok type: %d with val : ", tok->type);
                    node = tok->frags;
                    while (node)
                    {
                        i = (t_tok_frag *)node->data;
                        printf("%.*s\n",(int) i->len,i->val);
                        node = node->next;
                    }
                    tok = identify_tok(&lexer);
                }
                printf("tok type: %d\n", tok->type);
 }