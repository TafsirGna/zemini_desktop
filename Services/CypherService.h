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
//#include <openssl/applink.c>

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
    //char   *msg;//[KEY_LENGTH/8];  // Message to encrypt
    char   *encrypt = NULL;    // Encrypted message
    char   *err;               // Buffer for any error messages
    RSA *keypair;
    RSA *rsaPrivKey;
    RSA *rsaPubKey;
    int encrypt_len;
    RSA* serverPubKey;
    AppDataManager *appDataManager;

public:
    CypherService();
    ~CypherService();

    QString readFile(QString);
    void writeFile(QString, QString);
    void genRsaKeys();

    QByteArray encryptRsa(string message);
    QByteArray decryptRsa(QByteArray message);

    RSA *getPublicKey();
    RSA* getPrivateKey();
};

#endif // CYPHERSERVICE_H
