#include <confuse/general.h>
#include <confuse/utarray.h>
#include <confuse/options.h>

typedef UT_array cfg_opts_array_t;

cfg_opts_array_t *cfg_opts_array_init(void);
void cfg_opts_array_free(cfg_opts_array_t *opts_array);
void cfg_opts_array_append_string(cfg_opts_array_t *opts_array, const char *key, const char *default_value, int flags);
void cfg_opts_array_append_integer(cfg_opts_array_t *opts_array, const char *key, const int default_value, int flags);
void cfg_opts_array_print(cfg_opts_array_t *opts_array);
