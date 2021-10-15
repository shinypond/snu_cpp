#include <iostream>
#include <time.h>
#define N 11
using namespace std;

void print_board(int board[N][N]) {
    // print a given chess board
    // 1 : Queen, 0 : Empty
    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

int is_valid(int board[N][N], int r, int c) {
    // Check whether we can place a queen at (r,c) or not
    // Note : For any column less (left) than c, each queen already exists 
    // (1) same row (to West)
    for (int j=0; j<c; j++) {
        if (board[r][j]) return 0;
    }
    // (2) diagonal line (to NorthWest)
    for (int i=r, j=c; i>=0 && j>=0; i--, j--) {
        if (board[i][j]) return 0;
    }
    // (3) diagonal line (to SouthWest)
    for (int i=r, j=c; i<N && j>=0; i++, j--) {
        if (board[i][j]) return 0;
    }
    return 1;
}

int solve_Nqueens(int board[N][N], int c) {
    // solved (i.e, N queens are all placed)
    if (c >= N) return 1;
    
    for (int i=0; i<N; i++) {
        if (is_valid(board, i, c)) {
            // Let a queen be on (i,c)
            board[i][c] = 1;

            // recursively check (for the next col)
            int result = solve_Nqueens(board, c+1);

            // If all the results for all c are TRUE, then return TRUE
            if (result == 1) {
                return 1;
            }
            // If a result for any c is FALSE, then let (i,c) be empty 
            else {
                board[i][c] = 0;
            }
        }
    }
    // No solution
    return 0;
}

void solve_and_show_Nqueens() {
    int board[N][N];
    // initialize the board
    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            board[i][j] = 0;
        }
    }
    if (solve_Nqueens(board, 0) == 0) {
        cout << "NO SOLUTION!" << endl;
    }
    else {
        print_board(board);
    }
}


int main() {
    clock_t startTime = clock();
    solve_and_show_Nqueens();
    clock_t endTime = clock();
    double elapsedTime = (double) (endTime - startTime);
    cout << "프로그램 수행 시간: " << elapsedTime << endl;
    return 0;
}

