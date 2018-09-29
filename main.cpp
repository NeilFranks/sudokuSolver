#include <iostream>
using namespace std;

//each square keeps track of possible values
struct possible{
    int committed = 0;
    //committed will be the number (1-9) in this square once you're sure of it.

    bool nums[9];
};

//column is an array of 9 squares
struct col {
    possible col[9];
};

//board is an array of 9 columns
col board[9];

//if square has just one possible value, commit it!
void onePos(int m, int n) {
    possible square = board[m].col[n];

    int numPos = 0; //keep track of number of possible values
    int val; //keep track of index of possible value (what the value is)
    if (square.committed == 0){
        for (int i = 0; i < 9; i++) {
            if (square.nums[i]) {
                numPos++;
                val = i + 1; //account for array starting at 0
            }
        }

        if(numPos == 1)
            square.committed = val;
    }
}

void disp() {

    for(int m = 0; m < 9; m++) {

        for(int n = 0; n < 9; n++) {
            int num = board[m].col[n].committed;
            if(num != 0)
                if(n == 2 || n==5)
                    cout << board[m].col[n].committed << "|";
                else if (n==8)
                    cout << board[m].col[n].committed;
                else
                    cout << board[m].col[n].committed << " ";
            else
            if(n == 2 || n==5)
                cout << " |";
            else if (n==8)
                cout << " ";
            else
                cout << "  ";
        }

        if(m == 2 || m==5)
            cout << endl << "-----------------" << endl;
        else
            cout << endl;
    }

}



void updatePoss(){
    bool changed;

    do {

        changed = false;
        //update possibilities within rows
        for (int m = 0; m < 9; m++) {
            possible rowPos; //if row has a number X committed, rowPos.nums[X-1] will be false
            for(int i = 0; i < 9; i++) {
                rowPos.nums[i] = true;
            }

            //find all committed nums in row
            for (int n = 0; n < 9; n++) {
                int num = board[m].col[n].committed;
                if (num != 0)
                    rowPos.nums[num - 1] = false;
            }

            //make all falses in rowPos false in each square
            //rowPos keeps track of any possible number that could still be in the row. update square possibilities
            for (int n = 0; n < 9; n++) {
                int num = board[m].col[n].committed;
                if (num == 0) //square has no committed value
                    //increment through rowPos possibilities
                    for (int i = 0; i < 9; i++) {
                        if (!rowPos.nums[i] && board[m].col[n].nums[i]) {
                            board[m].col[n].nums[i] = false;
                            changed = true;
                            onePos(m,n); //check if you can commit the square
                        }
                    }
            }

        }

        //update all possibilities within columns
        for (int n = 0; n < 9; n++) {
            possible colPos; //if col has a number X committed, colPos.nums[X-1] will be false
            for(int i = 0; i < 9; i++) {
                colPos.nums[i] = true;
            }

            //find all committed nums in col
            for (int m = 0; m < 9; m++) {
                int num = board[m].col[n].committed;
                if (num != 0)
                    colPos.nums[num - 1] = false;
            }

            //make all falses in colPos false in each square
            //colPos keeps track of any possible number that could still be in the col. update square possibilities
            for (int m = 0; m < 9; m++) {
                int num = board[m].col[n].committed;
                if (num == 0) //square has no committed value
                    //increment through colPos possibilities
                    for (int i = 0; i < 9; i++) {
                        if (!colPos.nums[i] && board[m].col[n].nums[i]) {
                            board[m].col[n].nums[i] = false;
                            changed = true;
                            onePos(m,n); //check if you can commit the square
                        }
                    }
            }

        }

        //update all possibilities within 3x3 cells
        for (int x = 0; x < 3; x++) { //x will be the row#/3. start at row 0,3,and 6
            for (int y = 0; y < 3; y++) { //y will be the col#/3. start at col 0,3,and 6

                possible cellPos; //if cell has a number X committed, colPos.nums[X-1] will be false
                for(int i = 0; i < 9; i++) {
                    cellPos.nums[i] = true;
                }

                //increment through all 3 columns in cell
                for (int n = 3 * y; n < 3 * y + 3; n++) { //0-2,3-5,6-8
                    //find all committed nums in cell
                    for (int m = 3 * x; m < 3 * x + 3; m++) { //0-2,3-5,6-8
                        int num = board[m].col[n].committed;
                        if (num != 0)
                            cellPos.nums[num - 1] = false;
                    }
                }

                //make all falses in rowPos false in each square
                //colPos keeps track of any possible number that could still be in the col. update square possibilities
                for (int n = 3 * y; n < 3 * y + 3; n++) { //0-2,3-5,6-8
                    for (int m = 3 * x; m < 3 * x + 3; m++) { //0-2,3-5,6-8
                        int num = board[m].col[n].committed;
                        if (num == 0) //square has no committed value
                            //increment through cellPos possibilities
                            for (int i = 0; i < 9; i++) {
                                if (!cellPos.nums[i] && board[m].col[n].nums[i]) {
                                    board[m].col[n].nums[i] = false;
                                    changed = true;
                                    onePos(m,n); //check if you can commit the square
                                }
                            }
                    }
                }
            }
        }
    }while(changed);
}

