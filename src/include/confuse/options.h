#ifndef _cfg_options_h_
#define _cfg_options_h_

#include <confuse/confuse.h>
#include <confuse/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void cfg_init_defaults(cfg_t *cfg);
cfg_value_t *_cfg_addval(cfg_opt_t *opt);
cfg_opt_t *cfg_dupopt_array(cfg_opt_t *opts);
int _call_function(cfg_t *cfg, cfg_opt_t *opt, cfg_opt_t *funcopt);
void cfg_free_opt_array(cfg_opt_t *opts);
cfg_value_t *cfg_opt_getval(cfg_opt_t *opt, unsigned int index);
void _cfg_addlist_internal(cfg_opt_t *opt, unsigned int nvalues, va_list ap);
cfg_opt_t *cfg_getopt_array(cfg_opt_t *rootopts, int cfg_flags, const char *name);

#ifdef __cplusplus
}
#endif

#endif	/* _cfg_options_h_ */
