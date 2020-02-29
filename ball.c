/* Ball simulation in 6-by-6 grid*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

// Setting up types and constants
enum direction {L, R, WrongDir}; //left, right
typedef enum direction direction;

enum movement {U, D, WrongMov}; //upwards, downwards
typedef enum movement movement;

enum {m=6,n=6}; //m-rows n-columns

// Constants to represent validity checking.
enum validity {BadFormat, BadLetter, BadDigit, BadCell, WrongPosition,InvalidDirection, InvalidMovement,OK };
typedef enum validity validity;

struct ball {
    int posx; //current position row
    int posy; //current position column
    direction dir;
    movement mov;
    char grid[6][6];
};
typedef struct ball ball;
//------------------------------------------------------------

//Initialize the states of the simulation
void Start(ball *b, int row, int col, direction direction,movement movement){

    b-> posx=row;
    b-> posy=col;
    b-> dir=direction;
    b-> mov=movement;

    for (int i=0; i<m; i++){
        for (int j=0; j<n; j++){
            b-> grid[i][j]='_';
        }
    }
    b-> grid[row][col]='1'; //initial position of  ball
}

//Show the current grid to the user
void Show(ball *b){

  for (int i=0; i<m; i++){
      for (int j=0; j<n; j++)
          printf("%c ",b->grid[i][j]);
      printf("\n");
  }
  printf("\n\n\n");
  sleep(1);
}

//Put the obstacles  into  the grid
void Obstacle(ball *b,int row,int col){

    b-> grid[row][col]='O';
}


//================CONVERSION AND VALIDITY CHECKING============
void printInvalid(validity v) {
    if (v == BadFormat) printf("\nNot the right format");
    else if (v == BadDigit) printf("\nPlease enter a valid digit from '1-%d'",n);
    else if (v == BadLetter) printf("\nPlease enter a valid letter 'a-%c'",'a'+m-1);
    else if (v==WrongPosition) printf("\nThe obstacle can't be in the edge of the matrix");
    else if (v==BadCell) printf("\nPick a cell that is empty");
    else if (v==InvalidDirection) printf("\nThe direction isn't compatible with the position of the ball");
    else if (v==InvalidMovement) printf("\nThe movement isn't compatible with the position of the ball");
    else if (v == BadCell) printf("Choose an empty cell");

    printf("\n");
}


validity valid(char *text) {

    //----------------BadLetter / BadFormat checking-----------
    int count=0;
    for (int i=0; i<m; i++){
        if (*text!='a'+i) count+=1;
    }
    if (count>=m) {
        if (*text=='\0') return BadFormat;
        else return BadLetter;
    }

    //---------------BadDigit / BadFormat checking-------------
    count=0;
    for (int i=0; i<n; i++){
        if (*(text+1)!=('0'+i+1)) count+=1;
    }
    if (count>=n) {
        if (*text=='\0') return BadFormat;
        else return BadDigit;
    }
    //--------------BadFormat checking-------------
    if (*(text+2)!='\0') return BadFormat;

    return OK;
}

//Convert into actual coordinates
int ConvRow (char *text){

    for (int i=0; i<m; i++){
        if (*text=='a'+i) return i;
    }
    return -1;
}

int ConvCol (char *text){

    for (int i=0; i<n; i++){
        if (*(text+1)==('0'+i+1)) return i;
    }
    return -1;
}

//Convert into valid arguments
direction ConvDir(char *text){
    if (*text=='L') return L;
    if (*text=='R') return R;
    printf("\nEnter a valid direction\n");
    return WrongDir;
}

movement ConvMov(char *text){
    if (*text=='U') return U;
    if (*text=='D') return D;
    printf("\nEnter a valid movement\n");
    return WrongMov;
}

validity CheckObstacles (ball *b,int row, int col){

    if (col==0 || col==n-1 || row==0 || row==m-1)
        return WrongPosition;

    if (b->grid[row][col]!='_')
        return BadCell;

    return OK;
}

validity InitialStateDir (int col,direction dir){

    if ((col==0 && dir==L) || (col==n-1 && dir==R))
        return InvalidDirection;

    return OK;
}

validity InitialStateMov (int row, movement mov){

    if ((row==0 && mov==U) || (row==m-1 && mov==D))
        return InvalidMovement;

    return OK;
}
//================CONVERSION AND VALIDITY CHECKING============


//=============================MOVE============================
//when it's time to go on to the next column
void ChangeCol(ball *b){

    if (b->dir==R) {
        b->posy+=1;

    }
    else if (b->dir==L) {
        b->posy-=1;
    }
}

//when it's time to hop to the next or previous row
void ChangeRow(ball *b){

    if (b->posy==n){
        if (b->mov==D) b->posx+=1;
        else b->posx-=1;
        b->posy=n-1;
        b->dir=L;
    }
    if (b->posy==-1) {
        if (b->mov==D) b->posx+=1;
        else b->posx-=1;
        b->posy=0;
        b->dir=R;
    }
    if (b->posx==n-1) b->mov=U;
    if (b->posx==0) b->mov=D;
}

//when an obstacle is on the way
void FoundObstacle(ball *b){

  if (b->dir==R) b->dir=L;
  else b->dir=R;

  ChangeCol(b);
  ChangeRow(b);
}

//Actually make  the simulation run forever
void NextStep (ball *b){

    while (1){
        b->grid[b->posx][b->posy]='_'; //to delete the previous one
        ChangeCol(b);
        ChangeRow(b);

        if (b->grid[b->posx][b->posy]=='O') FoundObstacle(b);

        b->grid[b->posx][b->posy]='1';
        Show(b);
    }
}

//Interact with user and start the simulation
void StartSimulation (ball *b,char *args[n]){

  validity v=valid(args[1]);
  printInvalid(v);
  if (v==OK){
    int row=ConvRow(args[1]);
    int col=ConvCol(args[1]);

    direction dir=ConvDir(args[2]);
    if (dir!=WrongDir){
      v=InitialStateDir(col,dir);
      printInvalid(v);
      if (v==OK){

        movement mov=ConvMov(args[3]);
        if (mov!=WrongMov){
          v=InitialStateMov(row,mov);
          printInvalid(v);

          if (v==OK){
            Start(b, row, col,  dir, mov);
            //-----OBSTACLE 1-------
            v=valid(args[4]);
            printInvalid(v);
            if (v==OK){
              row=ConvRow(args[4]);
              col=ConvCol(args[4]);


              v=CheckObstacles (b,row, col);
              printInvalid(v);
              if (v==OK){
                Obstacle(b,row,col);
              //---------------------

                //-----OBSTACLE 2---------
                v=valid(args[5]);
                printInvalid(v);
                if (v==OK){
                  row=ConvRow(args[5]);
                  col=ConvCol(args[5]);

                  v=CheckObstacles (b,row, col);
                  printInvalid(v);
                  if (v==OK){
                    Obstacle(b,row,col);
                  //-----------------------

                    NextStep(b);
                  }
                }
              }
            }
          }
        }
      }
    }
  }

}

//=============================MOVE============================


//-----------------------------------TESTING--------------------------------
void testInvalid() {
  assert(valid("p2") == BadLetter);
  assert(valid("g5") == BadLetter);
  assert(valid("a-9") == BadDigit);
  assert(valid("a7") == BadDigit);
  assert(valid("a10") == BadFormat);
}

void testObstacles(ball *b){
  Start(b, 3, 3, R, D);
  assert(CheckObstacles (b,0,0)==WrongPosition);
  assert(CheckObstacles (b,0,3)==WrongPosition);
  assert(CheckObstacles (b,n-1,2)==WrongPosition);
  assert(CheckObstacles (b,1,m-1)==WrongPosition);
  assert(CheckObstacles (b,3,3)==BadCell);
}

void testDirMov(){
  assert(InitialStateDir (0,L)==InvalidDirection);
  assert(InitialStateDir (n-1,R)==InvalidDirection);
  assert(InitialStateMov (0,U)==InvalidMovement);
  assert(InitialStateMov (m-1,D)==InvalidMovement);
}

void testCoversion(){
    assert(ConvRow ("a1")==0);
    assert(ConvRow ("f1")==5);
    assert(ConvCol ("f1")==0);
    assert(ConvCol ("f6")==5);
    assert(ConvCol ("f6")==5);
    assert(ConvDir ("L")==L);
    assert(ConvDir ("R")==R);
    assert(ConvDir ("p")==WrongDir);
    assert(ConvMov ("U")==U);
    assert(ConvMov ("D")==D);
    assert(ConvMov ("w")==WrongMov);
}


void test() {
    ball *b = malloc(sizeof(ball));
    //all the functions
    testInvalid();
    testObstacles(b);
    testDirMov();
    testCoversion();

    /*Obstacle(b,1,6);
    Obstacle(b,5,5);
    Show(b);
    NextStep (b);*/

    free(b);
    printf("All tests passed.\n");

}


//Run
int main(int n, char *args[n]) {

    if (n == 1) test(); //no arguments
    else if (n==6){
        ball *b = malloc(sizeof(ball));
        printf("Initial position: %s\nDirection: %s\nMovement: %s\nObstacles: %s %s\n",args[1],args[2],args[3],args[4],args[5]);

        StartSimulation (b,args);
        free(b);
    }
    else printf("\nNot appropriate number of arguments\n");

    return 0;
}
