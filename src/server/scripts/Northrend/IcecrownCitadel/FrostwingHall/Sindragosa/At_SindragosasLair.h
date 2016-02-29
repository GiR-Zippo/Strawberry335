#ifndef __AT_SINDRAGOSASLAIR_H_
#define __AT_SINDRAGOSASLAIR_H_

#include "Sindragosa.h"

class AtSindragosaLair : public AreaTriggerScript
{
    public:
        AtSindragosaLair() : AreaTriggerScript("at_sindragosa_lair") { }

    bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/);
};

#endif