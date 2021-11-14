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
        FileManipulator manip;

        // CONNECTION

        /**
         * @brief Create a Connection objet and open connection to server specified by args 
         */
        void startConnection();
        /**
         * @brief Close connection to the server
         */
        void endConnection();

        // AUTHENTICATION

        /**
         * @brief Use USER & PASS msgs to try to login to the server with args.username & args.password
         */
        void login();
        /**
         * @brief Send QUIT msg in AUTHORIZATION phase & call close connection
         * This is called in case of invalid username or password!
         */
        void loginAbort();

        // MSG MANIPULATION

        /**
         * @brief Retrieves message from the server
         * 
         * @param id number of the msg
         * @return true Message retrieved and saved
         * @return false Message retrieved and not saved or not retrieved at all
         */
        bool retrieveMessage(int id);

        //HELPERS

        /**
         * @brief Detect end of msg received from the server
         * 
         * @param msg Content of Connection::read()
         * @return true End of msg found
         * @return false End of msg not found
         */
        bool endOfMsg(const string msg);

        /**
         * @brief Get number of msgs availiable from STAT cmd
         * 
         * @return int Returns <number_of_msgs> from -> +OK <number_of_msgs> <size>
         */
        int getNumberOfMsgs();

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