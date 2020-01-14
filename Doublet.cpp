#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include "heap.h"
using namespace std;

//adds current node's neighbors to heap 
void addNeighbors(MinHeap<string> &heap, set<string> &dictionary, string word, string target, map<string, int> &distances, map<string, int> &addedWords, set<string> &deletedWords){
    set<string>::iterator it_set;
    set<string>::iterator it_dict;
    bool differ;
    bool moreThanOne;
    int heuristic;
    int func;

    //cycles through the entire dictionary, comparing the current word with every word in the dicionary
    for(it_dict = dictionary.begin(); it_dict != dictionary.end(); it_dict++){
        differ = false;
        moreThanOne = false;
        heuristic = 0;
        func = 0;
        //determines if word from dictionary differs from current word by only one letter, if it does not differ or if it differs by more than one, dont enter if statement;
        for(unsigned int i = 0; i < word.size(); i++){
            if(word[i] != (*it_dict)[i] && !differ){
                differ = true;
            }
            else if(word[i] != (*it_dict)[i] && differ){
                moreThanOne = true;
                break;
            }
        }
        //determines priority and adds neighbor to heap if it hasn't already been added and removed, and if it has it does a little different stuff
        if(differ && !moreThanOne){
            //if the neighbor hasn't been added yet
            if(addedWords.find(*it_dict) == addedWords.end()){
                //determines the heuristic for this neighbor
                for(unsigned int i = 0; i < (*it_dict).size(); i++){
                    if((*it_dict)[i] != target[i]){
                        heuristic++;
                    }
                }
                func = distances[word] + 1 + heuristic;
                int priority = func * ((*it_dict).size()+1) + heuristic;
                addedWords.insert(pair<string, int>(*it_dict, 0));
                addedWords[*it_dict] = heap.add(*it_dict, priority);
                distances.insert(pair<string, int>(*it_dict, distances[word]+1));
            }
            //if the neighbor is an added word, but has not been visited
            else if(deletedWords.find(*it_dict) == deletedWords.end()){
                for(unsigned int i = 0; i < (*it_dict).size(); i++){
                    if((*it_dict)[i] != target[i]){
                        heuristic++;
                    }
                }
                if(distances[*it_dict] > distances[word] + 1){
                    distances[*it_dict] = distances[word] + 1;
                }
                func = distances[*it_dict] + heuristic;
                int priority = func * ((*it_dict).size()+1) + heuristic;
                heap.update(addedWords[*it_dict], priority);
            }
        }
    }
}


int main(int argc, char **argv){
    if(argc != 4){
        return -1;
    }

//file reading and parsing stuff
    string firstWord = argv[1];
    string target = argv[2];
    ifstream infile(argv[3]);

    set<string> dictionary;
    set<string>::iterator it_set;

    int numWords;

    string holder;
    getline(infile, holder);
    stringstream ss(holder);
    ss >> numWords;

//Make first and last words capitalized
    for(unsigned int j = 0; j < firstWord.length(); j++){
        firstWord[j] = toupper(firstWord[j]);
        target[j] = toupper(target[j]);
    }


//Reading in dictionary, but only words of the same size, also makes every letter capitalized
    for(int i = 0; i < numWords; i++){
        getline(infile, holder);
        if(holder.size() == firstWord.size()){
            for(unsigned int j = 0; j < holder.length(); j++){
                holder[j] = toupper(holder[j]);
            }
            dictionary.insert(holder);
        }
    }
 
//definitions of datastructures I will need
    MinHeap<string> heap = MinHeap<string>(2);
    map<string, int> addedWords;
    map<string, int> distances;
    set<string> deletedWords;

    heap.add(firstWord, 0);
    addedWords.insert(pair<string, int>(firstWord, 0));
    distances.insert(pair<string, int>(firstWord, 0));

    string current;
    int expansions = 0;

//runs through heap until current node is the target or the heap is empty
    while(!heap.isEmpty()){
        current = heap.peek();
        heap.remove();
        deletedWords.insert(current);

        if(current == target){
            break;
        }

        expansions++;
        addNeighbors(heap, dictionary, current, target, distances, addedWords, deletedWords);
    }

//determines what to cout
    if(current != target){
        cout << "No transformation" << endl << expansions << endl;
    }
    else{
        cout << distances[current] << endl << expansions << endl;
    }

    return 0;
}
