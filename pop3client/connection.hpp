/**
 * @file fileManipulator.hpp
 * @author Roman Janiczek - xjanic25(at)vutbr.cz
 * @brief Connetion class for POP3 Client
 * @version 0.1
 * @date 2021-11-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef CONNECTION_H
#define CONNECTION_H

#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"

#include <iostream>
#include <string>

using namespace std;

class Connection {
    private:
        BIO *bio, *bioSec, *proSsl;     // BIO objects - bioSec & proSsl used only with -S flag during session promotio to TLS
        SSL_CTX *ctx;                   // SSL_CTX object - used only with -T | -S flag
        SSL *ssl;                       // SSL object - used only with -T | -S flag
        char buffer[1024];              // Buffer for read data

        /**
         * @brief Clear buffer before next read
         * 
         */
        void clearBuffer();

    public:
        /**
         * @brief Construct a new Connection object 
         */
        Connection();

        /**
         * @brief Opens connection to the server on specified hostname and port (unsecured)
         * 
         * @param hostname Domain or IP
         * @param port Port for POP3
         * @return true Connection opened
         * @return false Connection failed to open
         */
        bool openConnection(string hostname, int port);
        /**
         * @brief Opens connection to the server on specified hostname and port (secured)
         * Ex. Used for comunication over secured port 995
         * @param hostname Domain or IP
         * @param port Port for POP3
         * @return true Connection opened
         * @return false Connection failed to open
         */
        bool openConnectionTLS(string hostname, int port, string certfile, string certaddr);
        /**
         * @brief Opens connection to the server on specified hostname and port (secured using STLS)
         * Ex. Used for comunication over unsecured port 110
         * @copyright https://github.com/openssl/openssl/blob/OpenSSL_1_1_0g/ssl/bio_ssl.c#L464
         * @param hostname Domain or IP
         * @param port Port for POP3
         * @return true Connection opened
         * @return false Connection failed to open
         */
        bool openConnectionSTLS(string hostname, int port, string certfile, string certaddr);

        /**
         * @brief Close connection to the server
         * 
         * @return true Connection closed
         * @return false Connection failed to close
         */
        bool closeConnection();
        /**
         * @brief Close secure connection to the server
         * 
         * @return true Connection closed
         * @return false Connection failed to close
         */
        bool closeConnectionTLS();
        /**
         * @brief Close secure connection (using STLS) to the server
         * 
         * @return true Connection closed
         * @return false Connection failed to close
         */
        bool closeConnectionSTLS();
        
        /**
         * @brief Read data from the server
         * @copyright https://developer.ibm.com/tutorials/l-openssl/
         * @return string First 1024 chars of the data received from the server
         */
        string read();

        /**
         * @brief Write data to the server
         * @copyright https://developer.ibm.com/tutorials/l-openssl/ 
         * @param command Command to be send to the server - should end with \r\n
         */
        void write(string command);
};

#endif /* CONNECTION_H */