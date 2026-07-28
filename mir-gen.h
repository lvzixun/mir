/* This file is a part of MIR project.
   Copyright (C) 2018-2024 Vladimir Makarov <vmakarov.gcc@gmail.com>.
*/

#ifndef MIR_GEN_H

#define MIR_GEN_H

#include "mir.h"

#ifndef MIR_NO_GEN_DEBUG
#define MIR_NO_GEN_DEBUG 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MIR_gen_stats {
  /* Explicit spill-slot memory instructions inserted by register allocation. */
  size_t ra_spill_store_count;
  size_t ra_reload_load_count;
} MIR_gen_stats_t;

extern void MIR_gen_init (MIR_context_t ctx);
extern void MIR_gen_set_debug_file (MIR_context_t ctx, FILE *f);
extern void MIR_gen_set_debug_level (MIR_context_t ctx, int debug_level);
extern void MIR_gen_set_optimize_level (MIR_context_t ctx, unsigned int level);
/* Stats accumulate across MIR_gen calls until reset.  Folded memory operands are not counted. */
extern void MIR_gen_reset_stats (MIR_context_t ctx);
extern void MIR_gen_get_stats (MIR_context_t ctx, MIR_gen_stats_t *stats);
extern void *MIR_gen (MIR_context_t ctx, MIR_item_t func_item);
extern size_t MIR_gen_code_size (MIR_item_t func_item);
extern void MIR_set_gen_interface (MIR_context_t ctx, MIR_item_t func_item);
extern void MIR_set_lazy_gen_interface (MIR_context_t ctx, MIR_item_t func_item);
extern void MIR_set_lazy_bb_gen_interface (MIR_context_t ctx, MIR_item_t func_item);
extern void MIR_gen_finish (MIR_context_t ctx);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef MIR_GEN_H */
