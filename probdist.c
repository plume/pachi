#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "move.h"
#include "probdist.h"
#include "random.h"

struct probdist *
probdist_init(struct probdist *pd, int bsize2)
{
	if (!pd) pd = malloc(sizeof(*pd));
	pd->bsize2 = bsize2;
	pd->moves = calloc(bsize2, sizeof(pd->moves[0]));
	pd->total = 0;
	return pd;
}

void
probdist_add(struct probdist *pd, coord_t c, float val)
{
	assert(pd);
	assert(c >= 0 && c < pd->bsize2);
	assert(val >= 0);
	pd->moves[c] += val;
	pd->total += val;
}

void
probdist_punch(struct probdist *pd, coord_t c)
{
	assert(pd);
	assert(c >= 0 && c < pd->bsize2);
	pd->total -= pd->moves[c];
	pd->moves[c] = 0;
}

coord_t
probdist_pick(struct probdist *pd)
{
	assert(!(pd->total < 0));
	if (pd->total < __FLT_EPSILON__)
		return pass;
	float stab = (float) fast_random(65536) / 65536 * pd->total;
	float sum = 0;
	for (coord_t c = 0; c < pd->bsize2; c++) {
		sum += pd->moves[c];
		if (stab < sum)
			return c;
	}
	assert(0); /* wtf */
}

void
probdist_done(struct probdist *pd) {
	free(pd->moves);
}
