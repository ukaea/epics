program memberTest

%%struct struct_t { int i; struct { double d; } s; };
%%union union_t { int i; struct struct_t *p; };

struct struct_t s;
union union_t u;

ss simple {
    struct struct_t *ps = &s;
    union union_t *pu = &u;

    state simple {
        when () {
            if (s.i == 1)
                s.i++;
            if (s.s.d == 0.1)
                s.s.d /= 3.0;

            if (ps->i == 1)
                ps->i++;
            if (ps->s.d == 0.1)
                ps->s.d /= 3.0;

            if (pu->i == 13)
                pu->p->i = pu->i;
            if (pu->p->s.d == 13.0)
                pu->p->s.d *= 3;
        } exit
    }
}
