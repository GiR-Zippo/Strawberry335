void LoadInstanceIcecrownCitadel();
void LoadIcecrownCitadel();
void LoadIcecrownCitadelTeleport();

void LoadLowerSpire();

void LoadUpperSpire();

void LoadLordMarrowgar();
void LoadLadyDeathwhisper();
void LoadGunShip();
void LoadDeathbringerSaurfang();
void LoadTrashLowerSpire();

void LoadTrashUpperSpire();

void LoadPlagueWorks();

void LoadFestergut();
void LoadProfessorPutricide();
void LoadRotface();
void LoadPlagueWorksTrash();


void LoadCrimsonHall();

void LoadBloodPrinceCouncil();
void LoadBloodQueenLanaThel();
void LoadTrashFrostwingHall();


void LoadFrostwingHall();

void LoadValithriaDreamwalker();
void LoadKriffel();
void LoadSindragosa();


void LoadFrozenThrone();

void LoadTheLichKing();

///////////////////////////
// Load the whole Script //
///////////////////////////

void AddSC_icecrowncitadel()
{
/*****************/
/* Load Instance */
/*****************/
    LoadInstanceIcecrownCitadel();
    LoadIcecrownCitadel();
    LoadIcecrownCitadelTeleport();

/*****************/
/*  Load Wings   */
/*****************/
    LoadLowerSpire();
    LoadUpperSpire();
    LoadPlagueWorks();
    LoadCrimsonHall();
    LoadFrostwingHall();
    LoadFrozenThrone();
}

/*****************/
/*  LowerSpire   */
/*****************/
void LoadLowerSpire()
{
    LoadTrashLowerSpire();
    LoadLordMarrowgar();
    LoadLadyDeathwhisper();
    LoadGunShip();
    LoadDeathbringerSaurfang();
}

/*****************/
/*  UpperSpire   */
/*****************/
void LoadUpperSpire()
{
    LoadTrashUpperSpire();
}

/*****************/
/*  PlagueWorks  */
/*****************/
void LoadPlagueWorks()
{
    LoadPlagueWorksTrash();
    LoadProfessorPutricide();
    LoadRotface();
    LoadFestergut();
}

/*****************/
/*  CrimsonHall  */
/*****************/
void LoadCrimsonHall()
{
    LoadBloodPrinceCouncil();
    LoadBloodQueenLanaThel();
}

/*****************/
/* FrostwingHall */
/*****************/
void LoadFrostwingHall()
{
    LoadTrashFrostwingHall();
    LoadValithriaDreamwalker();
    LoadKriffel();
    LoadSindragosa();
}

/*****************/
/*  FrozenThrone  */
/*****************/
void LoadFrozenThrone()
{
    LoadTheLichKing();
}
