#ifndef SERVICECONTAINER_H
#define SERVICECONTAINER_H

#include "Services/LocalDbService.h"
#include "Services/NetworkService.h"
#include "Services/DirectoryService.h"
#include "Services/CypherService.h"

class ServiceContainer
{

private:
    LocalDBService * localDbService;
    NetworkService * networkService;
    DirectoryService * directoryService;
    CypherService * cypherService;

public:
    ServiceContainer();

    // Getters
    ZeminiService * getService(QString);
};

#endif // SERVICECONTAINER_H
