#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CANDIDATE 9
#define MAX_VOTERS 100

//Each candidate has name, number of votes, and isEliminated boolean
typedef struct {
    string name;
    int votes;
    bool eliminated;
} candidate;

//Assigns the structure with an array
candidate candidates[MAX_CANDIDATE];

int preferences[MAX_VOTERS][MAX_CANDIDATE];

int candidateCount = 0;
int voterCount = 0;

bool vote(int voter, int rank, string name);
void tabulate(void);
int find_min(void);
bool print_winner(void);
bool is_tie(int min);
void eliminate(int min);

//main function that runs the program
int main(int argc, string args[]) {

    //check if there are sufficient argument(s)
    if (argc > 2) {
           candidateCount = argc - 1;
           if (candidateCount < MAX_CANDIDATE) {
               for (int i = 0; i < candidateCount; i++) {
                   candidates[i].name = args[i + 1];
                   candidates[i].votes = 0;
                   candidates[i].eliminated = false;
               }
           } else {
               printf("Candidate cannot exceed maximum candidate!\n");
               return 0;
           }

            //Asks the user how many voters there will be
            voterCount = get_int("How many voters are there: ");
            if ((voterCount < MAX_VOTERS) && (voterCount > 0)) {
                //iterates over number of counts and candidates
                for (int i = 0; i < voterCount; i++) {
                   for (int j = 0; j < candidateCount; j++) {
                        //gets the name and store it in name
                        string name = get_string("Rank %d: ", j + 1);

                        if (!(vote(i, j , name))) {
                            printf("Invalid vote!\n");
                            return 0;
                        }
                    }
                }

                printf("\n");

                //Keeps iterating until a winner is found
                while (true) {
                    tabulate();

                    bool winnerExists = print_winner();

                    if (winnerExists) {
                        break;
                    }

                    int min = find_min();
                    bool tie = is_tie(min);

                    //Check for possible ties
                    if (tie) {
                        for (int i = 0; i < candidateCount; i++) {
                            if (!(candidates[i].eliminated)) {
                                printf("%s\n", candidates[i].name);
                            }
                        }
                        break;
                    }

                    eliminate(min);

                    for (int i = 0; i < candidateCount; i++) {
                        candidates[i].votes = 0;
                    }
                }
                return 0;
            } else {
               printf("Voters cannot exceed maximum voters!\n");
               return 0;
           }
    } else {
        printf("Argumentation fault!\n");
        return 0;
    }
    return 0;
}

bool vote(int voter, int rank, string name) {

    bool exists = false;
    for (int i = 0; i < candidateCount; i++) {
        //if such name exists, then update the preference
        if (strcmp(name, candidates[i].name) == 0) {
            preferences[voter][rank] = i;
            exists = true;
            break;
        }
    }
    return exists;
}

void tabulate(void) {

    for (int i = 0; i < voterCount; i++) {
        //rank 1 is 0, rank 2 is 1 and rank 3 is 2
        for (int j = 0; j < candidateCount; j++) {
            if (!candidates[preferences[i][j]].eliminated) {
                candidates[preferences[i][j]].votes++;
                //after the vote is updated, break from iteration
                break;
            }
        }
    }
}

int find_min(void) {

    int min = voterCount;
    for (int i = 0; i < candidateCount; i++) {
        //check for a candidate with the least min votes and is still running
        if ((!candidates[i].eliminated) && (candidates[i].votes < min)) {
            //updates the minimum vote if the candidate after found
            min = candidates[i].votes;
        }
    }

    return min;
}

bool print_winner(void) {

    for (int i = 0; i < candidateCount; i++) {
        if (candidates[i].votes > voterCount / 2) {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

bool is_tie(int min) {

    for (int i = 0; i < candidateCount; i++) {
        if ((!candidates[i].eliminated) && (!(candidates[i].votes == 0))) {
            return false;
        }
    }
    return true;
}

void eliminate(int min) {

    for (int i = 0; i < candidateCount; i++) {
        if (candidates[i].votes == min) {
            candidates[i].eliminated = true;
        }
    }
    return;
}






