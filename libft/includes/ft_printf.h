/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 17:04:26 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/04 14:09:12 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include "libft.h"

typedef struct s_legend
{
	char	specifier;
	int		padding;
	int		zero;
	int		period[2];
	int		hash;
	int		plus;
	int		space;
	int		dash[2];
}			t_legend;

int			ft_printf(const char *input, ...);
int			convert_print(const char *s, unsigned int i, va_list *lst, int **c);
int			fill_legend(const char *str_legend, t_legend **legend);
void		check_ignores(t_legend **legend);
int			handle_per_specifier(t_legend **legend, va_list *list);
int			is_format_specifier(char c);
int			print_char(t_legend **legend, va_list *list);
int			print_percent(t_legend **legend);
int			print_string(t_legend **legend, va_list *list);
int			print_int_dec(t_legend **legend, va_list *list);
void		ft_putnbr_nosign(long int num, int fd);
int			get_num_len_long(long int n);
int			print_unsigned(t_legend **legend, va_list *list);
int			print_flag_char(char c, int repeat);
int			apply_hex_prefix(char s);
int			is_flag(char c);
int			print_hex(t_legend **legend, va_list *list);
char		*ft_itoa_ubase(unsigned long num, int base);
int			get_num_len_u(unsigned long n, int base);
int			get_zpad(t_legend **legend, int len);

#endif
