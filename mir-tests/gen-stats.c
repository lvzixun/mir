#include "../mir-gen.h"

#include <stdio.h>

#define PRESSURE_ARG_COUNT 24

static void barrier (void) {}

int main (void) {
  MIR_context_t ctx = MIR_init ();
  MIR_module_t module = MIR_new_module (ctx, "stats");
  MIR_item_t barrier_proto = MIR_new_proto_arr (ctx, "barrier_proto", 0, NULL, 0, NULL);
  MIR_item_t barrier_import = MIR_new_import (ctx, "barrier");
  MIR_var_t args[PRESSURE_ARG_COUNT];
  char arg_names[PRESSURE_ARG_COUNT][16];
  MIR_type_t result_type = MIR_T_I64;
  MIR_item_t func;
  MIR_reg_t arg_regs[PRESSURE_ARG_COUNT], sum;
  MIR_gen_stats_t stats;

  for (size_t i = 0; i < PRESSURE_ARG_COUNT; i++) {
    snprintf (arg_names[i], sizeof (arg_names[i]), "arg%lu", (unsigned long) i);
    args[i].type = MIR_T_I64;
    args[i].name = arg_names[i];
    args[i].size = 0;
  }
  func = MIR_new_func_arr (ctx, "pressure", 1, &result_type, PRESSURE_ARG_COUNT, args);
  for (size_t i = 0; i < PRESSURE_ARG_COUNT; i++)
    arg_regs[i] = MIR_reg (ctx, arg_names[i], func->u.func);
  sum = MIR_new_func_reg (ctx, func->u.func, MIR_T_I64, "sum");
  MIR_append_insn (ctx, func,
                   MIR_new_call_insn (ctx, 2, MIR_new_ref_op (ctx, barrier_proto),
                                      MIR_new_ref_op (ctx, barrier_import)));
  MIR_append_insn (ctx, func,
                   MIR_new_insn (ctx, MIR_MOV, MIR_new_reg_op (ctx, sum),
                                 MIR_new_reg_op (ctx, arg_regs[0])));
  for (size_t i = 1; i < PRESSURE_ARG_COUNT; i++)
    MIR_append_insn (ctx, func,
                     MIR_new_insn (ctx, MIR_ADD, MIR_new_reg_op (ctx, sum),
                                   MIR_new_reg_op (ctx, sum), MIR_new_reg_op (ctx, arg_regs[i])));
  MIR_append_insn (ctx, func, MIR_new_ret_insn (ctx, 1, MIR_new_reg_op (ctx, sum)));
  MIR_finish_func (ctx);
  MIR_finish_module (ctx);

  MIR_load_module (ctx, module);
  MIR_load_external (ctx, "barrier", (void *) barrier);
  MIR_gen_init (ctx);
  MIR_gen_get_stats (ctx, &stats);
  if (stats.ra_spill_store_count != 0 || stats.ra_reload_load_count != 0) return 1;
  MIR_link (ctx, MIR_set_gen_interface, NULL);
  MIR_gen_get_stats (ctx, &stats);
  if (stats.ra_spill_store_count == 0 || stats.ra_reload_load_count == 0) return 1;
  MIR_gen_reset_stats (ctx);
  MIR_gen_get_stats (ctx, &stats);
  if (stats.ra_spill_store_count != 0 || stats.ra_reload_load_count != 0) return 1;

  MIR_gen_finish (ctx);
  MIR_finish (ctx);
  return 0;
}
