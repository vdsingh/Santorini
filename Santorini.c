#include <stdio.h>
#include <stdlib.h>

//These chars keep track of the values of what is underneath each builder.
char occupiedPlayerValue = '2';
char occupiedAIValue = '2';

//These ints keep track of the player's current position
int playerRow = -1;
int playerCol = -1;

//These ints keep track of the AI's current position
int AIRow = -1;
int AICol = -1;

//These ints keep track of the number of 0 values and 4 values on the board
int numZeroValues = 0;
int numFourValues = 0;

// Finds the absolute value given an int.
//   param num: int that we're trying to find the absolute value of.
//   return: an int that is the absolute value of the int passed in.
int absVal(int num){
    if (num < 0){
        return num * -1;
    }
    return num;
}

// Prints the game board 
//   param matrix: 2D array of chars that holds the data for the state of the game board
void printBoard(char matrix[6][6]) {
    printf("  ");
    //prints the column indicators
    for (int i = 1; i <= 6; i++) {
        printf("%2i", i);
    }
    printf("\n");
    for (int i = 0; i < 6; i++) {
        //print the row indicators
        printf("%i ", i + 1);
        for (int j = 0; j < 6; j++) {
            printf("%2c", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Deterimes whether the player's desired move is a valid move or not
//   param destinationRow: int that represents the desired row that the player wants to move their builder to
//   param destinationCol: int that represents the desired column that the player wants to move their builder to
//   param matrix: 2D array of chars that holds the data for the state of the game board
//   return: an int that represents whether the desired position that the player wants to move their builder to is valid or not (0 = invalid, 1 = valid)
int isValidMove(int destinationRow, int destinationCol, char matrix[6][6]) {
    if (destinationRow < 0 || destinationRow > 5 || destinationCol < 0 || destinationCol > 5) {
        printf("That location is outside of the bounds of the board. Please enter a new location.\n");
        return 0;
    }
    if (playerRow == -1 && playerCol == -1) { //the user is moving for the first time, so P doesn't exist in the matrix
        return 1;
    }
    if (destinationRow == playerRow && destinationCol == playerCol) {
        printf("Your builder is already in that location. Please enter a new location. \n");
        return 0;
    }
    if (matrix[destinationRow][destinationCol] == 'A') {
        printf("You cannot move to that location because the other Player's builder is in that location. Please enter a new location.\n");
        return 0;
    }
    if (absVal(destinationRow - playerRow) > 1 || absVal(destinationCol - playerCol) > 1) {
        printf("That location is not adjacent to your Builder's current location. Please enter a new location.\n");
        return 0;
    }
    return 1;
}

// Places the AI on the screen at the beginning of the game either left or right of the player
//   param matrix: 2D array of chars that holds the data for the state of the game board
void placeAI(char matrix[6][6]) {
    if (playerCol == 5) {
        matrix[playerRow][playerCol - 1] = 'A';
        AIRow = playerRow;
        AICol = playerCol - 1;
    } else {
        matrix[playerRow][playerCol + 1] = 'A';
        AIRow = playerRow;
        AICol = playerCol + 1;
    }
}

// Method that is used for setting up the game in the beginning. Calls functions and gets user input to get the game set up
//   param matrix: 2D array of chars that holds the data for the state of the game board
void setGame(char matrix[6][6]) {
    printBoard(matrix);
    int startRow = -1;
    int startCol = -1;
    printf("Please enter the row and column that you would like to place your builder in.\n");
    scanf("%i %i", & startRow, & startCol);
    startRow -= 1;
    startCol -= 1;
    while (isValidMove(startRow, startCol, matrix) == 0) {
        printf("Please enter the row and column that you would like to place your builder in.\n");
        scanf("%i %i", & startRow, & startCol);
        startRow -= 1;
        startCol -= 1;
    }
    playerRow = startRow;
    playerCol = startCol;
    matrix[playerRow][playerCol] = 'P';
    placeAI(matrix);
    printBoard(matrix);
}

// Given a matrix, adds an int to all values in octagonal directions of a given position. Takes into consideration situations 
//   where a direction may be blocked by a builder.
//   param numToBuild: int that is added to all values in octagonal directions of the given location (1 for player, -1 for AI)
//   param builderRow: int that represents the row that the builder is in. This, in part, supplies the position from which we want to build out octagonally
//   param builderCol: int that represents the column that the builder is in. This, in part, supplies the position from which we want to build out octagonally
//   param matrix: 2D array of chars that holds the data for the state of the game board
void buildOctagonally(int numToBuild, int builderRow, int builderCol, char matrix[6][6]) {
    for (int i = builderCol + 1; i < 6; i++) { //This for loop fills in all rows to the right.
        if (matrix[builderRow][i] == 'A' || matrix[builderRow][i] == 'P') {
            if (numToBuild == -1) {
                printf("AI builder encountered another builder.\n");
            }
            break;
        } else {
            int valInSpot = matrix[builderRow][i] - '0';
            char newVal = '0' + (valInSpot + numToBuild);
            if (numToBuild == -1) {
                if (valInSpot == 4) {
                    numFourValues -= 1;
                } else if (valInSpot == 1) {
                    numZeroValues += 1;
                }
            } else if (numToBuild == 1) {
                if (valInSpot == 0) {
                    numZeroValues -= 1;
                } else if (valInSpot == 3) {
                    numFourValues += 1;
                }
            }
            if (valInSpot + numToBuild <= 4 && valInSpot + numToBuild >= 0)
                matrix[builderRow][i] = newVal;
        }
    }
    for (int i = builderCol - 1; i >= 0; i--) { //This for loop fills in all vals to the left.
        if (matrix[builderRow][i] == 'A' || matrix[builderRow][i] == 'P') {
            if (numToBuild == -1) {
                printf("AI builder encountered another builder.\n");
            }
            break;
        } else {
            int valInSpot = matrix[builderRow][i] - '0';
            char newVal = '0' + (valInSpot + numToBuild);
            if (numToBuild == -1) {
                if (valInSpot == 4) {
                    numFourValues -= 1;
                } else if (valInSpot == 1) {
                    numZeroValues += 1;
                }
            } else if (numToBuild == 1) {
                if (valInSpot == 0) {
                    numZeroValues -= 1;
                } else if (valInSpot == 3) {
                    numFourValues += 1;
                }
            }
            if (valInSpot + numToBuild <= 4 && valInSpot + numToBuild >= 0)
                matrix[builderRow][i] = newVal;
        }
    }
    for (int i = builderRow + 1; i < 6; i++) { //This for loop fills in all vals down.
        if (matrix[i][builderCol] == 'A' || matrix[i][builderCol] == 'P') {
            if (numToBuild == -1) {
                printf("AI builder encountered another builder.\n");
            }
            break;
        } else {
            int valInSpot = matrix[i][builderCol] - '0';
            char newVal = '0' + (valInSpot + numToBuild);
            if (numToBuild == -1) {
                if (valInSpot == 4) {
                    numFourValues -= 1;
                } else if (valInSpot == 1) {
                    numZeroValues += 1;
                }
            } else if (numToBuild == 1) {
                if (valInSpot == 0) {
                    numZeroValues -= 1;
                } else if (valInSpot == 3) {
                    numFourValues += 1;
                }
            }
            if (valInSpot + numToBuild <= 4 && valInSpot + numToBuild >= 0)
                matrix[i][builderCol] = newVal;
        }
    }
    for (int i = builderRow - 1; i >= 0; i--) { //This for loop fills in vals up.
        if (matrix[i][builderCol] == 'A' || matrix[i][builderCol] == 'P') {
            if (numToBuild == -1) {
                printf("AI builder encountered another builder.\n");
            }
            break;
        } else {
            int valInSpot = matrix[i][builderCol] - '0';
            char newVal = '0' + (valInSpot + numToBuild);
            if (numToBuild == -1) {
                if (valInSpot == 4) {
                    numFourValues -= 1;
                } else if (valInSpot == 1) {
                    numZeroValues += 1;
                }
            } else if (numToBuild == 1) {
                if (valInSpot == 0) {
                    numZeroValues -= 1;
                } else if (valInSpot == 3) {
                    numFourValues += 1;
                }
            }
            if (valInSpot + numToBuild <= 4 && valInSpot + numToBuild >= 0)
                matrix[i][builderCol] = newVal;
        }
    }
    for (int i = builderRow + 1, j = builderCol + 1; i < 6 && j < 6; i++, j++) { //This for loop fills in all vals down and right
        if (matrix[i][j] == 'P' || matrix[i][j] == 'A') {
            if (numToBuild == -1) {
                printf("AI builder encountered another builder.\n");
            }
            break;
        } else {
            int valInSpot = matrix[i][j] - '0';
            char newVal = '0' + (valInSpot + numToBuild);
            if (numToBuild == -1) {
                if (valInSpot == 4) {
                    numFourValues -= 1;
                } else if (valInSpot == 1) {
                    numZeroValues += 1;
                }
            } else if (numToBuild == 1) {
                if (valInSpot == 0) {
                    numZeroValues -= 1;
                } else if (valInSpot == 3) {
                    numFourValues += 1;
                }
            }
            if (valInSpot + numToBuild <= 4 && valInSpot + numToBuild >= 0)
                matrix[i][j] = newVal;
        }
    }
    for (int i = builderRow - 1, j = builderCol + 1; i >= 0 && j < 6; i--, j++) { //This for loop fills in all vals up and right
        if (matrix[i][j] == 'A' || matrix[i][j] == 'P') {
            if (numToBuild == -1) {
                printf("AI builder encountered another builder.\n");
            }
            break;
        } else {
            int valInSpot = matrix[i][j] - '0';
            char newVal = '0' + (valInSpot + numToBuild);
            if (numToBuild == -1) {
                if (valInSpot == 4) {
                    numFourValues -= 1;
                } else if (valInSpot == 1) {
                    numZeroValues += 1;
                }
            } else if (numToBuild == 1) {
                if (valInSpot == 0) {
                    numZeroValues -= 1;
                } else if (valInSpot == 3) {
                    numFourValues += 1;
                }
            }
            if (valInSpot + numToBuild <= 4 && valInSpot + numToBuild >= 0)
                matrix[i][j] = newVal;
        }
    }
    for (int i = builderRow - 1, j = builderCol - 1; i >= 0 && j >= 0; i--, j--) {
        if (matrix[i][j] == 'A' || matrix[i][j] == 'P') {
            if (numToBuild == -1) {
                printf("AI builder encountered another builder.\n");
            }
            break;
        } else {
            int valInSpot = matrix[i][j] - '0';
            char newVal = '0' + (valInSpot + numToBuild);
            if (numToBuild == -1) {
                if (valInSpot == 4) {
                    numFourValues -= 1;
                } else if (valInSpot == 1) {
                    numZeroValues += 1;
                }
            } else if (numToBuild == 1) {
                if (valInSpot == 0) {
                    numZeroValues -= 1;
                } else if (valInSpot == 3) {
                    numFourValues += 1;
                }
            }
            if (valInSpot + numToBuild <= 4 && valInSpot + numToBuild >= 0)
                matrix[i][j] = newVal;
        }
    }
    for (int i = builderRow + 1, j = builderCol - 1; i < 6 && j >= 0; i++, j--) { //This for loop fills in all vals up and right
        if (matrix[i][j] == 'A' || matrix[i][j] == 'P') {
            if (numToBuild == -1) {
                printf("AI builder encountered another builder.\n");
            }
            break;
        } else {
            int valInSpot = matrix[i][j] - '0';
            char newVal = '0' + (valInSpot + numToBuild);
            if (numToBuild == -1) {
                if (valInSpot == 4) {
                    numFourValues -= 1;
                } else if (valInSpot == 1) {
                    numZeroValues += 1;
                }
            } else if (numToBuild == 1) {
                if (valInSpot == 0) {
                    numZeroValues -= 1;
                } else if (valInSpot == 3) {
                    numFourValues += 1;
                }
            }
            if (valInSpot + numToBuild <= 4 && valInSpot + numToBuild >= 0)
                matrix[i][j] = newVal;
        }
    }
}

// Calculates the best possible move for the AI to take and handles the move (makes changes to the given matrix)
//   param matrix: 2D array of chars that holds the data for the state of the game board
void moveAI(char matrix[6][6]) {
    int bestValuesDecreased = 0;
    int directionToMove = 0; //0 = left, 1 = right, 2 = up, 3 = down, 4 = up right, 5 = down right, 6 = up left, 7 = down left
    //proposedPositions is a 2D array that keeps track of all possible moves. Values from this 2D array are used as builder positions when simulating the AI's move.
    int proposedPositions[8][2] = {{AIRow,AICol - 1},{AIRow,AICol + 1},{AIRow - 1,AICol},{AIRow + 1,AICol},{AIRow - 1,AICol + 1},{AIRow + 1,AICol + 1},{AIRow - 1,AICol - 1},{AIRow + 1,AICol - 1}};
    for (int i = 0; i < 8; i++) { //testing all possible moves
        int valuesDecreasedForMove = 1;
        int proposedRow = proposedPositions[i][0];
        int proposedCol = proposedPositions[i][1];
        if (matrix[proposedRow][proposedCol] == 'P' || proposedCol > 5 || proposedCol < 0 || proposedRow > 5 || proposedRow < 0) { //if the proposed move is in a spot that P is in, or is outside the bounds of the board, just skip the iteration.
            continue;
        }
        for (int j = proposedCol + 1; j < 6; j++) { //simulate all values to the right
            if (matrix[proposedRow][j] == 'P') {
                break;
            } else if (matrix[proposedRow][j] != '0') {
                valuesDecreasedForMove++;
            }
        }
        for (int j = proposedCol - 1; j >= 0; j--) { //simulate all values to the left
            if (matrix[proposedRow][j] == 'P') {
                break;
            } else if (matrix[proposedRow][j] != '0') {
                valuesDecreasedForMove++;
            }
        }
        for (int j = proposedRow - 1; j >= 0; j--) { //simulate all values above
            if (matrix[j][proposedCol] == 'P') {
                break;
            } else if (matrix[j][proposedCol] != '0') {
                valuesDecreasedForMove++;
            }
        }
        for (int j = proposedRow + 1; j < 6; j++) { //simulate all values below
            if (matrix[j][proposedCol] == 'P') {
                break;
            } else if (matrix[j][proposedCol] != '0') {
                valuesDecreasedForMove++;
            }
        }
        for (int j = proposedRow - 1, k = proposedCol + 1; j >= 0 && k < 6; j--, k++) { //simulate all values up right
            if (matrix[j][k] == 'P') {
                break;
            } else if (matrix[j][k] != '0') {
                valuesDecreasedForMove++;
            }
        }
        for (int j = proposedRow + 1, k = proposedCol + 1; j < 6 && k < 6; j++, k++) { //simulate all values down right
            if (matrix[j][k] == 'P') {
                break;
            } else if (matrix[j][k] != '0') {
                valuesDecreasedForMove++;
            }
        }
        for (int j = proposedRow - 1, k = proposedCol - 1; j >= 0 && k >= 0; j--, k--) { //simulate all values up left
            if (matrix[j][k] == 'P') {
                break;
            } else if (matrix[j][k] != '0') {
                valuesDecreasedForMove++;
            }
        }
        for (int j = proposedRow + 1, k = proposedCol - 1; j < 6 && k >= 0; j++, k--) { //simulate all values up down left
            if (matrix[j][k] == 'P') {
                break;
            } else if (matrix[j][k] != '0') {
                valuesDecreasedForMove++;
            }
        }
        if (valuesDecreasedForMove > bestValuesDecreased) {
            directionToMove = i;
            bestValuesDecreased = valuesDecreasedForMove;
        }
    }
    // printf("the best values decreased in this case was %i\n", bestValuesDecreased);
    matrix[AIRow][AICol] = occupiedAIValue;
    AIRow = proposedPositions[directionToMove][0];
    AICol = proposedPositions[directionToMove][1];
    if(matrix[AIRow][AICol] == '0'){
        occupiedAIValue = matrix[AIRow][AICol];
    }else{
        occupiedAIValue = matrix[AIRow][AICol] - 1;
    }
    matrix[AIRow][AICol] = 'A';

    // printf("the best move for the AI to take here is: row %i, col %i which is direction %i\n", proposedPositions[directionToMove][0], proposedPositions[directionToMove][1], directionToMove);

    buildOctagonally(-1, AIRow, AICol, matrix);
}

// Handles running the majority of the game (apart from setting it up). Gets user input and calls helper methods to run the game.
//   param matrix: 2D array of chars that holds the data for the state of the game board
void playRound(char matrix[6][6]) {
    int destinationRow;
    int destinationCol;
    printf("Please enter the row and column that you would like to move your builder to.\n");
    scanf("%i %i", & destinationRow, & destinationCol);
    destinationRow -= 1;
    destinationCol -= 1;
    while (isValidMove(destinationRow, destinationCol, matrix) == 0) {
        printf("Please enter the row and column that you would like to move your builder to.\n");
        scanf("%i %i", & destinationRow, & destinationCol);
        destinationRow -= 1;
        destinationCol -= 1;
    }
    matrix[playerRow][playerCol] = occupiedPlayerValue;
    occupiedPlayerValue = matrix[destinationRow][destinationCol];
    matrix[destinationRow][destinationCol] = 'P';
    playerRow = destinationRow;
    playerCol = destinationCol;

    buildOctagonally(1, playerRow, playerCol, matrix);
    printBoard(matrix);
    moveAI(matrix);
    printBoard(matrix);
}

// main method is where the game starts and ends. Continuously calls playRound until the game over conditions are met. This is also where the matrix is created.
//   where a direction may be blocked by a builder.
int main(void) {
    char matrix[6][6] = {{'2','2','2','2','2','2'},{'2','2','2','2','2','2'},{'2','2','2','2','2','2'},{'2','2','2','2','2', '2'},{'2','2','2','2','2','2'},{'2','2','2','2','2','2'}};
    printf("the starting row and col is: %i %i \n", playerRow, playerCol);
    setGame(matrix);

    while (numZeroValues < 10 && numFourValues < 10) { //!condition that means the game is over
        playRound(matrix);
    }
    if (numZeroValues >= 10) {
        printf("Game Over! You lose!");
    } else if (numFourValues >= 10) {
        printf("Game Over! You win!");
    }
    return 0;
}