#include "containers.h"

unsigned char gaelle (unsigned char);
int* init_intervals(void);
void insert_interval(int *intervals, int i1, int i2, int j, int *k);
void shrink(Obstacles* obstacles, int *intervals, int i1, int i2, int j);
void add_point(Polygon *polygon, Raw_point p);
void add_polygon(Obstacles* obstacles, int k);
void insert_polygon(Polygon *p, int maxSize);
Obstacles* init_obstacles(int maxSize);
void merge_polygons(Polygon* polygons, int base_index, int merged_index);
void forward_label_correcting(Raw_point *xi, Raw_point *xg);


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

Obstacles* camera(Obstacles *obstacles){ // intervals = [i1, i2, last_j]*
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
    Raw_point new1 = {0,0,DBL_MAX, NULL, NULL, NULL}, new2 = {0,0,DBL_MAX, NULL, NULL, NULL};
    int *last_used = &intervals[0], *free_index = &intervals[1];
    for(int i = 1; i < *last_used; i++){
        if (i2 >= intervals[3*i] && i1 <= intervals[3*i+1]) { // adding a point in growing polygon
            if (already_merged){ //merging two polygons (assert strengthened safety)
                merge_polygons(obstacles->polygons, base_index, i);
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
    }
}

void add_polygon(Obstacles* obstacles, int k){
    if(obstacles->size >= obstacles->maxSize) {
        obstacles->polygons = realloc(obstacles->polygons, 2*obstacles->maxSize * sizeof(*(obstacles->polygons)));
        assert(obstacles->polygons);
        obstacles->maxSize *= 2;
    }
    if(k < obstacles->size) obstacles->polygons[obstacles->size++] = obstacles->polygons[k];
    insert_polygon(&obstacles->polygons[k], nb_obs_points);
}

void insert_polygon(Polygon *p, int maxSize){
    p->size = 0; p->maxSize = maxSize;
    p->points = malloc(maxSize * sizeof(*(p->points)));
}

Obstacles* init_obstacles(int maxSize){
    Obstacles *obstacles = malloc(sizeof(*obstacles));
    assert(obstacles);
    obstacles->size = 0; obstacles->maxSize = maxSize;
    obstacles->polygons = malloc(maxSize * sizeof(*(obstacles->polygons)));
    assert(obstacles->polygons);
    return obstacles;
}

void insert_interval(int *intervals, int i1, int i2, int j, int *k){ 
    int *free_index = &intervals[1], *max_size = &intervals[2];
    if (*free_index >= *max_size) {
        intervals = realloc(intervals, 2 * 3 * (*max_size) *sizeof(int));
        *max_size *= 2;
    }
    *k = *free_index;
    intervals[3 * (*k)] = i1;
    intervals[3 * (*k) + 1] = i2;
    intervals[3 * (*k) + 2] = j;
    int i;
    for(i = *k + 1; i < *max_size; i++) {if (intervals[3*i] == -1) break;}
    *free_index = i;
}

void merge_polygons(Polygon* polygons, int base_index, int merged_index){
    return;
}

unsigned char gaelle(unsigned char c){
    return c;
}

void forward_label_correcting(Raw_point *xi, Raw_point *xg){
    // implementation of Dijkstra algorithm
    PQ* prior_queue = PQinit(10, false);
    xi.key = 0; // all other keys are std::DBL_MAX
    xi->prev = NULL;
    double next_dist;
    Raw_point *temp, *guess; AdjListNode *active_node;
    assert(PQinstert(prior_queue, xi));
    while(!PQEmpty(prior_queue)){
        temp = PQdelmax(prior_queue);
        active_node = temp->adjlist->head; // parcours de la liste d adjacence
        while(active_node){
            guess = active_node->item;
            next_dist = sqrt(pow(temp->x - guess->x, 2) + pow(temp->y - guess->y, 2));
            if (temp.key + next_dist < MIN(guess->key, xg->key)){
                guess->key = temp->key + next_dist;
                guess->prev = temp;
                if (guess != xg) assert(PQinstert(prior_queue, guess));
                else {
                    prior_queue->nElems = 0; // artificially emptying the queue
                    break;
                }
            }
            active_node = active_node->next; // accessing all sons of the temporary node
        } 
    }
    // reconstructing the path
    assert(guess == xg);
    while(guess->prev){
        list_delete(guess->prev->adjlist->head);
        list_insert(guess->prev->adjlist, guess);
        guess = guess->prev;
    }
}


int main(int agrc, char**argv){
    Obstacles * obs = init_obstacles(10);
    obs = camera(obs);
    printf("nombre d'obstacles : %d \n", obs->size);
    return 0;
}

