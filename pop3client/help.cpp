#include "help.hpp"

void usageMsg(string progName) {
    fprintf(stdout, "Usage: %s <server> [-p <port>] [-T|-S [-c <certfile> [-C <certaddr>]] [-d] [-n] -a <authfile> -o <outdir>\n", progName.c_str());
}

void helpMsg(string progName) {
    usageMsg(progName);
    fprintf(stdout, "\nRequired arguments & options:\n");
    fprintf(stdout, "\t<server>\tIP address or domain name of requested POP3 server\n");
    fprintf(stdout, "\t-a <authfile>\tPath to the authetication file (file containing login credentials)\n");
    fprintf(stdout, "\t-o <outdir>\tPath to the directory which will contain downloaded emails\n");
    fprintf(stdout, "\n");
    fprintf(stdout, "Options:\n");
    fprintf(stdout, "\t-p <port>\tSpecify port to be used for the communication\n");
    fprintf(stdout, "\t-T\t\tInitiate encrypted communication (whole communication is encrypted)\n");
    fprintf(stdout, "\t-S\t\tInitiate unecrypted communication and switch to encrypted using STLS\n");
    fprintf(stdout, "\t-c <certfile>\tPath to custom file with certificates\n\t\t\tTo be used only in conection with -T or -S option\n");
    fprintf(stdout, "\t-C <certaddr>\tPath to custom directory with certificates\n\t\t\tTo be used only in conection with -T or -S option\n");
    fprintf(stdout, "\t-d\t\tDelete downloaded msgs from the server\n");
    fprintf(stdout, "\t-n\t\tDownload only new msgs from the server\n");
}