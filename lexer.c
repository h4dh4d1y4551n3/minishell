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
// main.c

#include "lexer.h"
#include <stdio.h>

void print_tree(t_tree_node *node, int depth) {
    if (!node) return;

    for (int i = 0; i < depth; i++) printf("  ");

    switch (node->type) {
        case NODE_COMMAND:
            printf("COMMAND: ");
            for (int i = 0; node->command.args[i]; i++) {
                t_list *frag = node->command.args[i];
                while (frag) {
                    t_tok_frag *tf = frag->data;
                    printf("%.*s ", (int)tf->len, tf->val);
                    frag = frag->next;
                }
            }
            printf("\n");
            break;
        case NODE_PIPE:
            printf("PIPE\n");
            print_tree(node->op.left, depth + 1);
            print_tree(node->op.right, depth + 1);
            break;
        case NODE_AND:
            printf("AND\n");
            print_tree(node->op.left, depth + 1);
            print_tree(node->op.right, depth + 1);
            break;
        case NODE_OR:
            printf("OR\n");
            print_tree(node->op.left, depth + 1);
            print_tree(node->op.right, depth + 1);
            break;
        case NODE_REDIR:
        case NODE_APPEND:
        case NODE_REDIR_IN:
        case NODE_HEREDOC:
            printf("REDIRECTION: ");
            t_tok_frag *tf = node->redir.file->data;
            printf("%.*s\n", (int)tf->len, tf->val);
            print_tree(node->redir.command, depth + 1);
            break;
        default:
            printf("UNKNOWN NODE TYPE\n");
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s \"command\"\n", argv[0]);
        return 1;
    }

    t_lexer lexer;
    lexer.off = argv[1];
    lexer.state = LEX_UNQUOTED;

    analyse_prompt(&lexer);
    
    // Skip TOK_START
    lexer.toks = lexer.toks->next;

    t_tree_node *root = parse_tree(&lexer.toks);

    printf("Parsed Tree:\n");
    print_tree(root, 0);

    // TODO: Free the tree and tokens

    return 0;
}