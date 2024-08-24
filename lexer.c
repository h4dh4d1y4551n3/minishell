/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhadhadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:10:06 by yhadhadi          #+#    #+#             */
/*   Updated: 2024/08/23 04:08:17 by yhadhadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft/libft.h"

// eval_lex_state (This function will be handle the lexical analysis state transition this is the core of the lexer automaton)
static t_lex_state	eval_lex_state(t_lexer *lexer);

// identify_tok (like strtok but in the context of our lexer it will get next token each time called and also detect the end of the prompt thus there is no more token to identify)
t_tok		*identify_tok(t_lexer *lexer);

t_list	*analyse_prompt(t_lexer *lexer)
{
	t_list	*tok;
    lexer->state = LEX_UNQUOTED;
	while (*lexer->off)
	{
		// Populate toks using identify_tok
		lexer->state = eval_lex_state(lexer);
		tok->data = identify_tok(lexer);
		if (!tok)
			break ;
		// Expect
		if (!lexer->toks)
			lexer->toks = tok;
		lexer->recent_tok->next = tok;
		// This is just a basic version this might be subdue to changes since I
		// still ddin't decide how identify_tok will handle errors yet the
		// interface is too simple for now
	}
	return (lexer->toks);
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
struct s_tok_frag *frag_class(char *value, int size, bool xpandble)
{
    struct s_tok_frag *token = malloc(sizeof(struct s_tok_frag));
    if (!token)
        return (NULL);
    token->val = value ? value : NULL;
    token->len = size;
    token->xpandabl = xpandble;
    return (token);
}
char *word_frags(char *offset, t_lexer *lexer, t_list **frags)
{
    char *start = offset;
    t_lex_state state;

    while (*offset)
    {
        state = eval_lex_state(lexer);
        if (state == LEX_UNQUOTED &&
                (*offset == ' ' || *offset == '|' ||
                 (*offset == '|' && *(offset + 1) == '|') ||
                 (*offset == '&' && *(offset + 1) == '&') ||
                 (*offset == '<' && *(offset + 1) == '<') ||
                 (*offset == '<') ||
                 (*offset == '>' && *(offset + 1) == '>') ||
                 (*offset == '>') || 
                (*(offset + 1) == 0)
                ))
        {
            ft_lstadd_back(frags,ft_lstnew(frag_class(start, offset - start + 1, true)));
            
            if (*offset == ' ')
                break;
            start = offset;
        }
        else if (state == LEX_QUOTED)
        {
            while (eval_lex_state(lexer) == LEX_QUOTED)
                offset++;
            (ft_lstadd_back(frags,ft_lstnew(frag_class(offset, offset-start, false))));
            if (*offset == ' ')
                break;
            start = offset;
        }
        else if (state == LEX_PARTLY_QUOTED)
        {
            start++;
            while (eval_lex_state(lexer) == LEX_PARTLY_QUOTED)
                offset++;
            printf("%c %c\n", *offset, *start); 
            (ft_lstadd_back(frags,ft_lstnew(frag_class(offset, offset - start, false))));
                offset++;
                if (*offset == ' ')
                    break;
        }
        offset++;
    }
    return (offset);
}

t_tok *token_class(t_type type, char **offset, t_lexer *lexer, t_list **frags)
{
    
    t_tok *token = malloc(sizeof(t_tok));
    lexer->state = LEX_UNQUOTED;
    if (!token)
        return (NULL);
    token->type = type;
    if (type == TOK_CHAIN_OP)
         token->strm = ft_lstnew(frag_class(*offset, 2, false));
    else if (type == TOK_PIPE)
         token->strm = ft_lstnew(frag_class(*offset, 1, false));
    else if (type == TOK_WORD)
    {
       *offset = word_frags(*offset, lexer, &token->strm);
    }

    return (token);
}
t_tok	*identify_tok(t_lexer *lexer)
{
    char *start = (char *) lexer->off;
    while (*start)
    {
        while (*start == ' ')
            start++;
        if (*start == '\0')
            return (NULL);
        if (*start == '|' && *(start + 1) == '|')
        {
            start += 2;
            return (token_class(TOK_CHAIN_OP, &start, lexer,&lexer->toks)) ;
        }
        else
            return token_class(TOK_WORD, &start, lexer, &lexer->toks);
    }
}
int main()
{
    t_lexer lexer;
    lexer.off = "\"hello\"";
    t_tok *tok = identify_tok(&lexer);
    printf("%s\n", ((t_tok_frag *)((tok->strm)->data))->val);
    return (0);
}
