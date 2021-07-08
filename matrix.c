#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define upper 1000
#define lower 0
#define CASE_A 0 //The computation of each element of the output matrix happens in a thread
#define CASE_B 1 //The computation of each row of the output matrix happens in a thread
#define CASE_C -1 //One Thread

int getRandomMatrixElement();
void printMatrix(int rows,int columns,int** matrix);

void multiplyMatrices(int _case);
void *rowComputation(void *vargp);
void* elementComputation(void *vargp);
long getSpentTime(struct timespec start,struct timespec end);

struct mat_data {
    int matrix1RowsNum;
    int commonEntry;
    int matrix2ColumnsNum;
    int** matrix1;
    int** matrix2;
    int** matrix3;
    long nsTime_CASE_A;
    long nsTime_CASE_B;
    long nsTime_CASE_C;
} data;

void getInput(){
    FILE* inputFile;
    inputFile  = fopen ("input.txt", "r");
    
    fscanf (inputFile, "%d", &data.matrix1RowsNum); 
    if (feof (inputFile))
        return;
    fscanf (inputFile, "%d", &data.commonEntry); 



    int i;
    data.matrix1=(int**) malloc(data.matrix1RowsNum*sizeof(int*));
    for (i=0; i<data.matrix1RowsNum; i++) 
        data.matrix1[i] = (int *)malloc(data.commonEntry * sizeof(int));
    
    




    i=0;
    
    int j,i2,j2;
    i=0;
    for (i=0;i<data.matrix1RowsNum;i++){
        for (j=0;j<data.commonEntry;j++){
            if (feof (inputFile))
                return;
            
            fscanf (inputFile, "%d", &data.matrix1[i][j]);
        }
    }

    if (feof (inputFile))
        return;
    fscanf (inputFile, "%d", &data.commonEntry); 
    if (feof (inputFile))
        return;
    fscanf (inputFile, "%d", &data.matrix2ColumnsNum);

    i = 0;
    data.matrix2=(int**) malloc(data.commonEntry *sizeof(int*));
    for (i=0; i<data.commonEntry ; i++) 
        data.matrix2[i] = (int *)malloc(data.matrix2ColumnsNum * sizeof(int)); 


    

    for (i2=0;i2<data.commonEntry;i2++){
        for (j2=0;j2<data.matrix2ColumnsNum;j2++){
            feof (inputFile);
            fscanf (inputFile, "%d", &data.matrix2[i2][j2]);
        }
    }

    
    
    i=0;
    data.matrix3 = (int**) malloc(data.matrix1RowsNum*sizeof(int*));
    for (i=0; i<data.matrix1RowsNum; i++) 
        data.matrix3[i] = (int *)malloc(data.matrix2ColumnsNum * sizeof(int));


    fclose(inputFile);
}

int main(){

    //printf("Please enter Matrix1 Rows Number: ");
    //scanf("%d",&data.matrix1RowsNum);
    //data.matrix1RowsNum = 500;
    //data.commonEntry = 400;
    //data.matrix2ColumnsNum = 300;
    //printf("Please enter mat1 columns & mat2 rows Number: ");
    //scanf("%d",&data.commonEntry);


    //printf("Please enter Matrix2 Columns Number: ");
    //scanf("%d",&data.matrix2ColumnsNum);


    // int i;
    // data.matrix1=(int**) malloc(data.matrix1RowsNum*sizeof(int*));
    // for (i=0; i<data.matrix1RowsNum; i++) 
    //     data.matrix1[i] = (int *)malloc(data.commonEntry * sizeof(int));
    
    // i = 0;
    // data.matrix2=(int**) malloc(data.commonEntry *sizeof(int*));
    // for (i=0; i<data.commonEntry ; i++) 
    //     data.matrix2[i] = (int *)malloc(data.matrix2ColumnsNum * sizeof(int)); 
    
    // i=0;
    // data.matrix3 = (int**) malloc(data.matrix1RowsNum*sizeof(int*));
    // for (i=0; i<data.matrix1RowsNum; i++) 
    //     data.matrix3[i] = (int *)malloc(data.matrix2ColumnsNum * sizeof(int));
    
    getInput();

    // int j,i2,j2;
    // i=0;
    // for (i=0;i<data.matrix1RowsNum;i++){
    //     for (j=0;j<data.commonEntry;j++){
    //         data.matrix1[i][j] = getRandomMatrixElement();
    //     }
    // }

    // for (i2=0;i2<data.commonEntry;i2++){
    //     for (j2=0;j2<data.matrix2ColumnsNum;j2++){
    //         data.matrix2[i2][j2] = getRandomMatrixElement();
    //     }
    // }
    
    
    printf("\n***Matrix multiplication***\n\n\n");
    multiplyMatrices(CASE_A);
    //multiplyMatrices(CASE_B);
    //multiplyMatrices(CASE_C);

    printMatrix(data.matrix1RowsNum,data.commonEntry,data.matrix1);
    printMatrix(data.commonEntry,data.matrix2ColumnsNum,data.matrix2);
    printMatrix(data.matrix1RowsNum,data.matrix2ColumnsNum,data.matrix3);


    //printf("EACH ROW IN THREAD: Time in ns = %ld , Time in ms = %ld\n\n",(data.nsTime_CASE_A),(data.nsTime_CASE_A/1000000));
    //printf("EACH ELEMENT IN THREAD: Time in ns = %ld , Time in ms = %ld\n\n",(data.nsTime_CASE_B),(data.nsTime_CASE_B/1000000));
    //printf("ONE_THREAD: Time in ns = %ld , Time in ms = %ld\n\n",(data.nsTime_CASE_C),(data.nsTime_CASE_C/1000000));

    free(data.matrix1);
    free(data.matrix2);
    free(data.matrix3);


    return 0;
}

