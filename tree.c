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
