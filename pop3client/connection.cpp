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

    // Create BIO object
    bio = BIO_new_connect(address.c_str());
    if (!bio) {
        fprintf(stderr,"[ERROR][CONNECTION] Creation of BIO object failed!\n");
        return false;
    }

    // Attempt to create connection using BIO object
    if (BIO_do_connect(bio) <= 0) {
        fprintf(stderr,"[ERROR][CONNECTION] Connection failed!\n");
        return false;
    }

    // Handle server response
    string response = read();

    if (regex_search(response, match, rgxOk)) {
        string msg = match[1];
        TRACE_LOG("[INFO] %s\n", msg.c_str())
        return true;
    } else if (regex_search(response, match, rgxNok)) {
        string msg = match[1];
        fprintf(stderr,"[ERROR][CONNECTION] Error response from the server! %s\n", msg.c_str());
        return false;
    }

    // Should be unreachable
    return false;
}

bool Connection::openConnectionTLS(string hostname, int port, string certfile, string certaddr) {
    string address = hostname + ":" + to_string(port);
    smatch match;
    regex rgxOk("\\+OK (.*)\r\n");
    regex rgxNok("\\-ERR (.*)\r\n");

    // Prepare SSL_CTX object
    ctx = SSL_CTX_new(SSLv23_client_method());

    // Load CA
    if (!certfile.empty() && !certaddr.empty()) {
        if (!SSL_CTX_load_verify_locations(ctx, certfile.c_str(), certaddr.c_str())) { 
            fprintf(stderr, "[ERROR][SSL] CAfile or CApath is invalid!\n");
            SSL_CTX_free(ctx);
            return false;
        }
    } else if (!certfile.empty()) {
        if (!SSL_CTX_load_verify_locations(ctx, certfile.c_str(), NULL)) {
            fprintf(stderr, "[ERROR][SSL] CAfile is invalid!\n");
            SSL_CTX_free(ctx);
            return false;
        }        
    } else if (!certaddr.empty()) {
        if (!SSL_CTX_load_verify_locations(ctx, NULL, certaddr.c_str())) {
            fprintf(stderr, "[ERROR][SSL] CApath is invalid!\n");
            SSL_CTX_free(ctx);
            return false;
        }
    } else {
        if (!SSL_CTX_set_default_verify_paths(ctx)) {
            fprintf(stderr, "[ERROR][SSL] CAfile or CApath is invalid!\n");
            SSL_CTX_free(ctx);
            return false;
        }
    }

    // Create SSL BIO object
    bio = BIO_new_ssl_connect(ctx);
    BIO_get_ssl(bio, &ssl);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

    BIO_set_conn_hostname(bio, address.c_str());

    // Attempt to create connection using BIO object
    if(BIO_do_connect(bio) <= 0)
    {
        fprintf(stderr,"[ERROR][CONNECTION] Connection failed!\n");
        return false;
    }

    // Verify CA
    if(SSL_get_verify_result(ssl) != X509_V_OK)
    {
        fprintf(stderr,"[ERROR][SSL] Invalid CA!\n");
        return false;
    }

    // Handle server response
    string response = read();

    if (regex_search(response, match, rgxOk)) {
        string msg = match[1];
        TRACE_LOG("[INFO] %s\n", msg.c_str())
        return true;
    } else if (regex_search(response, match, rgxNok)) {
        string msg = match[1];
        fprintf(stderr,"[ERROR][CONNECTION] Error response from the server! %s\n", msg.c_str());
        return false;
    }

    // Should be unreachable
    return false;
}

bool Connection::openConnectionSTLS(string hostname, int port, string certfile, string certaddr) {
    string address = hostname + ":" + to_string(port);
    string promote = "STLS";
    smatch match;
    regex rgxOk("\\+OK (.*)\r\n");
    regex rgxNok("\\-ERR (.*)\r\n");

    // Prepare SSL_CTX object
    ctx = SSL_CTX_new(SSLv23_client_method());

    // Open insecure connection
    if (!openConnection(hostname, port)) {
        SSL_CTX_free(ctx);
        return false;
    }

    // Request for promotion to TLS
    write(promote);

    // Handle response to request for promotion
    string response = read();

    if (regex_search(response, match, rgxOk)) {
        string msg = match[1];
        TRACE_LOG("[INFO] %s\n", msg.c_str())
    } else if (regex_search(response, match, rgxNok)) {
        string msg = match[1];
        fprintf(stderr,"[ERROR][CONNECTION] Error response from the server! %s\n", msg.c_str());
        return false;
    }

    // Setup secure connection
    // Load CA
    if (!certfile.empty() && !certaddr.empty()) {
        if (!SSL_CTX_load_verify_locations(ctx, certfile.c_str(), certaddr.c_str())) { 
            fprintf(stderr, "[ERROR][SSL] CAfile or CApath is invalid!\n");
            SSL_CTX_free(ctx);
            return false;
        }
    } else if (!certfile.empty()) {
        if (!SSL_CTX_load_verify_locations(ctx, certfile.c_str(), NULL)) {
            fprintf(stderr, "[ERROR][SSL] CAfile is invalid!\n");
            SSL_CTX_free(ctx);
            return false;
        }        
    } else if (!certaddr.empty()) {
        if (!SSL_CTX_load_verify_locations(ctx, NULL, certaddr.c_str())) {
            fprintf(stderr, "[ERROR][SSL] CApath is invalid!\n");
            SSL_CTX_free(ctx);
            return false;
        }
    } else {
        if (!SSL_CTX_set_default_verify_paths(ctx)) {
            fprintf(stderr, "[ERROR][SSL] CAfile or CApath is invalid!\n");
            SSL_CTX_free(ctx);
            return false;
        }
    }

    // Handle promotion to TLS
    proSsl = BIO_new_ssl(ctx, 1);
    bioSec = BIO_push(proSsl, bio);

    BIO_get_ssl(bioSec, &ssl);

    // Attempt to connect using BIO promoted for TLS
    if(BIO_do_connect(bioSec) <= 0)
    {
        fprintf(stderr,"[ERROR][CONNECTION] Connection failed!\n");
        return false;
    }

    // Verify CA
    if(SSL_get_verify_result(ssl) != X509_V_OK)
    {
        fprintf(stderr,"[ERROR][CONNECTION] Invalid CA!\n");
        return false;
    }

    // Write & read are using bio
    bio = bioSec;

    // Connection estabilished
    return true;
}

bool Connection::closeConnection() {
    BIO_free_all(bio);
    return true;
}

bool Connection::closeConnectionTLS() {
    SSL_CTX_free(ctx);
    BIO_free_all(bio);
    return true;
}

bool Connection::closeConnectionSTLS() {
    closeConnectionTLS();
    return true;
}

string Connection::read() {
    clearBuffer();
    int x = BIO_read(bio, buffer, 1024);
    if(x == 0) {
        fprintf(stderr, "[ERROR][READ] Something went wrong during read operation!\n");
    } else if(x < 0) {
        if(! BIO_should_retry(bio)) {
            fprintf(stderr, "[ERROR][READ] Something went wrong during read operation!\n");
        }
        /* Do something to handle the retry */
    }
    
    return string(buffer, x);
}

void Connection::write(string command) {
    command += "\r\n";
    if(BIO_write(bio, command.c_str(), command.length()) <= 0)
    {
        if(! BIO_should_retry(bio))
        {
            fprintf(stderr, "[ERROR][READ] Something went wrong during write operation!\n");
        }
        /* Do something to handle the retry */
    }
}

void Connection::clearBuffer() {
    // Clearing buffer for read() operation
    memset(buffer, 0, 1024);
}