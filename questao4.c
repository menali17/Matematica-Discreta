
/*
 * =========================================================
 * Divisão Modular e Inverso Modular

* Autores: Enzo Menali e Geovanna Alves
 * Data: 15/10/2025
 * =========================================================
 */

#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif

int mdcComPassos(int a, int b) {
    int resto;
    while (b != 0) {                 // 1) enquanto bo segundo valor for diferente de 0
        resto = a % b;
        printf("Algoritmo de Euclides: %d mod %d = %d\n", a, b, resto);
        a = b;
        b = resto;                   // 2) atualiza a e b
    }
    return a;                        
}

int inversoModular(int a, int m) {
    int m0 = m, t, q;
    int x0 = 0;
    int x1 = 1;
    int A = a, B = m;

    if (mdcComPassos(a, m) != 1) {
        printf("Nao existe inverso de %d modulo %d.\n", a, m);
        return -1;
    }

    while (m != 0) {
        q = a / m;
        t = m;       m = a % m;   a = t;

        t = x0;      x0 = x1 - q * x0;   x1 = t;
    }

    if (x1 < 0)
        x1 += m0; // 4) garante resultado positivo

    printf("\nSubstituindo, temos que o inverso de %d em %d é %d.\n\n", A, B, x1);
    return x1;
}

int powMod(int base, int exp, int mod) {
    long long res = 1;
    long long b = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1)           // 5) se expoente é ímpar
            res = (res * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return (int)res;
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    int H, G, Zn, x, n1;

    printf("Insira H: ");
    scanf("%d", &H);
    printf("Insira G: ");
    scanf("%d", &G);
    printf("Insira Zn: ");
    scanf("%d", &Zn);
    printf("Insira x: ");
    scanf("%d", &x);
    printf("Insira n1: ");
    scanf("%d", &n1);
    printf("\n");

    
    int inverso = inversoModular(G, Zn); // 7) inverso modular
    if (inverso == -1) return 0;
    int a = (H * inverso) % Zn;

    printf("Fazendo a multiplicacao modular: %d * %d mod %d = %d\n", H, inverso, Zn, a);
    printf("  Sendo %d o inverso de %d.\n", inverso, G);

    int resultado = powMod(a, x, n1); // 8) exponenciacao modular
    printf("Valor final da congruencia: %d\n", resultado);

    return 0;
}


//Para H = 7, G = 3, Zn = 11, x = 10 e n1 = 13
 //O resultado final é 4