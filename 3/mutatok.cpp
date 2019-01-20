#include <iostream>

int main() {
    char a = 'a';
    char* char_ptr = &a; // karakterre mutato pointer
    int int_array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0}; // 10 es int tomb
    int (&ref_int_array)[10]= int_array; // referencia egy 10 es int tombre
    char char_string_array[][6]={"alpha","beta","gamma"};
    char (*char_string_array_ptr)[6] = char_string_array;// karakterlancokra allo tombre mutato pointer

    char** char_ptr_ptr=&char_ptr; // karakter pointerre pointer
    const int const_int=99; // constans int
    const int* const_int_ptr=&const_int;//const intre mutato pointer
    int integer;
    int *const const_ptr_to_int = &integer; // const pointer egy intre
}    
