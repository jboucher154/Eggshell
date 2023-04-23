#include "minishell.h"
#include "ft_ast.h"

// void check_commands(char *line)
// {
    //check for valid commands.
// }

// void check_pipes(char *line)
// {
    //pipe placement.
    //  - ‘<|’ ⇒ERROR
    // ‘;|’ OR ‘|’ (first) ⇒ ERROR
    //- ‘>>|’ ⇒ ERROR
    //- ‘>|’ ⇒ OKAY
// }

// void check_pair_symbols(char *line, char symbol)
// {
    //check for matching parenthases, quotes.

// }

// void check_redirection(char *line)
// {
    //redirection syntax.

// }

void syntax_validation(char *line)
{
    int index;

    //check_commands(line);
    index = 0;

    //if (ft_strchr(line, '|'))
        //check_pipes(line);
    //if (ft_strchr(line, '"'))
        //check_pair_symbols(line, '"');
    //if (ft_strchr(line, '('))
        //check_pair_symbols(line, '(');
}