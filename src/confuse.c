/*
 * Copyright (c) 2002,2003,2007 Martin Hedenfalk <martin@bzero.se>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/* #ifdef HAVE_CONFIG_H */
#include <confuse/config.h>
/* #endif */

#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <strings.h>

#ifndef _WIN32
# include <pwd.h>
#endif
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
/* #include <ctype.h> */

#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif

#include <confuse/confuse.h>
#include <confuse/options.h>

extern FILE *cfg_yyin;
extern int cfg_yylex(cfg_t *cfg);
extern int cfg_lexer_include(cfg_t *cfg, const char *fname);
extern void cfg_scan_string_begin(const char *buf);
extern void cfg_scan_string_end(void);
extern void cfg_scan_fp_begin(FILE *fp);
extern void cfg_scan_fp_end(void);
extern char *cfg_qstring;

char *cfg_yylval = 0;

const char confuse_version[] = PACKAGE_VERSION;
const char confuse_copyright[] = PACKAGE_STRING" by Martin Hedenfalk <martin@bzero.se>";
const char confuse_author[] = "Martin Hedenfalk <martin@bzero.se>";

DLLIMPORT cfg_opt_t *cfg_getopt(cfg_t *cfg, const char *name)
{
    unsigned int i;
    cfg_t *sec = cfg;

    assert(cfg && cfg->name && name);

    while(name && *name)
    {
        char *secname;
        size_t len = strcspn(name, "|");
        if(name[len] == 0 /*len == strlen(name)*/)
            /* no more subsections */
            break;
        if(len)
        {
            secname = strndup(name, len);
            sec = cfg_getsec(sec, secname);
            if(sec == 0)
                cfg_error(cfg, _("no such option '%s'"), secname);
            free(secname);
            if(sec == 0)
                return 0;
        }
        name += len;
        name += strspn(name, "|");
    }

    for(i = 0; sec->opts[i].name; i++)
    {
        if(is_set(CFGF_NOCASE, sec->flags))
        {
            if(strcasecmp(sec->opts[i].name, name) == 0)
                return &sec->opts[i];
        }
        else
        {
            if(strcmp(sec->opts[i].name, name) == 0)
                return &sec->opts[i];
        }
    }
    cfg_error(cfg, _("no such option '%s'"), name);
    return 0;
}

DLLIMPORT const char *cfg_title(cfg_t *cfg)
{
    if(cfg)
        return cfg->title;
    return 0;
}

DLLIMPORT const char *cfg_name(cfg_t *cfg)
{
    if(cfg)
        return cfg->name;
    return 0;
}

DLLIMPORT const char *cfg_opt_name(cfg_opt_t *opt)
{
    if(opt)
        return opt->name;
    return 0;
}

DLLIMPORT unsigned int cfg_opt_size(cfg_opt_t *opt)
{
    if(opt)
        return opt->nvalues;
    return 0;
}

DLLIMPORT unsigned int cfg_size(cfg_t *cfg, const char *name)
{
    return cfg_opt_size(cfg_getopt(cfg, name));
}

DLLIMPORT signed long cfg_opt_getnint(cfg_opt_t *opt, unsigned int index)
{
    assert(opt && opt->type == CFGT_INT);
    if(opt->values && index < opt->nvalues)
        return opt->values[index]->number;
    else if(opt->simple_value.number)
        return *opt->simple_value.number;
    else
        return 0;
}

DLLIMPORT signed long cfg_getnint(cfg_t *cfg, const char *name,
                                  unsigned int index)
{
    return cfg_opt_getnint(cfg_getopt(cfg, name), index);
}

DLLIMPORT signed long cfg_getint(cfg_t *cfg, const char *name)
{
    return cfg_getnint(cfg, name, 0);
}

DLLIMPORT double cfg_opt_getnfloat(cfg_opt_t *opt, unsigned int index)
{
    assert(opt && opt->type == CFGT_FLOAT);
    if(opt->values && index < opt->nvalues)
        return opt->values[index]->fpnumber;
    else if(opt->simple_value.fpnumber)
        return *opt->simple_value.fpnumber;
    else
        return 0;
}

DLLIMPORT double cfg_getnfloat(cfg_t *cfg, const char *name,
                               unsigned int index)
{
    return cfg_opt_getnfloat(cfg_getopt(cfg, name), index);
}

