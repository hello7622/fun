int FiboRecursive(int n) 
{
    if (n <= 1) return n;
    return FiboRecursive(n - 1) + FiboRecursive(n - 2);
}

int FiboIterative(int n) 
{
    if (n <= 1) return n;
    int a = 0, b = 1, c;
    for (int i = 2; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

#include <assert.h>

int main()
{
    for (int i = 0; i < 100; i++) 
    {
        assert(FiboRecursive(i) == FiboIterative(i));
    }
    return 0;
}