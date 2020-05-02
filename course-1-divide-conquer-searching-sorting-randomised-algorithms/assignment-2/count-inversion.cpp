#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

struct countInversionResult
{
    long inversionCount;
    std::vector<int>::iterator sortedResultIterator;
    long sortedDataLength;
    
};

struct splitInputTracker
{
    std::vector<int>::iterator part1;
    long part1Length;
    std::vector<int>::iterator part2;
    long part2Length;
};


splitInputTracker splitInputForRecursion(const std::vector<int>::iterator input, long inputLength)
{
    splitInputTracker result;
    result.part1 = input;
    /* Keep track of split in the input array instead of copying the elements each time.
    Keep a track of first left element, length of left split input, first right element, length of right split input.
     */
    result.part1Length = inputLength/2;
    result.part2Length = inputLength/2;
    if(inputLength%2 != 0)
    {
        result.part2Length = result.part2Length + 1;
    }
    result.part2 = input + inputLength/2;
    return result;
}


countInversionResult countSplitInversions( std::vector<int>::iterator part1,long part1Length, std::vector<int>::iterator part2, long part2Length)
{
    countInversionResult result;
    result.inversionCount = 0;
    
    std::vector<int>::iterator i = part1;
    std::vector<int>::iterator j = part2;
    std::vector<int> tempResult;
    
    /*This is merge sort's conquer part modified to
     stitch the sorted left and right input and also count inversions*/
    while( i < (part1 + part1Length)  && j < (part2 + part2Length))
    {
        if(*i < *j)
        {
            tempResult.push_back(*i);
            ++i;
        }
        else
        {
            tempResult.push_back(*j);
            result.inversionCount = result.inversionCount + std::distance(i,((part1 + part1Length)));
            ++j;
        }
    }
    if(i == (part1 + part1Length))
    {
        while(j < (part2 + part2Length))
        {
            tempResult.push_back(*j);
            ++j;
        }
    }
    if(j == (part2 + part2Length))
    {
        while(i < (part1 + part1Length))
        {
            tempResult.push_back(*i);
            ++i;
        }
    }
    std::copy(tempResult.begin(),tempResult.end(),part1);
    result.sortedResultIterator = part1;
    result.sortedDataLength = part1Length + part2Length;
    return result;
    
}
countInversionResult countInversion(std::vector<int>::iterator input, long inputLength)
{
    countInversionResult result;
    result.inversionCount = 0;
    if(inputLength <= 1)
    {
        result.sortedResultIterator = input;
        result.sortedDataLength = inputLength;
        return result;
    }
    
    splitInputTracker splitResult = splitInputForRecursion(input,inputLength);
    countInversionResult inversionResultLeft;
    countInversionResult inversionResultRight;
    inversionResultLeft = countInversion(splitResult.part1,splitResult.part1Length);
    inversionResultRight = countInversion(splitResult.part2, splitResult.part2Length);
    result = countSplitInversions(inversionResultLeft.sortedResultIterator,inversionResultLeft.sortedDataLength,inversionResultRight.sortedResultIterator,inversionResultRight.sortedDataLength);
    result.inversionCount = result.inversionCount + inversionResultLeft.inversionCount + inversionResultRight.inversionCount;
    
    return result;
}


int main(int argc, const char * argv[]) {
    std::vector<int> vecOfInt;
    int input;
    string fileName = "<Give the complete input file path here; see README for the input file for this assignment>";
    std::ifstream in(fileName);
    
    // Check if object is valid
    if(!in)
    {
        std::cerr << "Cannot open the File : "<<fileName<<std::endl;
    }
    
    std::string str;

    while (in >>input)
    {
        vecOfInt.push_back(input);
    }
    
    in.close();
    
   countInversionResult result = countInversion(vecOfInt.begin(),vecOfInt.size());
    std::cout << "No of inversions: " << result.inversionCount << endl;
    
    return 0;
}