DLLIMPORT double cfg_getfloat(cfg_t *cfg, const char *name)
{
    return cfg_getnfloat(cfg, name, 0);
}

DLLIMPORT cfg_bool_t cfg_opt_getnbool(cfg_opt_t *opt, unsigned int index)
{
    assert(opt && opt->type == CFGT_BOOL);
    if(opt->values && index < opt->nvalues)
        return opt->values[index]->boolean;
    else if(opt->simple_value.boolean)
        return *opt->simple_value.boolean;
    else
        return cfg_false;
}

DLLIMPORT cfg_bool_t cfg_getnbool(cfg_t *cfg, const char *name,
                                  unsigned int index)
{
    return cfg_opt_getnbool(cfg_getopt(cfg, name), index);
}

DLLIMPORT cfg_bool_t cfg_getbool(cfg_t *cfg, const char *name)
{
    return cfg_getnbool(cfg, name, 0);
}

DLLIMPORT char *cfg_opt_getnstr(cfg_opt_t *opt, unsigned int index)
{
    assert(opt && opt->type == CFGT_STR);
    if(opt->values && index < opt->nvalues)
        return opt->values[index]->string;
    else if(opt->simple_value.string)
        return *opt->simple_value.string;
    else
        return 0;
}

DLLIMPORT char *cfg_getnstr(cfg_t *cfg, const char *name, unsigned int index)
{
    return cfg_opt_getnstr(cfg_getopt(cfg, name), index);
}

DLLIMPORT char *cfg_getstr(cfg_t *cfg, const char *name)
{
    return cfg_getnstr(cfg, name, 0);
}

DLLIMPORT void *cfg_opt_getnptr(cfg_opt_t *opt, unsigned int index)
{
    assert(opt && opt->type == CFGT_PTR);
    if(opt->values && index < opt->nvalues)
        return opt->values[index]->ptr;
    else if(opt->simple_value.ptr)
        return *opt->simple_value.ptr;
    else
        return 0;
}

DLLIMPORT void *cfg_getnptr(cfg_t *cfg, const char *name, unsigned int index)
{
    return cfg_opt_getnptr(cfg_getopt(cfg, name), index);
}

DLLIMPORT void *cfg_getptr(cfg_t *cfg, const char *name)
{
    return cfg_getnptr(cfg, name, 0);
}

DLLIMPORT cfg_t *cfg_opt_getnsec(cfg_opt_t *opt, unsigned int index)
{
    assert(opt && opt->type == CFGT_SEC);
    if(opt->values && index < opt->nvalues)
        return opt->values[index]->section;
    return 0;
}

DLLIMPORT cfg_t *cfg_getnsec(cfg_t *cfg, const char *name, unsigned int index)
{
    return cfg_opt_getnsec(cfg_getopt(cfg, name), index);
}

DLLIMPORT cfg_t *cfg_opt_gettsec(cfg_opt_t *opt, const char *title)
{
    unsigned int i, n;

    assert(opt && title);
    if(!is_set(CFGF_TITLE, opt->flags))
        return 0;
    n = cfg_opt_size(opt);
    for(i = 0; i < n; i++)
    {
        cfg_t *sec = cfg_opt_getnsec(opt, i);
        assert(sec && sec->title);
        if(is_set(CFGF_NOCASE, opt->flags))
        {
            if(strcasecmp(title, sec->title) == 0)
                return sec;
        }
        else
        {
            if(strcmp(title, sec->title) == 0)
                return sec;
        }
    }
    return 0;
}

DLLIMPORT cfg_t *cfg_gettsec(cfg_t *cfg, const char *name, const char *title)
{
    return cfg_opt_gettsec(cfg_getopt(cfg, name), title);
}

DLLIMPORT cfg_t *cfg_getsec(cfg_t *cfg, const char *name)
{
    return cfg_getnsec(cfg, name, 0);
}

DLLIMPORT int cfg_parse_boolean(const char *s)
{
    if(strcasecmp(s, "true") == 0
       || strcasecmp(s, "on") == 0
       || strcasecmp(s, "yes") == 0)
        return 1;
    else if(strcasecmp(s, "false") == 0
            || strcasecmp(s, "off") == 0
            || strcasecmp(s, "no") == 0)
        return 0;
    return -1;
}

/* searchpath */

struct cfg_searchpath_t {
    char *dir;               /**< directory to search */
    cfg_searchpath_t *next;  /**< next in list */
};

