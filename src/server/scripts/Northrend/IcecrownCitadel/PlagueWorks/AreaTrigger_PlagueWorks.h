#ifndef __AREATRIGGER_PLAGUEWORKS_H
#define __AREATRIGGER_PLAGUEWORKS_H

#include "PlagueWorks.h"

class AreaTriggerPlagueWorksTrap : public AreaTriggerScript
{
public:
    AreaTriggerPlagueWorksTrap() : AreaTriggerScript("at_icc_putricide_trap_at5647") { }

    bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/);
};

#endif