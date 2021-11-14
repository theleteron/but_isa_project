/**
 * @file pop3.cpp
 * @author Roman Janiczek - xjanic25(at)vutbr.cz
 * @brief POP3 Client
 * @version 0.1
 * @date 2021-11-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "pop3.hpp"
#include "returnCodes.hpp"

#include <regex>

POP3::POP3() {}

bool POP3::run() {
    createConnection();
    login();
    return true;
}

void POP3::passArgs(ArgumentParser argArgs) {
    args = argArgs;
}

void POP3::createConnection() {
    conn = Connection();
    if (args.getEncryptionFlagT()) {
        /*if (!conn.openConnectionTLS(args.getServer(), args.getPort())) {
            exit(CONNECTION_ERROR);
        }*/
    } else if (args.getEncryptionFlagS()) {
        /*if (!conn.openConnectionSTLS(args.getServer(), args.getPort())) {
            exit(CONNECTION_ERROR);
        }*/
    } else {
        if (!conn.openConnection(args.getServer(), args.getPort())) {
            exit(CONNECTION_ERROR);
        }
    }    
}

void POP3::login() {
    string usernameCmd = "USER " + args.getUsername();
    string passwordCmd = "PASS " + args.getPassword();
    string response;
    smatch match;
    regex resOk("\\+OK (.*)\r\n");

    conn.write(usernameCmd);
    response = conn.read();
    if (!regex_search(response, match, resOk)) {
        /* TODO QUIT & close connection */
        fprintf(stderr, "Invalid username!\n");
        return;
    }
    response.clear();

    conn.write(passwordCmd);
    response = conn.read();
    if (!regex_search(response, match, resOk)) {
        /* TODO QUIT & close connection */
        fprintf(stderr, "Invalid password!\n");
        return;
    }
    response.clear();

    fprintf(stderr, "[INFO] Login successfull!\n");
}