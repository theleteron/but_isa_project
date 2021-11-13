/**
 * @file argumentsParser.cpp
 * @author Roman Janiczek - xjanic25(at)vutbr.cz
 * @brief Argument parser for POP3 Client
 * @version 0.1
 * @date 2021-11-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "argumentParser.hpp"
#include "help.hpp"
#include "returnCodes.hpp"

#include <iostream>
#include <fstream>
#include <regex>
#include <unistd.h>
#include <sys/stat.h>

ArgumentParser::ArgumentParser() {}

bool ArgumentParser::parse(int argc, char **argv) {
    progname = argv[0];
    // Options parser
    int opt;
    while((opt = getopt(argc, argv, "p:TSc:C:dna:o:h")) != -1) {
        switch (opt)
        {
            case 'p':
                setPortFlag();
                setPort(optarg);
                break;
            case 'T':
                setEncryptionFlagT();
                break;
            case 'S':
                setEncryptionFlagS();
                break;
            case 'c':
                setCertfileFlag();
                setCertfile(optarg);
                break;
            case 'C':
                setCertDirFlag();
                setCertDir(optarg);
                break;
            case 'd':
                setDeleteMsgsFlag();
            case 'n':
                setOnlyNewMsgsFlag();
                break;
            case 'a':
                setAuthfile(optarg);
                break;
            case 'o':
                setOutputDir(optarg);
                break;
            case 'h':
                helpMsg(progname);
                exit(SUCCESS);
            case '?':
                if (optopt == 'p' || optopt == 'c' ||optopt == 'C' ||optopt == 'a' ||optopt == 'o')
                    fprintf (stderr, "[ERROR][ARGUMENT] Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "[ERROR][ARGUMENT] Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr, "[ERROR][ARGUMENT] Unknown option character `\\x%x'.\n", optopt);
                usageMsg(progname);
                exit(ARGUMENT_PARSING_ERROR);    
            default:
                usageMsg(progname);
                exit(ARGUMENT_PARSING_ERROR); 
        }
    }

    // Arguments parser
    for (int index = optind; index < argc; index++) {
        setServer(argv[index]);
    }

    return validate();
}

bool ArgumentParser::validate() {
    // Check validity of arguments & options
    bool notValid = false;

    if (server.empty()) {
        fprintf(stderr, "[ERROR][ARGUMENT] Server argument is required!\n");
        notValid = true;
    }
    if (!getPortSpecifiedFlag()) {
        if (getEncryptionFlagT()) {
            setPort(to_string(ENCRYPTED_PORT));
        } else {
            setPort(to_string(UNENCRYPTED_PORT));
        }
    }
    if (authfile.empty()) {
        fprintf(stderr, "[ERROR][ARGUMENT] Authfile (-a AUTHFILE) argument is required!\n");
        notValid = true;
    }
    if (outdir.empty()) {
        fprintf(stderr, "[ERROR][ARGUMENT] Outdir (-o OUTDIR) argument is required!\n");
        notValid = true;
    }
    if (getEncryptionFlagT() && getEncryptionFlagS()) {
        fprintf(stderr, "[ERROR][ARGUMENT] Invalid arguments! Options -T & -S cannot be specified at the same time.\n");
        notValid = true;
    }

    // Check directories & files received
    if (getCertDirFlag() && !isDirectory(certaddr)) {
        fprintf(stderr, "[ERROR][FILDIR] %s is not a valid directory!\n", certaddr.c_str());
        notValid = true;
    }
    if (getCertfileFlag() && !isFile(certfile)) {
        fprintf(stderr, "[ERROR][FILDIR] %s is not a valid file!\n", certaddr.c_str());
        notValid = true;
    }
    if (!isFile(authfile)) {
        fprintf(stderr, "[ERROR][FILDIR] %s is not a valid file!\n", authfile.c_str());
        notValid = true;
    }
    if (!isDirectory(outdir)) {
        if(!createOutdir()) {
            fprintf(stderr, "[ERROR][FILDIR] %s is not a valid direcotry and program was unable to create it!\n", outdir.c_str());
            notValid = true;
        }
    }

    if (!loadCredentials()) {
        fprintf(stderr, "[ERROR][AUTHFIL] Program was unable to load login credentials from %s!\n", authfile.c_str());
    }

    if (notValid) {
        usageMsg(progname);
    }
    
    return (notValid) ? false : true;
}

bool ArgumentParser::loadCredentials() {
    ifstream data (authfile);
    string buffer;
    int loadedLines = 0;
    smatch match;
    regex rgxUser("username = (.*)");
    regex rgxPass("password = (.*)");

    if (!data.is_open()) {
        return false;
    }

    while (getline(data, buffer)) {
        if (loadedLines == 0) {
            if(regex_search(buffer, match, rgxUser)) {
                username = match[1];
                loadedLines++;
            } else {
                return false;
            }
        } else if (loadedLines == 1) {
            if(regex_search(buffer, match, rgxPass)) {
                password = match[1];
                loadedLines++;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    data.close();

    return true;
}

bool ArgumentParser::createOutdir() {
    return (mkdir(outdir.c_str(), 0777) == 0) ? true : false;
}

/**
 * SETTERS
 */

