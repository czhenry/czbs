/* ------------------------ sigtstp ----------------------------- */
/* czbs 2/2020 */

#include "m_pd.h"
#include <signal.h>

static t_class *sigtstp_class;

typedef struct _sigtstp
{
    t_object x_obj;
    t_outlet *x_out;
} t_sigtstp;

static t_sigtstp *only_sigtstp = NULL;

static void *sigtstp_new(void)
{
    t_sigtstp *x = (t_sigtstp *)pd_new(sigtstp_class);
    x->x_out = outlet_new(&x->x_obj, &s_bang);
    if (only_sigtstp)
        pd_error(x, "only one instance of sigtstp per process allowed");
    else
        only_sigtstp = x;
    return (x);
}

static void sigtstp_free(t_sigtstp *x)
{
    if (x == only_sigtstp)
        only_sigtstp = NULL;
}

static void sigtstp_bang_out(int sig)
{
    if (only_sigtstp && sig == SIGTSTP)
        outlet_bang(only_sigtstp->x_out);
}

void sigtstp_setup(void)
{
    sigtstp_class = class_new(gensym("sigtstp"),
        (t_newmethod)sigtstp_new, (t_method)sigtstp_free, 
            sizeof(t_sigtstp), 0, 0);
    signal(SIGTSTP, sigtstp_bang_out);
}
