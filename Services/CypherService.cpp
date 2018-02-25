#include "CypherService.h"

QByteArray CypherService::readFile(QString filename)
{

}

void CypherService::readFile(QString filename, QByteArray &data)
{

}

RSA *CypherService::genRSAKeys()
{
    qDebug() << "Start generating RSA keys" <<endl;
    RSA * keys = RSA_generate_key(2048,3, NULL, NULL);

    BIO * bio_Pri = BIO_new(BIO_s_mem());
    BIO * bio_Pub = BIO_new(BIO_s_mem());

    PEM_write_bio_RSAPrivateKey(bio_Pri, keys, NULL,NULL, 0, NULL, NULL);
    PEM_write_bio_RSAPublicKey(bio_Pub, keys);

    size_t priv_size = BIO_pending(bio_Pri);
    size_t pub_size = BIO_pending(bio_Pub);

    priv_key = (char *) malloc(priv_size+1);
    pub_key = (char *) malloc(priv_size+1);

    BIO_read(bio_Pri, priv_key, priv_size);
    BIO_read(bio_Pub, pub_key, pub_size);

    priv_key[priv_size] = '\0';
    pub_key[pub_size] = '\0';

    qDebug() << priv_key << endl;
}

CypherService::CypherService()
{
    this->genRSAKeys();
}

CypherService::~CypherService()
{
//    EVP_cleanup();
//    ERR_free_strings();
}

RSA *CypherService::getPublicKey(QByteArray &data)
{

}

RSA *CypherService::getPublicKey(QFile &file)
{

}

RSA *CypherService::getPrivateKey(QByteArray &data)
{

}

RSA *CypherService::getPrivateKey(QFile &file)
{

}

QByteArray CypherService::encryptRSA(RSA *key, QByteArray &data)
{

}

QByteArray CypherService::decryptRSA(RSA *key, QByteArray &data)
{

}

QByteArray CypherService::randomBytes(int size)
{

}

void CypherService::freeRSAKey(RSA *key)
{

}