/* prepend a new cfg_searchpath_t to the linked list */

DLLIMPORT int cfg_add_searchpath(cfg_t *cfg, const char *dir)
{
    cfg_searchpath_t *p;
    char *d;

    assert(cfg && dir);

    if ((d = cfg_tilde_expand(dir)) == NULL) return CFG_PARSE_ERROR;

    if ((p = malloc(sizeof(cfg_searchpath_t))) == NULL)
    {
	free(d);
	return CFG_PARSE_ERROR;
    }

    p->next = cfg->path;
    p->dir  = d;
    
    cfg->path = p;

    return CFG_SUCCESS;
}

DLLIMPORT cfg_errfunc_t cfg_set_error_function(cfg_t *cfg,
                                               cfg_errfunc_t errfunc)
{
    cfg_errfunc_t old;

    assert(cfg);
    old = cfg->errfunc;
    cfg->errfunc = errfunc;
    return old;
}

DLLIMPORT void cfg_error(cfg_t *cfg, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);

    if(cfg && cfg->errfunc)
        (*cfg->errfunc)(cfg, fmt, ap);
    else
    {
        if(cfg && cfg->filename && cfg->line)
            fprintf(stderr, "%s:%d: ", cfg->filename, cfg->line);
        else if(cfg && cfg->filename)
            fprintf(stderr, "%s: ", cfg->filename);
        vfprintf(stderr, fmt, ap);
        fprintf(stderr, "\n");
    }

    va_end(ap);
}

