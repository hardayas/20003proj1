//
//  func.c
//  20003proj1
//
//  Created by Har on 26/8/17.
//  Student ID: 692319
//

#include "func.h"

node_t *tree_node(){
    node_t *node = (node_t*) malloc(sizeof(node_t));
    assert(node != NULL);
    return node;
}

node_t *insert_tree(node_t *pNode, char *word, int weight) {
    if(pNode == NULL) {                   //pNode Null then insert the letter in equal pos
        pNode = tree_node();
        pNode->left = pNode->right = pNode->equal = NULL;
        pNode->character = *word;
        pNode->last_character = FALSE;
    }
    
    if (*word < pNode->character) {    //if current letter less then go left
        pNode->left = insert_tree(pNode->left, word, weight);
    }
    
    else if (*word == pNode->character) { //if it equals to node
        
        if (*(word+1) == STRING_END) {                //check if its last letter to insert
            pNode->last_character = TRUE;         //fill in weight and flag
            pNode->weight = weight;
        }
        
        else {                              //otherwise move on to next letter in word
            pNode->equal = insert_tree(pNode->equal, word+1, weight);
        }
    }
    
    else {          //if letter bigger then go right
        pNode->right = insert_tree(pNode->right, word, weight);
    }
    
    return pNode;
}

void find_and_traverse( node_t *pNode, char *prefix, list_t **head){
    /**
     * Find the node in the tree that represents the prefix
     * pNode will point there if we reached the '\0' symbol,
     * if prefix does not exist , then pNode should be NULL
     */
    char buffer[CHAR_LIMIT]; int index = 0, comparisons = 0;
    char *temp = prefix;
    while(*temp != STRING_END && pNode != NULL){
        /**
         * Find tree position for prefix
         */
        // left branch
        if(*temp < pNode->character){
            pNode = pNode->left;
            comparisons++;
            continue ;
        }
        
        // right branch
        if(*temp > pNode->character){
            pNode = pNode->right;
            comparisons++;
            continue ;
        }
        // equal branch
        if(*temp == pNode->character){
            if (pNode->equal && *(temp+1) != STRING_END) {
                pNode = pNode->equal;
                buffer[index++] = *temp;
                temp++;
                comparisons++;
                continue;
            }
            buffer[index++] = *temp;
            temp++;
            comparisons++;
        }
    }
    
    
    
    /**
     * At this point, pNode should be pointing to
     * the node where the prefix is contained.
     */
    printf("Prefix: %s took %d char comparisons\n", prefix, comparisons);
    if(pNode != NULL){
        
        /**
         * Include the prefix itself as a candidate
         * if prefix is a key
         */
        
        if (pNode->last_character == TRUE) {
            buffer[strlen(prefix)] = STRING_END;
            *head = insertList(buffer, pNode->weight, head);
            
        }
        
        /**
         * print all the keys that contain the prefix
         */
        traverse (pNode->equal, buffer , (int)strlen(prefix), head) ;
    }
    else {
        *head = insertList("NOTFOUND", UNAVAILABLE, head);
    }
}


/**
 * Tree traversal from a given node
 */
void traverse(node_t* pNode, char* buffer, int depth, list_t **head) {
    if (pNode == NULL) return;
    /**
     * Go first to the left most branch */
    traverse (pNode->left, buffer, depth, head) ;
    /**
     * If no more left branches , then save the character
     */
    buffer[depth] = pNode->character;
    if (pNode->last_character == TRUE /* pNode end of string flag is true*/) {
        buffer[depth+1] = STRING_END ;
        *head = insertList(buffer, pNode->weight, head);
    }
    
    /**
     * and go to the equal branch, advancing
     * to the next character of the key
     */
    traverse (pNode->equal , buffer , depth + 1, head) ;
    /**
     * Finally go to the branches that contain
     * characters greater than the current one in the buffer
     */
    traverse (pNode->right, buffer, depth, head) ;
}

list_t *list_node() {
    list_t *node = (list_t*) malloc(sizeof(list_t));
    assert(node != NULL);
    node->next = NULL;
    return node;
}

list_t *insertList(char *key, int weight, list_t **head) {
    list_t *temp = list_node();
    strncpy(temp->key, key, strlen(key));
    temp->weight = weight;
    temp->next = *head;
    *head = temp;
    return *head;
}


void print_list(list_t *head, char *outputfile, char *prefix) {
    FILE *f = fopen(outputfile, "a");
    assert(f != NULL);
    fprintf(f, "Prefix: %s\n", prefix);
    while (head) {
        
        if (head->weight == UNAVAILABLE) {
                fprintf(f, "%s\n", head->key);
        }
        else {
        fprintf(f, "KEY: %s ----> WEIGHT:%d\n", head->key, head->weight);
        }
        head = head->next;
    }
    fprintf(f, "\n\n");
    fclose (f);
}

void free_tree(node_t *root) {
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->equal);
    free(root);
    free_tree(root->right);
    
}

void free_list(list_t *head) {
    list_t *temp = (list_t*) malloc(sizeof(list_t));
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
