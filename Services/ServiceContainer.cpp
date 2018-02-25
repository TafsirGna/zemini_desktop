#include "ServiceContainer.h"

ServiceContainer::ServiceContainer()
{
    localDbService = NULL;
    networkService = NULL;
    directoryService = NULL;
    cypherService = NULL;
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

    if (service == ZeminiService::Cypher){
        if (cypherService == NULL)
            cypherService = new CypherService();
        return cypherService;
    }

    return NULL;
}