int cfg_parse_internal(cfg_t *cfg, int level,
		       int force_state, cfg_opt_t *force_opt)
{
    int state = 0;
    char *opttitle = 0;
    cfg_opt_t *opt = 0;
    cfg_value_t *val = 0;
    cfg_opt_t funcopt = CFG_STR(0, 0, 0);
    int num_values = 0; /* number of values found for a list option */
    int rc;

    if(force_state != -1)
        state = force_state;
    if(force_opt)
        opt = force_opt;

    while(1)
    {
        int tok = cfg_yylex(cfg);

        if(tok == 0)
        {
            /* lexer.l should have called cfg_error */
            return STATE_ERROR;
        }

        if(tok == EOF)
        {
            if(state != 0)
            {
                cfg_error(cfg, _("premature end of file"));
                return STATE_ERROR;
            }
            return STATE_EOF;
        }

        switch(state)
        {
            case 0: /* expecting an option name */
                if(tok == '}')
                {
                    if(level == 0)
                    {
                        cfg_error(cfg, _("unexpected closing brace"));
                        return STATE_ERROR;
                    }
                    return STATE_EOF;
                }
                if(tok != CFGT_STR)
                {
                    cfg_error(cfg, _("unexpected token '%s'"), cfg_yylval);
                    return STATE_ERROR;
                }
                opt = cfg_getopt(cfg, cfg_yylval);
                if(opt == 0)
                    return STATE_ERROR;
                if(opt->type == CFGT_SEC)
                {
                    if(is_set(CFGF_TITLE, opt->flags))
                        state = 6;
                    else
                        state = 5;
                }
                else if(opt->type == CFGT_FUNC)
                {
                    state = 7;
                }
                else
                    state = 1;
                break;

            case 1: /* expecting an equal sign or plus-equal sign */
                if(tok == '+')
                {
                    if(!is_set(CFGF_LIST, opt->flags))
                    {
                        cfg_error(cfg,
                                  _("attempt to append to non-list option '%s'"),
                                  opt->name);
                        return STATE_ERROR;
                    }
                    /* Even if the reset flag was set by
                     * cfg_init_defaults, appending to the defaults
                     * should be ok.
                     */
                    opt->flags &= ~CFGF_RESET;
                }
                else if(tok == '=')
                {
                    /* set the (temporary) reset flag to clear the old
                     * values, since we obviously didn't want to append */
                    opt->flags |= CFGF_RESET;
                }
                else
                {
                    cfg_error(cfg, _("missing equal sign after option '%s'"),
                              opt->name);
                    return STATE_ERROR;
                }
                if(is_set(CFGF_LIST, opt->flags))
                {
                    state = 3;
                    num_values = 0;
                } else
                    state = 2;
                break;

            case 2: /* expecting an option value */
                if(tok == '}' && is_set(CFGF_LIST, opt->flags))
                {
                    state = 0;
                    if(num_values == 0 && is_set(CFGF_RESET, opt->flags))
                        /* Reset flags was set, and the empty list was
                         * specified. Free all old values. */
                        cfg_free_value(opt);
                    break;
                }

                if(tok != CFGT_STR)
                {
                    cfg_error(cfg, _("unexpected token '%s'"), cfg_yylval);
                    return STATE_ERROR;
                }

                if(cfg_setopt(cfg, opt, cfg_yylval) == 0)
                    return STATE_ERROR;
                if(opt->validcb && (*opt->validcb)(cfg, opt) != 0)
                    return STATE_ERROR;
                if(is_set(CFGF_LIST, opt->flags))
                {
                    ++num_values;
                    state = 4;
                }
                else
                    state = 0;
                break;

            case 3: /* expecting an opening brace for a list option */
                if(tok != '{')
                {
                    if(tok != CFGT_STR)
                    {
                        cfg_error(cfg, _("unexpected token '%s'"), cfg_yylval);
                        return STATE_ERROR;
                    }

                    if(cfg_setopt(cfg, opt, cfg_yylval) == 0)
                        return STATE_ERROR;
                    if(opt->validcb && (*opt->validcb)(cfg, opt) != 0)
                        return STATE_ERROR;
                    ++num_values;
                    state = 0;
                }
                else
                    state = 2;
                break;

            case 4: /* expecting a separator for a list option, or
                     * closing (list) brace */
                if(tok == ',')
                    state = 2;
                else if(tok == '}')
                {
                    state = 0;
                    if(opt->validcb && (*opt->validcb)(cfg, opt) != 0)
                        return STATE_ERROR;
                }
                else
                {
                    cfg_error(cfg, _("unexpected token '%s'"), cfg_yylval);
                    return STATE_ERROR;
                }
                break;

            case 5: /* expecting an opening brace for a section */
                if(tok != '{')
                {
                    cfg_error(cfg, _("missing opening brace for section '%s'"),
                              opt->name);
                    return STATE_ERROR;
                }

                val = cfg_setopt(cfg, opt, opttitle);
                opttitle = 0;
                if(!val)
                    return STATE_ERROR;

		val->section->path = cfg->path;
                val->section->line = cfg->line;
		val->section->errfunc = cfg->errfunc;
                rc = cfg_parse_internal(val->section, level+1,-1,0);
                cfg->line = val->section->line;
                if(rc != STATE_EOF)
                    return STATE_ERROR;
                if(opt->validcb && (*opt->validcb)(cfg, opt) != 0)
                    return STATE_ERROR;
                state = 0;
                break;

            case 6: /* expecting a title for a section */
                if(tok != CFGT_STR)
                {
                    cfg_error(cfg, _("missing title for section '%s'"),
                              opt->name);
                    return STATE_ERROR;
                }
                else
                    opttitle = strdup(cfg_yylval);
                state = 5;
                break;

            case 7: /* expecting an opening parenthesis for a function */
                if(tok != '(')
                {
                    cfg_error(cfg, _("missing parenthesis for function '%s'"),
                              opt->name);
                    return STATE_ERROR;
                }
                state = 8;
                break;

            case 8: /* expecting a function parameter or a closing paren */
                if(tok == ')')
                {
                    int ret = _call_function(cfg, opt, &funcopt);
                    if(ret != 0)
                        return STATE_ERROR;
                    state = 0;
                }
                else if(tok == CFGT_STR)
                {
                    val = _cfg_addval(&funcopt);
                    val->string = strdup(cfg_yylval);
                    state = 9;
                }
                else
                {
                    cfg_error(cfg, _("syntax error in call of function '%s'"),
                              opt->name);
                    return STATE_ERROR;
                }
                break;

            case 9: /* expecting a comma in a function or a closing paren */
                if(tok == ')')
                {
                    int ret = _call_function(cfg, opt, &funcopt);
                    if(ret != 0)
                        return STATE_ERROR;
                    state = 0;
                }
                else if(tok == ',')
                    state = 8;
                else
                {
                    cfg_error(cfg, _("syntax error in call of function '%s'"),
                              opt->name);
                    return STATE_ERROR;
                }
                break;

            default:
                /* missing state, internal error, abort */
                assert(0);
        }
    }

    return STATE_EOF;
}

