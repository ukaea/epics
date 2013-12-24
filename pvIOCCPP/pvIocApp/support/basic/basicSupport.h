/* basicSupport.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
#ifndef BASICSUPPORT_H
#define BASICSUPPORT_H

#include <pv/support.h>

namespace epics { namespace pvIOC { 

extern SupportCreate noopSupportCreate;
extern SupportCreate genericSupportCreate;
extern SupportCreate scanFieldSupportCreate;

}}

#endif  /* BASICSUPPORT_H */
