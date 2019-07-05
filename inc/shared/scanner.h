#ifndef __SCANNER_MODULE_INTERFACEH
#define __SCANNER_MODULE_INTERFACEH 1

#include "common.h"
#include "shared/error.h"
#include "shared/tokens.h"
#include "shared/token_array.h"

Scanner* init_scanner(char* file_name);
void exit_scanner(Scanner* scanner);
TokenArray* get_tokens(Scanner* scanner);

#endif