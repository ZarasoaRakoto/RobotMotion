#include "containers.h"
void shrink(Polygon* obstacles, int *intervals, int n_actifs, int i1, int i2, int j);
void add_point(Raw_point *points, Raw_point p);
void add_polygon(Polygon* obstacles, int k);
void insert_interval();

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

Polygon* camera(Polygon *obstacles){ // intervals = [i1, i2, last_j]*
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
    int i1, i2, n_actifs = 0;
    int* intervals = NULL;
    for(int j=0; j<360; j++){
        for(int i=0; i<640; i++) {
            if(fscanf(stdin, "%hhu", &c) == 1) {  // Seule la position des obstacles compte
                if (c==0) continue; // missing somme operation on c /**********
                i1 = i;
                // not safe construct
                while((i<640) && (fscanf(stdin, "%hhu", &c) == 1) && (c==1)) i++; // missing somme operation on c /**********
                i2 = -1 + i++;  // take into account the eventual last read (no pb i i=640)
                shrink(obstacles, intervals, n_actifs, i1, i2, j);
            }
            else return NULL;
        }
    }
    if (intervals) free(intervals);
    return obstacles;
}

void shrink(Obstacles* obstacles, int *intervals, int n_actifs, int i1, int i2, int j){
    bool ok;
    for(int i = 0; i < n_actifs; i++){
        raw_point new1 = {,,NULL}, new2 = {,,NULL};
        if (i2 >= intervals[3*i] && i1 <= intervals[3*i+1]) { // adding a point in growing polygon
            add_point(obstacles->obstacles[i].raw_points, new1);
            add_point(obstacles[i]->obstacles[i].raw_points, new2);
            i++;
            ok = true;
        }
        if (j > intervals[3*i+2] + 1){ // terminated growth
            intervals[3*i] = (intervals[3*i+1] = -1);
            n_actifs--;
        }
    }
    if (!ok) { // creating a new polygon
        int k;
        insert_interval(intervals, i1, i2, j, &k);
        add_polygon(obstacles, k);
        add_point(obstacles->obstacles[k].points, new1);
        add_point(obstacles->obstacles[k].points, new2);
    }
    return;
}

void add_point(Raw_point *points, Raw_point p){
    if(points->size < points->maxSize) { points[++size] = p; return;}
    else {
        points = realloc(points, 2*maxSize*sizeof(*points));
        assert(points);
        points[++size] = p; 
        return;
}

void add_polygon(Polygon* obstacles, int k){
    if(obstacles->size < obstacles->maxSize) {
        obstacles[++size] = obstacles[k];
        obstacles[k] = // initialiser
    }
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
