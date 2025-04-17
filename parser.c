#include "parser.h"
static int next_available_address = 0;
Instruction * parse_data_instruction(const char *line, HashMap *memory_locations){
    Instruction *instruction = (Instruction *)malloc(sizeof(Instruction));
    if (!instruction) return NULL;
    char *line_copy = strdup(line);
    char *mnemonic = strtok(line_copy, " ");
    char *operand1 = strtok(NULL, " ");
    char *operand2 = strtok(NULL, " ");
    if (mnemonic && operand1 && operand2) {
        instruction->mnemonic = strdup(mnemonic);
        instruction->operand1 = strdup(operand1);
        instruction->operand2 = strdup(operand2);
        
        int operand2_size=1;
        for (int i=0; i<strlen(operand2); i++){
            if (operand2[i] == ','){
                operand2_size++;
            }
        }
        next_available_address += operand2_size;
        hashmap_insert(memory_locations, strdup(mnemonic), (void*)(intptr_t)(next_available_address));
        free(line_copy);
        return instruction;
    }
    else {
        free(instruction);
        free(line_copy);
        return NULL;
    }
}