#include "revision.h"
#include "revisiondefines.h"
namespace git
{
    char* getHash()
    {
        return _HASH;
    }
    char* getDate()
    {
        return _DATE;
    }
    char* getCompanyname()
    {
        return VER_COMPANYNAME_STR;
    }
    char* getLegalCopyRight()
    {
        return VER_LEGALCOPYRIGHT_STR;
    }
    int getFileVersion()
    {
        return VER_FILEVERSION;
    }
    char* getFileVersionStr()
    {
        return VER_FILEVERSION_STR;
    }
    int getProductVersion()
    {
        return VER_PRODUCTVERSION;
    }
    char* getProduktVersionStr()
    {
        return VER_PRODUCTVERSION_STR;
    }
}
#undef _HASH
#undef _DATE
#undef VER_COMPANYNAME_STR
#undef VER_LEGALCOPYRIGHT_STR
#undef VER_FILEVERSION
#undef VER_FILEVERSION_STR
#undef VER_PRODUCTVERSION
#undef VER_PRODUCTVERSION_STR
