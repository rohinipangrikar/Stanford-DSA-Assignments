//QuickSort After Optimization Final

#include <iostream>
#include <vector>
#include<fstream>

using namespace std;

enum pivotPositionSelector {FIRST = 0,LAST, MEDIAN};

struct sValuePositionPair
{
    int value;
    int position;
};

void choosePivot(std::vector<int> &input,int firstPosition, int lastPosition, pivotPositionSelector partitionPosition)
{
    /*Choose the pivot here:
     Based on the case we:
     1. find the pivot
     2. swap it with the first element -- Assignment notes:
     just before the main Partition subroutine, you should
     exchange the pivot element with the first element*/
    
    switch (partitionPosition) {
        case pivotPositionSelector::FIRST:
        {
            /*case 1 first element is pivot so no changes;
             else find pivot and swap it with first element*/
            break;
        }
        case pivotPositionSelector::LAST:
        {
            //case 2 last element is the pivot
            int temp = input[firstPosition];
            input[firstPosition] = input[lastPosition];
            input[lastPosition] = temp;
            break;
        }
        case pivotPositionSelector::MEDIAN:
        {
            //case 3 median of the first, middle, last is the pivot
            /*  Defining median position as per the assignment notes:
             for an array with even length 2k, use the kth element as the "middle" element
             so for the array 4 5 6 7, the "middle" element is the second one ---- 5 and not 6*/
            int medianPosition = 0;
            int totalElements = ((lastPosition - firstPosition) +1);
            
            if(0 == totalElements%2)
            {
                medianPosition = (firstPosition + (totalElements/2))-1;
            }
            else
            {
                medianPosition = firstPosition + (totalElements/2);
            }
            int medianValuePosition = firstPosition;
            // ---- Array size is > 2 ----
            if(totalElements > 2)
            {
                sValuePositionPair findMedianArray[3];
                findMedianArray[0].position = firstPosition;
                findMedianArray[0].value = input[firstPosition];
                findMedianArray[1].position = medianPosition;
                findMedianArray[1].value = input[medianPosition];
                findMedianArray[2].position = lastPosition;
                findMedianArray[2].value = input[lastPosition];
                for(int i = 0; i < 2;i++)
                {
                    if(findMedianArray[i].value > findMedianArray[i+1].value)
                    {
                        sValuePositionPair temp = findMedianArray[i];
                        findMedianArray[i] = findMedianArray[i+1];
                        findMedianArray[i+1] = temp;
                    }
                }
                if(findMedianArray[0].value > findMedianArray[1].value)
                {
                    medianValuePosition = findMedianArray[0].position;
                }
                else
                {
                    medianValuePosition = findMedianArray[1].position;
                }
            }
            else
            {
                //   std::cout << "---- Array size is <= 2 ---- " << endl;
                if(input[firstPosition] > input[lastPosition])
                {
                    medianValuePosition = lastPosition;
                }
            }
            if(firstPosition != medianValuePosition)
            {
                int temp = input[firstPosition];
                input[firstPosition] = input[medianValuePosition];
                input[medianValuePosition] = temp;
            }
            break;
        }
        default:
            break;
    }
    return;
}

int partitionInput(std::vector<int> &input,int firstPosition, int lastPosition)
{
    int pivotValue = input[firstPosition];
    int i = firstPosition + 1;
    int temp = 0;
    for (int j = firstPosition + 1; j <= lastPosition;)
    {
        if (input[j] < pivotValue)
        {
            
            temp = input[i];
            input[i] = input[j];
            input[j] = temp;
            i = i+1;
        }
        j = j+1;
    }
    if((i-1) > firstPosition)
    {
        std::swap(input[firstPosition],input[i-1]);
    }
    
    return i-1;
}
long quickSort(std::vector<int> &input,int firstPosition, int lastPosition, pivotPositionSelector pivotPosition)
{
    if (firstPosition < 0 || lastPosition > (input.size()-1) || lastPosition < firstPosition)
    {
        return 0;
    }
    if(1 == (lastPosition - firstPosition)+1)
    {
        return 0;
    }
    /* subarray of length m has m-1 total number of comparisons
     as pivot element is compared to each of the other m-1 elements*/
    long countComparison = lastPosition - firstPosition;
    choosePivot(input,firstPosition, lastPosition,pivotPosition);
    int partitionPosition = partitionInput(input,firstPosition,lastPosition);
    countComparison += quickSort(input,firstPosition,partitionPosition-1, pivotPosition);
    
    countComparison += quickSort(input,partitionPosition+1,lastPosition, pivotPosition);
    
    return countComparison;
    
    
}

int main(int argc, const char * argv[]) {
    
    std::vector<int> inputVector;
    int input;
    string fileName = "<fulll path to your input file here>";
    std::ifstream in(fileName);
    if(!in)
    {
        std::cerr << "Cannot open the File : "<<fileName<<std::endl;
        return 0;
    }
    
    while (in >> input)
    {
        inputVector.push_back(input);
    }
    in.close();
    
    long count = quickSort(inputVector,0,inputVector.size()-1, pivotPositionSelector::MEDIAN);
  
    std::cout << "total comparisons = " << count << endl;
    return 0;
}


