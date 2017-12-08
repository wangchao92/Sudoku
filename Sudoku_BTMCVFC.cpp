// A Backtracking program  in C++ to solve Sudoku problem
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <math.h>
#include <unordered_set>
#include <vector>

using namespace std;

#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)

// UNASSIGNED is used for empty cells in sudoku grid
#define UNASSIGNED 0
 
// N is used for size of Sudoku grid. Size will be NxN
#define N 16
 
// This function finds an entry in grid that is still unassigned
bool FindMCVLocation(int grid[N][N], const vector< vector< unordered_set<ushort> > > &sets, int &row, int &col);
 
// Checks whether it will be legal to assign num to the given row,col
bool isSafe(int grid[N][N], int row, int col, int num);

// Forward checking if unassigned gird has no available choice, return false
bool updateSets(int grid[N][N], int num, int row, int col, vector< vector< unordered_set<ushort> > > &sets);
 
/* Takes a partially filled-in grid and attempts to assign values to
  all unassigned locations in such a way to meet the requirements
  for Sudoku solution (non-duplication across rows, columns, and boxes) */
bool SolveSudoku(int grid[N][N], vector< vector< unordered_set<ushort> > > sets)
{
    int row = -1, col = -1;
 
    // If there is no unassigned location, we are done
    if (FindMCVLocation(grid, sets, row, col))
       return true; // success!

    if (row == -1 || col == -1) return false;
    
    // consider digits 1 to 16
    for (auto num : sets[row][col]) {
        // if looks promising
        // if (isSafe(grid, row, col, num))
        // {
            vector< vector< unordered_set<ushort> > > tmp = sets;

            // make tentative assignment
            grid[row][col] = num;

            if (updateSets(grid, num, row, col, tmp)) {
              
              // return, if success, yay!
              if (SolveSudoku(grid, tmp))
                  return true;

            }
            
            // failure, unmake & try again
            grid[row][col] = UNASSIGNED;
        // }
    }
    return false; // this triggers backtracking
}

bool updateSets(int grid[N][N], int num, int row, int col, vector< vector< unordered_set<ushort> > > &sets) {
  sets[row][col].clear();
  for (int k = 0; k < N; k++) {
    sets[row][k].erase(num);
    if (grid[row][k] == UNASSIGNED && sets[row][k].size() == 0) return false;
  }
  for (int k = 0; k < N; k++) {
    sets[k][col].erase(num);
    if (grid[k][col] == UNASSIGNED && sets[k][col].size() == 0) return false;
  }
  for (int ii = row - row%(int)sqrt(N); ii < row - row%(int)sqrt(N) + sqrt(N); ii++) {
    for (int jj = col - col%(int)sqrt(N); jj < col - col%(int)sqrt(N) + sqrt(N); jj++) {
      sets[ii][jj].erase(num);
      if (grid[ii][jj] == UNASSIGNED && sets[ii][jj].size() == 0) return false;
    }
  }
  return true;
}
 
/* Searches the grid to find an entry that is still unassigned. If
   found, the reference parameters row, col will be set the location
   that is unassigned and with minimal avaiable domain, and true is returned. If no unassigned entries
   remain, false is returned. */
bool FindMCVLocation(int grid[N][N], const vector< vector< unordered_set<ushort> > > &sets, int &row, int &col)
{
    int min = N;
    bool finished = true;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (grid[i][j] == UNASSIGNED) {
              int size = sets[i][j].size();
              if (size != 0 && size < min) {
                min = sets[i][j].size();
                row = i;
                col = j;
              }
              finished = false;
            }
    return finished;
}
 
/* Returns a boolean which indicates whether any assigned entry
   in the specified row matches the given number. */
bool UsedInRow(int grid[N][N], int row, int num)
{
    for (int col = 0; col < N; col++)
        if (grid[row][col] == num)
            return true;
    return false;
}
 
/* Returns a boolean which indicates whether any assigned entry
   in the specified column matches the given number. */
bool UsedInCol(int grid[N][N], int col, int num)
{
    for (int row = 0; row < N; row++)
        if (grid[row][col] == num)
            return true;
    return false;
}
 
/* Returns a boolean which indicates whether any assigned entry
   within the specified 4x4 box matches the given number. */
bool UsedInBox(int grid[N][N], int boxStartRow, int boxStartCol, int num)
{
    for (int row = 0; row < (int)sqrt(N); row++)
        for (int col = 0; col < (int)sqrt(N); col++)
            if (grid[row+boxStartRow][col+boxStartCol] == num)
                return true;
    return false;
}

