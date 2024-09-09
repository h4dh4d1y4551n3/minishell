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

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

void print_all_fragment(t_list *lst)
{
    while (lst)
    {
        printf("%.*s\n",  ((t_tok_frag *) (lst->data))->len, ((t_tok_frag *) (lst->data))->val);
        lst = lst->next;
    }
}
void print_tree( t_tree_node *tree)
{
    if (!tree)
        return;
    if (tree->type == NODE_COMMAND)
    {
        printf("COMMAND\n");
        int i = 0;
        while (tree->command.args[i])
        {
            t_list *lst = tree->command.args[i];
            while (lst)
            {
                printf("%.*s\n",  ((t_tok_frag *) (lst->data))->len, ((t_tok_frag *) (lst->data))->val);
                lst = lst->next;
            }
            i++;
        }
    }
    else if (tree->type == NODE_PIPE)
    {
        printf("PIPE\n");
        print_tree(tree->op.left);
        print_tree(tree->op.right);
    }
    else if (tree->type == NODE_AND)
    {
        printf("AND\n");
        print_tree(tree->op.left);
        print_tree(tree->op.right);
    }
    else if (tree->type == NODE_OR)
    {
        printf("OR\n");
        print_tree(tree->op.left);
        print_tree(tree->op.right);
    }
    else if (tree->type == NODE_REDIR)
    {
        printf("REDIR\n");
        print_tree(tree->redir.command);
        print_all_fragment(tree->redir.file);
    }
    else if (tree->type == NODE_REDIR_IN)
    {
        printf("REDIR_IN\n");
        print_tree(tree->redir.command);
        print_all_fragment(tree->redir.file);
    }
    else if (tree->type == NODE_REDIR)
    {
        printf("REDIR_OUT\n");
        print_tree(tree->redir.command);
        print_all_fragment(tree->redir.file);
    }
    else if (tree->type == NODE_HEREDOC)
    {
        printf("REDIR_APPEND\n");
        print_tree(tree->redir.command);
        print_all_fragment(tree->redir.file);
    }
    else if (tree->type == NODE_APPEND)
    {
        printf("REDIR_APPEND\n");
        print_tree(tree->redir.command);
        print_all_fragment(tree->redir.file);
    }
}

int main(void)
{
    t_lexer lexer;
    lexer.off = "< file | hello ";
    lexer.state = LEX_UNQUOTED;

    // Analyze the prompt and tokenize it
    analyse_prompt(&lexer);
    lexer.toks = lexer.toks->next;
    // Parse the tokens into a syntax tree
    t_tree_node *tree = parse_tree(&lexer.toks);

    // Execute the syntax tre
    int i = 0;
    // while (tree->command.args[i])
    // {
    //     t_list *lst = tree->command.args[i];
    //     while (lst)
    //     {
    //         printf("%.*s\n",  ((t_tok_frag *) (lst->data))->len, ((t_tok_frag *) (lst->data))->val);
    //         lst = lst->next;
    //     }
    //     i++;
    // }
    // Print the syntax tree
    print_tree(tree);
    return 0;
    
}
