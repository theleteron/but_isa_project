/**
 * @file fileManipulator.cpp
 * @author Roman Janiczek - xjanic25(at)vutbr.cz
 * @brief Connetion class for POP3 Client
 * @version 0.1
 * @date 2021-11-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "connection.hpp"
#include "help.hpp"
#include "returnCodes.hpp"

#include <regex>

Connection::Connection() {
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();
    SSL_library_init();
}

bool Connection::openConnection(string hostname, int port) {
    string address = hostname + ":" + to_string(port);
    smatch match;
    regex rgxOk("\\+OK (.*)\r\n");
    regex rgxNok("\\-ERR (.*)\r\n");

    bio = BIO_new_connect(address.c_str());
    if (!bio) {
        fprintf(stderr,"[ERROR][CONNECTION] Creation of BIO object failed!\n");
        return false;
    }

    if (BIO_do_connect(bio) <= 0) {
        fprintf(stderr,"[ERROR][CONNECTION] Connection failed!\n");
        return false;
    }

    string response = read();

    if (regex_search(response, match, rgxOk)) {
        string msg = match[1];
        fprintf(stderr, "[INFO] %s\n", msg.c_str());
        return true;
    } else if (regex_search(response, match, rgxOk)) {
        string msg = match[1];
        fprintf(stderr,"[ERROR][CONNECTION] Error response from the server! %s\n", msg.c_str());
        return false;
    }
    return false;
}

string Connection::read() {
    clearBuffer();
    int x = BIO_read(bio, buffer, 1024);
    if(x == 0) {
        /* Handle closed connection */
    } else if(x < 0) {
        if(! BIO_should_retry(bio)) {
            /* Handle failed read here */
        }
        /* Do something to handle the retry */
    }
    return buffer;
}

void Connection::write(string command) {
    command += "\r\n";
    if(BIO_write(bio, command.c_str(), command.length()) <= 0)
    {
        if(! BIO_should_retry(bio))
        {
            /* Handle failed write here */
        }
        /* Do something to handle the retry */
    }
}

void Connection::clearBuffer() {
    memset(buffer, 0, 1024);
}