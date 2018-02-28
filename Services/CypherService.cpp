#include "CypherService.h"

void CypherService::init()
{
    Crypto crypto;
    encryptRsa(&crypto);
}

void CypherService::encryptRsa(Crypto *crypto) {
  // Get the message to encrypt
  string message = getMessage("Message to RSA encrypt: ");

  // Encrypt the message with RSA
  // +1 on the string length argument because we want to encrypt the NUL terminator too
  unsigned char *encryptedMessage = NULL;
  unsigned char *encryptedKey;
  unsigned char *iv;
  size_t encryptedKeyLength;
  size_t ivLength;

  int encryptedMessageLength = crypto->rsaEncrypt((const unsigned char*)message.c_str(), message.size()+1,
    &encryptedMessage, &encryptedKey, &encryptedKeyLength, &iv, &ivLength);

  if(encryptedMessageLength == -1) {
    fprintf(stderr, "Encryption failed\n");
    return;
  }

  // Print the encrypted message as a base64 string
  char* b64Message = base64Encode(encryptedMessage, encryptedMessageLength);
  printf("Encrypted message: %s\n", b64Message);

  // Decrypt the message
  char *decryptedMessage = NULL;

  int decryptedMessageLength = crypto->rsaDecrypt(encryptedMessage, (size_t)encryptedMessageLength,
    encryptedKey, encryptedKeyLength, iv, ivLength, (unsigned char**)&decryptedMessage);

  if(decryptedMessageLength == -1) {
    fprintf(stderr, "Decryption failed\n");
    return;
  }

  printf("Decrypted message: %s\n", decryptedMessage);

  // Clean up
  free(encryptedMessage);
  free(decryptedMessage);
  free(encryptedKey);
  free(iv);
  free(b64Message);

  encryptedMessage = NULL;
  decryptedMessage = NULL;
  encryptedKey = NULL;
  iv = NULL;
  b64Message = NULL;
}

void CypherService::encryptAes(Crypto *crypto) {
  // Get the message to encrypt
  string message = getMessage("Message to AES encrypt: ");

  // Encrypt the message with AES
  unsigned char *encryptedMessage = NULL;
  int encryptedMessageLength = crypto->aesEncrypt((const unsigned char*)message.c_str(), message.size()+1, &encryptedMessage);

  if(encryptedMessageLength == -1) {
    fprintf(stderr, "Encryption failed\n");
    return;
  }

  // Print the encrypted message as a base64 string
  char *b64Message = base64Encode(encryptedMessage, encryptedMessageLength);
  printf("Encrypted message: %s\n", b64Message);

  // Decrypt the message
  char *decryptedMessage = NULL;
  int decryptedMessageLength = crypto->aesDecrypt(encryptedMessage, (size_t)encryptedMessageLength, (unsigned char**)&decryptedMessage);

  if(decryptedMessageLength == -1) {
    fprintf(stderr, "Decryption failed\n");
    return;
  }

  printf("Decrypted message: %s\n", decryptedMessage);

  // Clean up
  free(encryptedMessage);
  free(decryptedMessage);
  free(b64Message);

  encryptedMessage = NULL;
  decryptedMessage = NULL;
  b64Message = NULL;
}

string CypherService::getMessage(const char *prompt) {
  string message = "test";

//  printf(prompt);
//  fflush(stdout);

//  getline(std::cin, message);
  return message;
}

void CypherService::printKeys(Crypto *crypto) {
  // Write the RSA keys to stdout
  crypto->writeKeyToFile(stdout, KEY_SERVER_PRI);
  crypto->writeKeyToFile(stdout, KEY_SERVER_PUB);
  crypto->writeKeyToFile(stdout, KEY_CLIENT_PUB);

  // Write the AES key to stdout in hex
  unsigned char *aesKey;
  size_t aesLength = crypto->getAesKey(&aesKey);

  printf("AES key: ");
  for(unsigned int i=0; i<aesLength; i++) {
    printf("%x", aesKey[i]);
  }
  printf("\n");
}

void CypherService::writeFile(QString filename, QString data)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << data;
}

QString CypherService::readFile(QString filename)
{
    QString data = "";
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return data;

//    QTextStream in(&file);
//    while (!in.atEnd()) {
//        QString line = in.readLine();
//        data += line;
//    }

    //data = QString::QString(file.readAll());
}

