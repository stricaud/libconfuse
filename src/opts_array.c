#include <confuse/general.h>
#include <confuse/utarray.h>
#include <confuse/options.h>

#include <confuse/opts_array.h>

cfg_opts_array_t *cfg_opts_array_init(void)
{
  UT_array *opts_array;
  UT_icd opts_icd = {sizeof(cfg_opt_t), NULL, NULL, NULL};

  utarray_new(opts_array, &opts_icd);

  return (cfg_opts_array_t *)opts_array;
}

void cfg_opts_array_free(cfg_opts_array_t *opts_array)
{
  /* utarray_free((UT_array *)opts_array); */
}

/* CFG_STR("host", 0, CFGF_NODEFAULT), */
/* CFG_INT("port", 21, CFGF_NONE), */

void cfg_opts_array_append_string(cfg_opts_array_t *opts_array, const char *key, const char *default_value, int flags)
{
  /* cfg_opt_t cfg_opt[] = {CFG_STR(key, default_value, flags), CFG_END()}; */
    cfg_opt_t opts[] = {
        CFG_SIMPLE_BOOL("verbose", &verbose),
        CFG_SIMPLE_STR("server", &server),
        CFG_SIMPLE_STR("user", &username),
        CFG_SIMPLE_INT("debug", &debug),
        CFG_SIMPLE_FLOAT("delay", &delay),
        CFG_END()
    };

  cfg_opt_print(opts, stdout);

  /* utarray_push_back(opts_array, &cfg_opt); */
}

void cfg_opts_array_append_integer(cfg_opts_array_t *opts_array, const char *key, const int default_value, int flags)
{
  /* cfg_opt_t cfg_opt[] = { CFG_INT(key, default_value, flags) }; */
  /* utarray_push_back(opts_array, &cfg_opt); */
}


void cfg_opts_array_print(cfg_opts_array_t *opts_array)
{
  /* cfg_opt_t *cfg_opt_p = NULL; */

  /* while (cfg_opt_p=(cfg_opt_t *)utarray_next(opts_array, cfg_opt_p)) { */
  /*   /\* printf("name:%s\n", cfg_opt_p->name); *\/ */

  /*   printf("Pointer data:%X\n", *cfg_opt_p); */
  /*   /\* printf("OPT TYPE:%d\n", cfg_opt_p->type); *\/ */

  /*   /\* switch(*cfg_opt_p.type) { *\/ */
  /*   /\* case CFGT_INT: *\/ */
  /*   /\*   printf("INT\n"); *\/ */
  /*   /\*   break; *\/ */
  /*   /\* case CFGT_STR: *\/ */
  /*   /\*   printf("STR\n"); *\/ */
  /*   /\*   break; *\/ */
  /*   /\* } *\/ */
  /*   /\* printf("We have something\n"); *\/ */
  /* } */

}
