#include "SudokuSolver.h"
#include <cstring>

bool SudokuSolver::isValidPlacement(const int grid[9][9], int row, int col, int value)
{
    // Check row
    for (int c = 0; c < 9; ++c)
        if (grid[row][c] == value) return false;

    // Check column
    for (int r = 0; r < 9; ++r)
        if (grid[r][col] == value) return false;

    // Check 3x3 block
    int blockRow = (row / 3) * 3;
    int blockCol = (col / 3) * 3;
    for (int r = blockRow; r < blockRow + 3; ++r)
        for (int c = blockCol; c < blockCol + 3; ++c)
            if (grid[r][c] == value) return false;

    return true;
}

bool SudokuSolver::backtrack(int grid[9][9])
{
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (grid[r][c] == 0) {
                for (int v = 1; v <= 9; ++v) {
                    if (isValidPlacement(grid, r, c, v)) {
                        grid[r][c] = v;
                        if (backtrack(grid)) return true;
                        grid[r][c] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

bool SudokuSolver::hasSolution(int grid[9][9])
{
    int copy[9][9];
    std::memcpy(copy, grid, sizeof(copy));
    return backtrack(copy);
}

bool SudokuSolver::solve(int grid[9][9], int solution[9][9])
{
    std::memcpy(solution, grid, sizeof(int) * 9 * 9);
    return backtrack(solution);
}
