#include <iostream>
#include <pthread.h>
#include <time.h>


#define MAX 100
using namespace std;


int part = 0;
int a[MAX];
void merge(int low,  int high)
{
    int mid = low + (high - low) / 2;
    int* lb = new int[mid - low + 1];
    int* rb = new int[high - mid];

    int s1 = mid - low + 1, s2 = high - mid, i, j;

    for (i = 0; i < s1; i++)
        lb[i] = a[i + low];

    for (i = 0; i < s2; i++)
        rb[i] = a[i + mid + 1];

    int k = low;
    i = j = 0;

    while (i < s1 && j < s2) {
        if (lb[i] <= rb[j])
            a[k++] = lb[i++];
        else
            a[k++] = rb[j++];
    }

    while (i < s1) {
        a[k++] = lb[i++];
    }

    while (j < s2) {
        a[k++] = rb[j++];
    }
}

void merge_sort(int low, int high)
{

    int mid = low + (high - low) / 2;
    if (low < high) {

        merge_sort(low, mid);

        merge_sort(mid + 1, high);

        merge(low, high);
    }
}

void* merge_sort(void* arg)
{
    int thread_part = part++;
    int low = thread_part * (MAX / 4);
    int high = (thread_part + 1) * (MAX / 4) - 1;
    int mid = low + (high - low) / 2;
    if (low < high) {
        merge_sort(low, mid);
        merge_sort(mid + 1, high);
        merge(low, high);
    }
}

int main()
{
    int THREAD_MAX;
    int temp;
    int count1 = 0;
    cout<<"how many part of thread(only for the power of 2 and smaller then 64):";
    cin>>THREAD_MAX;
    temp = THREAD_MAX/2;
    for (int i = 0; i < MAX; i++)
        a[i] = rand() % 100;
    clock_t c1, c2;
    for (int i = 1; i < THREAD_MAX; i = i*2){
        count1++;
    }
    c1 = clock();
    pthread_t threads[THREAD_MAX];
    for (int i = 0; i < THREAD_MAX; i++)
        pthread_create(&threads[i], NULL, merge_sort,
                       (void*)NULL);
    for (int i = 0; i < THREAD_MAX; i++){
        pthread_join(threads[i], NULL);
    }
    for(int i = count1;i>0;i--){
        for(int j=0;j<i;j++){
            merge((j*MAX/temp),(j+1)*MAX/temp-1);
        }
        temp = temp/2;
    }

    c2 = clock();
    cout << "Sorted array: ";
    for (int i = 0; i < MAX; i++)
        cout << a[i] << " ";

    cout << "Time taken: " << (c2 - c1) /
                              (double)CLOCKS_PER_SEC << endl;

    return 0;
}
