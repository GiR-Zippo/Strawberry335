#ifndef __REVISION_H__
#define __REVISION_H__
namespace git
{
    char* getHash();
    char* getDate();
    char* getCompanyname();
    char* getLegalCopyRight();
    int getFileVersion();
    char* getFileVersionStr();
    int getProductVersion();
    char* getProduktVersionStr();
}
#endif // __REVISION_H__
