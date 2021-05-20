#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>

void main()
{
    //Membuat key untuk shared memory dengan file soal2b.c
    key_t key = 1234;
    int matrixA[4][3], matrixB[3][6];
    int (*value)[4][6];

    //Mendeklarasikan variabel untuk sharememory
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    //Membuat matriks dengan ukuran 4x3
    printf("Input Matrix A:\n");
    for (int i=0; i<4; i++) {
        for (int j=0; j<3; j++) {
            scanf("%d", &matrixA[i][j]);
        }
    }
    //Membuat matriks dengan ukuran 3x6
    printf("Input Matrix B:\n");
    for (int i=0; i<3; i++) {
        for (int j=0; j<6; j++) {
            scanf("%d", &matrixB[i][j]);
        }
    }

    //Menyimpan nilai perkalian di variabel tempRest dengan matriks 4x6
    int tempRest;
    for (int i=0; i<4; i++) {
        for (int j=0; j<6; j++) {
            tempRest = 0;
            for (int k=0; k<3; k++) {
                tempRest += matrixA[i][k] * matrixB[k][j];
            }
            (*value)[i][j] = tempRest;
        }
    }
 //Mencetak hasil  dari matriks yang telah dikalikan
    printf("Output Program a :\n");
    for (int i=0; i<4; i++) {
        for (int j=0; j<6; j++) {
            printf("%d", (*value)[i][j]);
            if (j < 5) printf(" ");
            else printf("\n");
        }
    }


    sleep(60);

    //share control
    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
}

