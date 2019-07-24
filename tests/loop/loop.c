int sum(int a, int b)
{
    int sum;
    sum = 0;
    while (a <= b)
    {
        sum += a;
        a++;
    }
    return sum;
}

int main(void)
{
    return sum(1, 10);
}
