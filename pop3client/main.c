#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
#define UNECRYPTED_PORT                 110
#define ENCRYPTED_PORT                  995
#define DEFAULT_CERT_STORAGE            SSL_CTX_set_default_verify_paths()

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
  // Options
  int flags = 0;
  // Arguments
  char *server = NULL;
  char *port = NULL;
  char *certfile = NULL;
  char *certaddr = NULL;
  char *authfile = NULL;
  char *outdir = NULL;
  // Argument parser  
  int opt;
  while((opt = getopt(argc, argv, "p:TSc:C:dna:o:")) != -1) {
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
    case '?':
      if (optopt == 'p' || optopt == 'c' ||optopt == 'C' ||optopt == 'a' ||optopt == 'o')
        fprintf (stderr, "Option -%c requires an argument.\n", optopt);
      else if (isprint (optopt))
        fprintf (stderr, "Unknown option `-%c'.\n", optopt);
      else
        fprintf (stderr,
               "Unknown option character `\\x%x'.\n",
                optopt);
      return 1;    
    default:
      return 1;
    }
  }
  for (int index = optind; index < argc; index++) {
    if (server == NULL) {
      server = argv[index];
    } else {
      fprintf(stderr, "Invalid argument -%s.\n",argv[index]);
      return 1;
    }
  }
  if (COMPARE_FLAGS(flags, ENCRYPTION_FLAG_T) && COMPARE_FLAGS(flags, ENCRYPTION_FLAG_S)) {
    fprintf(stderr, "Invalid arguments! Options -T & -S cannot be specified at the same time.\n");
    return 1;
  }

  DEBUG_FLAGS_AND_ARG(server, port, COMPARE_FLAGS(flags, ENCRYPTION_FLAG_T), COMPARE_FLAGS(flags, ENCRYPTION_FLAG_S), certfile, certaddr, COMPARE_FLAGS(flags, DELETE_MSGS_ON_SERVER), COMPARE_FLAGS(flags, PULL_ONLY_NEW_MESSAGES), authfile, outdir)

  return 0;
}