// Basic
#include <iostream>
#include <string>
#include <unistd.h>
// SSL
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

// Namespace
using namespace std;

// Custom
#include "argumentsParser.hpp"
#include "returnCodes.hpp"

// Defaults
#define DEFAULT_CERT_STORAGE(x)         SSL_CTX_set_default_verify_paths(x)

// Debug
#ifdef DEBUG
#define DEBUG_FLAGS_AND_ARG(args...)     \
  fprintf(stderr, "Server: %s\nPort: %s\nEncryption(Y/N): %d/%d\nCerfile: %s\nCertaddr: %s\nDelete msg on server: %d\nOnly new msgs: %d\nAuthfile: %s\nOutput directory: %s\n", ##args);     
#define TRACE_LOG(msg, args...)         \
  fprintf(stderr, msg, ##args);
#else
#define DEBUG_FLAGS_AND_ARG(args...) 
#define TRACE_LOG(msg, args...)
#endif

int main (int argc, char **argv)
{
  string server, port, certfile, certaddr, authfile, outdir;  
  int flags = argumentParse(argc, argv, server, port, certfile, certaddr, authfile, outdir);
  if (flags == -1) {
    return ARGUMENT_PARSING_ERROR;
  }

  if (!COMPARE_FLAGS(flags, CERTIFICATE_FILE_SPECIFIED) && !(COMPARE_FLAGS(flags, CERTIFICATE_ADDR_SPECIFIED))) {
  //  certfile = DEFAULT_CERT_STORAGE();
  //  certaddr = DEFAULT_CERT_STORAGE();
  }

  DEBUG_FLAGS_AND_ARG(server.c_str(), port.c_str(), COMPARE_FLAGS(flags, ENCRYPTION_FLAG_T), COMPARE_FLAGS(flags, ENCRYPTION_FLAG_S), certfile.c_str(), certaddr.c_str(), COMPARE_FLAGS(flags, DELETE_MSGS_ON_SERVER), COMPARE_FLAGS(flags, PULL_ONLY_NEW_MESSAGES), authfile.c_str(), outdir.c_str())

  return SUCCESS;
}