#include "containers.h"

void turn(double angle){
    // Send a TURN command to the rover
    fprintf(stdout, "CMD TURN %lf\n", angle);
    fflush(stdout);
    // Check that rover executed the command
    char check[3]; check[2] = '\0';
    assert(fscanf(stdin, "%2s", check) == 1 && strcmp(check, "OK")==0);
}

void forward(double distance){
    // Send a FORWARD command to the rover
    fprintf(stdout, "CMD FORWARD %lf\n", distance);
    fflush(stdout);
    // Check that rover executed the command
    char check[3]; check[2] = '\0';
    assert(fscanf(stdin, "%2s", check) == 1 && strcmp(check, "OK")==0);
}

Image* camera(Image *data){
    // Send a CAMERA command to the rover
    fprintf(stdout, "CMD CAMERA\n");
    fflush(stdout);
    int verif; char check[3]; check[2] = '\0';
    // Read Rover response and check image has the format
    assert(fscanf(stdin, "%2s", check) == 1 && strcmp(check, "P2")==0);
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
    return data;
}

int main(int agrc, char**argv){
    Image* data = malloc(sizeof(*data));
    data = camera(data);
    int sum = 0;
    for(int i=0; i<640*360; i++) {
        if (data->bytes[i] != 0) sum++;
    }
    fprintf(stderr, "%lf\n", (double)sum/(340*600));
    free(data);
    return 0;
}
