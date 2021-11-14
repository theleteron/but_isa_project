/**
 * @file fileManipulator.hpp
 * @author Roman Janiczek - xjanic25(at)vutbr.cz
 * @brief File manipulator for POP3 Client
 * @version 0.1
 * @date 2021-11-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef FILEMANIPULATOR_H
#define FILEMANIPULATOR_H

#include "openssl/md5.h"

#include <string>

using namespace std;

class FileManipulator {
    private:
        string outdir;

        /**
         * @brief Get the Message-ID to use as filename
         * 
         * @param msg Whole msg
         * @return string Message-ID
         */
        string getMessageID(string msg);

    public:
        /**
         * @brief Construct a new File Manipulator object 
         */
        FileManipulator();

        void setOutdir(string directory);

        bool saveMessage(string message, bool onlyNew);

};

#endif /* FILEMANIPULATOR_H */