#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cmath>

using namespace std;

class TheRani {
public:
    TheRani(char* input_path, char* output_path);
    ~TheRani();
    void START(int n);
    void ADD();
    void MOVE(int x, int y, int n, int m);
    void QUERY(int x, int n);


    // Call execute and handles exceptions
    void main();

private:
    int experiment_count;       // You will need to track the number of experiments
    int* subject_counts;        // For each, the number of subjects
    string** subject_history;   // And for each subject, their history
    int subjects;
    bool started;

    ifstream input;             // Input file stream
    ofstream output;            // Output file stream

    // Called in the main method
    void execute(const string& line);

    // Possible helper: deallocate all current members
};

TheRani::TheRani(char* input_path, char* output_path) : experiment_count(0), input(input_path), output(output_path) {

}

TheRani::~TheRani() {
    if(experiment_count>0){
        for(int i = 0; i < experiment_count; i++){
            delete[] subject_history[i];
        }
        delete[] subject_history;
        delete[] subject_counts;
    }
}
//initializes subject_counts and subject_history
void TheRani::START(int n){
    subjects = n;
    experiment_count = 1;
    subject_counts = new int[experiment_count];
    subject_counts[0] = subjects;

    subject_history = new string*[1];
    subject_history[0] = new string[n];
    for(int i = 0; i < n; i++){
        subject_history[0][i] = "";
    }
}
//creates temp 2-d array of larger size, copies over info from history to temp
//deletes history data and then sets the history pointer equal to new temp
void TheRani::ADD(){
    string** helper = new string*[experiment_count+1];
    for(int i = 0; i < experiment_count+1; i++){
        helper[i] = new string[subjects];
    }
    for(int i = 0; i < subjects; i++){
        helper[experiment_count][i] = "";
    }
    for (int i = 0; i < experiment_count; i++)
    {
        for(int j = 0; j < subjects; j++)
        {
            helper[i][j] = subject_history[i][j];
        }
    }
    for (int i = 0; i < experiment_count; i++)
    {
        delete[] subject_history[i];
    }
    delete[] subject_history;

    subject_history = helper;

 
    //does the same things with subject_counts
    int* temp = new int[experiment_count+1];
    temp[experiment_count] = 0;
    for(int i = 0; i < experiment_count; i++){
        temp[i] = subject_counts[i];
    }
    delete[] subject_counts;

    subject_counts = temp;

    experiment_count++;

}
//utilizes subject_counts to easily move data and append -
//the strings after they are moved within subject_history
//edits subject_counts values at the end
void TheRani::MOVE(int x, int y, int n, int m){
    int reach = m - n + 1;
    int begin = m + 1;
    int reach1 = subject_counts[x]-m-1;


    for(int i=0; i< reach; i++){
        subject_history[y][subject_counts[y]+i] = subject_history[x][n+i];
    }

    if(reach1 != 0){
        for(int i = 0; i<reach1; i++){
            subject_history[x][n+i] = subject_history[x][begin+i];
        }
    }

    
    if(x!=0){
        for(int i=0; i<reach; i++){
            subject_history[y][subject_counts[y]+i] += "";
            subject_history[y][subject_counts[y]+i] += to_string(y);
            subject_history[y][subject_counts[y]+i] += " ";
        }
    }
    else{
        for(int i=0; i<reach; i++){
            subject_history[y][subject_counts[y]+i] += to_string(y);
            subject_history[y][subject_counts[y]+i] += " ";
        }
    }

    subject_counts[x] -= reach;
    subject_counts[y] += reach;

}

//tests inputs and then outputs data
void TheRani::QUERY(int x, int n){
    if(x >= 0 && n >= 0 && x < experiment_count && n < subjects){
       output << subject_history[x][n] << endl;
    }
}

// Possible helper: read an integer argument from a stream

void TheRani::main() {
    string line;
    started = false;
    int line_number = -1;
    while (getline(input, line)) {
        try {
            this->execute(line);
        } catch(exception& e) {
            // If you use exceptions, make sure the line number is printed here
            this->output << "Error on line " << line_number << ": " << e.what() << endl;
        }
        line_number++;
    }
}

void TheRani::execute(const string& line) {

    string command;
    stringstream stream(line);  // Initialize the stream with the line
                                // Read the first word, which is the command   
    stream >> command;
    //tests if input is START first time through and runs if it is
        if (command == "START"){
            stream >> subjects;
            if(subjects < 0){
                throw out_of_range("argument out of range");
            }
            started = true;
            START(subjects);
        }
        else if(started == false){
            throw out_of_range("no subjects yet");
        }

        else if(command == "ADD"){
            ADD();
        }

        //tests if input is alphabetical letter or double
        //also tests if input is within bounds
        else if(command == "MOVE"){
            double fraction, integer;
            double start, finish, first, last;

            if(stream.eof()){
                throw out_of_range("too few arguments");
            }

            //tests if input was too small
            stream >> start >> finish >> first >> last;


            if(stream.fail()){
                throw out_of_range("expected integer argument");
           }
            
            //tests if double was input
            fraction = modf(start, &integer);
            if(fraction != 0){
                throw out_of_range("expected integer argument");
            }
            fraction = modf(finish, &integer);
            if(fraction != 0){
                throw out_of_range("expected integer argument");
            }
            fraction = modf(first, &integer);
            if(fraction != 0){
                throw out_of_range("expected integer argument");
            }
            fraction = modf(last, &integer);
            if(fraction != 0){
                throw out_of_range("expected integer argument");
            }

            //tests if input is within bounds
            if(start < 0 || start > experiment_count){
                throw out_of_range("argument out of range");
            }
            if(finish < 0 || finish > experiment_count){
                throw out_of_range("argument out of range");
            }
            if(first < 0 || first > subjects){
                throw out_of_range("argument out of range");
            }
            if(last < 0 || last > subjects){
                throw out_of_range("argument out of range");
            }
            if(first > last){
                throw out_of_range("invalid range of subjects to move");
            } 
            MOVE(start, finish, first, last);
        }

        //tests if input is alphabetical letter or double
        //also tests if input is within bounds
        else if(command == "QUERY"){
            double fraction, integer;
            double x, n;
            if(stream.eof()){
                throw out_of_range("too few arguments");
            } 
            stream >> x >> n;
            if(stream.fail()){
                throw out_of_range("expected integer argument");
            }
            fraction = modf(x, &integer);
            if(fraction != 0){
                throw out_of_range("expected integer argument");
            }
            fraction = modf(n, &integer);
            if(fraction != 0){
                throw out_of_range("expected integer argument");
            } 
            QUERY(x, n);
        }
        else{
            throw out_of_range("command does not exist");
        }

}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Please provide an input and output file!" << endl;
        return 1;
    }

    TheRani tr(argv[1], argv[2]);
    tr.main();
    return 0;
}