/* Returns a boolean which indicates whether it will be legal to assign
   num to the given row,col location. */
bool isSafe(int grid[N][N], int row, int col, int num)
{
    /* Check if 'num' is not already placed in current row,
       current column and current 4x4 box */
    return !UsedInRow(grid, row, num) &&
           !UsedInCol(grid, col, num) &&
           !UsedInBox(grid, row - row%(int)sqrt(N) , col - col%(int)sqrt(N), num);
}
 
/* A utility function to print grid  */
void printGrid(int grid[N][N])
{
    for (int row = 0; row < N; row++)
    {
       for (int col = 0; col < N; col++)
             printf("%3d", grid[row][col]);
        printf("\n");
    }
}

void buildMap(vector< vector< unordered_set<ushort> > > &sets, int grid[N][N]) {
  for (int i = 0; i < N; i++) {
    vector< unordered_set<ushort> > row;
    for (int j = 0; j < N; j++) {
      if (grid[i][j] != UNASSIGNED) {
        unordered_set<ushort> empty;
        row.push_back(empty);
      } else {
        unordered_set<ushort> domain({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
        row.push_back(domain);
      }
    }
    sets.push_back(row);
  }
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (grid[i][j] != UNASSIGNED) {
        for (int k = 0; k < N; k++) sets[i][k].erase(grid[i][j]);
        for (int k = 0; k < N; k++) sets[k][j].erase(grid[i][j]);
        for (int ii = i - i%(int)sqrt(N); ii < i - i%(int)sqrt(N) + sqrt(N); ii++) {
          for (int jj = j - j%(int)sqrt(N); jj < j - j%(int)sqrt(N) + sqrt(N); jj++) {
            sets[ii][jj].erase(grid[i][j]);
          }
        }
      }
    }
  }
}
 
/* Driver Program to test above functions */
int main()
{
    // 0 means unassigned cells
    int grid[N][N] = {
{  11,   2,   0,   0,   0,   3,   0,   0,  15,   9,   0,   0,  16,   0,  12,    0 },
{   1,   9,   0,   0,  11,  12,   0,   0,   0,   5,   0,   0,   0,   0,   0,    2 },
{   0,   0,   0,   0,   5,   0,   4,  16,   0,   0,   7,   0,   0,   0,   9,    0 },
{   0,   0,   0,   0,   7,   0,   0,   0,  12,   0,  13,   0,   0,   0,  10,   11 },
{   0,   0,   2,   5,   0,  15,   0,   0,   0,   0,   0,   0,  14,   4,   0,    6 },
{   0,   0,   0,   4,  10,   2,   5,   7,   6,  14,   0,   0,   0,   0,   0,   16 },
{   0,   8,   0,   0,  12,   0,   0,   0,   0,   4,   3,   0,   0,   0,   0,    0 },
{   0,   0,   9,   0,   0,   0,   6,  14,   0,  10,   0,   0,  15,   0,   0,    0 },
{   3,   1,   0,   0,   0,  10,   0,   0,  11,   0,  15,   0,  12,   0,   8,    0 },
{  13,   0,   0,  10,   0,  14,  16,   0,   0,   1,   8,   0,   6,   0,   0,    0 },
{  16,   7,   0,   0,   3,   0,   8,   0,   0,  12,   0,   5,   0,   9,  11,    0 },
{  14,   5,  15,   0,   0,   1,   0,   0,   7,   0,   6,  10,  13,   0,   0,    0 },
{  15,   0,   0,   0,   0,   0,   0,   0,   8,   0,   0,   1,   9,   0,   0,    0 },
{   0,   0,   0,   0,   0,   0,  14,  13,   0,   0,   0,   0,   0,   5,   0,    8 },
{   0,   4,   0,   6,   0,   5,   0,   0,   0,   0,  14,   3,  11,   0,   0,   12 },
{   0,  13,   0,   0,   0,   0,   2,   0,   0,   7,  16,   0,   3,   0,  15,   14 },
};
    vector< vector< unordered_set<ushort> > > sets;
    clock_t t1;
    t1 = clock();
    buildMap(sets, grid);
    if (SolveSudoku(grid, sets) == true)
          printGrid(grid);
    else
         printf("No solution exists");
    unsigned timeElapsed = (clock() - t1) / CLOCKS_PER_MS;
    cout << timeElapsed << endl;
    return 0;
}