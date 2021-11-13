// Basic
#include <iostream>
#include <string>

// Namespace
using namespace std;

// Custom
#include "argumentParser.hpp"
#include "pop3.hpp"
#include "returnCodes.hpp"
#include "help.hpp"

int main (int argc, char **argv)
{
    ArgumentParser args = ArgumentParser();
    if(!args.parse(argc, argv)) {
        exit(ARGUMENT_PARSING_ERROR);
    }
    DEBUG_FLAGS_AND_ARG(args.getServer().c_str(), to_string(args.getPort()).c_str(), args.getEncryptionFlagT(), args.getEncryptionFlagS(), args.getCertfile().c_str(), args.getCertDir().c_str(), args.getDeleteMsgsFlag(), args.getOnlyNewMsgsFlag(), args.getAuthfile().c_str(), args.getOutputDir().c_str(), args.getUsername().c_str(), args.getPassword().c_str());

    POP3 client = POP3();
    client.passArgs(args);

    if (client.run()) {
        return SUCCESS;
    }
    return POP3_RUN_FAILED;
}