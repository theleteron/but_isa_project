/**
 * @file help.hpp
 * @author Roman Janiczek - xjanic25(at)vutbr.cz
 * @brief Usage, help & debug macros
 * @version 0.1
 * @date 2021-11-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */
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

/**
 * @brief How to use program message outputed to stderr
 * 
 * @param progName name of the program
 */
void usageMsg(string progName);
/**
 * @brief Help printout (to stdout) if program is called with -h
 * 
 * @param progName name of the program
 */
void helpMsg(string progName);

#endif /* HELP_H */