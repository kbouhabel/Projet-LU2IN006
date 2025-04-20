#include "parser.h"
static int next_available_address = 0;
Instruction *parse_data_instruction(const char *line, HashMap *memory_locations) {
    Instruction *instruction = (Instruction *)malloc(sizeof(Instruction));
    if (!instruction) return NULL;

    char *line_copy = strdup(line);
    if (!line_copy) {
        free(instruction);
        return NULL;
    }

    char *mnemonic = strtok(line_copy, " ");
    char *operand1 = strtok(NULL, " ");
    char *operand2 = strtok(NULL, " ");
    if (mnemonic && operand1) {
        if (operand2 == NULL) {
            operand2 = ""; 
        }
        instruction->mnemonic = strdup(mnemonic); // Only one strdup
        instruction->operand1 = strdup(operand1);
        instruction->operand2 = strdup(operand2);

        int operand2_size = 1;
        for (int i = 0; i < strlen(operand2); i++) {
            if (operand2[i] == ',') {
                operand2_size++;
            }
        }
        next_available_address += operand2_size;
        int *address = (int *)malloc(sizeof(int));
        *address = next_available_address;

        // Use instruction->mnemonic as the key
        hashmap_insert(memory_locations, instruction->mnemonic, address);

        free(line_copy);
        return instruction;
    } else {
        free(instruction);
        free(line_copy);
        return NULL;
    }
}

Instruction *parse_code_instruction(const char *line, HashMap *labels, int code_count) {
    char *line_copy = strdup(line);
    if (!line_copy) return NULL;

    char *labels_str = NULL;
    char *mnemonic = NULL;
    char *operand1 = NULL;
    char *operand2 = NULL;

    if (strchr(line, ':')) {
        labels_str = strtok(line_copy, ":");
        mnemonic = strtok(NULL, " ");
    } else {
        mnemonic = strtok(line_copy, " ");
    }

    operand1 = strtok(NULL, ",");
    operand2 = strtok(NULL, ",");

    if (strchr(line, ':') && labels_str) {
        int *code_count_ptr = (int *)malloc(sizeof(int));
        *code_count_ptr = code_count;

        // Use labels_str directly as the key
        hashmap_insert(labels, labels_str, code_count_ptr);
    }

    Instruction *instruction = (Instruction *)malloc(sizeof(Instruction));
    if (!instruction) {
        free(line_copy);
        return NULL;
    }

    if (operand2 == NULL) {
        operand2 = ""; 
    }

    instruction->mnemonic = strdup(mnemonic);
    instruction->operand1 = strdup(operand1);
    instruction->operand2 = strdup(operand2);

    free(line_copy);
    return instruction;
}

ParserResult *parse(const char *filename){
    FILE *file= fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }
    ParserResult *result = (ParserResult *)malloc(sizeof(ParserResult));
    if (!result) {
        fclose(file);
        return NULL;
    }
    char line[256];
    result->data_instructions = NULL;
    result->data_count = 0;
    result->code_instructions = NULL;
    result->code_count = 0;
    result->labels = hashmap_create();
    result->memory_locations = hashmap_create();
    if (!result->labels || !result->memory_locations) {
        fclose(file);
        free(result);
        return NULL;
    }
    int in_data_section = 0; // Si dans la section .DATA = 1 is dans la section .CODE = -1

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, ".DATA")) {
            in_data_section = 1;
            continue;
        }
        if (strstr(line, ".CODE")) {
            in_data_section = -1;
            continue;
        }
        if (in_data_section==1){
            printf("Parsing data instruction: %s", line);
            Instruction *instruction = parse_data_instruction(line, result->memory_locations);
            if (instruction) {
                result->data_instructions = realloc(result->data_instructions, sizeof(Instruction *) * (result->data_count + 1));
                result->data_instructions[result->data_count] = instruction;
                result->data_count++;
            }
        }
        else if (in_data_section == -1){
            printf("Parsing code instruction: %s", line);
            Instruction *instruction = parse_code_instruction(line, result->labels, result->code_count);
            if (instruction) {
                result->code_instructions = realloc(result->code_instructions, sizeof(Instruction *) * (result->code_count + 1));
                result->code_instructions[result->code_count] = instruction;
                result->code_count++;
            }
        }

    }
    fclose(file);
    return result;
}

void free_parser_result(ParserResult *result) {
    if (result) {
        for (int i = 0; i < result->data_count; i++) {
            free(result->data_instructions[i]->mnemonic);
            free(result->data_instructions[i]->operand1);
            free(result->data_instructions[i]->operand2);
            free(result->data_instructions[i]);
        }
        for (int i = 0; i < result->code_count; i++) {
            free(result->code_instructions[i]->mnemonic);
            free(result->code_instructions[i]->operand1);
            free(result->code_instructions[i]->operand2);
            free(result->code_instructions[i]);
        }
        hashmap_destroy(result->labels);
        hashmap_destroy(result->memory_locations);
        free(result->data_instructions);
        free(result->code_instructions);
        free(result);
    }
}