#include "Boss_Festergut.h"
#include "Spell_Festergut.h"
#include "Achievement_Festergut.h"

void LoadFestergut() {
    new Festergut();
    
    new SpellFestergutSpores();
    new SpellFestergutBlightedSpores();
    new SpellFestergutGastricBloat();
    new SpellFestergutPungentBlight();

    new AchievementFluShotShortage();
}