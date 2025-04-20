#include "parser.h"

int main(){
    ParserResult* parser= parse("test.txt");
    free_parser_result(parser);
    return 0;
}