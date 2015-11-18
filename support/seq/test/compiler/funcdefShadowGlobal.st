program funcdefShadowGlobalTest

option +r;

int x;
assign x;

void pvSetX(int x)
{
    pvPut(x); /* error: parameter 1 to 'pvPut' was not assigned to a pv */
}

ss simple {
    state simple {
        when () {
            pvSetX(1.0);
        } exit
    }
}
