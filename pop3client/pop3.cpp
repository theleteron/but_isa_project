/**
 * @file pop3.cpp
 * @author Roman Janiczek - xjanic25(at)vutbr.cz
 * @brief POP3 Client
 * @version 0.1
 * @date 2021-11-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "pop3.hpp"

POP3::POP3() {}

bool POP3::run() {
    return true;
}

void POP3::passArgs(ArgumentParser argArgs) {
    args = argArgs;
}