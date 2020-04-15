#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void levelMen(int plane[12][6], char manifest[72][32]);
void assignSeat(int s, int e, int plane[12][6], char manifest[72][32]);
int avail(int s, int e, int plane[12][6]);
void finalize(int row, int seat, int plane[12][6], char manifest[72][32]);
char seatC(int s);
void printSeats(int plane[12][6], char manifest[72][32]);
int seatN(char seat);
char* getLevel(int row);
int main(void)
{
    int plane[12][6];
    char manifest[72][32];
    int i, j;
    for(i=0; i<12; i++) {
        for(j=0;j<6;j++) {
            plane[i][j] = 0;
         }
    }


    for(i=0; i<72; i++) {
        strcpy(manifest[i], "unassigned");
    }

    levelMen(plane, manifest);
    return 0;
}


//main menu for selecting from first class, business, or economy

void levelMen(int plane[12][6], char manifest[72][32]){
    char class;
    printf("Please type:\n(F)  for \"First Class\"\n(B) for \"Business Class\"\n(E) for \"Economy Class\": ");
    scanf(" %c", &class);

    switch(class){
    case 'F':
        assignSeat(0, 1, plane, manifest);
        break;
    case 'B':
        assignSeat(2, 4, plane, manifest);
        break;
    case 'E':
        assignSeat(5, 11, plane, manifest);
        break;
}

    printSeats(plane, manifest);
}


//function for displaying output after seat assignment
void printSeats(int plane[12][6], char manifest[72][32]){
    int c;
    printf("Show Seating Chart(1)\nShow Passenger Manifest(2)\nPrint Boarding Pass(3)\n");
    scanf("%d", &c);
    int i, j;
    if(c == 1){

        //option 1: print seat chart
        printf(" A  B  C       D  E  F\n");
    for(i = 0; i<12;i++){
        for(j = 0;j<6;j++){
            printf("|");
            if(plane[i][j] == 0){
                printf(" ");
            }else {printf("X");}
            printf("|");
            if(j == 2){
                printf("     ");
            }
        }
        printf("%d\n", i);
    }
    }
    else if(c == 2){

        //option 2: print manifest

    for(i=0;i<12;i++){
        for(j=0;j<6;j++){
            if(plane[i][j] == 1){
                char *level = getLevel(i);
            printf("%s, Seat: %d%c, Level: %s\n", manifest[i*j], i,seatC(j), level);
        }
        }
    }
    }
    else{

        //option 3: print boarding pass

        char s;
        int  r;
        printf("Please enter seat number to print boarding pass: ");
        scanf("%d%c", &r, &s);
        char *level = getLevel(r);
        if(strcmp(manifest[seatN(s)*r], "unassigned") == 0){
            printf("Seat Unassigned - No boarding pass available. Try again.");
        }else {
        printf("%s, Seat: %d%c, Level: %s\n", manifest[seatN(s)*r], r,s,level);
        }

}

    levelMen(plane, manifest);
}


