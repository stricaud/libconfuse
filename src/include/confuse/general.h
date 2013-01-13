#ifndef _cfg_general_h_
#define _cfg_general_h_


#ifdef __cplusplus
extern "C" {
#endif


#if defined(_WIN32) && !defined(__GNUC__)
# ifdef HAVE__FILENO
#  define fileno _fileno
# endif
# include <io.h>
# ifdef HAVE__ISATTY
#  define isatty _isatty
# endif
# ifdef BUILDING_DLL
#  define DLLIMPORT __declspec (dllexport)
# else /* ! BUILDING_DLL */
#  define DLLIMPORT __declspec (dllimport)
# endif /* BUILDING_DLL */
#else /* ! _WIN32 || __GNUC__ */
# define DLLIMPORT
#endif /* _WIN32 */

#ifndef __BORLANDC__
# define __export
#endif

#if defined(ENABLE_NLS) && defined(HAVE_GETTEXT)
# include <locale.h>
# include <libintl.h>
# define _(str) dgettext(PACKAGE, str)
#else
# define _(str) str
#endif
#define N_(str) str

#define is_set(f, x) (((f) & (x)) == (f))

#define STATE_CONTINUE 0
#define STATE_EOF -1
#define STATE_ERROR 1

#ifdef __cplusplus
}
#endif

#endif	/* _cfg_general_h_ */
