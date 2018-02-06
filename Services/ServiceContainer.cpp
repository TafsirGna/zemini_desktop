#include "ServiceContainer.h"

ServiceContainer::ServiceContainer()
{
    localDbService = NULL;
    networkService = NULL;
    directoryService = NULL;
}

ZeminiService * ServiceContainer::getService(QString service)
{
    if (service == ZeminiService::Network){
        if (networkService == NULL)
            networkService = new NetworkService();
        return networkService;
    }

    if (service == ZeminiService::LocalDatabase){
        if (localDbService == NULL)
            localDbService = new LocalDBService();
        return localDbService;
    }

    if (service == ZeminiService::FileSystem){
        if (directoryService == NULL)
            directoryService = new DirectoryService();
        return directoryService;
    }

    return NULL;
}


