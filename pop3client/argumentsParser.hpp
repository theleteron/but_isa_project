#ifndef ARGUMENTSPARSER_H
#define ARGUMENTSPARSER_H

#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

// Flags
#define COMPARE_FLAGS(flags, flagConst) ((flags & flagConst) == flagConst)      
#define PORT_SPECIFIED                  (1 << 0)
#define ENCRYPTION_FLAG_T               (1 << 1)
#define ENCRYPTION_FLAG_S               (1 << 2)
#define CERTIFICATE_FILE_SPECIFIED      (1 << 3)
#define CERTIFICATE_ADDR_SPECIFIED      (1 << 4)
#define DELETE_MSGS_ON_SERVER           (1 << 5)
#define PULL_ONLY_NEW_MESSAGES          (1 << 6)

// Defaults
#define UNENCRYPTED_PORT                 110
#define ENCRYPTED_PORT                  995

int argumentParse(int argc, char **argv, string& server, string& port, string& certfile, string& certaddr, string& authfile, string& outdir);

#endif /* ARGUMENTSPARSER_H */