/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

//define tie structure for a node
typedef struct _node
{
    bool is_word;
    struct _node* children[27];
}
node;

//prototypes
node* newNode();
bool recursiveFree(node* trav);

//globally define the root node and dictionary count
node* root = NULL;
unsigned int dictionary_count = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    //create trav node and set to root
    node* trav = root;
    //create int index for zero indexing letters
    int index;
    //loop through each letter
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        //check if its a letter or apostrophe 
        if (isalpha(word[i]) || word[i] == '\'')
        {
            if(isalpha(word[i]))
            {
                //make sure all letters are lower case and then index to zero
                index = tolower(word[i]);
                index -= 97;
            }
            //index apostrophe to 26
            else
                index = word[i] - 13;
            //returns false if it reaches a null
            if (trav -> children[index] == NULL)
            {
                return false;
            }
            //if not goes sets trav to that child
            else if (trav -> children[index] != NULL)
            {
                trav = trav -> children[index];
            }
        }
    }
    //we have reached the end of the word so now we check if trav's is_word value is true
    if (trav -> is_word == true)
        return true;
    else
        return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    //open the dictionary 
    FILE* infile = fopen(dictionary, "r");
    if (infile == NULL)
    {
        printf("Could not open file");
        return false;
    }
    //init root and set trav to root
    root = newNode();
    node* trav = root;
    
    //get chars from file checking for EOF
    for (int c = fgetc(infile); c != EOF; c = fgetc(infile))
    {
        if (isalpha(c) || c == '\'')
        {
            //index to 0 from ascii and special case of apostrophe
            if (isalpha(c))
                c -= 97;
            else
                c -= 13;
            //check if node exists
            if (trav -> children[c] == NULL)
            {
                //create a new node and set child[c] to it and move trav
                node* nextNode = newNode();
                trav -> children[c] = nextNode;
                trav = trav -> children[c];
            }
            //if node exists move trav 
            else if(trav -> children[c] != NULL)
                trav = trav -> children[c];
        }
        //if we reach a new line change the value of trav is_word to true
        else if (c == '\n')
        {
            trav -> is_word = true;
            trav = root;
            dictionary_count++;
        }
    }
    fclose(infile);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return dictionary_count;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    //set trav to root
    node* trav = root;
    bool end = recursiveFree(trav);
    if (end == true)
        return true;
    else
        return false;
}

node* newNode()
{
    //creates a new node
    node* next_node = malloc(1 * sizeof(node));
    next_node -> is_word = false;
    for (int i = 0; i < 27; i++)
    {
        next_node -> children[i] = NULL;
    }
    return next_node;
}

bool recursiveFree(node* trav)
{
    //loops through and if finds pointer to child calls itself on that
    for (int i = 0; i < 27; i++)
    {
        if (trav -> children[i] != NULL)
        {
            recursiveFree(trav -> children[i]);
        }
    }
    free(trav);
    return true;
}
