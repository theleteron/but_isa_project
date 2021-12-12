# ISA - POP3 Client with TLS support
Program that allows user to download & delete mail messages using POP3 protocol.
## Usage
### Compile
If you have all requirements you can compile this program using
```bash
$ make
```
Compilation will create `build/` directory which will contain copy of `popcl` executable and `obj` files in `bin/` subdirectory.
### Run
```bash
$ ./popcl <server> [-p <port>] [-T|-S [-c <certfile>] [-C <certaddr>]] [-d] [-n] -a <auth_file> -o <out_dir>
```

| Option           	| Description                                                                       	| Required           	|
|------------------	|-----------------------------------------------------------------------------------	|--------------------	|
| `<server>`       	| POP3 server's domain name or IP                                                   	| :heavy_check_mark: 	|
| `-p <port>`      	| Port over which we want to communicate with the POP3 server                       	|                    	|
| `-T`             	| Use secure connection from the start                                              	|                    	|
| `-S`             	| Initiate unsecure connection and then use TLS to switch to secure one             	|                    	|
| `-c <certfile>`  	| Path to the file that contains certificates                                       	|                    	|
| `-C <certaddr>`  	| Path to the directory that contains certfiles                                     	|                    	|
| `-d`             	| Delete downloaded messages                                                        	|                    	|
| `-n`             	| Download only new messages                                                        	|                    	|
| `-a <auth_file>` 	| Authorization file - file containing username & password (detailed syntax bellow) 	| :heavy_check_mark: 	|
| `-o <out_dir>`   	| Directory where you want downloaded mails to be saved at                          	| :heavy_check_mark: 	|

## Requirements
- `make`
- `g++`
- `openssl` library
- `getopt` from `util-linux`
## Authfile syntax
Authfile must be written in the following form to be accepted
```
username = your_user
password = your_pass
```
## All project files
```
.
├── Makefile
├── main.cpp
├── pop3client
│   ├── pop3.hpp
│   ├── pop3.cpp
│   ├── argumentParser.hpp
│   ├── argumentParser.cpp
│   ├── connection.hpp
│   ├── connection.cpp
│   ├── fileManipulator.hpp
│   ├── fileManipulator.cpp
│   ├── help.hpp
│   ├── help.cpp
│   └── returnCodes.hpp
├── doc
│   ├── src
│   │   ├── img
│   │   │   ├── logo_cz.png
│   │   │   └── logo_en.png
│   │   └── main.tex
│   └── manual.pdf
└── README.md
```
## Notes
- Default ports are 110 (unsecured, STLS) & 995 (POP3s)
- Final mark 7/20
## Known limitations & bugs
- Program is unable to handle mail messages with attachements, it will start looping and has to be stopped using `Ctrl + C`
- If program is not able to estabilish connection using custom port it will start looping and has to be stopped using `Ctrl + C`
- During message download, client expects message to have `Message-ID`. If `Message-ID` is not present file is not saved and user is informed on `stderr`.

## Examples of program calls
### Download of all messages
```bash
$ ./popcl pop3.seznam.cz -a config/seznam.conf -o temp
Downloaded 3 messages.
$
```
### Download of only new messages
```bash
$ ./popcl pop3.seznam.cz -a config/seznam.conf -o temp -n
Downloaded 0 new messages.
$
```
### Download & delete of all messages
```bash
$ ./popcl pop3.seznam.cz -a config/seznam.conf -o temp -d
Downloaded 3 messages.
$
```
