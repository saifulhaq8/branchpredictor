#include "ooo_cpu.h"

#define Tag uint16_t
#define Index uint16_t
#define Path uint64_t
#define History uint64_t
#define USEFUL_BITS_TAGE 2
#define GLOBAL_HISTORY_BUFFER_LENGTH_TAGE 1024
#define PATH_HISTORY_BUFFER_LENGTH_TAGE 32
#define MIN_HISTORY_LENGTH_TAGE 4
#define HISTORY_ALPHA_TAGE 1.6
#define TAGE_RESET_USEFUL_INTERVAL 512000
#define BIMODAL_TABLE_SIZE_TAGE 16384
#define MAX_INDEX_BITS_TAGE 12
#define TAG_NUM_COMPONENTS 12 // TODO
#define BASE_COUNTER_BITS_TAGE 2
#define COUNTER_BITS_TAGE 3


const uint8_t TAG_BITS_TAGE[TAG_NUM_COMPONENTS] = {7, 7, 8, 8, 9, 10, 11, 12, 12, 13, 14, 15};
const uint8_t INDEX_BITS_TAGE[TAG_NUM_COMPONENTS] = {10, 10, 11, 11, 11, 11, 10, 10, 10, 10, 9, 9};

struct predictor_table_entry_tage
{
    Tag tag; // Stores the tag
    uint8_t useful; // Variable to store the usefulness of the entry Range - 0-3
    uint8_t ctr; // The counter on which prediction is based Range - 0-7
 };

class Tage
{
private:
    /* data */

    uint8_t usealtonna; // 4 bit counter to decide between alternate and provider component prediction
    int history_lengths_component[TAG_NUM_COMPONENTS]; // History lengths used to compute hashes for different components
    uint8_t tage_pred, pred, alt_pred; // Final prediction , provider prediction, and alternate prediction
    int pred_comp, alt_comp; // Provider and alternate component of last branch PC
    int STRONG; //Strength of provider prediction counter of last branch PC

    int number_of_branches; // Stores the number of branch instructions since the last useful reset
    uint8_t bimodal_table[BIMODAL_TABLE_SIZE_TAGE]; // Array represent the counters of the bimodal table
    struct predictor_table_entry_tage predictor_table[TAG_NUM_COMPONENTS][(1 << MAX_INDEX_BITS_TAGE)];
    uint8_t global_history[GLOBAL_HISTORY_BUFFER_LENGTH_TAGE]; // Stores the global branch history
    uint8_t path_history[PATH_HISTORY_BUFFER_LENGTH_TAGE]; // Stores the last bits of the last N branch PCs

public:
    void init();  // initialise the member variables
    uint8_t predict(uint64_t ip);  // return the prediction from tage
    void update(uint64_t ip, uint8_t taken);  // updates the state of tage

    void update_ctrl(uint8_t &ctr, int cond, int low, int high);   // counter update helper function (including clipping)
    uint8_t find_prediction(uint64_t ip, int comp);   // helper function for prediction
    Path find_hash_path_history(int component);   // hepoer hash function to compress the path history
    History find_global_history_compressed(int inSize, int outSize); // Compress global history of last 'inSize' branches into 'outSize' by wrapping the history
    Index index_of_bimodal(uint64_t ip);   // helper hash function to index into the bimodal table
    Index index_of_predictor(uint64_t ip, int component);   // helper hash function to index into the predictor table using histories
    Tag find_tag(uint64_t ip, int component);   // helper hash function to get the tag of particular ip and component
    int find_below_n_match(uint64_t ip, int component);   // helper function to find the hit component strictly before the component argument

    Tage();
    ~Tage();
};

uint8_t Tage::find_prediction(uint64_t ip, int comp)
{
    /*
    Get the prediction according to a specific component 
    */
    if(comp == 0) // Check if component is the bimodal table
    {
        Index index = index_of_bimodal(ip); // Get bimodal index
        return bimodal_table[index] >= (1 << (BASE_COUNTER_BITS_TAGE - 1));
    }
    else
    {
        Index index = index_of_predictor(ip, comp); // Get component-specific index
        return predictor_table[comp - 1][index].ctr >= (1 << (COUNTER_BITS_TAGE - 1));
    }
}

void Tage::init()
{
    /*
    Initializes the member variables
    */
    usealtonna = 8;
    tage_pred = 0;
    for (int i = 0; i < BIMODAL_TABLE_SIZE_TAGE; i++)
    {
        bimodal_table[i] = (1 << (BASE_COUNTER_BITS_TAGE - 1)); // weakly taken
    }
    for (int i = 0; i < TAG_NUM_COMPONENTS; i++)
    {
        for (int j = 0; j < (1 << INDEX_BITS_TAGE[i]); j++)
        {
            predictor_table[i][j].ctr = (1 << (COUNTER_BITS_TAGE - 1)); // weakly taken
            predictor_table[i][j].useful = 0;                           // not useful
            predictor_table[i][j].tag = 0;
        }
    }

    double power = 1;
    for (int i = 0; i < TAG_NUM_COMPONENTS; i++)
    {
        history_lengths_component[i] = int(MIN_HISTORY_LENGTH_TAGE * power + 0.5); // set component history lengths
        power *= HISTORY_ALPHA_TAGE;
    }

    number_of_branches = 0;
}