DLLIMPORT int cfg_parse_fp(cfg_t *cfg, FILE *fp)
{
    int ret;
    assert(cfg && fp);

    if(cfg->filename == 0)
        cfg->filename = strdup("FILE");
    cfg->line = 1;

    cfg_yyin = fp;
    cfg_scan_fp_begin(cfg_yyin);
    ret = cfg_parse_internal(cfg, 0, -1, 0);
    cfg_scan_fp_end();
    if(ret == STATE_ERROR)
        return CFG_PARSE_ERROR;
    return CFG_SUCCESS;
}

static char *cfg_make_fullpath(const char *dir, const char *file)
{
    assert(dir && file);

    ssize_t n = strlen(dir) + strlen(file) + 2;
    char *path = malloc(n);

    assert(path);

    int np = snprintf(path, n, "%s/%s", dir, file);

    /* 
       np is the number of characters that would have
       been printed if there was enough room in path.
       if np >= n then the snprintf() was truncated
       (which must be a bug).
    */

    assert(np < n);

    return path;
}

DLLIMPORT char *cfg_searchpath(cfg_searchpath_t *p, const char *file)
{
    char *fullpath;
    struct stat st;
    int err;

    if (!p) return NULL;

    if ((fullpath = cfg_searchpath(p->next, file)) != NULL)
        return fullpath;

    if ((fullpath = cfg_make_fullpath(p->dir, file)) == NULL)
        return NULL;

#ifdef HAVE_SYS_STAT_H

    err = stat((const char*)fullpath, &st);
    if ((!err) && S_ISREG(st.st_mode)) return fullpath;

#else

    /* needs an alternative check here for win32 */

#endif

    free(fullpath);
    return NULL;
}

DLLIMPORT int cfg_parse(cfg_t *cfg, const char *filename)
{
    int ret;
    FILE *fp;

    assert(cfg && filename);

    if (cfg->path)
    {
	char *f;

	if ((f = cfg_searchpath(cfg->path, filename)) == NULL)
	    return CFG_FILE_ERROR;

	free(cfg->filename);
	cfg->filename = f;

    }
    else
    {
	free(cfg->filename);
	cfg->filename = cfg_tilde_expand(filename);
    }

    if ((fp = fopen(cfg->filename,"r")) == 0)
        return CFG_FILE_ERROR;

    ret = cfg_parse_fp(cfg, fp);
    fclose(fp);

    return ret;
}

DLLIMPORT int cfg_parse_buf(cfg_t *cfg, const char *buf)
{
    int ret;

    assert(cfg);
    if(buf == 0)
        return CFG_SUCCESS;

    free(cfg->filename);
    cfg->filename = strdup("[buf]");
    cfg->line = 1;

    cfg_scan_string_begin(buf);
    ret = cfg_parse_internal(cfg, 0, -1, 0);
    cfg_scan_string_end();
    if(ret == STATE_ERROR)
        return CFG_PARSE_ERROR;
    return CFG_SUCCESS;
}

DLLIMPORT cfg_t *cfg_init(cfg_opt_t *opts, cfg_flag_t flags)
{
    cfg_t *cfg;

    cfg = calloc(1, sizeof(cfg_t));
    assert(cfg);

    cfg->name = strdup("root");
    cfg->opts = cfg_dupopt_array(opts);
    cfg->flags = flags;
    cfg->filename = 0;
    cfg->line = 0;
    cfg->errfunc = 0;

    cfg_init_defaults(cfg);

#if defined(ENABLE_NLS) && defined(HAVE_GETTEXT)
    setlocale(LC_MESSAGES, "");
    setlocale(LC_CTYPE, "");
    bindtextdomain(PACKAGE, LOCALEDIR);
#endif

    return cfg;
}

DLLIMPORT char *cfg_tilde_expand(const char *filename)
{
    char *expanded = 0;

#ifndef _WIN32
    /* do tilde expansion
     */
    if(filename[0] == '~')
    {
        struct passwd *passwd = 0;
        const char *file = 0;

        if(filename[1] == '/' || filename[1] == 0)
        {
            /* ~ or ~/path */
            passwd = getpwuid(geteuid());
            file = filename + 1;
        }
        else
        {
            /* ~user or ~user/path */
            char *user;

            file = strchr(filename, '/');
            if(file == 0)
                file = filename + strlen(filename);
            user = malloc(file - filename);
            strncpy(user, filename + 1, file - filename - 1);
            passwd = getpwnam(user);
            free(user);
        }

        if(passwd)
        {
            expanded = malloc(strlen(passwd->pw_dir) + strlen(file) + 1);
            strcpy(expanded, passwd->pw_dir);
            strcat(expanded, file);
        }
    }
#endif
    if(!expanded)
        expanded = strdup(filename);
    return expanded;
}


