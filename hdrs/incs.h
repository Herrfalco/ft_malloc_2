/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   incs.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 12:28:15 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/15 22:52:33 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCS_H
#define INCS_H

#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>

//A supprimer
#include <stdio.h>

#define CELL_NB			128
#define SMALL_CELL_SZ	(uint64_t)getpagesize()
#define TINY_CELL_SZ	(SMALL_CELL_SZ / 8)

#endif // INCS_H
