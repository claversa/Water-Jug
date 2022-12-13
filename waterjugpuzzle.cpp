 /* Author      : Caroline LaVersa
 * Date        : 10/15/22
 */
 
#include <iostream>
#include <sstream>
#include <queue>
#include <stack>
#include <string>
#include <utility>
using namespace std;
// Struct to represent state of water in the jugs.
struct State {
    int a, b, c;
    string directions;
    State *parent;
    
    State(int _a, int _b, int _c, string _directions) : 
        a{_a}, b{_b}, c{_c}, directions{_directions}, parent{nullptr} { }

    //my own constructor that takes in array as input and splits into a,b,c 
     State(int _values[], string _directions) : 
        a{_values[0]}, b{_values[1]}, c{_values[2]}, directions{_directions}, parent{nullptr} { }
    
    // String representation of state in tuple form.
    string to_string() {
        ostringstream oss;
        oss << "(" << a << ", " << b << ", " << c << ")";
        return oss.str();
    }

    int get(char jug){ //get method for value stored in a given jug, used with pair array of orders
        if(jug == 'A'){
            return a;
        }
        if(jug == 'B'){
            return b;
        }
        return c;
        
    }

};
//GLOBAL, will not change
pair<char, char> combos[] = {{'C','A'}, {'B', 'A'},{'C','B'},{'A','B'},{'B','C'},{'A','C'}}; //the correct order of pours


void pour(State* arr[], State *s, int caps[]){ // takes in empty array of states to fill with combos, current state, and integer array of capacities
    int i = 0;
    for(auto [from, to] : combos){ //for every combo pair in array combos
        int available = caps[to-'A']- s->get(to); // capacity of 'to' jug - current value of 'to' jug
        int values[3]; // store new a,b,c
        if((s->get(from) - available)<=0){ // all of from can be added to 'to' without overflow
            values[from-'A']= 0; // new from value is original from - available from to, at index of the letter
            values[to-'A'] = s->get(to)+s->get(from); // new to value is original to + what poured from from (all), at index of letter
            values[3-(from+to-(2*'A'))] =  s->get((3-(from+to-(2*'A')))+ 'A'); // value that stays the same, at index of letter
            ostringstream oss;
            oss<<"Pour "<<s->get(from)<<" gallon" << (s->get(from) == 1 ? "" : "s") << " from "<< from<< " to "<< to<<". "; //ternary for if should include s or not
            arr[i] = new State(values, oss.str()); //using our own constructor
            arr[i]->parent = s; //set parent of next state to current state s
        }
        else{
            values[from-'A'] = s->get(from) - available; // new from is whatever was originally in from-what was available in to
            values[to-'A'] = caps[to-'A']; // new to is filled to cap
            values[3-(from+to-(2*'A'))] =  s->get((3-(from+to-(2*'A')))+ 'A'); // value that stays the same, at index of letter
            ostringstream oss;
            oss<<"Pour "<<available<<" gallon" << (available == 1 ? "" : "s") << " from "<< from<< " to "<< to<<". "; //ternary for if should include s or not
            arr[i] = new State(values, oss.str()); //using our own constructor
            arr[i]->parent = s; //set parent of next state to current state s
        }
        i++;
    }
}

