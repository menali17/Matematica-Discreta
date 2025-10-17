/*
 * =========================================================
 * Razão de Eficiencia entre o(N) e t(N)

* Autores: Enzo Menali e Geovanna Alves
 * Data: 15/10/2025
 * =========================================================
 */


#include <stdio.h>

int ipow(int base, int exp) {
    int r = 1, i;
    for (i = 0; i < exp; i++) r = r * base;
    return r;
}

int fatorar(int n, int primos[], int expoentes[]) {
    int i, e, k = 0;
    for (i = 2; i <= n; i++) {
        e = 0;
        while (n % i == 0) {
            n = n / i;
            e++;
        }
        if (e > 0) {
            primos[k] = i;
            expoentes[k] = e;
            k++;
        }
        if (n == 1) break;
    }
    return k;
}

int calcula_t(int primos[], int expoentes[], int qtd, int N) {
    int i, t = 1;
    printf("\nCalculo intermediario de t(%d):\n", N);
    printf("t(%d) = ", N);
    for (i = 0; i < qtd; i++) {
        printf("(%d+1)", expoentes[i]);
        t = t * (expoentes[i] + 1);
        if (i < qtd - 1) printf(" x ");
    }
    printf(" = %d\n", t);
    return t;
}

int calcula_o(int primos[], int expoentes[], int qtd, int N) {
    int i, sigma = 1;
    printf("\nCalculo intermediario de o(%d):\n", N);
    printf("o(%d) = ", N);
    for (i = 0; i < qtd; i++) {
        int p = primos[i];
        int e = expoentes[i];
        int num = ipow(p, e + 1) - 1;
        int den = p - 1;
        int termo = num / den;
        printf("((%d^(%d+1)-1)/(%d-1))", p, e, p);
        sigma = sigma * termo;
        if (i < qtd - 1) printf(" x ");
    }
    printf(" = %d\n", sigma);
    return sigma;
}

int main() {
    int N, primos[10], expoentes[10], qtd;
    int tN, oN;
    double razao;

    printf("Digite um numero: ");
    scanf("%d", &N);

    if (N < 1 || N > 105) {
        printf("Erro: o valor de N deve estar entre 1 e 105.\n");
        return 0;
    }
    else if (N == 1) {
            printf("\n O número 1 não pode ser decomposto em fatores primos\n");
            printf("\nRazao de Eficiencia = o(1)/t(1) = 1/1 = 1.00\n");
            return 0;
    }

    qtd = fatorar(N, primos, expoentes);

    printf("\nFatores primos de %d: ", N);
    for (int i = 0; i < qtd; i++) {
        printf("%d^%d", primos[i], expoentes[i]);
        if (i < qtd - 1) printf(" * ");
    }
    printf("\n");

    tN = calcula_t(primos, expoentes, qtd, N);
    oN = calcula_o(primos, expoentes, qtd, N);
    razao = (double)oN / (double)tN;

    printf("\nRazao de Eficiencia = o(N)/t(N) = %d/%d = %.2f\n", oN, tN, razao);

    return 0;
}
