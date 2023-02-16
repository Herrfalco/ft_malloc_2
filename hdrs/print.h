/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 10:39:16 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/16 11:25:43 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINT_H
#define PRINT_H

#include "../hdrs/incs.h"

#define BUFF_SZ		1024

typedef enum		base_e {
	BIN,
	OCT,
	DEC,
	HEX,
}					base_t;

void			print_str(const char *str);
void			print_ln(void);
void			print_nb_base(uint64_t nb, base_t base);

#endif // PRINT_H
