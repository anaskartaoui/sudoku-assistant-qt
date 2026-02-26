#ifndef SUDOKUSOLVER_H
#define SUDOKUSOLVER_H

/**
 * @brief The SudokuSolver class
 * Pure logic class (no QObject) — provides static utility methods.
 * Used by the controller for hint generation and solution validation.
 */
class SudokuSolver
{
public:
    SudokuSolver() = delete;

    // Returns true if a solution exists for the given grid
    static bool hasSolution(int grid[9][9]);

    // Fills 'solution' with a valid solution, returns false if unsolvable
    static bool solve(int grid[9][9], int solution[9][9]);

    // Returns true if placing 'value' at (row,col) is valid
    static bool isValidPlacement(const int grid[9][9], int row, int col, int value);

private:
    static bool backtrack(int grid[9][9]);
};

#endif // SUDOKUSOLVER_H