void BFS(int arr[]){ // takes in integer array of 
    State* **array = new State* *[arr[0]+1];
    // For each row, make an array
    for(int i = 0; i < (arr[0]+1); i++) { //a +1 columns
        array[i] = new State* [arr[1]+1]; // b +1 rows
        // Initialize the new row.
        fill(array[i], array[i] + (arr[1]+1), nullptr); // fill initially with null pointers
    }
    State *s = new State(0,0,arr[2], "Initial state. "); //initial state
    queue<State*> q; //queue of state pointers
    q.push(s); //add initial state to queue
    while(!q.empty()){ //until stack empty 
        State* curr = q.front(); // curr is value at front of queue
        q.pop(); //removes curr
        if(curr->a == arr[3]&& curr->b == arr[4]&&curr->c==arr[5]){ // IS GOAL STATE
            stack<string> rev; // stack of strings
            State *tmp = curr; // before lose access to current state, make temp state pointer to point to it so can delete later
            while(curr!=nullptr){ //nullptr would be before the initial state, stop
                rev.push(curr->directions+curr->to_string()); // directions for curr on stack
                curr = curr->parent; // update current to its parent
            } 
            while(!(rev.empty())){ //prints directions in reverse order (start at top of stack)
                cout<<rev.top()<<endl; // reads off top of stack
                rev.pop(); // removes element on top of stack
            }
            while(!q.empty()){
                delete q.front(); //deletes memory at front of queue
                q.pop(); //removes
            }

            for(int i = 0; i < (arr[0]+1); i++) {
                for(int j =0; j< (arr[1]+1); j++){
                    if(array[i][j])
                        delete array[i][j];// deletes element
                }
                delete [] array[i]; //deletes row
            }
            // Delete the array itself.
            delete [] array;
            delete tmp; // delete the temp state pointer that points to original current state
            return;
        }

        //NOT GOAL STATE:
        else if(array[curr->a][curr->b]==nullptr){ //not in matrix yet
            array[curr->a][curr->b]= curr; // add to matrix at a,b
        }
        else{
            delete curr; //already exists on a shorter path so delete this one 
            continue; // back to top of loop
        }
        State* states[6]; //array of state pointers to fill in pour with the state permutations
        pour(states, curr, arr); // fills states with the combos from curr
        for(int i =0; i<6; i++){
            if(states[i]!=nullptr){
                q.push(states[i]);// add NON NULL new states to queue
            }
        }
    }


    cout << "No solution." << endl; // goal wasn't found by end

    // Delete each row first.
    for(int i = 0; i < (arr[0]+1); i++) {
        for(int j =0; j< (arr[1]+1); j++){
            delete array[i][j]; //delete each element in row
        }
        delete [] array[i]; //delete row
    }
    // Delete the array itself.
    delete [] array;
}


int main(int argc, char * const argv[]) {
    if(argc!=7){ //not name and 6 values given
        cout<<"Usage: ./waterjugpuzzle <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>"<<endl;
        return 1;
    }
    istringstream iss;
    int check[6]; //int array to check input against
    for(int i = 0; i<argc-4; i++){ // only the capacities
        iss.str(argv[i+1]); // read first capacity into iss
        if(!(iss>>check[i])){ // if iss cant read into int array, not integer, throw error
            cout<<"Error: Invalid capacity '" <<argv[i+1]<<"' for jug "<<(char)(65+i)<<"."<<endl; //ascii 'A' + (i-3) gives a,b, or c
            return 1;
        }
        if(check[i]<=0){ //current is less than or equal to 0
            cout<<"Error: Invalid capacity '"<<check[i]<<"' for jug "<<(char)(65+i)<<"."<<endl; //ascii 'A' + (i-3) gives a,b, or c
            return 1;
        }
        iss.str("");
        iss.clear();
    }

    for(int i = 3; i<argc-1; i++){ // only the goals
        iss.str(argv[i+1]); // read first goal into iss
        if(!(iss>>check[i])){ // if iss cant read into int array, not integer, throw error
            cout<<"Error: Invalid goal '"<<argv[i+1]<<"' for jug "<<(char)(65+i-3)<<"."<<endl; //ascii 'A' + (i-3) gives a,b, or c
            return 1;
        }
        if(check[i]<0){ // current is less than 0
            cout<<"Error: Invalid goal '"<<check[i]<<"' for jug "<<(char)(65+i-3)<<"."<<endl; //ascii 'A' + (i-3) gives a,b, or c
            return 1;
        }
        iss.str("");
        iss.clear();
    }
    for(int i=3; i<argc-1;i++){
        if(check[i]> check[i-3]){//goal greater than capacity
            cout<<"Error: Goal cannot exceed capacity of jug "<<(char)(65+i-3)<<"."<<endl; //ascii 'A' + (i-3) gives a,b, or c
            return 1;
        }
    
    }
    int sum=0;
    for(int i = 3; i<6; i++){
    sum+=check[i]; //total # gallons of goal states
    }
    if(sum!=check[2]){ //total # gallons goal state> than capacity c
        cout<<"Error: Total gallons in goal state must be equal to the capacity of jug C."<<endl;
        return 1;
    }
    BFS(check); //use array of capacities and goals for BFS function
    return 0;

}