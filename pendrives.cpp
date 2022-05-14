#include <iostream>
#include <stdio.h>
using namespace std;

struct File{

        int size;
        int index;
};

void merge(File files_array[], int left, int middle, int right)//left, middle, right ->first, middle and last index of an array
{
    int size1 = middle - left + 1;
    int size2 = right - middle;

    File *L = new File[size1];
        File *R = new File[size2];

    for (int i = 0; i < size1; i++)
        {
                L[i] = files_array[left + i];
        }
        
    for (int j = 0; j < size2; j++)
        {
                R[j] = files_array[middle + 1 + j];
        }

    int i = 0;// first index of first left array
    int j = 0;//first index of right array
    int k = left;
        
    while (i < size1 && j < size2) 
        {
        if (L[i].size <= R[j].size) {
            files_array[k] = L[i];
            i++;
        }
        else {
            files_array[k] = R[j];
            j++;
        }
        k++;
    }
        //after above operation, one of the array becomes empty, so we have to copy left over files to target array
        
    while (i < size1)//if any elements are left in array 
        {
        files_array[k] = L[i];
        i++;
        k++;
    }
    while (j < size2) 
        {
        files_array[k] = R[j];
        j++;
        k++;
    }
        delete[] L;
        delete[] R;
}
void mergeSort(File files_array[], int left, int right){//left and right -> first and last index of an array
    if(left >= right)
        {
        return;
    }
    int middle = (left + right)/2;
    mergeSort(files_array, left, middle);
    mergeSort(files_array, middle + 1, right);
    merge(files_array, left, middle, right);
}
int max_files(File *a, int n_of_files, int pendrive_size)//to calculate maximum number of files we can fit in one pendrive 
{                                                                                                                
    int sum = 0;
    int count = 0;
    for (int i = 0; i < n_of_files; i++) {
        sum = sum + a[i].size;
        if (sum > pendrive_size) {
            break;
        }
        count++;
    }
    return count;
}int* sort_pen(int* a, int n){//sorting files on pendrive by indexes
        for(int i = n; i>0; i--){
                if (a[i] > a[i-1]){
                        int tmp;
                        tmp = a[i];
                        a[i] = a[i-1];
                        a[i-1] = tmp;
                }
        }
        return a;
}

void fit_files(File* files, int number_of_files, int pendrive_size)//assigning files to pendrives
{
        
        int maximum_files = max_files(files, number_of_files, pendrive_size);
        int* indexesp1 = new int[maximum_files];//files on pendrive 1
        int* indexesp2 = new int[maximum_files];//files on pendrive 2
        
        int ps = pendrive_size;
        int files_number1 = 0;
        
        for (int i=0; i<maximum_files ; i++){//iterate as many times as maximum number of files we can fit
                if (ps - files[i].size >= 0)//if size of the file is less that pendrive size, assign it
                {
                        indexesp1[i] = files[i].index;
                        files_number1 += 1;//increase files count
                        ps -= files[i].size;//decrease pendrive size
                        sort_pen(indexesp1, i);//sort pendrive
                }
                else{break;}
        }
        ps = pendrive_size;
        int files_number2 = 0;

        for (int i=maximum_files; i<number_of_files ; i++){
                if (ps - files[i].size >= 0)
                {
                        indexesp2[i - maximum_files] = files[i].index;
                        files_number2 += 1;
                        ps -= files[i].size;
                        sort_pen(indexesp2, i - maximum_files);
                }
                else{break;}
        }


        printf("%i \n1: ", files_number1+files_number2);
        for(int i=0; i<files_number1; i++)
        {
                printf("%i ", indexesp1[i]);
        }
        printf("\n2: ");
        for(int i=0; i<files_number2; i++)
        {
                printf("%i ", indexesp2[i]);
        }
        
        delete[] indexesp1;
        delete[] indexesp2;

}

int main()
{
        int number_of_files, pendrive_size;
        scanf("%i %i", &number_of_files, &pendrive_size);       
        File* files = new File[number_of_files];
        
        for (int i = 0; i < number_of_files; i++)
        {
                scanf("%i %i", &files[i].index, &files[i].size);
        }
        if(number_of_files > 0)
        {
                mergeSort(files, 0, number_of_files -1);
        }
        fit_files(files, number_of_files, pendrive_size);
        delete[] files;
        return 0;
}