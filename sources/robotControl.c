#include "containers.h"

unsigned char gaelle (unsigned char);
int* init_intervals(void);

int* init_intervals(void){
    int* intervals = malloc(3*10*sizeof(int));
    intervals[0] = 1; // last_used (account for basic info on top)
    intervals[1] = 1; // free_index (filled polygons)
    intervals[2] = 9; // max_size (allocated size for filling with obstacles)
    return intervals;
}

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
    int i1, i2;
    int* intervals = init_intervals();
    for (int j=0; j<360; j++){
        for (int i=0; i<640; i++) {
            if (fscanf(stdin, "%hhu", &c) == 1) {  // Seule la position des obstacles compte
                // missing somme operation on c /**** Gaelle ****/
                c = gaelle(c); // 1 if within an obstacle, 0 if not
                if (c==0) continue; 
                i1 = i;
                // not safe construct
                while((i<640) && (fscanf(stdin, "%hhu", &c) == 1) && (gaelle(c)==1)) i++;
                i2 = -1 + i++;  // take into account the eventual last read (no pb if i=640)
                shrink(obstacles, intervals, i1, i2, j);
            }
            else return NULL;
        }
    }
    if (intervals) free(intervals);
    return obstacles;
}

void shrink(Obstacles* obstacles, int *intervals, int i1, int i2, int j){
    bool already_merged = false; int base_index = 0;
    raw_point new1 = {,,NULL}, new2 = {,,NULL};
    int *last_used = &intervals[0], *free_index = &intervals[1], *max_size = &intervals[2];
    for(int i = 1; i < *last_used; i++){
        if (i2 >= intervals[3*i] && i1 <= intervals[3*i+1]) { // adding a point in growing polygon
            if (already_merged){ //merging two polygons (assert strengthened safety)
                merge_polygons(&obstacles->polygons, base_index, i);
                intervals[3*i] = (intervals[3*i + 1] = -1); // free intervals
                *free_index = MIN(*free_index, i);
            }
            already_merged = true;
            if (!base_index) {  // allways add points to the former obstacle
                add_point(&obstacles->polygons[i], new1);
                add_point(&obstacles->polygons[i], new2);
                intervals[3*i] = i1;
                intervals[3*i + 1] = i2; 
                intervals[3*i + 2] = j;
                base_index = i;
            }
        }
        if (j > intervals[3*i+2] + 1){ // terminated growth, since one or multiple steps
            intervals[3*i] = (intervals[3*i+1] = -1);
            *free_index = MIN(*free_index, i);
        }
    }
    if (!already_merged) { // creating a new polygon
        int k;
        insert_interval(intervals, i1, i2, j, &k);
        add_polygon(obstacles, k);
        add_point(&obstacles->polygons[k], new1);
        add_point(&obstacles->polygons[k], new2);
    }
    return;
}

void add_point(Polygon *polygon, Raw_point p){
    if(polygon->size < polygon->maxSize) { polygon->points[polygon->size++] = p; return;}
    else {
        polygon->points = realloc(polygon->points, 2*polygon->maxSize*sizeof(*(polygon->points)));
        assert(polygon->points);
        polygon->maxSize *= 2;
        polygon->points[polygon->size++] = p; 
        return;
}

void add_polygon(Obstacles* obstacles, int k){
    if(obstacles->size >= obstacles->maxSize) {
        obstacles->polygons = realloc(obstacles->polyons, 2*obstacles->maxSize * sizeof(*(obstacle->polygons)));
        assert(obstacles->polygons);
        obstacles->maxSize *= 2;
    }
    if(k < obstacles->size) obstacles->polygons[obstacles->size++] = obstacles->polygons[k];
    insert_polygon(&obstacles->polygon[k], nb_obs_points);
}

void insert_polygon(Polygon *p, int maxSize){
    p->size = 0; p->maxSize = maxSize;
    p->points = malloc(maxSize * sizeof(*points));
}

void init_obstacles(Obstacles *obstacles, int maxSize){
    obstacles = malloc(sizeof(*obstacles));
    obstacles->size = 0; obstacles->maxSize = maxSize;
    obstacles->polygons = malloc(maxSize * sizeof(*(obstacles->polygons)));
}

void insert_interval(int *intervals, int i1, int i2, int j){ 
    int *free_index = &intervals[1], *max_size = &intervals[2];
    if (*free_index >= *max_size) {
        intervals = realloc(intervals, 2 * 3 * (*max_size) *sizeof(int));
        *max_size *= 2;
    }
    intervals[3 * (*free_index)] = i1;
    intervals[3 * (*free_index + 1)] = i2;
    intervals[3 * (*free_index + 2)] = j;
    for(int i = *free_index + 1; i < *max_size; i++) {if (intervals[3*i] == -1) break;}
    *free_index = i;
}

unsigned char gaelle(unsigned char c){
    return c;
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
