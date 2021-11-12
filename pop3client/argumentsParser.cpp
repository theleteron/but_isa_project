#include "argumentsParser.hpp"

int argumentParse(int argc, char **argv, string& server, string& port, string& certfile, string& certaddr, string& authfile, string& outdir) {
    // Flags to save selected options
    int flags = 0;

    // Options parser
    int opt;
    while((opt = getopt(argc, argv, "p:TSc:C:dna:o:h")) != -1) {
        switch (opt)
        {
            case 'p':
                flags |= PORT_SPECIFIED;
                port = optarg;
                break;
            case 'T':
                flags |= ENCRYPTION_FLAG_T;
                break;
            case 'S':
                flags |= ENCRYPTION_FLAG_S;
                break;
            case 'c':
                flags |= CERTIFICATE_FILE_SPECIFIED;
                certfile = optarg;
                break;
            case 'C':
                flags |= CERTIFICATE_ADDR_SPECIFIED;
                certaddr = optarg;
                break;
            case 'd':
                flags |= DELETE_MSGS_ON_SERVER;
                break;
            case 'n':
                flags |= PULL_ONLY_NEW_MESSAGES;
                break;
            case 'a':
                authfile = optarg;
                break;
            case 'o':
                outdir = optarg;
                break;
            case 'h':
                helpMsg(argv[0]);
                exit(0);
            case '?':
                if (optopt == 'p' || optopt == 'c' ||optopt == 'C' ||optopt == 'a' ||optopt == 'o')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
                usageMsg(argv[0]);
                return -1;    
            default:
                usageMsg(argv[0]);
                return -1;
        }
    }

    // Arguments parser
    for (int index = optind; index < argc; index++) {
        if (server.empty()) {
            server = argv[index];
        } else {
            fprintf(stderr, "Invalid argument -%s.\n",argv[index]);
            usageMsg(argv[0]);
            return -1;
        }
    }

    // Check validity of arguments & options
    bool notValid = false;

    if (server.empty()) {
        fprintf(stderr, "Server argument is required!\n");
        notValid = true;
    }
    if (!COMPARE_FLAGS(flags, PORT_SPECIFIED)) {
        if (COMPARE_FLAGS(flags, ENCRYPTION_FLAG_T) || COMPARE_FLAGS(flags, ENCRYPTION_FLAG_S)) {
            port = to_string(ENCRYPTED_PORT);
        } else {
            port = to_string(UNENCRYPTED_PORT);
        }
    }
    if (authfile.empty()) {
        fprintf(stderr, "Authfile (-a AUTHFILE) argument is required!\n");
        notValid = true;
    }
    if (outdir.empty()) {
        fprintf(stderr, "Outdir (-o OUTDIR) argument is required!\n");
        notValid = true;
    }
    if (COMPARE_FLAGS(flags, ENCRYPTION_FLAG_T) && COMPARE_FLAGS(flags, ENCRYPTION_FLAG_S)) {
        fprintf(stderr, "Invalid arguments! Options -T & -S cannot be specified at the same time.\n");
        notValid = true;
    }
    if (notValid) {
        usageMsg(argv[0]);
    }
    
    return (!notValid) ? flags : -1;
}