int getRandomMatrixElement(){
    return (rand() % (2 * (upper - lower + 1))) + lower - upper;
}

void printMatrix(int rows,int columns,int** matrix){
    int i,j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < columns; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void* rowComputation(void *vargp){
    int i = ((int*)vargp)[0];
    int j,k;
    //printf("%ld,rowComputation, i= %d\n",pthread_self(),i);
    for (int j = 0; j < data.matrix2ColumnsNum; ++j) {
        for (int k = 0; k < data.commonEntry; ++k) {
            data.matrix3[i][j] += data.matrix1[i][k] * data.matrix2[k][j];
        }
    }
    pthread_exit(NULL);
}

void* elementComputation(void *vargp){
    int i = ((int*)vargp)[0];
    int j = ((int*)vargp)[1];
    int k;
    //printf("%ld,elementComputation, i= %d, j= %d\n",pthread_self(),i,j);
    for (int k = 0; k < data.commonEntry; ++k) {
        data.matrix3[i][j] += data.matrix1[i][k] * data.matrix2[k][j];
    }
    pthread_exit(NULL);
}

void multiplyMatrices(int _case){
    if (_case == CASE_B){
        //ROW
        struct timespec start, stop;
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        int i,j;
        int args[data.matrix1RowsNum];
        pthread_t tid[data.matrix1RowsNum];
        for (i = 0; i < data.matrix1RowsNum; ++i){
            int rowNum = i;
            args[i] = rowNum;
            pthread_create(&tid[i],NULL,rowComputation,&args[i]);
            //printf("rowNum= %d\n",rowNum);
        }
        for (j = 0; j < data.matrix1RowsNum; ++j){
            //printf("%ld\n",tid[j]);
            pthread_join(tid[j],NULL);
        }
        clock_gettime(CLOCK_MONOTONIC_RAW, &stop);
        long diff = getSpentTime(start,stop);
        data.nsTime_CASE_B = diff;
    }else if (_case == CASE_A){
        //ELEMENT
        struct timespec start, stop;
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        int i,j;
        int args[data.matrix1RowsNum][data.matrix2ColumnsNum][2];
        pthread_t tid[data.matrix1RowsNum][data.matrix2ColumnsNum];
        for (i = 0; i < data.matrix1RowsNum; ++i){
            for (int j = 0; j < data.matrix2ColumnsNum; ++j) {
                args[i][j][0] = i;
                args[i][j][1] = j;
                pthread_create(&tid[i][j],NULL,elementComputation,(void*)args[i][j]);
            }
        }
        int k,l;
        for (l = 0; l < data.matrix1RowsNum; ++l){
            for (k = 0; k < data.matrix2ColumnsNum; ++k){
                pthread_join(tid[l][k],NULL);
            }
        }
        clock_gettime(CLOCK_MONOTONIC_RAW, &stop);
        long diff = getSpentTime(start,stop);
        data.nsTime_CASE_A = diff;

    }else{
        //ONE THREAD
        struct timespec start, stop;
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        int i,j,k;
        for (i = 0; i < data.matrix1RowsNum; ++i){
            for (int j = 0; j < data.matrix2ColumnsNum; ++j) {
                for (int k = 0; k < data.commonEntry; ++k) {
                    data.matrix3[i][j] += data.matrix1[i][k] * data.matrix2[k][j];
                }
            }
        }
        clock_gettime(CLOCK_MONOTONIC_RAW, &stop);
        long diff = getSpentTime(start,stop);
        data.nsTime_CASE_C = diff;
    }
}

long getSpentTime(struct timespec start,struct timespec end)
{
    struct timespec temp;

    if ((end.tv_nsec-start.tv_nsec)<0)
    {
            temp.tv_sec = end.tv_sec-start.tv_sec-1;
            temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    }
    else 
    {
            temp.tv_sec = end.tv_sec-start.tv_sec;
            temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }

    long nsDiff = temp.tv_sec * 1000000000 + temp.tv_nsec;
    return nsDiff;
}