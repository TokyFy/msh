/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: franaivo <franaivo@student.42antanana      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 13:23:43 by franaivo          #+#    #+#             */
/*   Updated: 2024/02/20 13:23:43 by franaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>

typedef unsigned long	t_size_t;

typedef struct s_list
{
	void				*content;
	struct s_list		*next;
}						t_list;

int						ft_isalpha(int c);
int						ft_isdigit(int c);
int						ft_isalnum(int c);
int						ft_isascii(int c);
int						ft_isprint(int c);
t_size_t				ft_strlen(const char *s);
void					*ft_memset(void *byte, int c, t_size_t n);
void					ft_bzero(void *s, unsigned int n);
void					*ft_memcpy(void *dest, const void *src, t_size_t n);
void					*ft_memmove(void *dest, const void *src, t_size_t n);
unsigned int			ft_strlcat(char *dest, char *src, unsigned int size);
unsigned int			ft_strlcpy(char *dest, char *src, unsigned int size);

int						ft_toupper(int c);
int						ft_tolower(int c);

char					*ft_strchr(const char *s, int c);
char					*ft_strrchr(const char *s, int c);
int						ft_chrc(char *str, char c);
int						ft_strncmp(char *s1, char *s2, unsigned int n);
int						ft_strcmp(char *s1, char *s2);

void					*ft_memchr(const void *s, int c, t_size_t n);
int						ft_memcmp(const void *s1, const void *s2, t_size_t n);
char					*ft_strnstr(const char *str, const char *needle,
							t_size_t len);

int						ft_atoi(const char *str);

void					*ft_calloc(t_size_t nmemb, t_size_t size);
char					*ft_strdup(char *src);

char					*ft_strndup(const char *s, t_size_t n);

char					*ft_substr(char const *s, unsigned int start,
							t_size_t len);
char					*ft_strjoin(char const *s1, char const *s2);
char					*ft_strtrim(char const *s1, char const *set);
char					**ft_split(char const *s, char c);

char					*ft_itoa(int n);

char					*ft_strmapi(char const *s, char (*f)(unsigned int,
								char));
void					ft_striteri(char *s, void (*f)(unsigned int, char *));

void					ft_putchar_fd(char c, int fd);
void					ft_putstr_fd(char *s, int fd);
void					ft_putendl_fd(char *s, int fd);
void					ft_putnbr_fd(int n, int fd);

t_list					*ft_lstnew(void *content);
void					ft_lstadd_front(t_list **lst, t_list *el);
int						ft_lstsize(t_list *lst);
t_list					*ft_lstlast(t_list *lst);
void					ft_lstadd_back(t_list **lst, t_list *el);
void					ft_lstdelone(t_list *lst, void (*del)(void *));
void					ft_lstclear(t_list **lst, void (*del)(void *));
void					ft_lstiter(t_list *lst, void (*f)(void *));
t_list					*ft_lstmap(t_list *lst, void *(*f)(void *),
							void (*del)(void *));

unsigned long			ft_abs(long n);
int						ft_strcmp(char *s1, char *s2);
#endif
