/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:14:30 by sranaivo          #+#    #+#             */
/*   Updated: 2024/10/30 17:24:37 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

// char *handle_quotes(char *input) {
//     if (input[0] == '\'' && input[ft_strlen(input) - 1] == '\'') {
//         return ft_strndup(input + 1, ft_strlen(input) - 2);
//     } else if (input[0] == '"' && input[ft_strlen(input) - 1] == '"') {
//         return expand_variables_in_string(ft_strndup(input + 1, ft_strlen(input) - 2));
//     } else {
//         return expand_variables_in_string(input);
//     }
// }

char    *get_env_name_in_string(char *str)
{
    char    *name;
    char    *start;
    char    *string;
    int     len;

    string = str;
    len = 0;
    while (*string)
    {
        if (*string == '$')
        {
            string++;
            if (ft_isalpha(*string) || *string == '_')
            {
                start = string;
                while (ft_isalnum(*string) || *string == '_')
                {
                    len++;
                    string++;
                }
                name = ft_strndup(start, len);
                return (name);
            }
        }
        string++;
    }
    return (NULL);
}

char    *get_element_value(t_list *element)
{
    char    *value;

    value = ((t_env *)element->content)->value;
    return (value);
}

char *handle_quote(char *input) {
    int len = ft_strlen(input);
    char *result = malloc(len + 1);  // Allouer de la mémoire pour la chaîne résultante
    if (!result) return NULL;

    int i = 0, j = 0;
    char current_quote = '\0';  // Variable pour suivre le type de quote en cours (' ou ")

    while (input[i]) {
        if (input[i] == '\'' || input[i] == '"') {
            if (current_quote == '\0') {
                // Début d'une quote si aucune quote n'est encore ouverte
                current_quote = input[i];
            } else if (input[i] == current_quote) {
                // Fin de la quote courante
                current_quote = '\0';
            } else {
                // Quote différente imbriquée, elle est ajoutée à la chaîne résultante
                result[j++] = input[i];
            }
        } else {
            // Caractère normal, ajouté à la chaîne résultante
            result[j++] = input[i];
        }
        i++;
    }

    // Si une quote est toujours ouverte, elle n'a pas été fermée correctement
    if (current_quote != '\0') {
        free(result);
        return NULL;
    }

    result[j] = '\0';  // Terminer la chaîne résultante

    // Réallouer pour ajuster la mémoire si nécessaire
    char *final_result = strndup(result, j);
    free(result);

    return final_result;
}

char *expand_variables_in_string( t_list *env, char *input) {
    char *result = ft_strdup("");  // Initialiser avec une chaîne vide pour stocker le résultat final
    int i = 0, start = 0;       // `i` parcourt `input`, `start` marque le début d'une portion sans variable
    int in_double_quotes = 0;   // Indicateur pour suivre si nous sommes dans des doubles quotes
    int in_single_quotes = 0;   // Indicateur pour suivre si nous sommes dans des simples quotes

    while (input[i]) {
        if (input[i] == '\'' && !in_double_quotes) {  // Basculer l'état de `in_single_quotes` uniquement hors des doubles quotes
            in_single_quotes = !in_single_quotes;
            i++;
            continue;
        } else if (input[i] == '"' && !in_single_quotes) {  // Basculer `in_double_quotes` uniquement si on n'est pas dans des simples quotes
            in_double_quotes = !in_double_quotes;
            i++;
            continue;
        }

        // Expander uniquement si `$` est dans des doubles quotes et hors des simples quotes
        if (input[i] == '$' && in_double_quotes && !in_single_quotes) {
            // Ajouter la portion de chaîne avant la variable trouvée
            if (i > start) {
                char *before_var = ft_substr(input, start, i - start);
                char *temp = ft_strjoin(result, before_var);
                free(result);
                free(before_var);
                result = temp;
            }

            // Extraire le nom de la variable d'environnement
            char *env_name = get_env_name_in_string(&input[i]);
            if (env_name) {
                char *env_value = get_env( env, env_name);  // Récupérer la valeur de la variable d'environnement
                free(env_name);  // Libérer la mémoire allouée pour le nom de la variable

                // Si la variable existe, l'ajouter dans la chaîne résultante
                if (env_value) {
                    char *temp = ft_strjoin(result, env_value);
                    free(result);
                    result = temp;
                }

                // Avancer l'indice `i` après le nom de la variable
                while (isalnum(input[i + 1]) || input[i + 1] == '_') i++;
            }
            start = i + 1;  // Mettre à jour `start` après la variable expandée
        }
        i++;
    }

    // Ajouter la portion restante après la dernière variable, s'il y en a
    if (i > start) {
        char *remaining = ft_substr(input, start, i - start);
        char *temp = ft_strjoin(result, remaining);
        free(result);
        free(remaining);
        result = temp;
    }

    return result;
}

void    expand(t_list *env, void *tree)
{
    t_node  *ast;
    (void) env;
    
    ast = tree;
    if (ast->type == CMD)
    {
        t_cmd   *cmd = tree;
        char    **temp;
        int i = -1;
        temp = cmd->argv;
        while(temp[++i])
        {
            char *input = ft_strdup(temp[i]);
            free(temp[i]);
            char *tmp = expand_variables_in_string(env, input);
            temp[i] = handle_quote(tmp);
            free(tmp);
            free(input);
        }
    }
}