bool commitSingles(){
    bool changed = false;

    //iterate through rows
    for(int m = 0; m < 9; m++){
        //check if a sum only appears in one square
        for(int i = 0; i < 9; i++){
            //looking to see if bool for nums[i] is true exactly once
            int timesTrue = 0;
            int col; //stores column index that last had i as possible

            //increment through columns
            for(int n = 0; n < 9; n++){
                if(board[m].col[n].committed == 0 && board[m].col[n].nums[i]){
                    timesTrue++;
                    col = n;
                }
            }

            if (timesTrue == 1){
                board[m].col[col].committed = i+1;
                changed = true;
            }
        }
    }
    updatePoss();

    //iterate through cols
    for(int n = 0; n < 9; n++){
        //check if a sum only appears in one square
        for(int i = 0; i < 9; i++){
            //looking to see if bool for nums[i] is true exactly once
            int timesTrue = 0;
            int row; //stores row index that last had i as possible

            //increment through rows
            for(int m = 0; m < 9; m++){
                if(board[m].col[n].committed == 0 && board[m].col[n].nums[i]){
                    timesTrue++;
                    row = m;
                }
            }

            if (timesTrue == 1) {
                board[row].col[n].committed = i + 1;
                changed = true;
            }
        }
    }
    updatePoss();

    //update all possibilities within 3x3 cells
    for (int x = 0; x < 3; x++) { //x will be the row#/3. start at row 0,3,and 6
        for (int y = 0; y < 3; y++) { //y will be the col#/3. start at col 0,3,and 6
            for(int i = 0; i < 9; i++) {
                //looking to see if bool for nums[i] is true exactly once
                int timesTrue = 0;
                int row;
                int col; //stores col index that last had i as possible
                for (int m = 3 * x; m < 3 * x + 3; m++) { //0-2,3-5,6-8
                    for (int n = 3 * y; n < 3 * y + 3; n++) { //0-2,3-5,6-8
                        if (board[m].col[n].committed == 0 && board[m].col[n].nums[i]) {
                            timesTrue++;
                            row = m;
                            col = n;
                        }
                    }
                }

                if (timesTrue == 1) {
                    board[row].col[col].committed = i + 1;
                    changed = true;
                }
            }
        }
    }
    updatePoss();

    return changed;
}

void solve(){
    bool changed;
    updatePoss();

    do{
        changed = commitSingles();
    }while(changed);

}

void parseInput() {
    string input;
    for (int m = 0; m < 9; m++) {
        cout << "Row " << m + 1 << ": ";
        getline(cin, input);
        if (input.length() != 9) {
            cout << "Error: row should be 9 chars long. Enter space for blank squares" << endl;
            m--;
        } else {
            for (int i = 0; i < 9; i++) {
                if (input[i] == 32){} //32 is a space
                    //do nothing
                else
                if (input[i] < 49 || input[i] > 57) {
                    cout << "Error: number invalid; make it 1-9" << endl;
                    m--;
                } else
                    board[m].col[i].committed = input[i] - 48; //acount for ascii
            }
        }
    }

    bool quit = false;
    while (!quit) {
        getline(cin, input);
        if(input.compare("s") == 0)
            solve();
        else if (input.compare("d") == 0)
            disp();
        else if (input.compare("q") == 0)
            quit = true;
    }
}

int main() {

    //intilialize board to make all numbers possible
    for(int m = 0; m < 9; m++) {
        for(int n = 0; n < 9; n++) {
            for(int i = 0; i < 9; i++) {
                board[m].col[n].nums[i] = true;
            }
        }
    }

    //fill board with known numbers
    parseInput();

    return 0;
}