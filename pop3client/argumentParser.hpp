/**
 * @file argumentsParser.hpp
 * @author Roman Janiczek - xjanic25(at)vutbr.cz
 * @brief Argument parser for POP3 Client
 * @version 0.1
 * @date 2021-11-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef ARGUMENTSPARSER_H
#define ARGUMENTSPARSER_H

#include <string>

using namespace std;

// Flags
#define COMPARE_FLAGS(flags, flagConst) ((flags & flagConst) == flagConst)      
#define PORT_SPECIFIED                  (1 << 0)
#define ENCRYPTION_FLAG_T               (1 << 1)
#define ENCRYPTION_FLAG_S               (1 << 2)
#define CERTIFICATE_FILE_SPECIFIED      (1 << 3)
#define CERTIFICATE_ADDR_SPECIFIED      (1 << 4)
#define DELETE_MSGS_ON_SERVER           (1 << 5)
#define PULL_ONLY_NEW_MESSAGES          (1 << 6)

// Defaults
#define UNENCRYPTED_PORT                110
#define ENCRYPTED_PORT                  995

/**
 * @brief Class responsible for handling parsing of arguments
 */
class ArgumentParser {
    private:
        string progname;    // Name of the program
        int flags = 0;     // Flags saved here using BITWISE operations
        string server;      // Domain name or IP -> <server>:<port>
        int port = 0;       // Port -> <server>:<port>
        string certfile;    // File containing certificates
        string certaddr;    // Directory containing certification files
        string authfile;    // Authorization file - username&password
        string outdir;      // Output directory - where to save downloaded data

        string username;    // Username from authfile
        string password;    // Password from authfile

        // INTERNAL FUNCTIONS

        /**
         * @brief Validates arguments, load credentials, checks directories & creates outdir if not present
         * 
         * @return true Arguments are OK
         * @return false Arguments are NOK
         */
        bool validate();

        /**
         * @brief Attempts to load credentials from authfile
         * 
         * @return true Credentials loaded OK
         * @return false Crenedtials load failed
         */
        bool loadCredentials();

        /**
         * @brief Create outdir
         * 
         * @return true Create OK
         * @return false Create fail
         */
        bool createOutdir();

        // HELPERS

        /**
         * @brief Check if string is valid number
         * @copyright https://www.delftstack.com/howto/cpp/how-to-determine-if-a-string-is-number-cpp/
         * @param str String to be checked
         * @return true Is number
         * @return false Contains non-digit char
         */
        bool isNumber(const string& str);

        /**
         * @brief Check if string is path to the directory
         * @copyright https://stackoverflow.com/questions/4553012/checking-if-a-file-is-a-directory-or-just-a-file
         * @param str Presumed path to the directory
         * @return true Is directory
         * @return false Is Not
         */
        bool isDirectory(const string& str);

        /**
         * @brief Check if string is path to the regular file
         * @copyright https://stackoverflow.com/questions/4553012/checking-if-a-file-is-a-directory-or-just-a-file
         * @param str Presumed path to the regular file
         * @return true Is regular file
         * @return false Is Not
         */
        bool isFile(const string& str);

    public:
        /**
         * @brief Construct a new Argument Parser object 
         */
        ArgumentParser();
        
        /**
         * @brief Main parsing function
         * 
         * @param argc Number of arguments received
         * @param argv Array of arguments
         * @return true Parsing successfull
         * @return false Parsing failed
         */
        bool parse(int argc, char **argv);

        // SETTERS
        void setServer(string serverArg);
        void setPort(string portArg);
        void setPortFlag();
        void setCertfile(string certfileArg);
        void setCertfileFlag();
        void setCertDir(string certaddrArg);
        void setCertDirFlag();
        void setAuthfile(string authfileArg);
        void setOutputDir(string outdirArg);
        void setEncryptionFlagT();
        void setEncryptionFlagS();
        void setDeleteMsgsFlag();
        void setOnlyNewMsgsFlag();

        // GETTERS
        string getServer();
        int getPort();
        bool getPortSpecifiedFlag();
        string getCertfile();
        bool getCertfileFlag();
        string getCertDir();
        bool getCertDirFlag();
        string getAuthfile();
        string getOutputDir();
        bool getEncryptionFlagT();
        bool getEncryptionFlagS();
        bool getDeleteMsgsFlag();
        bool getOnlyNewMsgsFlag();
        string getUsername();
        string getPassword();
};

#endif /* ARGUMENTSPARSER_H */
