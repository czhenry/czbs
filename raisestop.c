/* ------------------------ raisestop ----------------------------- */
/* czbs 2/2020 */

#include "m_pd.h"
#include <signal.h>

static t_class *raisestop_class;

typedef struct _raisestop
{
    t_object x_obj;
    t_inlet *x_in;
    t_clock *x_clock;
} t_raisestop;

static void raisestop_raise(void)
{
    raise(SIGSTOP);
}

static void *raisestop_new(void)
{
    t_raisestop *x = (t_raisestop *)pd_new(raisestop_class);
    x->x_clock = clock_new(x, (t_method) raisestop_raise);
    return (x);
}    

static void raisestop_bang(t_raisestop *x)
{
    clock_delay(x->x_clock, 0L);
}

static void raisestop_free(t_raisestop *x)
{
    clock_free(x->x_clock);
}

void raisestop_setup(void)
{
    raisestop_class = class_new(gensym("raisestop"),
        (t_newmethod)raisestop_new, (t_method)raisestop_free, 
            sizeof(t_raisestop), 0, 0);
    class_addbang(raisestop_class, raisestop_bang);
}