void Tage::update_ctrl(uint8_t &ctr, int cond, int low, int high)
{
    /*
    Function to update bounded counters according to some condition
    */
    if(cond && ctr < high)
        ctr++;
    else if(!cond && ctr > low)
        ctr--;
}

uint8_t Tage::predict(uint64_t ip)
{
    pred_comp = find_below_n_match(ip, TAG_NUM_COMPONENTS + 1); // Get the first predictor from the end which matches the PC
    alt_comp = find_below_n_match(ip, pred_comp); // Get the first predictor below the provider which matches the PC 

    //Store predictions for both components for use in the update step
    pred = find_prediction(ip, pred_comp); 
    alt_pred = find_prediction(ip, alt_comp);

    if(pred_comp == 0)
        tage_pred = pred;
    else
    {
        Index index = index_of_predictor(ip, pred_comp);
        STRONG = abs(2 * predictor_table[pred_comp - 1][index].ctr + 1 - (1 << COUNTER_BITS_TAGE)) > 1;
        if (usealtonna < 8 || STRONG) // Use provider component only if usealtonna < 8 or the provider counter is strong
            tage_pred = pred;
        else
            tage_pred = alt_pred;
    }
    return tage_pred;
}



void Tage::update(uint64_t ip, uint8_t taken)
{
    /*
    function to update the state (member variables) of the tage class
    */
    if (pred_comp > 0)  // the predictor component is not the bimodal table
    {
        struct predictor_table_entry_tage *entry = &predictor_table[pred_comp - 1][index_of_predictor(ip, pred_comp)];
        uint8_t useful = entry->useful;

        if(!STRONG)
        {
            if (pred != alt_pred)
                update_ctrl(usealtonna, !(pred == taken), 0, 15);
        }

        if(alt_comp > 0)  // alternate component is not the bimodal table
        {
            struct predictor_table_entry_tage *alt_entry = &predictor_table[alt_comp - 1][index_of_predictor(ip, alt_comp)];
            if(useful == 0)
                update_ctrl(alt_entry->ctr, taken, 0, ((1 << COUNTER_BITS_TAGE) - 1)); // update ctr for alternate predictor if useful for predictor is 0
        }
        else
        {
            Index index = index_of_bimodal(ip);
            if (useful == 0)
                update_ctrl(bimodal_table[index], taken, 0, ((1 << BASE_COUNTER_BITS_TAGE) - 1));  // update ctr for alternate predictor if useful for predictor is 0
        }

        // update u
        if (pred != alt_pred)
        {
            if (pred == taken)
            {
                if (entry->useful < ((1 << USEFUL_BITS_TAGE) - 1))
                    entry->useful++;  // if prediction from preditor component was correct
            }
            else
            {
                if(usealtonna < 8)
                {
                    if (entry->useful > 0)
                        entry->useful--;  // if prediction from altpred component was correct
                } 
            }
        }

        update_ctrl(entry->ctr, taken, 0, ((1 << COUNTER_BITS_TAGE) - 1));  // update ctr for predictor component
    }
    else
    {
        Index index = index_of_bimodal(ip);
        update_ctrl(bimodal_table[index], taken, 0, ((1 << BASE_COUNTER_BITS_TAGE) - 1));  // update ctr for predictor if predictor is bimodal
    }

    // allocate tagged entries on misprediction
    if (tage_pred != taken)
    {
        long rand = random();
        rand = rand & ((1 << (TAG_NUM_COMPONENTS - pred_comp - 1)) - 1);  
        int start_component = pred_comp + 1;

        //compute the start-component for search
        if (rand & 1)  // 0.5 probability
        {
            start_component++;
            if (rand & 2)  // 0.25 probability
                start_component++;
        }

        //Allocate atleast one entry if no free entry
        int isFree = 0;
        for (int i = pred_comp + 1; i <= TAG_NUM_COMPONENTS; i++)
        {
            struct predictor_table_entry_tage *entry_new = &predictor_table[i - 1][index_of_predictor(ip, i)];
            if (entry_new->useful == 0)
                isFree = 1;
        }
        if (!isFree && start_component <= TAG_NUM_COMPONENTS)
            predictor_table[start_component - 1][index_of_predictor(ip, start_component)].useful = 0;
        
        
        // search for entry to steal from the start-component till end
        for (int i = start_component; i <= TAG_NUM_COMPONENTS; i++)
        {
            struct predictor_table_entry_tage *entry_new = &predictor_table[i - 1][index_of_predictor(ip, i)];
            if (entry_new->useful == 0)
            {
                entry_new->tag = find_tag(ip, i);
                entry_new->ctr = (1 << (COUNTER_BITS_TAGE - 1));
                break;
            }
        }
    }

    // update global history
    for (int i = GLOBAL_HISTORY_BUFFER_LENGTH_TAGE - 1; i > 0; i--)
        global_history[i] = global_history[i - 1];
    global_history[0] = taken;

    // update path history
    for (int i = PATH_HISTORY_BUFFER_LENGTH_TAGE - 1; i > 0; i--)
        path_history[i] = path_history[i - 1];
    path_history[0] = ip & 1;
    
    // graceful resetting of useful counter
    number_of_branches++;
    if (number_of_branches % TAGE_RESET_USEFUL_INTERVAL == 0)
    {
        number_of_branches = 0;
        for (int i = 0; i < TAG_NUM_COMPONENTS; i++)
        {
            for (int j = 0; j < (1 << INDEX_BITS_TAGE[i]); j++)
                predictor_table[i][j].useful >>= 1;
        }
    }
}


