#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <chrono>
using namespace std;
using namespace std::chrono;

#define CASE_ITEMS 2000 // number of items in each case
#define CASES 10     // number of cases
#define RNGKEYS rand() % CASE_ITEMS
#define INSKEYS CASE_ITEMS - i //worst case for insertion sort
#define UNSORTED "./tosort.txt"
#define SORTED "./sorted.txt"
#define TIMEREC "./timer.txt"

struct node
{
    int data;
};

class entry
{
public:
    friend class node;
    long key;
    entry(long k, node *d)
    {
        key = k;
        data = d;
    }
    void setkey(int k) { key = k; }
    void outputkey(FILE *file)
    {
        fprintf(file, " key: %ld\n", key);
    }

private:
    node *data;
};

void makeCases(int, vector<vector<entry>> &, FILE *,string); // forward declare

// sort a case of array with insertion
// returns runtime in microseconds
unsigned long InsertionSort(vector<entry> &arr, int c)
{
    cout << "Start insertion sort\n";
    auto timer = high_resolution_clock::now();
    for (long i = 0; i < arr.size(); i++)
    {
        long min = i;
        for (long j = i + 1; j < arr.size(); j++) // get min key and swap then go forward and exclude sorted
        {
            if (arr[j].key < arr[min].key)
            {
                min = j;
            }
        }
        swap(arr[i], arr[min]);
        // cout<<arr[i].key<<endl;
    }
    auto stop = high_resolution_clock::now();
    auto dur = duration_cast<milliseconds>(stop - timer);
    //output performance record
    cout << "Sorted array in " << dur.count() << " microseconds\n";
    FILE *file = fopen(TIMEREC, "a");
    fprintf(file, "Sorted case #%d with %lu items in %ld microseconds with Insertion\n",
            c, CASE_ITEMS, dur.count());
    fclose(file);
    return dur.count();
}

int main(void)
{
    remove(SORTED);
    cout << "Removed old " << SORTED << endl; // remove old files
    remove(UNSORTED);
    cout << "Removed old " << UNSORTED << endl;
    remove(TIMEREC);
    cout << "Removed old " << TIMEREC << endl;

    FILE *f_Unsorted = fopen(UNSORTED, "a"); // create new files
    FILE *f_Sorted = fopen(SORTED, "a");

    srand(time(0));                   // set random
    vector<vector<entry>> superarray; // all cases stored here

    makeCases(CASES, superarray, f_Unsorted,"INSERTION"); // create cases, provide file path for unsorted cases

    for (int i = 0, c = 1; i < superarray.size(); i++, c++) // sort each case
    {
        auto duration = InsertionSort(superarray[i], c); // returns runtime after sorting
        fprintf(f_Sorted, "\nCase %d of %lu items finished in %lu microseconds\n", c, CASE_ITEMS, duration);
        for (int j = 0; j < CASE_ITEMS; j++) // output keys of sorted array
        {
            superarray[i][j].outputkey(f_Sorted); // i for array, j for entry
        }
        cout << "output sorted array to file" << SORTED << endl;
    }

    fclose(f_Unsorted);
    fclose(f_Sorted);
    return 0;
}

void makeCases(int cases, vector<vector<entry>> &superarray, FILE *unsortedfile,string mode)
{
    for (int c = 0; c < cases; c++) // create cases
    {
        vector<entry> array;
        long key;
        fprintf(unsortedfile, "\nCase %d with %lu items:\n", c + 1, CASE_ITEMS);
        for (int i = 0; i < CASE_ITEMS; i++) // create array with nodes
        {
            node *n = new node;
            if(mode=="INSERTION")   //generate key nums by mode
                key = INSKEYS; 
            else if(mode=="RANDOM")
                key = RNGKEYS;
            else
                key = i; //will process after array is created
            entry e(key, n); // create entry with key and node
            array.push_back(e);
        }
        if(mode=="QUICK"||mode=="HEAP") //do the Permutation() provided
        {
            for (int i = CASE_ITEMS; i > 2;i--)
            {
                int j = rand() % i + 1;
                swap(array[i], array[j]);
            }
        }
        for (int i = 0; i < CASE_ITEMS; i++) // output key of each node in array just made
        {
            array[i].outputkey(unsortedfile);
        }
        cout << "output unsorted array to file" << UNSORTED << endl;
        superarray.push_back(array); // add array to cases
        cout << "Created unsorted array for case #" << c + 1 << endl;
    }
}