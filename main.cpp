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
    ArgumentParser args = ArgumentParser();
    if(!args.parse(argc, argv)) {
        exit(ARGUMENT_PARSING_ERROR);
    }
    
    POP3 client = POP3();
    client.passArgs(args);

    if (client.run()) {
        return SUCCESS;
    }
    return POP3_RUN_FAILED;
}