void ArgumentParser::setServer(string serverArg) {
    if (!server.empty()) {
        fprintf(stderr, "[ERROR][ARGUMENT] Invalid argument %s\n", serverArg.c_str());
        exit(ARGUMENT_PARSING_ERROR);
    }
    server = serverArg;
}

void ArgumentParser::setPort(string portArg) {
    if (!isNumber(portArg)) {
        fprintf(stderr, "[ERROR][ARGUMENT] Port is not a number!\n");
        exit(ARGUMENT_PARSING_ERROR);
    }
    port = atoi(portArg.c_str());
}

void ArgumentParser::setPortFlag() {
    if ((flags & PORT_SPECIFIED) == PORT_SPECIFIED) {
        fprintf(stderr, "Multiple -p arguments!\n");
        exit(ARGUMENT_PARSING_ERROR);
    }
    flags |= PORT_SPECIFIED;
}

void ArgumentParser::setCertfile(string certfileArg) {
    if (!certfile.empty()) {
        fprintf(stderr, "[ERROR][ARGUMENT] Multiple -c arguments!\n");
        exit(ARGUMENT_PARSING_ERROR);
    }
    certfile = certfileArg;
}

void ArgumentParser::setCertfileFlag() {
    if ((flags & CERTIFICATE_FILE_SPECIFIED) == CERTIFICATE_FILE_SPECIFIED) {
        fprintf(stderr, "[ERROR][ARGUMENT] Multiple -c arguments!\n");
        exit(ARGUMENT_PARSING_ERROR);
    }
    flags |= CERTIFICATE_FILE_SPECIFIED;
}

void ArgumentParser::setCertDir(string certaddrArg) {
    if (!certaddr.empty()) {
        fprintf(stderr, "[ERROR][ARGUMENT] Multiple -C arguments!\n");
        exit(ARGUMENT_PARSING_ERROR);
    }
    certaddr = certaddrArg;
}

void ArgumentParser::setCertDirFlag() {
    if ((flags & CERTIFICATE_ADDR_SPECIFIED) == CERTIFICATE_ADDR_SPECIFIED) {
        fprintf(stderr, "[ERROR][ARGUMENT] Multiple -C arguments!\n");
        exit(ARGUMENT_PARSING_ERROR);
    }
    flags |= CERTIFICATE_ADDR_SPECIFIED;
}

void ArgumentParser::setAuthfile(string authfileArg) {
    if (!authfile.empty()) {
        fprintf(stderr, "[ERROR][ARGUMENT] Multiple -a arguments!\n");
        exit(ARGUMENT_PARSING_ERROR);
    }
    authfile = authfileArg;
}

void ArgumentParser::setOutputDir(string outdirArg) {
    if (!outdir.empty()) {
        fprintf(stderr, "[ERROR][ARGUMENT] Multiple -o arguments!\n");
        exit(ARGUMENT_PARSING_ERROR);
    }
    outdir = outdirArg;
}

