/**
 * @file fileManipulator.cpp
 * @author Roman Janiczek - xjanic25(at)vutbr.cz
 * @brief File manipulator for POP3 Client
 * @version 0.1
 * @date 2021-11-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "fileManipulator.hpp"
#include "help.hpp"
#include "returnCodes.hpp"

#include <iostream>
#include <fstream>
#include <regex>

FileManipulator::FileManipulator() {}

void FileManipulator::setOutdir(string directory) {
    outdir = directory;
}

bool FileManipulator::saveMessage(string message, bool onlyNew) {
    if (outdir.empty()) {
        fprintf(stderr, "[ERROR][MESSAGE] Outdir not specified for FileManipulator.\n");
        return false;
    }
    // Full message path
    string MID = getMessageID(message);
    if (MID.empty()) {
        return false;
    }
    string path = outdir + "/" + MID;

    // Check if file already exists & continue or return based on the -n flag
    fstream stream;
    stream.open(path);
    if (!stream.fail()) {
        stream.close();
        if (onlyNew) {
            return false;
        }
    }

    // Create/Update file & fill it with message content
    ofstream output(path);
    output << message;
    output.close();

    return true;
}

string FileManipulator::getMessageID(string msg) {
    string MID;
    smatch match;
    regex msgID("Message-ID: (.*)\r\n");

    if (!regex_search(msg, match, msgID)) {
        fprintf(stderr, "[ERROR][MESSAGE] Message doesn't contain msgID.\n");
        return "";
    }
    MID = match[1];
    return MID;
}