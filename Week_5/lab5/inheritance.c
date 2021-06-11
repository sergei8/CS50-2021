    // Simulate genetic inheritance of blood type

    #include <stdbool.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h>

    // Each person has two parents and two alleles
    typedef struct person
    {
        struct person *parents[2];
        char alleles[2];
    }
    person;

    const int GENERATIONS = 3;
    const int INDENT_LENGTH = 4;

    person *create_family(int generations);
    void print_family(person *p, int generation);
    void free_family(person *p);
    char random_allele();
    char get_random_allele(char allele_1, char allele_2);

    int main(void)
    {
        // Seed random number generator
        srand(time(0));

        // Create a new family with three generations
        person *p = create_family(GENERATIONS);

        // Print family tree of blood types
        print_family(p, 0);

        // Free memory
        free_family(p);
    }

    // Create a new individual with `generations`
    person *create_family(int generations)
    {
        // Allocate memory for new person
        person *p = (person*) malloc(sizeof(person));

        // Generation with parent data
        if (generations > 1)
        {
            // Recursively create blood type histories for parents
            generations--;
            p->parents[0] = create_family(generations);
            p->parents[1] = create_family(generations);

            p->alleles[0] = get_random_allele(p->parents[0]->alleles[0], p->parents[0]->alleles[1]);
            p->alleles[1] = get_random_allele(p->parents[1]->alleles[0], p->parents[1]->alleles[1]);
        }

        // Generation without parent data
        else
        {
            // Set parent pointers to NULL
            p->parents[0] = NULL;
            p->parents[1] = NULL;

            // Randomly assign alleles
            p->alleles[0] = random_allele();
            p->alleles[1] = random_allele();
        }

        // Return newly created person
        return p;
    }

    char get_random_allele(char allele_1, char allele_2)
    {
        double r;
        
        r = (double) (rand() % 10001) / 10000.0;
        if (r > 0.5)
            return allele_1;
        else
            return allele_2;
    }

    // Free `p` and all ancestors of `p`.
    void free_family(person *p)
    {
        // TODO: Handle base case
        free(p->parents[0]->parents[0]);
        free(p->parents[0]->parents[1]);
        free(p->parents[1]->parents[0]);
        free(p->parents[1]->parents[1]);

        // TODO: Free parents
        free(p->parents[0]);
        free(p->parents[1]);

        // TODO: Free child
        free(p);
    }

    // Print each family member and their alleles.
    void print_family(person *p, int generation)
    {
        // Handle base case
        if (p == NULL)
        {
            return;
        }

        // Print indentation
        for (int i = 0; i < generation * INDENT_LENGTH; i++)
        {
            printf(" ");
        }

        // Print person
        printf("Generation %i, blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
        print_family(p->parents[0], generation + 1);
        print_family(p->parents[1], generation + 1);
    }

    // Randomly chooses a blood type allele.
    char random_allele()
    {
        int r = rand() % 3;
        if (r == 0)
        {
            return 'A';
        }
        else if (r == 1)
        {
            return 'B';
        }
        else
        {
            return 'O';
        }
    }
