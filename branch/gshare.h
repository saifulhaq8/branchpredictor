#include "ooo_cpu.h"

#define GLOBAL_HISTORY_LENGTH 14
#define GLOBAL_HISTORY_MASK (1 << GLOBAL_HISTORY_LENGTH) - 1
#define GS_HISTORY_TABLE_SIZE 16384


class Gshare
{
private:
    /* data */
    int branch_history_vector;
    int gs_history_table[GS_HISTORY_TABLE_SIZE];
    int my_last_prediction;

public:
    void init();  // initialise the member variables
    uint8_t predict(uint64_t ip);  // return the prediction from tage
    void update(uint64_t ip, uint8_t taken);  // updates the state of tage
    unsigned int gs_table_hash(uint64_t ip, int bh_vector);

    Gshare();
    ~Gshare();
};

void Gshare::init()
{
    //cout << "CPU " << cpu << " GSHARE branch predictor" << endl;

    branch_history_vector = 0;
    my_last_prediction = 0;

    for(int i=0; i<GS_HISTORY_TABLE_SIZE; i++)
        gs_history_table[i] = 2; // 2 is slightly taken
}


uint8_t Gshare::predict(uint64_t ip)
{
    int prediction = 1;

    int gs_hash = gs_table_hash(ip, branch_history_vector);

    if(gs_history_table[gs_hash] >= 2)
        prediction = 1;
    else
        prediction = 0;

    my_last_prediction = prediction;

    return prediction;
}


void Gshare::update(uint64_t ip, uint8_t taken)
{
    int gs_hash = gs_table_hash(ip, branch_history_vector);

    if(taken == 1) {
        if(gs_history_table[gs_hash] < 3)
            gs_history_table[gs_hash]++;
    } else {
        if(gs_history_table[gs_hash] > 0)
            gs_history_table[gs_hash]--;
    }

    // update branch history vector
    branch_history_vector <<= 1;
    branch_history_vector &= GLOBAL_HISTORY_MASK;
    branch_history_vector |= taken;

}

unsigned int Gshare::gs_table_hash(uint64_t ip, int bh_vector)
{
    unsigned int hash = ip^(ip>>GLOBAL_HISTORY_LENGTH)^(ip>>(GLOBAL_HISTORY_LENGTH*2))^bh_vector;
    hash = hash%GS_HISTORY_TABLE_SIZE;

    //printf("%d\n", hash);

    return hash;
}


Gshare::Gshare()
{
}

Gshare::~Gshare()
{
}