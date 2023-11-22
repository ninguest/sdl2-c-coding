#include <stdio.h>
#include <string.h>
#include <math.h>
#include "easyGameLogic.h"

/* Constants Initialisation */
#define DATASET_PATH "tic-tac-toe.data"
#define POSITIVE_OUTCOMES 626
#define POSITIVE_TRAINING 500                                  // 626 * 0.8 = 500 of the 626 data with positive outcome used for training.
#define POSITIVE_TESTING POSITIVE_OUTCOMES - POSITIVE_TRAINING // The remaining data with positive outcome used for testing.
#define NEGATIVE_OUTCOMES 332
#define NEGATIVE_TRAINING 265                                  // 332 * 0.8 = 265 of the 332 data with negative outcome used for training.
#define NEGATIVE_TESTING NEGATIVE_OUTCOMES - NEGATIVE_TRAINING // 332 - 265 = 67 of the 332 data with negative outcome used for testing.

#define TRAINING_ELEMENTS POSITIVE_TRAINING + NEGATIVE_TRAINING
#define TESTING_ELEMENTS POSITIVE_TESTING + NEGATIVE_TESTING

#define BLANK 0
#define X 1
#define O -1

#define WIN 1
#define LOSE -1

double trainingDatasetGameState[TRAINING_ELEMENTS][9];
double trainingOutcome[TRAINING_ELEMENTS];
double testingDatasetGameState[TESTING_ELEMENTS][9];
double testingOutcome[TESTING_ELEMENTS];
double weight[9] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
double bias = 0.0;

void getDatasetML();
void trainLR();
void printErrorProbability();
int easyComputerTurn(int gameState[9]);
int assignMoveValue(char *move);
int assignOutcomeValue(char *outcome);
double predict(double gameState[9]);
void gradientDescent(double gameState[9], double outcome, double prediction, double learningRate);

/* Returns the position the computer has chosen based on linear regression algorithm. */
int easyComputerTurn(int gameState[9])
{
    int backupMove = 999;           // In case there is no winning move, we resort to using the last available move.
    double convertedGameState[9]; // The predict function expects an array of doubles.
    for (int i = 0; i < 9; i++)
    {
        // Converts the game state which is in int to double.
        if (gameState[i] == 2)
            convertedGameState[i] = (double)-1;
        convertedGameState[i] = (double)gameState[i];
    }

    for (int i = 0; i < 9; i++)
    {
        // Executes when the current square is empty.
        if (gameState[i] == 0)
        {
            backupMove = i;                               // Sets the backup move as the current square.
            convertedGameState[i] = (double)-1;           // Sets the current empty square to O.
            double outcome = predict(convertedGameState); // Predicts the outcome after setting.

            // Since the outcome of the dataset only represents X,
            // we have to flip the logic around and treat negative outcomes
            // as positive since the game is programmed in the sense that the
            // computer always uses the O piece.
            if (outcome < 0)
            {
                return i; // Returns the current square position if the outcome is perceived to be positive.
            }
            else
                convertedGameState[i] = (double)0; // Resets the current square back to empty to not mess with subsequent calculations.
        }
    }
    if (backupMove == 999) return 11;

    return backupMove; // Returns the backup move if there are no positive outcomes.
}

/* Loads the tic-tac-toe data file and stores each data point into their respective arrays. */
void getDatasetML()
{
    FILE *fp;                      // Inititalises the file pointer.
    fp = fopen(DATASET_PATH, "r"); // Opens the tic-tac-toe.data file.
    char string[30];               // Used to store a line of the tic-tac-toe.data file.
    int trainingCounter = 0;
    int testingCounter = 0;
    int count = 0;

    // Iterating through every line in the dataset file.
    while (fgets(string, 30, fp))
    {
        char *token = strtok(string, ","); // Gets the first move from the dataset.

        // Loop to assign data into the its respective arrays.
        for (int i = 0; i < 10; i++)
        {
            // This executes when iterating through game states.
            if (i != 9)
            {
                int moveValue = assignMoveValue(token);
                // This executes when the counter is less than 500.
                if (count < POSITIVE_TRAINING)
                {
                    trainingDatasetGameState[trainingCounter][i] = moveValue; // Appends the move to the trainingDatasetGameState array.
                }
                // This executes when the counter is between 500 and 626.
                else if (count >= POSITIVE_TRAINING && count < POSITIVE_OUTCOMES)
                {
                    testingDatasetGameState[testingCounter][i] = moveValue; // Appends the move to the testingDatasetGameState array.
                }
                // This executes when the counter is between 626 and 891 (626 + 265).
                else if (count >= POSITIVE_OUTCOMES && count < POSITIVE_OUTCOMES + NEGATIVE_TRAINING)
                {
                    trainingDatasetGameState[trainingCounter][i] = moveValue; // Appends the move to the trainingDatasetGameState array.
                }
                else
                {
                    testingDatasetGameState[testingCounter][i] = moveValue; // Appends the move to the testingDatasetGameState array.
                }
                token = strtok(NULL, ",");
            }
            // This executes when it's the last element, which is the game outcome.
            else
            {
                int targetOutcome = assignOutcomeValue(token);
                // This executes when the counter is less than 500.
                if (count < POSITIVE_TRAINING)
                {
                    trainingOutcome[trainingCounter] = targetOutcome; // Appends the outcome to the trainingOutcome array.
                    trainingCounter++;                                // Increments the training counter by 1.
                }
                // This executes when the counter is between 500 and 626.
                else if (count >= POSITIVE_TRAINING && count < POSITIVE_OUTCOMES)
                {
                    testingOutcome[testingCounter] = targetOutcome; // Appends the outcome to the testingOutcome array.
                    testingCounter++;                               // Increments the testing counter by 1.
                }
                // This executes when the counter is between 626 and 891 (626 + 265).
                else if (count >= POSITIVE_OUTCOMES && count < POSITIVE_OUTCOMES + NEGATIVE_TRAINING)
                {
                    trainingOutcome[trainingCounter] = targetOutcome; // Appends the outcome to the trainingOutcome array.
                    trainingCounter++;                                // Increments the training counter by 1.
                }
                else
                {
                    testingOutcome[testingCounter] = targetOutcome; // Appends the outcome to the testingOutcome array.
                    testingCounter++;                               // Increments the testing counter by 1.
                }
            }
        }
        count++; // Increments the counter by 1.
    }

    fclose(fp); // Closes the file pointer.
}

