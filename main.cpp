/**
 * @file main.cpp
 * @author Roman Janiczek - xjanic25(at)vutbr.cz
 * @brief Main - connecting Args and POP3
 * @version 0.1
 * @date 2021-11-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */
// Basic
#include <iostream>
#include <string>

// Namespace
using namespace std;

// Custom
#include "pop3client/argumentParser.hpp"
#include "pop3client/pop3.hpp"
#include "pop3client/returnCodes.hpp"

int main (int argc, char **argv)
{
    // Create arguments & parse them
    ArgumentParser args = ArgumentParser();
    if(!args.parse(argc, argv)) {
        exit(ARGUMENT_PARSING_ERROR);
    }
    
    // Create POP3 client
    POP3 client = POP3();
    client.passArgs(args);

    // Initiate program run
    if (client.run()) {
        return SUCCESS;
    }

    // Unreachable as POP3::run() is always true
    return POP3_RUN_FAILED;
}