Path Tage::find_hash_path_history(int component)
{
    /*
    Use a hash-function to compress the path history
    */
    Path A = 0;
    
    int size = history_lengths_component[component - 1] > 16 ? 16 : history_lengths_component[component-1]; // Size of hash output
    for (int i = PATH_HISTORY_BUFFER_LENGTH_TAGE - 1; i >= 0; i--)
    {
        A = (A << 1) | path_history[i]; // Build the bit vector a using the path history array
    }

    A = A & ((1 << size) - 1);
    Path A1;
    Path A2;
    A1 = A & ((1 << INDEX_BITS_TAGE[component - 1]) - 1); // Get last M bits of A
    A2 = A >> INDEX_BITS_TAGE[component - 1]; // Get second last M bits of A

    // Use the hashing from the CBP-4 L-Tage submission
    A2 = ((A2 << component) & ((1 << INDEX_BITS_TAGE[component - 1]) - 1)) + (A2 >> abs(INDEX_BITS_TAGE[component - 1] - component));
    A = A1 ^ A2;
    A = ((A << component) & ((1 << INDEX_BITS_TAGE[component - 1]) - 1)) + (A >> abs(INDEX_BITS_TAGE[component - 1] - component));
    
    return (A);
}

Index Tage::index_of_bimodal(uint64_t ip)
{
    /*
    Return index of the PC in the bimodal table using the last K bits
    */
    return ip & (BIMODAL_TABLE_SIZE_TAGE - 1);
}

Index Tage::index_of_predictor(uint64_t ip, int component)
{
    /*
    Get index of PC in a particular predictor component
    */
    Path path_history_hash = find_hash_path_history(component); // Hash of path history

    // Hash of global history
    History global_history_hash = find_global_history_compressed(history_lengths_component[component - 1], INDEX_BITS_TAGE[component - 1]);

    return (global_history_hash ^ ip ^ (ip >> (abs(INDEX_BITS_TAGE[component - 1] - component) + 1)) ^ path_history_hash) & ((1 << INDEX_BITS_TAGE[component-1]) - 1);
}

History Tage::find_global_history_compressed(int inSize, int outSize)
{
    /*
    Compress global history of last 'inSize' branches into 'outSize' by wrapping the history
    */
    History compressed_history = 0; // Stores final compressed history
    History temporary_history = 0; // Temorarily stores some bits of history
    int compressed_history_length = outSize;
    for (int i = 0; i < inSize; i++)
    {
        if (i % compressed_history_length == 0)
        {
            compressed_history ^= temporary_history; // XOR current segment into the compressed history
            temporary_history = 0;
        }
        temporary_history = (temporary_history << 1) | global_history[i]; // Build history bit vector
    }
    compressed_history ^= temporary_history;
    return compressed_history;
}


int Tage::find_below_n_match(uint64_t ip, int component)
{
    /*
    Get component number of first predictor which has an entry for the IP below a specfic component number
    */
    for (int i = component - 1; i >= 1; i--)
    {
        Index index = index_of_predictor(ip, i);
        Tag tag = find_tag(ip, i);

        if (predictor_table[i - 1][index].tag == tag) // Compare tags at a specific index
        {
            return i;
        }
    }

    return 0; // Default to bimodal in case no match found
}


Tag Tage::find_tag(uint64_t ip, int component)
{
    /*
    Get tag of a PC for a particular predictor component
    */
    History global_history_hash = find_global_history_compressed(history_lengths_component[component - 1], TAG_BITS_TAGE[component - 1]);
    global_history_hash ^= find_global_history_compressed(history_lengths_component[component - 1], TAG_BITS_TAGE[component - 1] - 1);
    
    return (global_history_hash ^ ip) & ((1 << TAG_BITS_TAGE[component - 1]) - 1);
}


Tage::Tage()
{
}

Tage::~Tage()
{
}