void ArgumentParser::setEncryptionFlagT() {
    if ((flags & ENCRYPTION_FLAG_T) == ENCRYPTION_FLAG_T) {
        fprintf(stderr, "[ERROR][ARGUMENT] Multiple -T arguments!\n");
        exit(ARGUMENT_PARSING_ERROR);
    }
    flags |= ENCRYPTION_FLAG_T;

}

void ArgumentParser::setEncryptionFlagS() {
    if ((flags & ENCRYPTION_FLAG_S) == ENCRYPTION_FLAG_S) {
        fprintf(stderr, "[ERROR][ARGUMENT] Multiple -S arguments!\n");
        exit(ARGUMENT_PARSING_ERROR);
    }
    flags |= ENCRYPTION_FLAG_S;
}

void ArgumentParser::setDeleteMsgsFlag() {
    if ((flags & DELETE_MSGS_ON_SERVER) == DELETE_MSGS_ON_SERVER) {
        fprintf(stderr, "[ERROR][ARGUMENT] Multiple -d arguments!\n");
        exit(ARGUMENT_PARSING_ERROR);
    }
    flags |= DELETE_MSGS_ON_SERVER;
}

void ArgumentParser::setOnlyNewMsgsFlag() {
    if ((flags & PULL_ONLY_NEW_MESSAGES) == PULL_ONLY_NEW_MESSAGES) {
        fprintf(stderr, "[ERROR][ARGUMENT] Multiple -n arguments!\n");
        exit(ARGUMENT_PARSING_ERROR);
    }
    flags |= PULL_ONLY_NEW_MESSAGES;
}

/**
 * GETTERS
 */

string ArgumentParser::getServer() {
    return server;
}

int ArgumentParser::getPort() {
    return port;
}

bool ArgumentParser::getPortSpecifiedFlag() {
    return ((flags & PORT_SPECIFIED) == PORT_SPECIFIED);
}

string ArgumentParser::getCertfile() {
    return certfile;
}

bool ArgumentParser::getCertfileFlag() {
    return ((flags & CERTIFICATE_FILE_SPECIFIED) == CERTIFICATE_FILE_SPECIFIED);
}

string ArgumentParser::getCertDir() {
    return certaddr;
}

bool ArgumentParser::getCertDirFlag() {
    return ((flags & CERTIFICATE_ADDR_SPECIFIED) == CERTIFICATE_ADDR_SPECIFIED);
}

string ArgumentParser::getAuthfile() {
    return authfile;
}

string ArgumentParser::getOutputDir() {
    return outdir;
}

bool ArgumentParser::getEncryptionFlagT() {
    return ((flags & ENCRYPTION_FLAG_T) == ENCRYPTION_FLAG_T);
}

bool ArgumentParser::getEncryptionFlagS() {
    return ((flags & ENCRYPTION_FLAG_S) == ENCRYPTION_FLAG_S);
}

bool ArgumentParser::getDeleteMsgsFlag() {
    return ((flags & DELETE_MSGS_ON_SERVER) == DELETE_MSGS_ON_SERVER);
}

bool ArgumentParser::getOnlyNewMsgsFlag() {
    return ((flags & PULL_ONLY_NEW_MESSAGES) == PULL_ONLY_NEW_MESSAGES);
}

string ArgumentParser::getUsername() {
    return username;
}

string ArgumentParser::getPassword() {
    return password;
}

/**
 * HELPERS
 */
bool ArgumentParser::isNumber(const string& str) {
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}

bool ArgumentParser::isDirectory(const string& str) {
    struct stat buffer;
    if (stat(str.c_str(), &buffer) != 0) {
        return false;
    }
    return S_ISDIR(buffer.st_mode);
}

bool ArgumentParser::isFile(const string& str) {
    struct stat buffer;
    if (stat(str.c_str(), &buffer) != 0) {
        return false;
    }
    return S_ISREG(buffer.st_mode);
}