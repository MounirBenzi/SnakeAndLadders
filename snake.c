#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Structure for the players in the game
typedef struct player{
  int xPos; //Players x-coord in the array
  int yPos;//Players y-coord in the array
  int xPrev;//Players previous x-coord in the array
  int yPrev;//Players previous y-coord in the array
  char prevState;//Store the ASCII char of the grid on the players current location (before their symbol is on it)
  int numRolls;//Number of rolls player has had
  char symb;//The players symbol in the game
} player;

/*Function that calculates the players new position and stores its old position.
This function also implements the bounce back game rule*/
void playerPos(int dice, player *currUser, player *othUser, char arr[10][10]){
  //Setting previous of player before moving them
  currUser->xPrev = currUser->xPos;
  currUser->yPrev = currUser->yPos;
  //Checking if we need to move down a row
  if(currUser->xPos + dice >= 10){
    if(currUser->yPos == 9){//Implementing bounce back game rule
      currUser->xPos = currUser->xPos - ((currUser->xPos + dice)-10);//Moving player back if the dice val is greater than that needed to win
    }
    else{//If player reaches end of row, move them down a row
      currUser->xPos = ((currUser->xPos + dice) % 10);
      currUser->yPos++;
      }
    }
  else{//Move player along row
    currUser->xPrev = currUser->xPos;
    currUser->xPos += dice; //moving player along by die amount
   }
   arr[currUser->xPrev][currUser->yPrev]=currUser->prevState; //Overwriting the previous location of the player with the locations appropriate state
 }


/*Function to print the array of elements*/
void printHelper(char arr[10][10]){
  int temp = 0;
  //Creating 2D array
  for(int i = 0; i<10; i++){
    for (int j = 0; j < 10; j++){
      if (temp % 10 == 0){
        printf("\n");
      }
      printf("%c", arr[j][i]); //We swap i and j to flip array into correct orientation
      temp++;
      }
     }
    printf("\n");
}

/*Function used to assign grid elements to an array value
i.e. creating the game grid*/
void grid(char arr[10][10]){  //create 10x10 array
  for (int i = 0; i < 10; i++){
    for (int j = 0; j < 10; j++){
      if(arr[i][j] == 0){ //All array elems are declared as 0 so we can overwrite them with ASCII char
          arr[i][j] = '.';//250
      }
    }
  }
  for(int j = 2; j<8; j++){//Creating ladder
    arr[3][j] = '!';//change to 186
  }
  int j = 8;
  for(int i =5; i<10; i++){//creating snake
    arr[i][j]='/'; // CHANGE TO SNAKE 245
    j--;
  }
  //creating start and end arrows
  arr[0][0]='>';//CHANGE TO 175
  arr[9][9]='>';//CHANGE TO 175
}

/*Function to move the player to the next location and set their symbol in the array.
Implements the ladder and snake functionality. Also deals with the case where a player lands on a another player*/

void movePlayer(player *curUser, player *othUser, char arr[10][10]){
  //if statement check if the previous element is 0 if it is, set prev element to .
  if(arr[curUser->xPos][curUser->yPos] == othUser->symb){ //Checking if a player is in the position we want to move to
    curUser->prevState = othUser->prevState; //Set the current user's previous state to the the other users state
    othUser->prevState = curUser->symb; //Ensuring that once the other player moves, the current players icon remains
  }
  else{ // if this is not the case we set the current symbol to be the previous
      curUser->prevState = arr[curUser->xPos][curUser->yPos];
  }
  arr[0][0]='>'; // Ensuring the start arrow is always drawn
  if(curUser->xPos == 3 && curUser->yPos == 2){ // Checking if user lands on ladder and moving them to new pos
    curUser->xPos = 3;
    curUser->yPos = 7;
  }
  else if(curUser->xPos == 5 && curUser->yPos == 8){ //Checking if user lands on snake and moving them
    curUser->xPos = 9;
    curUser->yPos = 4;
  }
    arr[curUser->xPos][curUser->yPos] = curUser->symb; //setting the player icon in new location
}


int main(){
  char myArray[10][10]= {{0}};//initialising array
  grid(myArray);//creating array with elements
  player *user1 = &(player){0,0,0,0,0,0,'X'}; //initialising user1 as a pointer to the struct to allow us to access its values anywhere
  player *user2 = &(player){0,0,0,0,0,0,'O'};//initialising user2 as a pointer to the struct
  int playerNumber; //Used for player selection
  int dice;
  bool gameRunning = true;

  while(gameRunning == true){ //While loop that allows the game to run
    printf("Enter player number ");
    scanf("%d", &playerNumber);
    if(playerNumber == 1){
      user1->numRolls++;//increment number of rolls of the user
      srand(time(NULL));
      dice=rand()%6+1;
      printf("\nYou have rolled a %d.\n", dice);
      playerPos(dice, user1,user2, myArray);//calculate pos
      movePlayer(user1, user2, myArray);//move player1
      if(user1->xPos == 9 && user1->yPos ==9){ //checking if game won
        printf("Player 1 Wins!!!\n");
        gameRunning = false; //end game
      }

    }
    else if(playerNumber == 2){
      user2->numRolls++;
      srand(time(NULL));
      dice=rand()%6+1;
      printf("\nYou have rolled a %d.\n", dice);
      playerPos(dice, user2, user1, myArray);
      movePlayer(user2, user1, myArray);
      if(user2->xPos == 9 && user2->yPos ==9){//checking if game won
        printf("\nPlayer2 Wins!!!\n");
        gameRunning = false;
      }
    }
    else{
      printf("\nPlease enter player number!\n");
    }
    printHelper(myArray);//print array
    printf("\nPlayer 1: %d times   Player 2: %d times\n", user1->numRolls, user2->numRolls);
  }
  return 0;
}
