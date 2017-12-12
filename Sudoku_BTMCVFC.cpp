// A Backtracking program  in C++ to solve Sudoku problem
#include <stdio.h>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <math.h>
#include <unordered_set>
#include <unordered_map>
#include <vector>

using namespace std;
using namespace std::chrono;

// UNASSIGNED is used for empty cells in sudoku grid
#define UNASSIGNED 0
 
// N is used for size of Sudoku grid. Size will be NxN
#define N 16
 
void LCV(const vector< vector< unordered_set<ushort> > > &sets, vector<ushort> &candidates, int row, int col);

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
    
    // vector<ushort> candidates;
    // LCV(sets, candidates, row, col);

    // consider digits LCV
    for (auto num : sets[row][col]) {
      // LCV
      // if (sets[row][col].find(num) == sets[row][col].end()) continue;
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

bool sortbysec(const pair<ushort,ushort> &a,
              const pair<ushort,ushort> &b)
{
    return (a.second < b.second);
}

void LCV(const vector< vector< unordered_set<ushort> > > &sets, vector<ushort> &candidates, int row, int col) {
  unordered_map<ushort, ushort> hashmap;

  for (int k = 0; k < N; k++) {
    for (auto num : sets[row][k]) {
      hashmap[num] += 1;
    }
  }
  for (int k = 0; k < N; k++) {
    for (auto num : sets[k][col]) {
      hashmap[num] += 1;
    }
  }
  for (int ii = row - row%(int)sqrt(N); ii < row - row%(int)sqrt(N) + sqrt(N); ii++) {
    for (int jj = col - col%(int)sqrt(N); jj < col - col%(int)sqrt(N) + sqrt(N); jj++) {
      for (auto num : sets[ii][jj]) {
        hashmap[num] += 1;
      }
    }
  }
  vector< pair<ushort, ushort> > v(hashmap.begin(), hashmap.end());
  sort(v.begin(), v.end(), sortbysec);
  for (auto num : v) {
    candidates.push_back(num.first);
  }
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
{   0,   3,   0,   0,  12,  16,   0,   0,   0,   0,  15,   0,   9,   0,   0,    0 },
{   0,   0,  13,  12,   9,  14,  11,   0,  16,   0,   0,   0,   0,   0,   0,   10 },
{   0,   0,  10,   0,   0,   4,  15,   5,   0,   0,   9,  12,  14,   0,   0,   13 },
{   0,  16,   0,   0,  13,   0,   0,   0,   0,  10,   3,   5,   0,   0,   0,   15 },
{   7,  12,   9,   0,   0,   0,   0,  15,   0,   0,   0,   0,   5,   0,  13,    1 },
{   5,  11,   0,   0,   0,  10,   0,   0,   0,  15,   0,   0,   0,  14,   0,   16 },
{  15,   0,   0,   0,   5,   0,   0,   0,   0,   0,   0,   9,   0,   0,   3,    0 },
{   0,   0,  16,  13,   0,  12,   3,   0,   2,   0,   8,   6,   0,   0,  15,    0 },
{  12,  13,   0,  15,   0,   0,   0,  16,   0,   0,   0,   0,   0,   8,   1,    7 },
{   8,   0,   0,  16,  10,  15,   0,   0,  14,  11,   0,   0,   6,   0,   0,    5 },
{   0,   0,  11,   0,   0,   0,   2,   0,   0,   0,   0,   0,  12,   0,   0,    0 },
{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   5,  10,   0,   0,   9,    4 },
{   0,   7,   0,   0,   0,  11,   0,  12,   0,   0,   0,   0,   8,  16,   6,    0 },
{   0,   8,   0,   0,  16,   6,  10,   7,  11,  13,   0,  15,   0,   0,   0,    0 },
{   0,   0,   0,   0,  14,   0,   0,   0,   6,   0,  16,   0,   0,   0,   0,   11 },
{   0,   0,   0,   0,   0,   0,   4,   0,   8,   3,  14,   1,  15,   0,   0,    0 }
};
    vector< vector< unordered_set<ushort> > > sets;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    buildMap(sets, grid);
    if (SolveSudoku(grid, sets) == true)
          printGrid(grid);
    else
         printf("No solution exists");
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>( t2 - t1 ).count();
    cout << "milliseconds: " << duration << endl;
    return 0;
}