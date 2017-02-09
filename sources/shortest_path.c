#include "containers.h"

bool forward_label_correcting(Raw_point *xi, Raw_point *xg){
    PQ* prior_queue = PQinit(10, false);
    xi.key = 0; // all other keys are std::DBL_MAX
    xi->prev = NULL;
    double next_dist;
    Raw_point *temp, *guess;
    assert(PQinstert(prior_queue, xi));
    while(!PQEmpty(prior_queue)){
        temp = PQdelmax(prior_queue);
        guess = temp->next; // leftmost son
        while(guess){
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
            guess = guess->right; // accessing all sons of the temporary node
        } 
    }
    // reconstructing the path
    assert(guess == xg);
    while(guess->prev){
        guess->prev->next = guess;
        guess = guess->prev;
    }

}



