/**
 * @file pop3.hpp
 * @author Roman Janiczek - xjanic25(at)vutbr.cz
 * @brief POP3 Client
 * @version 0.1
 * @date 2021-11-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef POP3_H
#define POP3_H

#include "argumentParser.hpp"
#include "connection.hpp"
#include "fileManipulator.hpp"

class POP3 {
    private:
        ArgumentParser args;
        Connection conn;

        void createConnection();

        void login();

    public:
        /**
         * @brief Construct a new File Manipulator object 
         */
        POP3();

        /**
         * @brief Main logic of POP3 client
         * 
         * @return true Run finished OK
         * @return false Something failed during execution
         */
        bool run();

        // SETTERS
        /**
         * @brief Sets ArgumentParser as args for POP3 client
         * This function expects that argsArg is valid - this is ensured in earlier program processing in main.c
         * @param argsArg ArgumentParser class holding parsed & valid arguments
         */
        void passArgs(ArgumentParser argsArg);
        
};

#endif /* POP3_H */