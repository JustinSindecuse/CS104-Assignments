#include <string>
#include <iostream>
using namespace std;

void helper(string a, int x, int y){
    if (x == y)  
        cout << a << endl;  
    else
    {
        for (int i = x; i <= y; i++)  
        {  
            
            //swap
            char letter;
            letter = a[x];
            a[x] = a[i];
            a[i] = letter;
  
            // Recursion
            helper(a, x+1, y);  
  
            //swap back
            letter = a[x];
            a[x] = a[i];
            a[i] = letter; 
        }  
    }  
}


void permutations(string a)  
{  
    helper(a, 0, int(a.size())-1);
}