//returns level associated with row number
    char* getLevel(int row){
        char *level;
        if(row > 1){
            if(row> 4){
                level = "Economy";
            }else
                level = "Business";
        }else
        {
            level = "First Class";
        }
        return level;
    }


    void assignSeat(int s, int e, int plane[12][6], char manifest[72][32]){
        int row, seat;
        int d;
        srand(time(NULL));


        // if there are no seats in given section available
        if(avail(s, e, plane) == 0){
            printf("Section full. ");
            if(e == 1){
                //if first class.... downgrade
                printf("Downgrade to business class? (0):Yes (1):No");
                scanf("%d", &d);
                if(d == 0){
                    assignSeat(2, 4, plane, manifest);
                }else printf("Next flight leaves in 3 hours");
            }
            if(e == 4){
                //if business, upgrade or downgrade
                if(avail(0, 1, plane) == 1){
                    printf("Seat available in First Class. Upgrade? (0):Yes (1):No");
                    scanf("%d", &d);
                    if(d == 0){
                        assignSeat(0, 1, plane, manifest);
                    }
                    else{
                printf("Downgrade to economy class? (0):Yes (1):No");
                scanf("%d", &d);
                if(d == 0){
                    assignSeat(5, 11, plane, manifest);
                }else printf("Next flight leaves in 3 hours");
                    }
                }
            }
            //if economy class, check for possible upgrades to B or F
            else{
                if(avail(2, 4, plane) == 0){
                    if(avail(0, 1, plane) == 0){
                        printf("No seats available. Next flight leaves in 3 hours");
                    }else {
                        printf("Seat available in First Class. Upgrade? (0):Yes (1):No");
                        scanf("%d", &d);
                        if(d == 0){
                            assignSeat(0, 1, plane, manifest);
                        }else printf("No seats available. Next flight leaves in 3 hours");
                    }
                }
                else {
                    printf("Seat available in Business Class. Upgrade? (0):Yes (1):No");
                    scanf("%d", &d);
                    if(d == 0){
                        assignSeat(2, 4, plane, manifest);
                    }else printf("No seats available. Next flight leaves in 3 hours");
                }
            }
        }

        // If there are available seats in given section
        else {
            int w;
            printf("\nGuarantee me a window seat: (0):Yes (1):No\n");
            scanf("%d", &w);
            int rand_num = (rand() % (e - s + 1)) + s;
            if(w == 0){
                int wSeats = e*2;
                row = rand_num;
                while(plane[row][5] != 0 && plane[row][0] != 0){
                    if(wSeats == 0){
                        break;
                    }
                    row = (rand() % (e - s + 1)) + s;
                    //keep track of total possible unchecked window seats
                    wSeats--;
                }
                if(plane[row][5] == 0){
                    seat = 5;
                }else if(plane[row][0]==0){
                    seat = 0;
                }else {
                    printf("No more window seats in this section, restarting selection\n");
                    assignSeat(s, e, plane, manifest);
                }
            }
            else {
                row = (rand() % (e - s + 1)) + s;
                seat = rand() % 6;
                while(plane[row][seat] != 0){
                    row = (rand() % (e - s + 1)) + s;
                    seat = rand() % 6;

                }
            }
            finalize(row, seat, plane, manifest);

        }

    }


//function for finalizing seat selection (adds to plane and manifest)
void finalize(int row, int seat, int plane[12][6], char manifest[72][32]){
    char s;
    int d;
    s = seatC(seat);
    if(row > 1){
        printf("Seat %d%c is available. Finalize selection:(0) or Upgrade:(1) " , row, s);
        scanf("%d", &d);
        if(d == 0){
            //Finalizing seat selection
            plane[row][seat] = 1;
            printf("Please enter passenger name: ");
            scanf("%s", manifest[row * seat]);

        }else{
            //determining appropriate upgrade
            if(row > 5){
                assignSeat(2, 4, plane, manifest);
            }else {
                assignSeat(0, 1, plane, manifest);
            }
        }
    }else {
        //finalizing seat selection
        printf("Seat %d%c is available. Finalize selection:(0) or Try Again:(1) " , row, s);
        scanf("%d", &d);
        if(d == 0){
            //Finalizing seat selection
            plane[row][seat] = 1;
            printf("Please enter passenger name: ");
            scanf("%s", manifest[row * seat]);

        }else{
            assignSeat(0, 1, plane, manifest);
        }

    }
}



//Function for checking availability in given section of plane (from s to e)
    int avail(int s, int e, int plane[12][6]){
        int i,j;
        for(i = e; i>s;i--){
            for(j = 6;j>0;j--){
                if(plane[i][j]==0){
                    return 1;
                }
            }
        }
        return 0;
    }

//returns the character associated with the seat number

char seatC(int seat){
    char s;
    switch(seat){
    case 0:
        s = 'A';
        break;
    case 1:
        s='B';
        break;
    case 2:
        s= 'C';
        break;
    case 3:
        s= 'D';
        break;
    case 4:
        s= 'D';
        break;
    case 5:
        s= 'F';
        break;
    default:
        printf("invalid seat");
    }
    return s;
}


//returns the number associated with the seat character

int seatN(char seat){
    int s;
    switch(seat){
    case 'A':
        s = 0;
        break;
    case 'B':
        s= 1;
        break;
    case 'C':
        s= 2;
        break;
    case 'D':
        s= 3;
        break;
    case 'E':
        s= 4;
        break;
    case 'F':
        s= 5;
        break;
    default:
    printf("invalid seat");
    }

    return s;
}
