// Copyright @lucabotez

#include <bits/stdc++.h>

using namespace std;

#define ALPHABET_SIZE 26

// number of DFA states
int num_of_states;

// DFA transition matrix
vector<vector<int>> transitions(1000, vector<int>(ALPHABET_SIZE, 0));

// trie code taken from https://www.geeksforgeeks.org/trie-insert-and-search/
struct TrieNode {

    // pointer array for child nodes of each node
    TrieNode* child[ALPHABET_SIZE];

    // Used for indicating ending of string
    bool wordEnd;

    // adding two flags that indicate if the prefix forms words that are
    // accepted or rejected
    int accept_flag = 0, reject_flag = 0;

    TrieNode()
    {
        // constructor
        // initialize the wordEnd variable with false
        // initialize every index of childNode array with
        // NULL
        wordEnd = false;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            child[i] = NULL;
        }
    }
};

// trie code taken from https://www.geeksforgeeks.org/trie-insert-and-search/
void insertKey(TrieNode* root, const string& key, int accepted, int rejected) {
  
    // Initialize the curr pointer with the root node
    TrieNode* curr = root;

    // if the node is the root we know that it has both accepted and rejected
    // words
    root->accept_flag = 1;
    root->reject_flag = 1;

    // Iterate across the length of the string
    for (char c : key) {
      
        // Check if the node exists for the 
        // current character in the Trie
        if (curr->child[c - 'a'] == nullptr) {
          
            // If node for current character does 
            // not exist then make a new node
            TrieNode* newNode = new TrieNode();
          
            // Keep the reference for the newly
            // created node
            curr->child[c - 'a'] = newNode;
        }
      
        // Move the curr pointer to the
        // newly created node
        curr = curr->child[c - 'a'];

        // adding the accepted / rejected logic
        if (accepted)
            curr->accept_flag = 1;

        if (rejected)
            curr->reject_flag = 1;
    }

    // Mark the end of the word
    curr->wordEnd = true;
}

// function used to read data from the input file and
// save the words in a trie
void process_input(TrieNode *root) {
    ifstream fin("input.txt");
    
    int cnt_accept, cnt_fail, len_string;
    fin >> cnt_accept >> cnt_fail >> len_string;

    string temp;
    for (int i = 0; i < cnt_accept; ++i) {
        fin >> temp;
        insertKey(root, temp, 1, 0);
    }

    for (int i = 0; i < cnt_fail; ++i) {
        fin >> temp;
        insertKey(root, temp, 0, 1);
    }

    fin.close();
}

// function used to print the dfa
void print_to_file() {
    ofstream fout("output.txt");
    fout << "dfa" << "\n";

    // we have one final state -> 1, and the initial
    // state is 2
    fout << num_of_states << " 1 2\n";
    fout << "1\n";

    for (int i = 0; i < num_of_states; i++) {
        for (int j = 0; j < ALPHABET_SIZE; j++)
            fout << transitions[i][j] << " ";

        fout << "\n";
    }

    fout.close();
}

// function used to create the DFA based on a trie that contains
// both the accepted and rejected words
void create_dfa(TrieNode *node, int curr_state) {
    if (node == nullptr) return;

    // if a node has both accept and reject flags set to 1, we count
    // it as a new state in the DFA, otherwise we just update the
    // transition matrix accordingly
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (node->child[i] != nullptr)
            if (node->child[i]->accept_flag && node->child[i]->reject_flag) {
                num_of_states++;
                transitions[curr_state - 1][i] = num_of_states - 1;

                // recursively go through the trie
                create_dfa(node->child[i], num_of_states);
            } else {
                // no need to continue on this path, we already know that
                // everything formed beneath is either accepted or rejected
                if (node->child[i]->accept_flag == 1)
                    transitions[curr_state - 1][i] = 1; // accept state
                else if (node->child[i]->reject_flag == 1)
                    transitions[curr_state - 1][i] = 0; // reject state
            }
}

// function used to erase a line in the transition matrix, any reference
// to the erased line will point towards the new_index line; all the indexes
// and size of the matrix are updated
void erase_line(int erase_index, int new_index) {
    // updating the lines (ignore accept and fail states)
    for (int j = 2; j < num_of_states; j++)
        for (int k = 0; k < ALPHABET_SIZE; k++)
            if (transitions[j][k] == erase_index)
                transitions[j][k] = new_index;

    for (int j = 2; j < num_of_states; j++)
        for (int k = 0; k < ALPHABET_SIZE; k++)
            if (transitions[j][k] >= erase_index)
                transitions[j][k]--;

    // erasing the line
    transitions.erase(transitions.begin() + erase_index);
    num_of_states--;
}

// function used to minimize the number of state of the DFA
void optimize_dfa() {
    int optimization_found = 0;

    // run the loop until no more optimizations are found
    while (1) {
        // line to be erased
        int erase_index = -1;
    
        for (int i = 0; i < num_of_states - 1; i++) {
            // line that will "replace" the erased line
            int remain_index = i;

            // check if the line contains only 0s and 1s, mark it in the line_flag
            int line_flag = 1;
            for (int j; j < ALPHABET_SIZE; j++)
                if (transitions[i][j] != 0 && transitions[i][j] != 1) {
                    line_flag = 0;
                    break;
                }

            // if the line only contains 0s and 1s, we try to find an identical line
            // to remove
            if (line_flag == 1)
                for (int j = i + 1; j < num_of_states; j++) {
                    // check if the lines are equal, mark it in the equal_flag
                    int equal_flag = 1;
                    for (int k = 0; k < ALPHABET_SIZE; k++)
                        if (transitions[remain_index][k] != transitions[j][k]) {
                            equal_flag = 0;
                            break;
                        }

                    // save the index of the newly found line to be erased, exit
                    // the for loop
                    if (equal_flag == 1) {
                        erase_index = j;
                        break;
                    }
                }

            // if we found a line, we delete it, and mark that an optimization has
            // been done
            if (erase_index != -1) {
                erase_line(erase_index, remain_index);
                optimization_found = 1;
                erase_index = -1;
            }
        }

        // if no optimiziation has been done, exit the while loop
        if (optimization_found == 0)
            break;

        // reset the value
        optimization_found = 0;
    }
}

int main() {
    TrieNode *root;
    root = new TrieNode();
    root->accept_flag = 1;
    root->reject_flag = 1;

    process_input(root);

    // accept state, fail state, root
    num_of_states = 3;

    // initialising accept state
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        transitions[1][i] = 1;
    }

    // printTrieTree(root);

    // creating the DFA
    create_dfa(root, 3);

    // minimizing the number of states
    optimize_dfa();

    print_to_file();

    return 0;
}
