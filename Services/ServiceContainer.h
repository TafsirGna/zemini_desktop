#ifndef SERVICECONTAINER_H
#define SERVICECONTAINER_H

#include "Services/LocalDbService.h"
#include "Services/NetworkService.h"
#include "Services/DirectoryService.h"

class ServiceContainer
{

private:
    LocalDBService * localDbService;
    NetworkService * networkService;
    DirectoryService * directoryService;

public:
    ServiceContainer();

    // Getters
    ZeminiService * getService(QString);
};

#endif // SERVICECONTAINER_H
