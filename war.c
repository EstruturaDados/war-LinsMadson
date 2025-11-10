// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

void exibirMapa(Territorio* mapa, int n) {
    printf("\n==== MAPA DE TERRITÓRIOS ====\n");
    for (int i = 0; i < n; i++)
        printf("[%d] %-15s | Cor: %-8s | Tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    printf("=================================\n");
}

int atacar(Territorio* a, Territorio* d) {
    int da = rand() % 6 + 1;
    int dd = rand() % 6 + 1;

    printf("\n%s tirou %d | %s tirou %d\n",
           a->nome, da, d->nome, dd);

    if (da > dd) {
        d->tropas--;
        printf("%s venceu! %s perdeu 1 tropa.\n", a->nome, d->nome);
        if (d->tropas <= 0) {
            printf("%s foi conquistado!\n", d->nome);
            strcpy(d->cor, a->cor);
            d->tropas = a->tropas / 2;
            if (d->tropas < 1) d->tropas = 1;
            a->tropas -= d->tropas;
            if (a->tropas < 1) a->tropas = 1;
            return 1; // conquistou
        }
    } else {
        a->tropas--;
        if (a->tropas < 0) a->tropas = 0;
        printf("%s defendeu! %s perdeu 1 tropa.\n", d->nome, a->nome);
    }
    return 0;
}

void atribuirMissao(char* destino, int totalTerritorios) {
    int qtd = rand() % (totalTerritorios - 1) + 1;
    sprintf(destino, "Conquistar %d territórios", qtd);
}

int verificarMissao(char* missao, int conquistados) {
    int alvo;
    sscanf(missao, "Conquistar %d", &alvo);
    return conquistados >= alvo;
}

int main() {
    srand(time(NULL));
    printf("=== WAR ESTRUTURADO ===\n");

    int n;
    printf("Digite a quantidade de territórios: ");
    scanf("%d", &n);

    Territorio* mapa = calloc(n, sizeof(Territorio));

    for (int i = 0; i < n; i++) {
        printf("\nTerritório %d:\nNome: ", i + 1);
        scanf(" %29[^\n]", mapa[i].nome);
        printf("Cor: ");
        scanf(" %9[^\n]", mapa[i].cor);
        printf("Tropas: ");
        scanf("%d", &mapa[i].tropas);
    }

    char* missao = malloc(100);
    atribuirMissao(missao, n);
    printf("\nMissão do jogador: %s\n", missao);

    int conquistados = 0, opcao;
    do {
        exibirMapa(mapa, n);
        printf("\n1 - Atacar\n2 - Verificar Missão\n3 - Sair\nOpção: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            int a, d;
            printf("Atacante: ");
            scanf("%d", &a);
            printf("Defensor: ");
            scanf("%d", &d);

            if (a < 1 || a > n || d < 1 || d > n || a == d) {
                printf("Índices inválidos!\n");
                continue;
            }
            if (strcmp(mapa[a - 1].cor, mapa[d - 1].cor) == 0) {
                printf("Não pode atacar território da mesma cor!\n");
                continue;
            }

            int ganhou = atacar(&mapa[a - 1], &mapa[d - 1]);
            if (ganhou) conquistados++;

            if (verificarMissao(missao, conquistados)) {
                printf("\nMissão cumprida! Você conquistou %d territórios e venceu o jogo!\n", conquistados);
                break;
            }
        }

        if (opcao == 2) {
            int alvo;
            sscanf(missao, "Conquistar %d", &alvo);
            printf("\nProgresso: %d de %d territórios conquistados.\n", conquistados, alvo);
            if (verificarMissao(missao, conquistados)) {
                printf("Parabéns! Missão concluída!\n");
                break;
            } else {
                printf("Missão ainda não concluída.\n");
            }
        }

    } while (opcao != 3);

    free(missao);
    free(mapa);
    printf("\nFim do jogo!\n");
    return 0;
}