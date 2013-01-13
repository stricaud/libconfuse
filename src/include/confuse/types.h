#ifndef _cfg_types_h_
#define _cfg_types_h_

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Flags. */
#define CFGF_NONE 0
#define CFGF_MULTI 1       /**< option may be specified multiple times (only applies to sections) */
#define CFGF_LIST 2        /**< option is a list */
#define CFGF_NOCASE 4      /**< configuration file is case insensitive */
#define CFGF_TITLE 8       /**< option has a title (only applies to sections) */
#define CFGF_NODEFAULT 16  /**< option has no default value */
#define CFGF_NO_TITLE_DUPES 32  /**< multiple section titles must be unique
                                  (duplicates raises an error, only applies to
                                  sections) */

#define CFGF_RESET 64
#define CFGF_DEFINIT 128

/** Return codes from cfg_parse(). */
#define CFG_SUCCESS 0
#define CFG_FILE_ERROR -1
#define CFG_PARSE_ERROR 1

/** Fundamental option types */
enum cfg_type_t {
    CFGT_NONE,
    CFGT_INT,     /**< integer */
    CFGT_FLOAT,   /**< floating point number */
    CFGT_STR,     /**< string */
    CFGT_BOOL,    /**< boolean value */
    CFGT_SEC,     /**< section */
    CFGT_FUNC,    /**< function */
    CFGT_PTR      /**< pointer to user-defined value */
};
typedef enum cfg_type_t cfg_type_t;

typedef struct cfg_t cfg_t;
typedef struct cfg_searchpath_t cfg_searchpath_t;
typedef int cfg_flag_t;
typedef struct cfg_defvalue_t cfg_defvalue_t;
typedef struct cfg_opt_t cfg_opt_t;
typedef union cfg_value_t cfg_value_t;
typedef union cfg_simple_t cfg_simple_t;

/** Function prototype used by CFGT_FUNC options.
 *
 * This is a callback function, registered with the CFG_FUNC
 * initializer. Each time libConfuse finds a function, the registered
 * callback function is called (parameters are passed as strings, any
 * conversion to other types should be made in the callback
 * function). libConfuse does not support any storage of the data
 * found; these are passed as parameters to the callback, and it's the
 * responsibility of the callback function to do whatever it should do
 * with the data.
 *
 * @param cfg The configuration file context.
 * @param opt The option.
 * @param argc Number of arguments passed. The callback function is
 * responsible for checking that the correct number of arguments are
 * passed.
 * @param argv Arguments as an array of character strings.
 *
 * @return On success, 0 should be returned. All other values
 * indicates an error, and the parsing is aborted. The callback
 * function should notify the error itself, for example by calling
 * cfg_error().
 *
 * @see CFG_FUNC
 */
typedef int (*cfg_func_t)(cfg_t *cfg, cfg_opt_t *opt,
                          int argc, const char **argv);

/** Function prototype used by the cfg_print_ functions.
 *
 * This callback function is used to print option values. For options
 * with a value parsing callback, this is often required, especially
 * if a string is mapped to an integer by the callback. This print
 * callback must then map the integer back to the appropriate string.
 *
 * Except for functions, the print callback function should only print
 * the value of the option, not the name and the equal sign (that is
 * handled by the cfg_opt_print function). For function options
 * however, the name and the parenthesis must be printed by this
 * function. The value to print can be accessed with the cfg_opt_get
 * functions.
 *
 * @param opt The option structure (eg, as returned from cfg_getopt())
 * @param index Index of the value to get. Zero based.
 * @param fp File stream to print to, use stdout to print to the screen.
 *
 * @see cfg_print, cfg_set_print_func
 */
typedef void (*cfg_print_func_t)(cfg_opt_t *opt, unsigned int index, FILE *fp);
    
/** Value parsing callback prototype
 *
 * This is a callback function (different from the one registered with the
 * CFG_FUNC initializer) used to parse a value. This can be used to override
 * the internal parsing of a value.
 *
 * Suppose you want an integer option that only can have certain values, for
 * example 1, 2 and 3, and these should be written in the configuration file as
 * "yes", "no" and "maybe". The callback function would be called with the
 * found value ("yes", "no" or "maybe") as a string, and the result should be
 * stored in the result parameter.
 *
 * @param cfg The configuration file context.
 * @param opt The option.
 * @param value The value found in the configuration file.
 * @param result Pointer to storage for the result, cast to a void pointer.
 *
 * @return On success, 0 should be returned. All other values indicates an
 * error, and the parsing is aborted. The callback function should notify the
 * error itself, for example by calling cfg_error().
 */
