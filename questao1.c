/*
 * =========================================================
 * Atividade de Criptografia RSA com Fatoração de Pollard Rho

* Autores: Enzo Menali e Geovanna Alves
 * Data: 15/10/2025
 * =========================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int pre_codificar(char c);
char decodificar(int codigo);
long long mdc(long long a, long long b);
long long func_pollard(long long x, long long n);
long long pollard_rho(long long n);
long long solicitar_num(char *nome);
long long inverso_mod(long long a, long long m);
long long escolher_E(long long z);
long long exp_mod(long long M, long long E, long long n, long long phi_n);
void codificar_decodificar(long long E, long long D, long long n, long long phi_n);
int is_primo(long long n);

// Pre-codificacao
int pre_codificar(char c) {
    if (c >= 'A' && c <= 'Z') return 11 + (c - 'A');
    else if (c >= 'a' && c <= 'z') return 11 + (c - 'a');  
    else if (c == ' ') return 0;
    else return -1; 
}

// Decodificar 
char decodificar(int codigo) {
    if (codigo == 0) return ' ';
    else return 'A' + (codigo - 11);
}

// MDC por Algoritmo de Euclides 
long long mdc(long long a, long long b) {
    long long a_original = a, b_original = b;
    printf("   Calculando mdc(%lld, %lld):\n", a, b);
    int passo = 0;
    while (b != 0) {
        long long resto = a % b;
        passo++;
        printf("     [%d] %lld = (%lld * %lld) + %lld\n", passo, a, a / b, b, resto);
        a = b;
        b = resto;
    }
    printf("   Resultado: mdc(%lld, %lld) = %lld\n", a_original, b_original, a);
    return a;
}

// Fatoracao de Pollard Rho
long long func_pollard(long long x, long long n) {
    long long x2 = x * x;
    long long soma = x2 + 1;
    long long resultado = soma % n;
    printf("   Calculando g(%lld) = (%lld^2 + 1) mod %lld = (%lld + 1) mod %lld = %lld\n",
           x, x, n, x2, n, resultado);
    return resultado;
}

long long pollard_rho(long long n) {
    if (n % 2 == 0) {
        printf("Como %lld e par, um fator e 2.\n", n);
        return 2;
    }

    long long x = 2, y = 2, d = 1;
    int iter = 0;

    printf("\n Iniciando Pollard Rho para N = %lld ---\n", n);
    printf("Semente x0 = 2\n");

    while (d == 1) {
        iter++;
        printf("\nIteracao %d:\n", iter);

        x = func_pollard(x, n);
        long long temp = func_pollard(y, n);
        y = func_pollard(temp, n);

        long long diff = llabs(x - y);
        printf("   |x - y| = |%lld - %lld| = %lld\n", x, y, diff);

        d = mdc(diff, n);

        if (d == 1) {
            printf("   Nenhum fator encontrado nesta iteracao.\n");
        } else if (d == n) {
            printf("   Falha: mdc = n. Tente outro numero ou outro metodo.\n");
            return -1;
        } else {
            printf("   Fator encontrado: %lld\n", d);
            break;
        }
    }

    return d;
}

// Solicitando N1 e N2
long long solicitar_num(char *nome) {
    long long N;
    int valido = 0;
    do {
        printf("Digite %s (produto de primos distintos, 3 ou 4 digitos, 100 a 9999): ", nome);
        if (scanf("%lld", &N) != 1) {
            printf("Entrada invalida. Digite apenas numeros.\n");
            while (getchar() != '\n');
            continue;
        }
        if (N < 100 || N > 9999) {
            printf("Numero fora do intervalo! Tente novamente.\n");
        } else {
            valido = 1;
        }
    } while (!valido);
    return N;
}

// Inverso modular 
long long inverso_mod(long long a, long long m) {
    long long r1 = a, r2 = m;        // restos
    long long s1 = 1, s2 = 0;        // coeficientes para a
    long long t1 = 0, t2 = 1;        // coeficientes para 'm'
    long long q, r, s, t;
    int iter = 0;

    printf("\n=== Resolvendo a equacao modular inversa ===\n");
    printf("Procurando D tal que %lld * D equiv 1 (mod %lld)\n", a, m);
    printf("Equivalente a resolver: %lld*D - k*%lld = 1\n\n", a, m);

    while (r2 != 0) {
        iter++;
        q = r1 / r2;
        r = r1 - q * r2;
        s = s1 - q * s2;
        t = t1 - q * t2;

    printf("Iteracao %d:\n", iter);
        printf("  q = %lld\n", q);
        printf("  r = r1 - q*r2 = %lld - %lld*%lld = %lld\n", r1, q, r2, r);
        printf("  s = s1 - q*s2 = %lld - %lld*%lld = %lld\n", s1, q, s2, s);
        printf("  t = t1 - q*t2 = %lld - %lld*%lld = %lld\n", t1, q, t2, t);
        printf("  Atualizando: r1 = %lld, r2 = %lld, s1 = %lld, s2 = %lld, t1 = %lld, t2 = %lld\n\n",
               r2, r, s2, s, t2, t);

        r1 = r2;
        r2 = r;

        s1 = s2;
        s2 = s;

        t1 = t2;
        t2 = t;
    }

    if (r1 != 1) {
        printf("Nao existe inverso modular, pois MDC(%lld, %lld) = %lld != 1\n", a, m, r1);
        return -1;
    }

    long long D = s1;
    if (D < 0) D += m;  // ajustar para positivo

    printf("Inverso modular encontrado: D = %lld\n", D);
    printf("Verificacao: (%lld * %lld) mod %lld = %lld\n", a, D, m, (a*D)%m);

    return D;
}

// Escolhendo o expoente publico 
long long escolher_E(long long z) {
    long long E;
    printf("\nEscolhendo expoente publico E (menor inteiro >1 coprimo de z = %lld)...\n", z);
    for (E = 2; E < z; E++) {
        if (mdc(E, z) == 1) {
            printf("E = %lld\n", E);
            return E;
        }
    }
    return -1; 
}

// Exponenciacao modular
long long exp_mod(long long M, long long E, long long n, long long phi_n) {
    printf("\nCalculando %lld^%lld mod %lld\n", M, E, n);

    long long expo_reduzido = E;

    if (M % n == 0) {
        printf("M e multiplo de n, resultado sera 0.\n");
        return 0;
    }

    if (mdc(M, n) == 1) {
        if (phi_n != n - 1) {
            printf("Aplicando Teorema de Euler, pois mdc(M,n)=1 e n nao e primo.\n");
            long long r = E % phi_n;
            expo_reduzido = r;
            printf("Expoente reduzido: %lld mod %lld = %lld\n", E, phi_n, expo_reduzido);
        } else {
            printf("Aplicando Pequeno Teorema de Fermat, pois n e primo.\n");
            long long r = E % (n - 1);
            expo_reduzido = r;
            printf("Expoente reduzido: %lld mod %lld = %lld\n", E, (n - 1), expo_reduzido);
        }
    } else {
        printf("Aplicando Divisao Euclidiana.\n");
        long long r = E % (n - 1);
        expo_reduzido = r; 
        printf("Expoente reduzido: %lld mod %lld = %lld\n", E, (n - 1), expo_reduzido);
    }

    long long resultado = 1;
    long long base = M % n;
    long long e = expo_reduzido;

    while (e > 0) {
        if (e % 2 == 1) {
            long long resultado_ant = resultado;
            resultado = (resultado * base) % n;
            printf("  Multiplica: (%lld * %lld) mod %lld equiv %lld\n", resultado_ant, base, n, resultado);
        }
        long long base_ant = base;
        base = (base * base) % n;
        e = e / 2;
        printf("  Quadrado da base: (%lld^2) mod %lld equiv %lld, expoente = %lld\n", base_ant, n, base, e);
    }

    printf("Resultado final: %lld\n", resultado);
    return resultado;
}
 
// Etapa 3: Codificacao (Criptografia) e Decodificacao (Descriptografia)

void codificar_decodificar(long long E, long long D, long long n, long long phi_n) {
    printf("\n=== Etapa 3: Codificacao (Criptografia) e Decodificacao (Descriptografia) ===\n");
    char mensagem[1024];
    printf("\nDigite a mensagem (letras maiusculas, minusculas e espacos): ");
    while(getchar() != '\n'); 
    fgets(mensagem, sizeof(mensagem), stdin);

    // Pre-codificacao 
    int blocos[1024];
    int tamanho = 0;
    for(int i=0; mensagem[i] != '\0' && mensagem[i] != '\n'; i++) {
        int cod = pre_codificar(mensagem[i]);
        if(cod == -1) continue; 
        blocos[tamanho++] = cod;
    }

    // Codificacao
    long long cifrados[1024];
    printf("\n=== Criptografando mensagem ===\n");
    for(int i=0; i<tamanho; i++) {
        printf("\nBloco M = %02d -> letra: '%c'\n", blocos[i], decodificar(blocos[i]));
        cifrados[i] = exp_mod(blocos[i], E, n, phi_n);
        printf("Bloco Codificado C = %lld\n", cifrados[i]);
    }

    printf("\nMensagem criptografada:\n");
    for(int i=0; i<tamanho; i++) {
        printf("%lld ", cifrados[i]);
    }
    printf("\n");

    //  Decodificacao 
    printf("\n=== Decodificando mensagem ===\n");
    char decodificada[1024];
    for(int i=0; i<tamanho; i++) {
        printf("\nBloco C = %lld\n", cifrados[i]);
        long long M = exp_mod(cifrados[i], D, n, phi_n);
        char letra = decodificar((int)M);
        printf("Bloco decodificado M = %lld -> letra: '%c'\n", M, letra);
        decodificada[i] = letra;
    }
    decodificada[tamanho] = '\0';

 
    printf("\nMensagem decodificada: %s\n", decodificada);
}

// Teste de primalidade simples (suficiente para 3-4 digitos)
int is_primo(long long n) {
    if (n < 2) return 0;
    if (n % 2 == 0) return n == 2;
    for (long long i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}


// Etapa 1: Fatoracao Interativa (Metodo ρ de Pollard)
int main() {
    printf("\n=== Etapa 1: Fatoracao Interativa (Metodo rho de Pollard) ===\n");
    printf("Aviso: cada numero deve ser produto de primos distintos.\n\n");

    long long N1 = solicitar_num("N1");
    long long N2 = solicitar_num("N2");

    long long p = pollard_rho(N1);
    if (p == -1) { printf("\nFalha na fatoracao de N1. Encerrando.\n"); return 1; }
    long long cofator_N1 = (p != 0) ? (N1 / p) : 0;
    if (p <= 1 || cofator_N1 <= 1 || p == cofator_N1 || !is_primo(p) || !is_primo(cofator_N1)) {
        printf("\n[Validacao] N1 nao atende: deve ser produto de primos distintos. Fatores obtidos: %lld e %lld\n", p, cofator_N1);
        return 1;
    }

    long long q = pollard_rho(N2);
    if (q == -1) { printf("\nFalha na fatoracao de N2. Encerrando.\n"); return 1; }
    long long cofator_N2 = (q != 0) ? (N2 / q) : 0;
    if (q <= 1 || cofator_N2 <= 1 || q == cofator_N2 || !is_primo(q) || !is_primo(cofator_N2)) {
        printf("\n[Validacao] N2 nao atende: deve ser produto de primos distintos. Fatores obtidos: %lld e %lld\n", q, cofator_N2);
        return 1;
    }

    printf("\n=== Fatores primos ===\n");
    printf("p (N1) = %lld\n", p);
    printf("q (N2) = %lld\n", q);

// Etapa 2: Geracao das Chaves RSA
    printf("\n=== Etapa 2: Geracao das Chaves RSA ===\n");
    long long n = p * q;
    printf("\nModulo n = p * q = %lld * %lld = %lld\n", p, q, n);

    long long z = (p-1)*(q-1);
    printf("Totiente de Euler: z(n) = (p-1)*(q-1) = (%lld-1)*(%lld-1) = %lld\n", p, q, z);

    long long E = escolher_E(z);
    long long D = inverso_mod(E, z);

    printf("\n=== Gerando Chaves RSA ===\n");
    printf("Chave publica  (n, E) = (%lld, %lld)\n", n, E);
    printf("Chave privada (n, D) = (%lld, %lld)\n", n, D);

   
    codificar_decodificar(E, D, n, z);

    return 0;
}
