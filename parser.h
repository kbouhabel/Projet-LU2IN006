#ifndef PARSER_H
#define PARSER_H
#include <stdint.h>
#include "tableHachage.h"
#include "gestionMemoire.h"

typedef struct {
    char *mnemonic;  // Instruction mnemonic (or variable name for .DATA)
    char *operand1;  // First operand (or type for .DATA)
    char *operand2;  // Second operand (or initialization for .DATA)
} Instruction;

typedef struct {
    Instruction **data_instructions;   // Array of .DATA instructions
    int data_count;                   // Number of .DATA instructions
    Instruction **code_instructions; // Array of instructions
    int code_count;                // Number of .CODE instructions
    HashMap *labels;               // Labels -> indices in code_instructions
    HashMap *memory_locations;     // Variable names -> memory addresses
} ParserResult;
#endif // PARSER_H