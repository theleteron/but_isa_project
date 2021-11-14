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
#include "help.hpp"

#include <regex>

POP3::POP3() {}

bool POP3::run() {
    startConnection();
    login();
    retrieveMessage(2);
    return true;
}

void POP3::passArgs(ArgumentParser argArgs) {
    args = argArgs;
    manip = FileManipulator();
    manip.setOutdir(args.getOutputDir());
}

// CONNECTION

void POP3::startConnection() {
    conn = Connection();
    if (args.getEncryptionFlagT()) {
        if (!conn.openConnectionTLS(args.getServer(), args.getPort(), args.getCertfile(), args.getCertDir())) {
            exit(CONNECTION_ERROR);
        }
    } else if (args.getEncryptionFlagS()) {
        if (!conn.openConnectionSTLS(args.getServer(), args.getPort(), args.getCertfile(), args.getCertDir())) {
            exit(CONNECTION_ERROR);
        }
    } else {
        if (!conn.openConnection(args.getServer(), args.getPort())) {
            exit(CONNECTION_ERROR);
        }
    }    
}

void POP3::endConnection() {
    if (args.getEncryptionFlagT()) {
        if (!conn.closeConnectionTLS()) {
            exit(CONNECTION_ERROR);
        }
    } else if (args.getEncryptionFlagS()) {
        if (!conn.closeConnectionSTLS()) {
            exit(CONNECTION_ERROR);
        }
    } else {
        if (!conn.closeConnection()) {
            exit(CONNECTION_ERROR);
        }
    } 
}

// AUTHENTICATION

void POP3::login() {
    string usernameCmd = "USER " + args.getUsername();
    string passwordCmd = "PASS " + args.getPassword();
    string response;
    smatch match;
    regex resOk("\\+OK (.*)\r\n");

    conn.write(usernameCmd);
    response = conn.read();
    if (!regex_search(response, match, resOk)) {
        fprintf(stderr, "[INFO] Invalid username!\n");
        loginAbort();
        return;
    }
    response.clear();

    conn.write(passwordCmd);
    response = conn.read();
    if (!regex_search(response, match, resOk)) {
        fprintf(stderr, "[INFO] Invalid password!\n");
        loginAbort();
        return;
    }
    response.clear();

    fprintf(stderr, "[INFO] Login successfull!\n");

    return;
}

void POP3::loginAbort() {
    string abortSignIn = "QUIT";
    string response;
    smatch match;
    regex resOk("\\+OK (.*)\r\n");

    
    conn.write(abortSignIn);
    response = conn.read();
    
    if (!regex_search(response, match, resOk)) {
        fprintf(stderr, "[INFO] Login Abort failed!\n");
        /* TODO close connection */
        return;
    }
    response.clear();

    
    /* TODO close connection */
    response = match[1];
    fprintf(stderr, "[INFO] %s\n", response.c_str());

    return;
}

// MSG MANIPULATION

bool POP3::retrieveMessage(int id) {
    string retrCmd = "RETR " + to_string(id);
    string response;
    smatch match;
    regex resOk("\\+OK (.*)\r\n");

    conn.write(retrCmd);

    // Confirm that download request was accepted
    response = conn.read();
    if (!regex_search(response, resOk)) {
        fprintf(stderr, "[ERROR][MSG] Message with number %d not retrieved!\n", id);
        return false;
    }

    // Download message
    string message = "";
    do {
        message.append(conn.read());
    } while(!endOfMsg(message));

    // Save message
    manip.saveMessage(message, args.getOnlyNewMsgsFlag());

    // Delete message if flag -d
    if (args.getDeleteMsgsFlag()) {
        string deleCmd = "DELE " + to_string(id);
        response = conn.read();
        if (!regex_search(response, match, resOk)) {
            string res = match[1];
            fprintf(stderr, "[ERROR][MSG] Delete fail! %s\n", res.c_str());
            return false;
        }
    }

    return true;
}

// HELPERS

bool POP3::endOfMsg(const string msg) {
    regex resEnd("\r\n\\.\r\n$");    // \r\n.\r\n

    if (regex_search(msg, resEnd)) {
        return true;
    }
    return false;
}

int POP3::getNumberOfMsgs() {
    smatch match;
    regex regNum("\\+OK (\\d*) \\d*\r\n");
    string result;
    
    conn.write("STAT");
    string r = conn.read();

    if (!regex_search(r, match, regNum)) {
        fprintf(stderr, "[ERROR][STAT] Unable to get number of msgs!\n");
        return -1;
    }

    result = match[1];

    return atoi(result.c_str());
}