/* Assigns an integer value to the move. */
int assignMoveValue(char *move)
{
    // If the symbol is X, assign value 1 to it.
    if (strcmp(move, "x") == 0)
    {
        return X;
    }
    // If the symbol is O, assign value -1 to it.
    else if (strcmp(move, "o") == 0)
    {
        return O;
    }
    else
        return BLANK; // If there is no symbol, assign value 0 to it.
}


/* Assigns an integer value to the outcome. */
int assignOutcomeValue(char *outcome)
{
    // If the outcome is negative, assign value -1 to it.
    if (strcmp(outcome, "negative\n") == 0)
    {
        return LOSE;
    }
    // If the outcome is positive, assign value 1 to it.
    else if (strcmp(outcome, "positive\n") == 0)
    {
        return WIN;
    }
    
    // Add a default return statement here
    return 0;
}

/* Predicts the outcome based on the game state. */
double predict(double gameState[9])
{
    double sum = bias;

    // Loops through each square of the game board.
    for (int i = 0; i < 9; i++)
    {
        sum += weight[i] * gameState[i]; // Calculates the outcome.
    }
    return sum;
}

/* Performs the gradient descent for linear regression. */
void gradientDescent(double gameState[9], double outcome, double prediction, double learningRate)
{
    // Loops through each square of the game board.
    for (int i = 0; i < 9; i++)
    {
        double derivativeWeight = (prediction - outcome) * gameState[i]; // Calculates the derivative weight.
        weight[i] -= learningRate * derivativeWeight;                    // Updates the weight.
    }
    double derivativeBias = prediction - outcome; // Calculates the derivative bias.
    bias -= learningRate * derivativeBias;        // Updates the bias.
}

/* Trains the linear regression. */
void trainLR()
{
    // Loops through the number of times to go through the dataset.
    for (int i = 0; i < 1000; i++)
    {
        // Loops through the data points set aside for training.
        for (int j = 0; j < TRAINING_ELEMENTS; j++)
        {
            double prediction = predict(trainingDatasetGameState[i]);
            gradientDescent(trainingDatasetGameState[i], trainingOutcome[i], prediction, 0.0000001);
        }
    }
}

/* Prints the probability of error. */
void printErrorProbability()
{
    int wrongTrainingPredictionCount = 0;
    int wrongTestingPredictionCount = 0;

    int trainingTruePositiveCount = 0;
    int trainingTrueNegativeCount = 0;
    int trainingFalsePositiveCount = 0;
    int trainingFalseNegativeCount = 0;

    int testingTruePositiveCount = 0;
    int testingTrueNegativeCount = 0;
    int testingFalsePositiveCount = 0;
    int testingFalseNegativeCount = 0;

    // Loops through the training data points.
    for (int i = 0; i < TRAINING_ELEMENTS; i++)
    {
        double predictedOutcome = predict(trainingDatasetGameState[i]);
        // Executes when the actual outcome is WIN and predicted outcome is WIN (TP).
        if (trainingOutcome[i] > 0 && predictedOutcome > 0)
        {
            trainingTruePositiveCount++;
        }

        // Executes when the actual outcome is LOSE and predicted outcome is LOSE (TN).
        if (trainingOutcome[i] < 0 && predictedOutcome < 0)
        {
            trainingTrueNegativeCount++;
        }

        // Executes when the actual outcome is LOSE but the predicted outcome is WIN (FP).
        if (trainingOutcome[i] < 0 && predictedOutcome > 0)
        {
            wrongTrainingPredictionCount++;
            trainingFalsePositiveCount++;
        }

        // Executes when the actual outcome is WIN but the predicted outcome is LOSE (FN).
        if (trainingOutcome[i] > 0 && predictedOutcome < 0)
        {
            wrongTrainingPredictionCount++;
            trainingFalseNegativeCount++;
        }
    }

    // Loops through the testing data points.
    for (int j = 0; j < TESTING_ELEMENTS; j++)
    {
        double predictedOutcome = predict(testingDatasetGameState[j]);
        // Executes when the actual outcome is WIN and predicted outcome is WIN (TP).
        if (testingOutcome[j] > 0 && predictedOutcome > 0)
        {
            testingTruePositiveCount++;
        }

        // Executes when the actual outcome is LOSE and predicted outcome is LOSE (TN).
        if (testingOutcome[j] < 0 && predictedOutcome < 0)
        {
            testingTrueNegativeCount++;
        }

        // Executes when the actual outcome is LOSE but the predicted outcome is WIN (FP).
        if (testingOutcome[j] < 0 && predictedOutcome > 0)
        {
            wrongTestingPredictionCount++;
            testingFalsePositiveCount++;
        }

        // Executes when the actual outcome is WIN but the predicted outcome is LOSE (FN).
        if (testingOutcome[j] > 0 && predictedOutcome < 0)
        {
            wrongTestingPredictionCount++;
            testingFalseNegativeCount++;
        }
    }

}
