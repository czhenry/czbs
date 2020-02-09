/* ------------------------ sigcont ----------------------------- */
/* czbs 2/2020 */

#include "m_pd.h"
#include <signal.h>

static t_class *sigcont_class;

typedef struct _sigcont
{
    t_object x_obj;
    t_outlet *x_out;
} t_sigcont;

static t_sigcont *only_sigcont = NULL;

static void *sigcont_new(void)
{
    t_sigcont *x = (t_sigcont *)pd_new(sigcont_class);
    x->x_out = outlet_new(&x->x_obj, &s_bang);
    if (only_sigcont)
        pd_error(x, "only one instance of sigcont per process allowed");
    else
        only_sigcont = x;
    return (x);
}

static void sigcont_free(t_sigcont *x)
{
    if (x == only_sigcont)
        only_sigcont = NULL;
}

static void sigcont_bang_out(int sig)
{
    if (only_sigcont && sig == SIGCONT)
        outlet_bang(only_sigcont->x_out);
}

void sigcont_setup(void)
{
    sigcont_class = class_new(gensym("sigcont"),
        (t_newmethod)sigcont_new, (t_method)sigcont_free, 
            sizeof(t_sigcont), 0, 0);
    signal(SIGCONT, sigcont_bang_out);
}
