/*************************************************************************\
Copyright (c) 2014-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
program delay_in_action

ss test {
    state test {
        when (delay(1)) {
            delay(2);
        } exit
    }
}
