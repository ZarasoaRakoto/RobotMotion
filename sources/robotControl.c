#include "containers.h"

void turn(double angle){
    // Send a TURN command to the rover
    fprintf(stdout, "CMD TURN %lf", angle);
    fflush(stdout);
    // Check that rover executed the command
    char check; 
    check = fgetc(stdin); assert (check = 'O');
    check = fgetc(stdin); assert (check = 'K');
}

void forward(double distance){
    // Send a FORWARD command to the rover
    fprintf(stdout, "CMD FORWARD %lf", distance);
    fflush(stdout);
    // Check that rover executed the command
    char check; 
    check = fgetc(stdin); assert (check = 'O');
    check = fgetc(stdin); assert (check = 'K');
}

Image* camera(Image *data){
    // Send a CAMERA command to the rover
    fprintf(stdout, "CMD CAMERA");
    fflush(stdout);
    int verif; char verifs[3];
    // Read Rover response and check image has the format
    assert(fscanf(stdin, "%3s", verifs) == 1 && strcmp(verifs, "OK")==0);
    assert(fscanf(stdin, "%d", &verif) == 1 && verif==640);
    assert(fscanf(stdin, "%d", &verif) == 1 && verif==360);
    assert(fscanf(stdin, "%d", &verif) == 1 && verif==255);
    unsigned char c;
    for(int j=0; j<360; j++){
        for(int i=0; i<640; i++) {
            if(fscanf(stdin, "%hhu", &c) == 1) data->bytes[i + j*640] = c;
            else return NULL;
        }
    }
    fflush(stdin);
    return data;
}

int main(int agrc, char**argv){
    Image* data = malloc(sizeof(*data));
    data = camera(data);
    return 0;
}
