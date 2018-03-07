#include "CypherService.h"

CypherService::CypherService()
{
    // Loading the public key of the server
    QByteArray byteServerKey = readFile(Parameters::KEYS_DIR+Parameters::FILE_SYS_SEPARATOR+"server_public_key.pem").toUtf8();

    //qDebug() << "Public key test ! " <<endl;
    //qDebug() << byteServerKey <<endl;

    const char* str = byteServerKey.constData();
    BIO * bio = BIO_new_mem_buf((void*) str, -1);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    serverPubKey = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL);
    if (serverPubKey == NULL){
        qDebug() << "Failed to load the server public key " << endl;
    }
    else{
        qDebug() << "succeed in loading the server public key " << endl;
    }

    // checking if the keys already exist
    appDataManager = new AppDataManager();
    AppData *pubKeyData = appDataManager->getByKey("PublicKey");
    AppData *privKeyData = appDataManager->getByKey("PrivateKey");

    if (pubKeyData == NULL || privKeyData == NULL){
        qDebug() << "not existing files" << endl;
        genRsaKeys();
    }
    //genRsaKeys();
    rsaPrivKey = getPrivateKey();
    rsaPubKey = getPublicKey();
}

CypherService::~CypherService()
{

}

void CypherService::genRsaKeys()
{
    // Generate key pair
    char   *pri_key;           // Private key
    char   *pub_key;           // Public key
    size_t pri_len;            // Length of private key
    size_t pub_len;            // Length of public key

    printf("Generating RSA (%d bits) keypair...", KEY_LENGTH);
    fflush(stdout);
    keypair = RSA_generate_key(KEY_LENGTH, PUB_EXP, NULL, NULL);

    // To get the C-string PEM form:
    BIO *pri = BIO_new(BIO_s_mem());
    BIO *pub = BIO_new(BIO_s_mem());

    PEM_write_bio_RSAPrivateKey(pri, keypair, NULL, NULL, 0, NULL, NULL);
    PEM_write_bio_RSAPublicKey(pub, keypair);

    pri_len = BIO_pending(pri);
    pub_len = BIO_pending(pub);

    pri_key = (char*)malloc(pri_len + 1);
    pub_key = (char*)malloc(pub_len + 1);

    BIO_read(pri, pri_key, pri_len);
    BIO_read(pub, pub_key, pub_len);

    pri_key[pri_len] = '\0';
    pub_key[pub_len] = '\0';

#ifdef PRINT_KEYS
    //printf("\n%s\n%s\n", pri_key, pub_key);
#endif
    printf("done.\n");

    // After generating the Rsa keys, i store them in files
    QString privKeyFilePath = Parameters::KEYS_DIR+Parameters::FILE_SYS_SEPARATOR+"private_key.pem";
    QString privKeyStr = QString(pri_key);
    writeFile(privKeyFilePath, privKeyStr);

    QString pubKeyFilePath = Parameters::KEYS_DIR+Parameters::FILE_SYS_SEPARATOR+"public_key.pem";
    QString pubKeyStr = QString(pub_key);
    writeFile(pubKeyFilePath, pubKeyStr);

    // And then, i save that in the db
    appDataManager->addAppData(new AppData("PublicKey", pubKeyFilePath));
    appDataManager->addAppData(new AppData("PrivateKey", privKeyFilePath));
}

QByteArray CypherService::encryptRsa(string message) {

    // Get the message to encrypt
    printf("Message to encrypt: ", "test");
    //fgets(msg, KEY_LENGTH-1, stdin);
    std::string test = message;
    char * msg = (char*)(test.c_str());
    msg[strlen((const char*)msg)] = '\0';

    // Encrypt the message
    encrypt = (char*)malloc(RSA_size(serverPubKey));
    err = (char*)malloc(130);
    if((encrypt_len = RSA_public_encrypt(strlen((const char*)msg)+1, (unsigned char*)msg, (unsigned char*)encrypt,
                                         serverPubKey, RSA_PKCS1_OAEP_PADDING)) == -1) {
        //ERR_load_crypto_strings();
        //ERR_error_string(ERR_get_error(), err);
        fprintf(stderr, "Error encrypting message: %s\n", err);
        //goto free_stuff;
    }


#ifdef WRITE_TO_FILE

    // Write the encrypted message to a file
    FILE *out = fopen("out.bin", "w");
    fwrite(encrypt, sizeof(*encrypt),  RSA_size(serverPubKey), out);
    fclose(out);
    /*
    QByteArray encr(encrypt);
    qDebug() << encrypt << endl;
    qDebug() << encr << endl;
    encr = encr.toBase64();
    qDebug() << encr << endl;
    encr = QByteArray::fromBase64(encr);
    qDebug() << encr << endl;
    */

    printf("Encrypted message written to file.\n");
    //free(encrypt);
    //encrypt = NULL;

    //encrypt = (char*)malloc(RSA_size(rsaPubKey));
    out = fopen("out.bin", "r");
    fread(encrypt, sizeof(*encrypt), RSA_size(serverPubKey), out);
    //qDebug() << encrypt << endl;
    //fclose(out);

#endif


    //free_stuff:
    /*
    RSA_free(keypair);
    BIO_free_all(pub);
    BIO_free_all(pri);
    free(pri_key);
    free(pub_key);
    free(encrypt);
    free(decrypt);
    free(err);
    */

    QByteArray result(encrypt);
    return result;
}

QByteArray CypherService::decryptRsa(QByteArray message)
{
    // Decrypt it
    char   *decrypt = NULL;    // Decrypted message
    decrypt = (char*)malloc(RSA_size(rsaPubKey));
    if(RSA_private_decrypt(RSA_size(rsaPubKey), (unsigned char*) encrypt, (unsigned char*)decrypt,
                           rsaPrivKey, RSA_PKCS1_OAEP_PADDING) == -1) {
        //ERR_load_crypto_strings();
        //ERR_error_string(ERR_get_error(), err);
        qDebug() << "Error decrypting message: " << err <<endl;
        //goto free_stuff;
        return "";
    }
    qDebug() << "Decrypted message: " << decrypt << endl;

    QByteArray result(decrypt);
    return result;
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
    QFile file(filename);
    QString data = "";
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return data;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine()+"\n";
        data += line;
    }
    return data;
}

RSA* CypherService::getPrivateKey()
{
    // reading the file for the key
    QString str_key(readFile(Parameters::KEYS_DIR+Parameters::FILE_SYS_SEPARATOR+"private_key.pem"));
    //qDebug() << "Private key test ! " <<endl;
    //qDebug() << str_key.toUtf8() << endl;

    const char* str = str_key.toUtf8().constData();
    BIO * bio = BIO_new_mem_buf((void*) str, -1);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    RSA *rsaPrivKey = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL, NULL);
    if (rsaPrivKey == NULL){
        qDebug() << "Failed to load the private key " << endl;
        return NULL;
    }
    return rsaPrivKey;
}

RSA* CypherService::getPublicKey()
{
    QByteArray byteKey = readFile(Parameters::KEYS_DIR+Parameters::FILE_SYS_SEPARATOR+"public_key.pem").toUtf8();

    const char* str = byteKey.constData();
    BIO * bio = BIO_new_mem_buf((void*) str, -1);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    RSA *rsaPubKey = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL);
    if (rsaPubKey == NULL){
        qDebug() << "Failed to load the public key " << endl;
    }

    return rsaPubKey;
}


