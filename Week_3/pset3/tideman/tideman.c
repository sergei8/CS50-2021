#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
int get_candidate_position(string name);
void update_rank(int rank, int ranks[]);
int find_source(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // validate correct candidate name
    int pos = get_candidate_position(name);
    if (pos == -1)
        return false;
    
    // update candidate rank
    ranks[rank] = pos;

    return true;
}

// find  candidate position (-1 if incorrect candidate name)
int get_candidate_position(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
            return i;
    }
    return -1;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
        for (int j = 0; j < candidate_count; j++)
            if (i < j)
                preferences[ranks[i]][ranks[j]]++;

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int margin[pair_count];
    
    // calculate voters margins for pairs
    for (int i = 0; i < pair_count; i++)
            margin[i] = preferences[pairs[i].winner][pairs[i].loser] -
                        preferences[pairs[i].loser][pairs[i].winner];


    // sort margins and pairs
    int i_curr = 0;
    for (int i = i_curr; i < pair_count; i++)
    {
        for(int j = 0; j < pair_count; j++)
        {
            if (margin[j] < margin[i])
            {
                // swap margin & pairs
                int tmp = margin[i];    pair tmp_pairs = pairs[i];
                margin[i] = margin[j];  pairs[i] = pairs[j];
                margin[j] = tmp;        pairs[j] = tmp_pairs;  
                break;
            }
        }
        i_curr++;
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // find for source
    int source = find_source();

    // if source found - lock all pairs
    if (source >= 0)
        for (int i = 0; i < pair_count; i++)
            locked[pairs[i].winner][pairs[i].loser] = true;

    // if not - unlock one last
    else
        for (int i = 0; i < pair_count - 1; i++)
            locked[pairs[i].winner][pairs[i].loser] = true;
    
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // find source of the graph - column with all `falses`
    for (int j = 0; j < candidate_count; j++)
    {
        bool sum_j = false;
        for (int i = 0; i < candidate_count; i++)
            sum_j = sum_j || locked[i][j];
        if (!sum_j)
            printf("\n%s", candidates[j]);
    }
    
    return;
}

// find source of graph (index - yes, -1 - no: cycle)
int find_source(void)
{
    for (int j = 0; j < candidate_count; j++)
    {
        bool sum_j = false;
        for (int i = 0; i < candidate_count; i++)
            sum_j = sum_j || locked[i][j];
        if (!sum_j)
            return j;
    }
    return -1;
} 