static void cfg_free_searchpath(cfg_searchpath_t* p)
{
  if (p)
    {
      cfg_free_searchpath(p->next);
      free(p);
    }
}

DLLIMPORT void cfg_free(cfg_t *cfg)
{
    int i;

    if(cfg == 0)
        return;

    for(i = 0; cfg->opts[i].name; ++i)
        cfg_free_value(&cfg->opts[i]);

    cfg_free_opt_array(cfg->opts);
    cfg_free_searchpath(cfg->path);

    free(cfg->name);
    free(cfg->title);
    free(cfg->filename);

    free(cfg);
}

DLLIMPORT void cfg_setnint(cfg_t *cfg, const char *name,
                     long int value, unsigned int index)
{
    cfg_opt_setnint(cfg_getopt(cfg, name), value, index);
}

DLLIMPORT void cfg_setint(cfg_t *cfg, const char *name, long int value)
{
    cfg_setnint(cfg, name, value, 0);
}

DLLIMPORT void cfg_setnfloat(cfg_t *cfg, const char *name,
                   double value, unsigned int index)
{
    cfg_opt_setnfloat(cfg_getopt(cfg, name), value, index);
}

DLLIMPORT void cfg_setfloat(cfg_t *cfg, const char *name, double value)
{
    cfg_setnfloat(cfg, name, value, 0);
}

DLLIMPORT void cfg_setnbool(cfg_t *cfg, const char *name,
                            cfg_bool_t value, unsigned int index)
{
    cfg_opt_setnbool(cfg_getopt(cfg, name), value, index);
}

DLLIMPORT void cfg_setbool(cfg_t *cfg, const char *name, cfg_bool_t value)
{
    cfg_setnbool(cfg, name, value, 0);
}

DLLIMPORT void cfg_setnstr(cfg_t *cfg, const char *name,
                           const char *value, unsigned int index)
{
    cfg_opt_setnstr(cfg_getopt(cfg, name), value, index);
}

DLLIMPORT void cfg_setstr(cfg_t *cfg, const char *name, const char *value)
{
    cfg_setnstr(cfg, name, value, 0);
}

DLLIMPORT void cfg_setlist(cfg_t *cfg, const char *name,
                           unsigned int nvalues, ...)
{
    va_list ap;
    cfg_opt_t *opt = cfg_getopt(cfg, name);

    assert(opt && is_set(CFGF_LIST, opt->flags));

    cfg_free_value(opt);
    va_start(ap, nvalues);
    _cfg_addlist_internal(opt, nvalues, ap);
    va_end(ap);
}

DLLIMPORT void cfg_addlist(cfg_t *cfg, const char *name,
                           unsigned int nvalues, ...)
{
    va_list ap;
    cfg_opt_t *opt = cfg_getopt(cfg, name);

    assert(opt && is_set(CFGF_LIST, opt->flags));

    va_start(ap, nvalues);
    _cfg_addlist_internal(opt, nvalues, ap);
    va_end(ap);
}

DLLIMPORT void cfg_print_indent(cfg_t *cfg, FILE *fp, int indent)
{
    int i;

    for(i = 0; cfg->opts[i].name; i++)
        cfg_opt_print_indent(&cfg->opts[i], fp, indent);
}

DLLIMPORT void cfg_print(cfg_t *cfg, FILE *fp)
{
    cfg_print_indent(cfg, fp, 0);
}

DLLIMPORT cfg_print_func_t cfg_set_print_func(cfg_t *cfg, const char *name,
                                              cfg_print_func_t pf)
{
    return cfg_opt_set_print_func(cfg_getopt(cfg, name), pf);
}

DLLIMPORT cfg_validate_callback_t cfg_set_validate_func(cfg_t *cfg,
                                                        const char *name,
                                                        cfg_validate_callback_t vf)
{
    cfg_opt_t *opt = cfg_getopt_array(cfg->opts, cfg->flags, name);
    cfg_validate_callback_t oldvf;
    assert(opt);
    oldvf = opt->validcb;
    opt->validcb = vf;
    return oldvf;
}

