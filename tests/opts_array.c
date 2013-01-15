#include <confuse/opts_array.h>

int main(void)
{
  cfg_opts_array_t *opts_array;

  opts_array = cfg_opts_array_init();

  cfg_opts_array_append_string(opts_array, "test", NULL, 0);
  cfg_opts_array_append_integer(opts_array, "number", 0, 0);

  cfg_opts_array_print(opts_array);

  cfg_opts_array_free(opts_array);

  return 0;
}
