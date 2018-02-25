#include "ZeminiService.h"

const QString ZeminiService::Network = "NetworkService";
const QString ZeminiService::FileSystem = "FileSystem";
const QString ZeminiService::LocalDatabase = "LocalDatabase";
const QString ZeminiService::Cypher = "Encryption";

/**
 * @brief ZeminiService::ZeminiService
 */
ZeminiService::ZeminiService()
{

}

const User * ZeminiService::getUser()
{
    return user;
}

void ZeminiService::setUser(User * user)
{
    this->user = user;
}
