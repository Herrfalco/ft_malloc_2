/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 17:59:23 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/18 20:52:31 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/core.h"

static void		*malloc_no_lock(uint64_t size) {
	if (!size)
		return (NULL);
	zone_init(&g_zones.tiny, TINY_CELL_SZ);
	zone_init(&g_zones.small, SMALL_CELL_SZ);
	if (size <= TINY_CELL_SZ
			&& zone_inited(&g_zones.tiny)
			&& !zone_full(&g_zones.tiny))
		return (zone_alloc(&g_zones.tiny, size));
	if (size <= SMALL_CELL_SZ
			&& zone_inited(&g_zones.small)
			&& !zone_full(&g_zones.small))
		return (zone_alloc(&g_zones.small, size));
	return (big_zone_alloc(&g_zones.big, size));
}

void	*malloc(uint64_t size) {
	glob_lock(FT_MLC, &size, NULL);
	return (glob_unlock(FT_MLC, malloc_no_lock(size)));
}

static void		free_no_lock(void *ptr) {
	if (!ptr)
		return;
	if (zone_ptr_in(&g_zones.tiny, ptr))
		zone_free(&g_zones.tiny, ptr);
	else if (zone_ptr_in(&g_zones.small, ptr))
		zone_free(&g_zones.small, ptr);
	else if (big_zone_ptr_in(&g_zones.big, ptr))
		big_zone_free(&g_zones.big, ptr);
}

void	free(void *ptr) {
	glob_lock(FT_FRE, ptr, NULL);
	free_no_lock(ptr);
	glob_unlock(FT_FRE, NULL);
}

static void		mem_cpy(void *dst, void *src, uint64_t size) {
	uint64_t		i;

	for (i = 0; i < size; ++i)
		((uint8_t *)dst)[i] = ((uint8_t *)src)[i];
}

void	*realloc(void *ptr, uint64_t size) {
	void		*new = NULL;
	uint64_t	o_sz;

	glob_lock(FT_RLC, ptr, &size);
	if (!size) {
		if (ptr)
			free_no_lock(ptr);
		return (glob_unlock(FT_RLC, NULL));
	} else if (!ptr)
		return (glob_unlock(FT_RLC, malloc_no_lock(size)));
	if (zone_ptr_in(&g_zones.tiny, ptr))
		o_sz = zone_csize(&g_zones.tiny, ptr);
	else if (zone_ptr_in(&g_zones.small, ptr))
		o_sz = zone_csize(&g_zones.small, ptr);
	else if (big_zone_ptr_in(&g_zones.big, ptr))
		o_sz = big_zone_csize(ptr);
	else
		return (glob_unlock(FT_RLC, NULL));
	if (!(new = malloc_no_lock(size)))
		return (glob_unlock(FT_RLC, NULL));
	mem_cpy(new, ptr, o_sz < size ? o_sz : size);
	free_no_lock(ptr);
	return (glob_unlock(FT_RLC, new));
}

void	zones_dtor(void) {
	zone_dest(&g_zones.tiny);
	zone_dest(&g_zones.small);
	if (g_zones.log > 0)
		close(g_zones.log);
	pthread_mutex_destroy(&g_zones.mut);
}
