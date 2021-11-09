#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//the file names shouldn't be longer than this
#define LENGTH 20

FILE *input_file;
FILE *output_file;
char input_name[LENGTH], output_name[LENGTH], pass[2], line[120];
int letter, i, j;
char hexaletter[2];

int encrypt(char *argv[], int n){
    
    //makes input_name what the user defines and opens that file
    strcpy(input_name, argv[n]);
    input_file = fopen(input_name, "r");
    if (input_file == NULL){
        printf("Can't open file");
        exit(1);
    }
    puts(input_name);

    //takes everything before the '.' in input_name and adds it to output_name with ".crp" at the end
    //this means that if input_file is not a text file or is undfined this will still work
    //also opens the output_file
    for(i=0; i<strlen(input_name); i++){
        pass[0] = input_name[i];
        if(input_name[i] != '.') strcat(output_name, pass);
        else break;
    }
    strcat(output_name, ".crp");
    puts(output_name);
    output_file = fopen(output_name, "w");

    //runs untill the end of input_file is reached
    while(!feof(input_file)){
        //sets line to the next line in input_file (never more than 120 characters)
        fgets(line, 120, input_file);
        puts(line);
        
        //for every char in the line
        for(i=0; i<120; i++){
            
            letter = line[i];
            //if letter is a line break, add a line break to output_file and skips to the next line
            if(letter == '\n'){
                fputc('\n', output_file);

                break;
            }
            //if letter is a tab, adds "TT" to output_file
            else if(letter == 9) fputs("TT", output_file);
            //else adds letter in hexadecimal to output_file
            else {
                letter -= 16;
                if(letter < 32) letter = (letter - 32) +144;
                fprintf(output_file, "%x", letter);
            }
        }
        
    }
    if (feof(input_file)) printf("\nEnd of file reached.");
    else printf("\nSomething went wrong.");

    return(0);
}

int decrypt(char *argv[], int n){
    
    //makes input_name what the user defines and opens that file
    strcpy(input_name, argv[n]);
    input_file = fopen(input_name, "r");
    if (input_file == NULL){
        printf("Can't open file");
        exit(1);
    }
    puts(input_name);

    //takes everything before the '.' in input_name and adds it to output_name with ".txt" at the end
    //this means that if input_file is not a text file or is undfined this will still work
    //also opens the output_file
    for(i=0; i<strlen(input_name); i++){
        pass[0] = input_name[i];
        if(input_name[i] != '.') strcat(output_name, pass);
        else break;
    }
    strcat(output_name, ".txt");
    puts(output_name);
    output_file = fopen(output_name, "w");

    //runs untill the end of input_file is reached
    while(!feof(input_file)){
        //sets line to the next line in input_file (never more than 120 characters)
        fgets(line, 255, input_file);
        puts(line);
        
        //for every char in the line
        for(i=0; i<255; i=i+2){
            hexaletter[0] = line[i];
            hexaletter[1] = line[i+1];
            //if hexaletter is a line break, add a line break to output_file and skips to the next line
            if(hexaletter[0] == '\n'){
                fputc('\n', output_file);
                break;
            }
            //if hexaletter is "TT", adds a tab to output_file
            else if(hexaletter[0] == 'T' && hexaletter[1] == 'T') fprintf(output_file, "\t");
            //else converts hexaletter to in letter and prints letter as char to output_file
            else {
                sscanf(hexaletter, "%x", &letter);
                
                letter += 16;
                if(letter > 127) letter = (letter - 144) + 32;
                fprintf(output_file, "%c", letter);
            }
        }
    }
    if (feof(input_file)) printf("\nEnd of file reached.");
    else printf("\nSomething went wrong.");

    return(0);
}

int main(int argc, char *argv[]){

    //this will encrypt with the file in argv[2], since argv[1] is -encrypt
    if(strcmp(argv[1],"-encrypt") == 0){printf("encrypt\n"); encrypt(argv, 2);}

    //this will decrypt with the file in argv[2], since argv[1] is -decrypt
    else if(strcmp(argv[1],"-decrypt") == 0){
        printf("decrypt\n");
        decrypt(argv, 2);
    }

    //this will encrypt with the file in argv[1], since there is no -encrypt or -decrypt
    else{printf("default\n"); encrypt(argv, 1);}

    fclose(input_file);
    fclose(output_file);
    return(0);
}