typedef int (*cfg_callback_t)(cfg_t *cfg, cfg_opt_t *opt,
                              const char *value, void *result);

/** Validating callback prototype
 *
 * This callback function is called after an option has been parsed and set.
 * The function is called for both fundamental values (strings, integers etc)
 * as well as lists and sections. This can for example be used to validate that
 * all required options in a section has been set to sane values.
 *
 * @return On success, 0 should be returned. All other values indicates an
 * error, and the parsing is aborted. The callback function should notify the
 * error itself, for example by calling cfg_error().
 *
 * @see cfg_set_validate_func
 */
typedef int (*cfg_validate_callback_t)(cfg_t *cfg, cfg_opt_t *opt);

/** User-defined memory release function for CFG_PTR values
 *
 * This callback is used to free memory allocated in a value parsing callback
 * function. Especially useful for CFG_PTR options, since libConfuse will not
 * itself release such values. If the values are simply allocated with a
 * malloc(3), one can use the standard free(3) function here.
 *
 */
typedef void (*cfg_free_func_t)(void *value);

/** Boolean values. */
typedef enum {cfg_false, cfg_true} cfg_bool_t;

/** Error reporting function. */
typedef void (*cfg_errfunc_t)(cfg_t *cfg, const char *fmt, va_list ap);

/** Data structure holding information about a "section". Sections can
 * be nested. A section has a list of options (strings, numbers,
 * booleans or other sections) grouped together.
 */
struct cfg_t {
    cfg_flag_t flags;       /**< Any flags passed to cfg_init() */
    char *name;             /**< The name of this section, the root
                             * section returned from cfg_init() is
                             * always named "root" */
    cfg_opt_t *opts;        /**< Array of options */
    char *title;            /**< Optional title for this section, only
                             * set if CFGF_TITLE flag is set */
    char *filename;         /**< Name of the file being parsed */
    int line;               /**< Line number in the config file */
    cfg_errfunc_t errfunc;  /**< This function (if set with
                             * cfg_set_error_function) is called for
                             * any error message. */
    cfg_searchpath_t *path; /**< Linked list of directories to search */ 
};

/** Data structure holding the value of a fundamental option value.
 */
union cfg_value_t {
    long int number;        /**< integer value */
    double fpnumber;        /**< floating point value */
    cfg_bool_t boolean;     /**< boolean value */
    char *string;           /**< string value */
    cfg_t *section;         /**< section value */
    void *ptr;              /**< user-defined value */
};

/** Data structure holding the pointer to a user provided variable
 *  defined with CFG_SIMPLE_*
 */
union cfg_simple_t {
    long int *number;
    double *fpnumber;
    cfg_bool_t *boolean;
    char **string;
    void **ptr;
};

/** Data structure holding the default value given by the
 *  initialization macros.
 */
struct cfg_defvalue_t {
    long int number;        /**< default integer value */
    double fpnumber;        /**< default floating point value */
    cfg_bool_t boolean;     /**< default boolean value */
    char *string;           /**< default string value */
    char *parsed;           /**< default value that is parsed by
                             * libConfuse, used for lists and
                             * functions */
};

/** Data structure holding information about an option. The value(s)
 * are stored as an array of fundamental values (strings, numbers,
 * etc).
 */
struct cfg_opt_t {
    char *name;             /**< The name of the option */
    cfg_type_t type;        /**< Type of option */
    unsigned int nvalues;   /**< Number of values parsed */
    cfg_value_t **values;   /**< Array of found values */
    cfg_flag_t flags;       /**< Flags */
    cfg_opt_t *subopts;     /**< Suboptions (only applies to sections) */
    cfg_defvalue_t def;     /**< Default value */
    cfg_func_t func;        /**< Function callback for CFGT_FUNC options */
    cfg_simple_t simple_value;     /**< Pointer to user-specified variable to
                             * store simple values (created with the
                             * CFG_SIMPLE_* initializers) */
    cfg_callback_t parsecb; /**< Value parsing callback function */
    cfg_validate_callback_t validcb; /**< Value validating callback function */
    cfg_print_func_t pf;    /**< print callback function */
    cfg_free_func_t freecb; /***< user-defined memory release function */
};


#ifdef __cplusplus
}
#endif

#endif	/* _cfg_types_h_ */
