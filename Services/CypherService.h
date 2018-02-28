#ifndef CYPHERSERVICE_H
#define CYPHERSERVICE_H

#include "openssl/rsa.h"
#include "openssl/pem.h"
#include "openssl/engine.h"
#include "openssl/conf.h"
#include "openssl/evp.h"
#include "openssl/rand.h"
//#include "openssl/aes.h"
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
#include <Vendor/OpenSSL_Wrapper/Crypto.h>
#include <Vendor/OpenSSL_Wrapper/base64.h>
//#include <fstream>
//#include <iostream>

//#define PADDING RSA_PKCS1_PADDING
//#define IVSIZE 32
//#define BLOCKSIZE 256
//#define KEYSIZE 32
//#define SALTSIZE 8

class CypherService: public ZeminiService
{
private:

public:
    CypherService();
    ~CypherService();
    static void init();

    static QString readFile(QString);
    static void writeFile(QString, QString);

    static RSA *genRSAKeys();
    static QString getPrivateKey();
    static QString getPublicKey();

    static QString encryptRSA(RSA *key, QString data);
    static QString decryptRSA(RSA *key, QString data);

    static void encryptRsa(Crypto *crypto);
    static void encryptAes(Crypto *crypto);
    static void printKeys(Crypto *crypto);
    static std::string getMessage(const char *prompt);
};

#endif // CYPHERSERVICE_H
