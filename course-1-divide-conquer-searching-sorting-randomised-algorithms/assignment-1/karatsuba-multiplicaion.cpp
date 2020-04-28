#include <iostream>
#include <cmath>

using namespace std;


string stringMultiply(string &inputX, string &inputY)
{
    int X = stoi(inputX,nullptr);
    int Y = stoi(inputY,nullptr);
    string result = std::to_string(X*Y);
    return result;
    
}

string stringSum(string a, string b)
{
    /* string sum code from: https\://ideone.com/ZMEGAh*/
    
    if(a.size() < b.size())
        swap(a, b);
    
    long j = a.size()-1;
    for(long i=b.size()-1; i>=0; i--, j--)
        a[j]+=(b[i]-'0'); //subtracting '0', subtracts from ascii code gives the number it represents
                          //"+ -" operator does implicit conversion to int value (the ascii values)
    for(long i=a.size()-1; i>0; i--)
    {
        if(a[i] > '9')
        {
            int d = a[i]-'0';
            a[i-1] = ((a[i-1]-'0') + d/10) + '0';
            a[i] = (d%10)+'0';
        }
    }
    if(a[0] > '9')
    {
        string k;
        k+=a[0];
        a[0] = ((a[0]-'0')%10)+'0';
        k[0] = ((k[0]-'0')/10)+'0';
        a = k+a;
    }
    return a;
}

bool isSmaller(string &str1, string &str2)
{
    // Calculate lengths of both string
    long n1 = str1.length(), n2 = str2.length();
    if (n1 < n2)
    {
        return true;
    }
    if (n1 > n2)
    {
        return false;
    }
    
    for (int i=0; i<n1; i++)
    {
        if (str1[i] < str2[i])
            return true;
        else if (str1[i] > str2[i])
            return false;
    }
    return false;
}

string stringSubtract(string a, string b)
{
    /* optimised string subtraction code from: https\://www.geeksforgeeks.org/difference-of-two-large-numbers/ */
    string result;
    bool negative = false;
    if (isSmaller(a, b))
    {
        swap(a,b);
        negative = true;
    }
    
    // Take an empty string for storing result
    
    // Calculate lengths of both string
    long n1 = a.length(), n2 = b.length();
    long diff = n1 - n2;
    
    // Initially take carry zero
    int carry = 0;
    
    // Traverse from end of both strings
    for (long i=n2-1; i>=0; i--)
    {
        int sub = ((a[i+diff]-'0') -
                   (b[i]-'0') -
                   carry);
        if (sub < 0)
        {
            sub = sub+10;
            carry = 1;
        }
        else
            carry = 0;
        
        result.push_back(sub + '0');
    }
    
    // subtract remaining digits of str1[]
    for (long i=n1-n2-1; i>=0; i--)
    {
        if (a[i]=='0' && carry)
        {
            result.push_back('9');
            continue;
        }
        int sub = ((a[i]-'0') - carry);
        if (i>0 || sub>0) // remove preceding 0's
            result.push_back(sub+'0');
        carry = 0;
        
    }
    
    // reverse resultant string
    reverse(result.begin(), result.end());
    if(negative)
    {
        result.insert(0,1,'-');
    }
    
    return result;
}


string karatsubaMultiplication(string& x, string& y)
{
    /*
     Karatsuba multiplication
     x = 10^(n/2)*a + b
     y = 10^(n/2)*c + d
     x*y = 10^(n)*ac + 10^(n/2)*(ad + bc) + bd
     ad + bc = (a + b)(c+d) - ac-bd
     1. compute ac recursively
     2. compute bd recursively
     3. compute (a+b)(c+d) recursively .... ad+bc = (a+b)(c+d) - ac - bd
     4. Calculate x*y by stitching all pieces together
     */
    std::size_t DigitsInx = x.length();
    std::size_t DigitsIny = y.length();
    unsigned long totalDigits = 0;
    
    if (DigitsInx ==1  && DigitsIny ==1)
    {
        return stringMultiply(x,y);
    }
    /*+++ make the length of the strings same by adding preceding zeros to the shorter string+++ */
    if (DigitsIny > DigitsInx)
    {
        totalDigits = DigitsIny;
        
        x.insert(0,(totalDigits-DigitsInx),'0');
    }
    else if(DigitsInx > DigitsIny)
    {
        totalDigits = DigitsInx;
        y.insert(0,(totalDigits-DigitsIny), '0');
    }
    else
    {
        totalDigits = DigitsInx;
    }
    /*---make the length of the strings same by adding preceding zeros to the shorter string--- */
    string a, b, c, d;
    a = x.substr(0,totalDigits/2);
    b = x.substr((totalDigits/2));
    
    c = y.substr(0,totalDigits/2);
    d = y.substr((totalDigits/2));
    
    /*+++ recurcively compute ac, bd and (a+b)(c+d)+++ */
    string ac;
    ac = karatsubaMultiplication(a,c);
    
    string bd;
    bd = karatsubaMultiplication(b,d);
    
    string midsum;
    string stringSumAB = stringSum(a,b);
    string stringSumCD = stringSum(c,d);
    midsum = karatsubaMultiplication(stringSumAB,stringSumCD);
    /*--- recurcively compute ac, bd and (a+b)(c+d)--- */

    /*compute ad+bc = (a+b)(c+d) - ac - bd*/
    midsum = stringSubtract(midsum,stringSum(ac, bd));
    
    /*+++ stitch together x*y = 10^(n)*ac + 10^(n/2)*(ad + bc) + bd +++*/
    
    //adjust the string length
    if (totalDigits%2 != 0)
    {
        totalDigits = totalDigits + 1;
    }
    //10^(n)*ac
    for(int i = 0; i < totalDigits; i++)
    {
        ac.push_back('0');
    }
    //10^(n/2)*(ad + bc)
    for(int i = 0; i < totalDigits/2; i++)
    {
        midsum.push_back('0');
    }
    
    string result;
    string step1 = stringSum(ac, bd);
    if(midsum[0]=='-')
    {
        midsum.erase(0,1);
        result= stringSubtract(step1, midsum);
        return result;
    }
    else
    {
        result= stringSum(step1, midsum);
        return result;
    }
    /*--- stitch together x*y = 10^(n)*ac + 10^(n/2)*(ad + bc) + bd ---*/
    
}

bool isInputNumber(const string &input)
{
    return std::all_of(input.begin(), input.end(), ::isdigit);
}

int main(int argc, const char * argv[]) {
    string X;
    string Y;
    /*For this assignment input are 2 specific 64-digit numbers
     hence keeping reading and validating input code to minimum for now; 
     add input validation code for testing*/
    cout << "enter 1st positive number to multiply:\n";
    getline(cin,X);
    cout << "enter 2nd positive number to multiply:\n";
    getline(cin,Y);
    //Add code to check validity of the input
    //Later use library function or add more checks for leading/trailing spaces etc
    if(!isInputNumber(X) || !isInputNumber(Y))
    {
        cout << "Input should be a valid positive number:\n" << endl;
        return 0;
    }
    string result = karatsubaMultiplication(X,Y);
    size_t nonZeroPosition = result.find_first_not_of('0');
    if(nonZeroPosition == std::string::npos)
    {
         cout << "result:\n" << 0 <<  endl;
    }
    else
    {
        cout << "result:\n" << result.substr(nonZeroPosition) <<  endl;
    }
    return 0;
}



