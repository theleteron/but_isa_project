#ifndef HELP_H
#define HELP_H

#include <iostream>
#include <string>

using namespace std;

// Debug
#ifdef DEBUG
#define DEBUG_FLAGS_AND_ARG(args...)     \
    fprintf(stderr, "Server: %s\nPort: %s\nEncryption(Y/N): %d/%d\nCerfile: %s\nCertaddr: %s\nDelete msg on server: %d\nOnly new msgs: %d\nAuthfile: %s\nOutput directory: %s\n\nUsername: %s\nPassword: %s\n", ##args);     
#define TRACE_LOG(msg, args...)         \
    fprintf(stderr, msg, ##args);
#else
#define DEBUG_FLAGS_AND_ARG(args...) 
#define TRACE_LOG(msg, args...)
#endif

void usageMsg(string progName);
void helpMsg(string progName);

#endif /* HELP_H */