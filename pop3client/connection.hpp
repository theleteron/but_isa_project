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
        BIO *bio;
        char buffer[1024];

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
        bool openConnectionTLS(string hostname, int port);

        /**
         * @brief Opens connection to the server on specified hostname and port (secured using STLS)
         * Ex. Used for comunication over unsecured port 110
         * @param hostname Domain or IP
         * @param port Port for POP3
         * @return true Connection opened
         * @return false Connection failed to open
         */
        bool openConnectionSTLS(string hostname, int port);
        
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