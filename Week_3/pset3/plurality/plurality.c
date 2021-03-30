#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);
int find_candidate_name(string name);
void print_array(void);
void sort_votes(void);
int count_sim(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
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
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    // find candidate name in the candidate[]
    int idx = find_candidate_name(name);
    // printf("%i\n", idx);
    if (idx == -1)
        return false;
    
    // add vote
    candidates[idx].votes++;

    return true;
}   

// Print the winner (or winners) of the election
void print_winner(void)
{
    // print_array();
    sort_votes();
    print_array();
    printf("------------\n");

    // count semilar votes
    int sim_count = count_sim();
    printf("%i\n", sim_count);
    
    // print winner(s)
    for (int i = 0; i <= sim_count; i++)
        printf("%s\n", candidates[i].name);
    
    return;
}

/**** return index of candidate record in the `candidate` list
 or -1 if not found ****/
int find_candidate_name(string name)
{
    for (int i = 0; i <= candidate_count - 1; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
            return i;
    }
    
    return -1;
}

void print_array(void)
{
    for (int i= 0; i <= candidate_count - 1; i++)
        printf("%s %i\n", candidates[i].name, candidates[i].votes);
}

/***** sort candidates by votes value (method: selection sort) *****/
void sort_votes(void)
{
    int i_curr = 0;
    for (int i = i_curr; i <= candidate_count - 1; i++)
    {
        for (int j = i; j <= candidate_count -1; j++)
        {
            if (candidates[j].votes > candidates[i].votes)
            {
                // swap i and j
                candidate tmp = candidates[i];
                candidates[i] = candidates[j];
                candidates[j] = tmp;
                break;
            }
        }

        i_curr++;
    }
}

/***** return count of similar votes from start of candidates array ******/
int count_sim(void)
{
    int i = 0;
    for (i = 0; i < candidate_count - 1; i++)
        if (candidates[i+1].votes < candidates[i].votes)
            break;

    return i;
}