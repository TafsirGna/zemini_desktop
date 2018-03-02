#ifndef CYPHERSERVICE_H
#define CYPHERSERVICE_H

#include "openssl/rsa.h"
#include "openssl/pem.h"
#include "openssl/engine.h"
#include "openssl/conf.h"
#include "openssl/evp.h"
#include "openssl/rand.h"
#include "openssl/err.h"
#include <QByteArray>
#include <QFile>
#include <QDebug>
#include <Services/ZeminiService.h>
#include <Config/Parameters.h>
#include <Database/EntityManagers/AppDataManager.h>
#include <QIODevice>
#include <QTextStream>
#include <QFile>
//#include <Vendor/OpenSSL_Wrapper/Crypto.h>
//#include <Vendor/OpenSSL_Wrapper/base64.h>
#include <cstdio>

#define KEY_LENGTH  2048
#define PUB_EXP     3
#define PRINT_KEYS
#define WRITE_TO_FILE

class CypherService: public ZeminiService
{
private:

public:
    CypherService();
    ~CypherService();

    static QString readFile(QString);
    static void writeFile(QString, QString);
    static void genRsaKeys();

    static QString encryptRsa(string message);
    static QString decryptRsa(string message);
};

#endif // CYPHERSERVICE_H
