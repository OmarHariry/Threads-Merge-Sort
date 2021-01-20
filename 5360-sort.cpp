#include<bits/stdc++.h>
#include <iostream>
#include <pthread.h>
#include <time.h>

using namespace std;
struct data{
 int low,high,level;
};

vector<int>arr;

void read_file();        

void merge(vector<int>arr,int low, int mid, int high);
void merge_sort(vector<int>arr, int low, int high);
void* thread_merge_sort(void* arg);
void assign_thread_data(pthread_t&t1,data&d,int low,int high ,int level);
void merge_threads_together(pthread_t &t1, pthread_t &t2,int low, int mid, int high);
void solve();
void merge(vector<int>, int left, int middle, int right) {
    int i = 0;
    int j = 0;
    int k = 0;
    int left_length = middle - left + 1;
    int right_length = right - middle;
    int left_array[left_length];
    int right_array[right_length];
    
    /* copy values to left array */
    for (int i = 0; i < left_length; i ++) {
        left_array[i] = arr[left + i];
    }
    
    /* copy values to right array */
    for (int j = 0; j < right_length; j ++) {
        right_array[j] = arr[middle + 1 + j];
    }
    
    i = 0;
    j = 0;
    /** chose from right and left arrays and copy */
    while (i < left_length && j < right_length) {
        if (left_array[i] <= right_array[j]) {
            arr[left + k] = left_array[i];
            i ++;
        } else {
            arr[left + k] = right_array[j];
            j ++;
        }
        k ++;
    }
    
    /* copy the remaining values to the array */
    while (i < left_length) {
        arr[left + k] = left_array[i];
        k ++;
        i ++;
    }
    while (j < right_length) {
        arr[left + k] = right_array[j];
        k ++;
        j ++;
    }
}
void merge_sort(vector<int>arr, int low, int high) {
    if (low < high) {
        int mid = low + (high - low) / 2;
        merge_sort(arr, low, mid);
        merge_sort(arr, mid + 1, high);
        merge(arr, low, mid, high);	
    }
}
void read_file(){
    ifstream myFile("input.txt");
    int temp ;
    myFile >> temp;
  
                                        //then scan the array elements
    for(int i=0;myFile >> temp;++i){
        arr.push_back(temp);
    }
    myFile.close();
}
void solve(){
   // read_file();
    pthread_t threads;
    data d = {0,(int)arr.size()-1,(int)ceil(log2(arr.size()))};
    int tic = clock();
    pthread_create(&threads, NULL, thread_merge_sort, (void *)&d);
    pthread_join(threads, NULL);

    int toc = clock();
     for(int i=0;i<arr.size();++i)
    {
    	cout<<arr[i]<<" ";
    }
    cout << endl<<"Time taken: " << (toc - tic) /
              (double)CLOCKS_PER_SEC <<" Seconds"<< endl;
}
void* thread_merge_sort(void* arg)
{
    data *d = (data*)arg;
    int low = d->low,high = d->high,level = d->level;
    if(high<=low) return NULL; //one element
    if(level ==0) 
    	merge_sort(arr,low,high);
   else{
    pthread_t t1, t2;
    data d1, d2;
    int mid = (low+high)/2;
    assign_thread_data(t1, d1, low, mid, level-1);
    assign_thread_data(t2, d2, mid+1, high, level-1);
    merge_threads_together(t1, t2, low, mid, high);
    }
    return NULL;

}
void assign_thread_data(pthread_t&t1,data&d,int low,int high ,int level)
{
  d.low = low;
  d.high = high;
  d.level = level;
  void*ptr_data = (void*)&d;
  pthread_create(&t1,NULL,thread_merge_sort,ptr_data);
}
void merge_threads_together(pthread_t &t1, pthread_t &t2,int low, int mid, int high)
{
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);  
  merge(arr, low, mid, high);
}
int main()
{
    read_file();
    solve();
    
    return 0;
}

