#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define P 896717ULL
#define Q 549863ULL

typedef uint64_t u64; // uint64_t er egentlig bare unsigned long long, men typedef gør det nemmere at skrive som u64.

u64 gcd(u64 a, u64 b) // Beregner den største fælles divisor (GCD) af a og b ved hjælp af Euklids algoritme
{
    while (b != 0) {
        u64 tmp = a % b;
        a = b;
        b = tmp;
    }
    return a;
}

u64 choose_e(int phi) // Vælger en offentlig eksponent e, der er relativt primær til phi
{
    for (u64 e = 2; e < (u64)phi; e++) { // Starter fra 2, da 1 ikke er en gyldig kandidat
        if (gcd(e, (u64)phi) == 1ULL) { // Hvis gcd(e, phi) er 1, så er e en kandidat
            return e; // Returnerer den første kandidat fundet
        }
    }
    return 0ULL; // Returnerer 0 hvis ingen kandidat blev fundet
}

u64 modular_inverse(u64 e, u64 phi) {
    long long t = 0;
    long long new_t = 1;
    long long r = (long long)phi;
    long long new_r = (long long)e;

    while (new_r != 0) {
        long long quotient = r / new_r;

        long long temp_t = t - quotient * new_t;
        t = new_t;
        new_t = temp_t;

        long long temp_r = r - quotient * new_r;
        r = new_r;
        new_r = temp_r;
    }

    if (r != 1) {
        return 0;
    }

    if (t < 0) {
        t += (long long)phi;
    }

    return (u64)t;
}

u64 add_mod(u64 a, u64 b, u64 mod) {
    a %= mod;
    b %= mod;
    if (a >= mod - b) {
        return a - (mod - b);
    }
    return a + b;
}

u64 mul_mod(u64 a, u64 b, u64 mod) {
    u64 result = 0ULL;
    a %= mod;

    while (b > 0) {
        if (b & 1ULL) {
            result = add_mod(result, a, mod);
        }
        a = add_mod(a, a, mod);
        b >>= 1;
    }

    return result;
}

u64 mod_pow(u64 base, u64 exp, u64 mod) {
    u64 result = 1ULL;
    base %= mod;

    while (exp > 0) {
        if (exp & 1ULL) {
            result = mul_mod(result, base, mod);
        }
        base = mul_mod(base, base, mod);
        exp >>= 1;
    }

    return result;
}

u64 encrypt(u64 message, u64 e, u64 n) {
    return mod_pow(message, e, n);
}

u64 decrypt(u64 cipher, u64 d, u64 n) {
    return mod_pow(cipher, d, n);
}

int main(void) {
    u64 n = (u64)P * Q;
    u64 phi = ((u64)P - 1) * ((u64)Q - 1);
    u64 e = choose_e(phi);
    u64 d = modular_inverse(e, phi);

    const char *str = "Hello, RSA!";
    size_t len = strlen(str);

    u64 encrypted[64] = {0};
    char decrypted[64] = {0};

    if (len >= sizeof(decrypted)) // Hvis input-strengen er for lang, udskrives en fejlmeddelelse og programmet afsluttes
    {
        printf("Input-strengen er for lang (maks %llu tegn).\n", (unsigned long long)(sizeof(decrypted) - 1));
        return 1;
    }

    if (e == 0 || d == 0) // Hvis der ikke kan findes en gyldig e eller d, udskrives en fejlmeddelelse og programmet afsluttes
    {
        printf("Kunne ikke beregne gyldige RSA-nøgler for de valgte P og Q.\n");
        return 1;
    }

    printf("Starter RSA eksempel...\n");
    printf("P = %llu, Q = %llu\n", (unsigned long long)P, (unsigned long long)Q); // Udskriver de valgte primtal P og Q
    printf("N = %llu, PHI = %llu\n", (unsigned long long)n, (unsigned long long)phi); // Udskriver N og PHI
    printf("Offentlig nøgle: (%llu, %llu)\n", (unsigned long long)n, (unsigned long long)e); // Udskriver den offentlige nøgle (N, e)
    printf("Privat nøgle: (%llu, %llu)\n", (unsigned long long)n, (unsigned long long)d); // Udskriver den private nøgle (N, d). Man behøver egentlig ikke at usdskrive N, da den allerede er angivet.

    for (size_t i = 0; i < len; i++) 
    {
        encrypted[i] = encrypt((u64)(unsigned char)str[i], e, n);
        decrypted[i] = (char)decrypt(encrypted[i], d, n);
    }
    decrypted[len] = '\0';

    printf("Original: %s\n", str);
    printf("Krypterede blokke: ");
    for (size_t i = 0; i < len; i++) 
    {
        printf("%llu%s", (unsigned long long)encrypted[i], (i + 1 < len) ? " " : ""); // Udskriver krypterede blokke adskilt af mellemrum
    }
    printf("\n");
    printf("Dekrypteret: %s\n", decrypted);

    printf("Tryk Enter for at afslutte...\n");
    getchar(); // Venter på at brugeren trykker enter før programmet afsluttes

    return 0;
}