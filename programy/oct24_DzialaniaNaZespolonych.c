#include <stdio.h>
typedef struct
{
    float re, im;
} zespolona;

zespolona mnozenie(zespolona A, zespolona B)
{
    return (zespolona) { .re = A.re * B.re, .im = A.im * B.im };
}
    
zespolona dzielenie(zespolona A, zespolona B)
{
    return (zespolona) { .re = A.re / B.re, .im = A.im / B.im };
}

zespolona dodawanie(zespolona A, zespolona B)
{
    return (zespolona) { .re = A.re + B.re, .im = A.im + B.im };
}
zespolona odejmowanie(zespolona A, zespolona B)
{
    return (zespolona) { .re = A.re - B.re, .im = A.im - B.im };
}


void main()
{
    zespolona A = { .re = 7, .im = 5 }, B;

    B = dodawanie(A, (zespolona) { .re = -1, .im = 3});
    printf("dodawanie  (%.4f + i%.4f)\n", B.re, B.im);
    B = odejmowanie(A, (zespolona) { .re = -1, .im = 3});
    printf("odejmowanie  (%.4f - i%.4f)\n", B.re, B.im);
    B = mnozenie(A, (zespolona) { .re = -1, .im = 3});
    printf("mnozenie  (%.4f * i%.4f)\n", B.re, B.im);
    B = dzielenie(A, (zespolona) { .re = -1, .im = 3});
    printf("dzielenie  (%.4f / i%.4f)\n", B.re, B.im);
}