#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define TAM 1000 // Dimensão estática para os arrays

// 1. Definição das Estruturas de Dados
typedef struct
{
    char id[20], nome[100], nif[10], nib[22];
} Cliente;

typedef struct
{
    char id_cliente[20];
    int ano, mes;
    float consumo_kwh;
} Consumo;

typedef struct
{
    int ano, mes;
    float preco_kwh;
} Periodo;

// 2. Protótipos das Funções
bool validarNIF(char nif[]);
bool validarNIB(char nib[]);
int carregarClientes(Cliente lista[]);
int carregarConsumos(Consumo lista[]);
int carregarPeriodos(Periodo lista[]);
void gerarFicheiroPS2(int anoA, int mesA, Cliente cli[], int nC, Consumo con[], int nCo, Periodo per[], int nP);

int main()
{
    Cliente clientes[TAM];
    Consumo consumos[TAM];
    Periodo periodos[TAM];

    // Carregamento dos dados
    int nC = carregarClientes(clientes);
    int nCo = carregarConsumos(consumos);
    int nP = carregarPeriodos(periodos);

    if (nC == 0 || nCo == 0 || nP == 0)
    {
        printf("Erro ao carregar ficheiros. Verifique se clientes.txt, consumos.txt e periodos.txt existem.\n");
        return 1;
    }

    int anoInput, mesInput;
    printf("--- IPCA Energy - Gerador de Débitos Diretos ---\n");
    printf("Indique o ano (AAAA): ");
    scanf("%d", &anoInput);
    printf("Indique o mes (MM): ");
    scanf("%d", &mesInput);

    gerarFicheiroPS2(anoInput, mesInput, clientes, nC, consumos, nCo, periodos, nP);

    return 0;
}

// --- Validação de NIF (Anexo A) ---
bool validarNIF(char nif[])
{
    if (strlen(nif) != 9)
        return false;
    for (int i = 0; i < 8; i++)
    {
        if (!isdigit(nif[i]))
            return false;
    }

    char p = nif[0];
    if (p != '1' && p != '2' && p != '5' && p != '6' && p != '8' && p != '9')
        return false;

    int soma = 0;
    for (int i = 0; i < 8; i++)
    {
        soma += (nif[i] - '0') * (9 - i);
    }
    int resto = soma % 11;
    int digitoControlo = (resto == 0 || resto == 1) ? 0 : 11 - resto;

    return (digitoControlo == (nif[8] - '0'));
}

// --- Validação de NIB (Anexo B) ---
bool validarNIB(char nib[])
{
    if (strlen(nib) != 21)
        return false;
    long long resto = 0;
    for (int i = 0; i < 21; i++)
    {
        if (!isdigit(nib[i]))
            return false;
        resto = (resto * 10 + (nib[i] - '0')) % 97;
    }
    return (resto == 1);
}

// --- Leitura de Ficheiros ---
int carregarClientes(Cliente lista[])
{
    FILE *f = fopen("clientes.txt", "r");
    if (!f)
        return 0;
    int i = 0;
    while (i < TAM && fscanf(f, " %[^;]; %[^;]; %[^;]; %[^\n]", lista[i].id, lista[i].nome, lista[i].nif, lista[i].nib) == 4)
    {
        i++;
    }
    fclose(f);
    return i;
}

int carregarConsumos(Consumo lista[])
{
    FILE *f = fopen("consumos.txt", "r");
    if (!f)
        return 0;
    int i = 0;
    while (i < TAM && fscanf(f, " %[^;]; %d; %d; %f", lista[i].id_cliente, &lista[i].ano, &lista[i].mes, &lista[i].consumo_kwh) == 4)
    {
        i++;
    }
    fclose(f);
    return i;
}

int carregarPeriodos(Periodo lista[])
{
    FILE *f = fopen("periodos.txt", "r");
    if (!f)
        return 0;
    int i = 0;
    while (i < TAM && fscanf(f, " %d; %d; %f", &lista[i].ano, &lista[i].mes, &lista[i].preco_kwh) == 3)
    {
        i++;
    }
    fclose(f);
    return i;
}

// --- Geração do Ficheiro PS2 (Tipo 1, 2 e 9) ---
void gerarFicheiroPS2(int anoA, int mesA, Cliente cli[], int nC, Consumo con[], int nCo, Periodo per[], int nP)
{
    float preco = -1;
    for (int i = 0; i < nP; i++)
    {
        if (per[i].ano == anoA && per[i].mes == mesA)
        {
            preco = per[i].preco_kwh;
            break;
        }
    }

    if (preco < 0)
    {
        printf("Erro: Preço não definido para o período %02d/%d.\n", mesA, anoA);
        return;
    }

    char nomeF[50];
    sprintf(nomeF, "DD_%04d_%02d.ps2", anoA, mesA);
    FILE *f = fopen(nomeF, "w");

    int totalDebitos = 0;
    double valorTotalAcumulado = 0;
    char bufferDetalhes[TAM][150];

    for (int i = 0; i < nC; i++)
    {
        bool nifVal = validarNIF(cli[i].nif);
        bool nibVal = validarNIB(cli[i].nib);

        if (!nifVal || !nibVal)
        {
            printf("Cliente %s ignorado: Motivo - %s\n", cli[i].id, !nifVal ? "NIF Inválido" : "NIB Inválido");
            continue;
        }

        for (int j = 0; j < nCo; j++)
        {
            if (strcmp(con[j].id_cliente, cli[i].id) == 0 && con[j].ano == anoA && con[j].mes == mesA)
            {
                float valorFinal = con[j].consumo_kwh * preco;

                // Formato Tipo 2 (Detalhe)
                sprintf(bufferDetalhes[totalDebitos], "2%-8s%-21s%-9s%015.0f Fatura Eletricidade %02d/%04d",
                        cli[i].id, cli[i].nib, cli[i].nif, valorFinal * 100, mesA, anoA);

                valorTotalAcumulado += valorFinal;
                totalDebitos++;
            }
        }
    }

    // Escrita Cabeçalho (Tipo 1)
    fprintf(f, "1%04d%02d%-20s%015.0f%06d\n", anoA, mesA, "IPCA Energy", valorTotalAcumulado * 100, totalDebitos);

    // Escrita Detalhes
    for (int i = 0; i < totalDebitos; i++)
    {
        fprintf(f, "%s\n", bufferDetalhes[i]);
    }

    // Escrita Fim (Tipo 9)
    fprintf(f, "9%015.0f%06d\n", valorTotalAcumulado * 100, totalDebitos);

    fclose(f);
    printf("\nProcessamento concluído!\nFicheiro: %s\nTotal de Débitos: %d\nValor Total: %.2f EUR\n", nomeF, totalDebitos, valorTotalAcumulado);
}
