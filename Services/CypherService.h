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

#define PADDING RSA_PKCS1_PADDING
#define IVSIZE 32
#define BLOCKSIZE 256
#define KEYSIZE 32
#define SALTSIZE 8

class CypherService: public ZeminiService
{
private:
    char *priv_key;
    char *pub_key;
    QByteArray readFile(QString filename);
    void readFile(QString filename, QByteArray &data);
    RSA* genRSAKeys();

public:
    CypherService();
    ~CypherService();

    RSA* getPublicKey(QByteArray &data);
    RSA* getPublicKey(QFile &file);

    RSA* getPrivateKey(QByteArray &data);
    RSA* getPrivateKey(QFile &file);

    QByteArray encryptRSA(RSA *key, QByteArray &data);
    QByteArray decryptRSA(RSA *key, QByteArray &data);

    QByteArray randomBytes(int size);
    void freeRSAKey(RSA * key);
};

#endif // CYPHERSERVICE_H