RSA* CypherService::genRSAKeys()
{
    // then making the dir of the thumbnails
    QString keysDirPath = Parameters::KEYS_DIR;
    QDir keysDir(keysDirPath.replace("\\", "/"));
    if (!keysDir.exists()){
        if (!keysDir.mkdir("."))
            qDebug()<<"Error when creating the " << Parameters::KEYS_DIR <<" folder"<<endl;
    }

    //qDebug() << "Start generating RSA keys" <<endl;
    RSA * keys = RSA_generate_key(2048,3, NULL, NULL);

    BIO * bio_Pri = BIO_new(BIO_s_mem());
    BIO * bio_Pub = BIO_new(BIO_s_mem());

    PEM_write_bio_RSAPrivateKey(bio_Pri, keys, NULL, NULL, 0, NULL, NULL);
    PEM_write_bio_RSAPublicKey(bio_Pub, keys);

    size_t priv_size = BIO_pending(bio_Pri);
    size_t pub_size = BIO_pending(bio_Pub);

    char* priv_key = (char *) malloc(priv_size+1);
    char* pub_key = (char *) malloc(priv_size+1);

    BIO_read(bio_Pri, priv_key, priv_size);
    BIO_read(bio_Pub, pub_key, pub_size);

    priv_key[priv_size] = '\0';
    pub_key[pub_size] = '\0';

    QString pv_key(priv_key);
    QString pb_key(pub_key);
    //qDebug() << pv_key <<endl;

    /*
    RSA * rsaPrivKey = PEM_read_bio_RSAPrivateKey(bio_Pri, &keys, NULL, NULL);
    if (!rsaPrivKey){
        qDebug() << "1- Failed to load the public key !" << endl;
    }
    */

    writeFile(Parameters::KEYS_DIR+Parameters::FILE_SYS_SEPARATOR+"private_key.pem", pv_key);
    writeFile(Parameters::KEYS_DIR+Parameters::FILE_SYS_SEPARATOR+"public_key.pem", pb_key);

    AppDataManager * appDataManager = new AppDataManager();
    appDataManager->addAppData(new AppData("PublicKey", Parameters::KEYS_DIR+Parameters::FILE_SYS_SEPARATOR+"public_key.pem"));
    appDataManager->addAppData(new AppData("PrivateKey", Parameters::KEYS_DIR+Parameters::FILE_SYS_SEPARATOR+"private_key.pem"));

    return keys;
}

CypherService::CypherService()
{
}

CypherService::~CypherService()
{
}

QString CypherService::getPublicKey()
{
    // i check first if a public key already exists in the database
    AppDataManager * appDataManager = new AppDataManager();
    AppData * pubKeyData = appDataManager->getByKey("PublicKey");
    if (pubKeyData == NULL){
        qDebug() << "again" <<endl;
        genRSAKeys();
        return readFile(appDataManager->getByKey("PublicKey")->getValue());
    }
    return readFile(pubKeyData->getValue());
}

QString CypherService::getPrivateKey()
{
    // i check first if a private key already exists in the database
    AppDataManager * appDataManager = new AppDataManager();
    AppData * privKeyData = appDataManager->getByKey("PrivateKey");
    if (privKeyData == NULL){
        genRSAKeys();
        return readFile(appDataManager->getByKey("PrivateKey")->getValue());
    }
    return readFile(privKeyData->getValue());
}

QString CypherService::encryptRSA(RSA *keys, QString data)
{
    /*
    QFile file(Parameters::KEYS_DIR+Parameters::FILE_SYS_SEPARATOR+"private_key.pem");
    if (!file.open(QIODevice::ReadOnly)){
        qDebug() << "Failed to open file !" << endl;
        return data;
    }

    QByteArray bytePubKey = file.readAll();
    const char* pubKeyStr = bytePubKey.constData();
    BIO * bio = BIO_new_mem_buf(pubKeyStr, strlen(pubKeyStr));
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    RSA * rsaPubKey = NULL;
    rsaPubKey = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL);
    if (rsaPubKey == NULL){
        qDebug() << "Failed to load the public key !" << endl;
        //return data;
    }
    //BIO_free(bio);
    */

    QByteArray byteData = data.toUtf8();
    int data_size = byteData.length();
    const unsigned char* str = (const unsigned char*) data.constData();
//    qDebug() << msg << endl;
    char * encrypt =(char*) malloc(RSA_size(keys));
    //int encrypt_size = RSA_public_encrypt(strlen(data.toStdString().c_str()), (unsigned char*) data.toStdString().c_str(), (unsigned char*) encrypt, keys, PADDING);
    //if ((encrypt_size) == -1){
//        ERR_load_crypto_strings();
//        ERR_error_string(ERR_get_error(), err);
    //    qDebug() << "Error when encrypting message : " << endl;
    //    return "";
    //}
    //qDebug() << encrypt <<endl;
    //qDebug() << " ---" << endl;
    //qDebug() << *(new QString(encrypt))<< endl;
    return *(new QString(encrypt));
}

QString CypherService::decryptRSA(RSA *keys, QString data)
{
    /*
    QByteArray bytePrivKey = getPrivateKey().toUtf8();
    const char* privKeyStr = bytePrivKey.constData();
    BIO * bio = BIO_new_mem_buf((void*) privKeyStr, -1);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    RSA * rsaPrivKey = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL, NULL);
    if (!rsaPrivKey){
        qDebug() << "Failed to load the private key !" << endl;
        //return "";
    }
    BIO_free(bio);
    */

    QByteArray byteData = data.toUtf8();
    const unsigned char* encryptData = (const unsigned char*)byteData.constData();
    char * decrypt =(char*) malloc(RSA_size(keys));
    //qDebug() << "---" << endl;
    //qDebug() << data.toStdString().c_str() << endl;
//    if (RSA_private_decrypt(strlen(data.toStdString().c_str()),
//                           (const unsigned char*) data.toStdString().c_str(),
//                           (unsigned char*) decrypt, keys, PADDING) == -1){
//        qDebug() << "Error when decrypting message : " << endl;
//        return "";
//    }
    return *(new QString(decrypt));
}
