#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

// definindo as direções
#define CIMA 72
#define BAIXO 80
#define ESQUERDA 75
#define DIREITA 77

// definindo a estrutura de cada parte do corpo da cobra
typedef struct NO {
    int x, y; // coordenadas
    struct NO *next; // ponteiro para o proximo da lista
} NO;

// função para criar um novo nó
NO* novoNo(int x, int y) {
    NO *no = (NO*)malloc(sizeof(NO)); // aloca memoria para o novo nó
    no->x = x;
    no->y = y;
    no->next = NULL; // nao esta ligado a nenhum outro nó
    return no; // retorna o ponteiro
}

// função para adicionar um novo nó no inicio da lista, empurrando os nós anteriores para posições subsequentes
void adicionarNo(NO **cabeca, int x, int y) { // cabeca é um ponteiro para ponteiro de NO
    NO *novo_no = novoNo(x, y);
    novo_no->next = *cabeca; //atribui a next o endereço do primeiro nó
    *cabeca = novo_no;
}

// função para imprimir o cenário
void cenario(NO *cabeca, int macaX, int macaY, int altura, int largura) {
    system("cls"); // limpa a tela
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            if (i == 0 || i == altura - 1 || j == 0 || j == largura - 1) // desenha as bordas
                printf("#");
            else if (i == macaY && j == macaX) // desenha a maçã 
                printf("@");
            else {
                NO *atual = cabeca;
                int imprimir_cobra = 0;
                while (atual != NULL) {
                    if (atual->x == j && atual->y == i) {
                        if (atual == cabeca)
                            printf("O"); // desenha a cabeça da cobra
                        else
                            printf("o"); // desenha o corpo da cobra
                        imprimir_cobra = 1;
                        break;
                    }
                    atual = atual->next;
                }
                if (!imprimir_cobra) // se nenhuma parte da cobra foi encontrada na posição atual
                    printf(" ");
            }
        }
        printf("\n");
    }
}

// controlar o cursor 
void coluna_linha(int coluna, int linha) {
    COORD c;
    c.X = coluna;
    c.Y = linha;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

// imprimir a tela de loading
void carregar_jogo(char jogador[100]) {
    system("cls");
    printf("Nome do jogador?\n");
    scanf("%[^\n]s", jogador);
    system("cls");
    printf("Aguarde o jogo ser carregado...\n");
    char load[11] = "carregando";
    for (int i = 0; i < 11; ++i) {
        printf("%c ", load[i]);
        Sleep(((rand() % 100) + 350));
    }
    printf("...\nCarregamento concluido\n");
    Sleep(1000);
    printf("Iniciando...");
    Sleep(500);
    system("cls");
}

// imprimir a tela de gameover
int game_over(){
    int gameover = 1;
    Sleep(800);
    system("cls");
    coluna_linha(4, 4);
    printf("==============================");
    coluna_linha(10, 5);
    printf("G A M E  O V E R");
    coluna_linha(4, 6);
    printf("==============================\n\n\n");
    system("pause");
    return gameover; 
}

// mostrar a pontuação final e fechar o jogo
void perdeu(char jogador[100], int pontos){
    system("cls");
    coluna_linha(4, 2);
    printf("%s, voce perdeu!", jogador);
    coluna_linha(4, 3);
    printf("Seu score final foi %d\n\n", pontos);
    exit(0);
}

int main() {
    int altura = 20, largura = 40, macaX = largura/2, macaY = altura/2, gameover = 0, pontos = 1;
    int a, b, L, L2, opcao; //L = linha ; a = tecla pressionada ; b = posição atual do cursor; L2 = linha anterior; opcao = do menu
    NO *cobra = novoNo(largura/2, altura/2); //começa no centro
    char direcao = DIREITA, jogador[100]; //começa indo para a direita

    do {
        inicio: // rotulo para goto
        opcao = 0, L = 2, L2 = 2, b = 2;
        system("cls");
        printf("\n\n      1 - JOGAR \n      2 - COMO JOGAR \n      3 - SAIR\n");

        do { //faz o menu
            coluna_linha(1, L);
            printf("-->"); //inicializa a seta na linha 3
            if (kbhit) {
                a = getch();
            }
            if (a == 80 && b < 4) { //tecla down desce até o limite
                L2 = L;
                L++;
                b++;
            }
            if (a == 72 && b > 2) { //tecla up sobe até o limite
                L2 = L;
                L--;
                b--;
            }
            if (L != L2) { //se não estiver na linha, limpa a seta 
                coluna_linha(1, L2);
                printf("   ");
                L2 = L;
            }
            if (a == 13) { // tecla enter seleciona a opção
                opcao = b - 1;
            }
        } while (opcao == 0); //enquanto a opção não é selecionada

        switch (opcao) {
            case 1: //começa o jogo
                system("cls"); // limpa a tela 
                carregar_jogo(jogador); // tela de loading solicitando o nome do jogador
                while (!gameover) { // enquanto nao perder 
                    cenario(cobra, macaX, macaY, altura, largura); // imprime o cenario 
                    coluna_linha(5, 22); 
                    printf("Pontos: %d - %s\t", pontos, jogador); // imprime a pontuação junto ao nome na posição mencionada acima

                    // controle de direção
                    if (kbhit()) {
                        a = getch(); // armazena a tecla pressionada em 'a'
                        switch (a) {
                            case CIMA:
                                if (direcao != BAIXO) // verifica se a cobra não está indo para baixo
                                    direcao = CIMA;
                                break;
                            case BAIXO:
                                if (direcao != CIMA) // verifica se a cobra não está indo para cima
                                    direcao = BAIXO;
                                break;
                            case ESQUERDA:
                                if (direcao != DIREITA) // verifica se a cobra não está indo para a direita
                                    direcao = ESQUERDA;
                                break;
                            case DIREITA:
                                if (direcao != ESQUERDA) // verifica se a cobra não está indo para a esquerda
                                    direcao = DIREITA;
                                break;
                            default:
                                break;
                        }
                    }
                    NO *proxima_posicao = NULL; // inicializa o ponteiro da prox posição
                    // movimento da cobra
                    if (a != 0) { // verifica se uma tecla foi pressionada antes de mover a cobra
                        int direcao_temp = direcao; // salva a direção atual da cobra em uma variável temporária
                        switch (direcao_temp) {
                            case CIMA:
                                proxima_posicao = novoNo(cobra->x, cobra->y - 1);
                                break;
                            case BAIXO:
                                proxima_posicao = novoNo(cobra->x, cobra->y + 1);
                                break;
                            case ESQUERDA:
                                proxima_posicao = novoNo(cobra->x - 1, cobra->y);
                                break;
                            case DIREITA:
                                proxima_posicao = novoNo(cobra->x + 1, cobra->y);
                                break;
                            default:
                                break;
                        }
                    }

                    if (proxima_posicao->x == macaX && proxima_posicao->y == macaY) { // verifica se a próxima posição é uma maçã 
                        pontos++; // aumenta a pontuação
                        proxima_posicao->next = cobra; // aumenta a cobra 
                        cobra = proxima_posicao;
                        // gera uma nova maçã
                        macaX = rand() % (largura - 2) + 1;
                        macaY = rand() % (altura - 2) + 1;
                    } 
                    else if (proxima_posicao->x <= 0 || proxima_posicao->x >= largura - 1 || proxima_posicao->y <= 0 || proxima_posicao->y >= altura - 1) { // verifica se bateu nas paredes
                        game_over(); 
                        perdeu(jogador, pontos);
                    } 
                    else {
                        NO *atual = cobra->next; // ponteiro inicializado apontando para o próximo no após a cabeça
                        while (atual != NULL) {
                            if (proxima_posicao->x == atual->x && proxima_posicao->y == atual->y) { // se a coordenada da proxima posição for igual ao corpo da cobra
                                game_over();
                                perdeu(jogador, pontos);
                            }
                            atual = atual->next; // aponta o próximo nó 
                        }
                        // movimenta a cobra
                        proxima_posicao->next = cobra;
                        cobra = proxima_posicao;
                        // remove a última parte da cobra quando ela se move para uma nova posição
                        atual = cobra;
                        while (atual->next->next != NULL) {
                            atual = atual->next;
                        }
                        free(atual->next);
                        atual->next = NULL;
                    }
                    
                    Sleep(100);
            }

            case 2: // como jogar
                system("cls");
                printf("\n         TUTORIAL\n\n"
                       "- A cobra ficara em constante movimento, basta usar as setas do teclado para controla-la.\n"
                       "- O corpo crescera conforme voce coleta todos as frutas no campo.\n"
                       "- Tome muito cuidado para nao trombar nas paredes ou trombar em qualquer parte do corpo da cobra.\n\n"
                       " PARA VOLTAR AO MENU PRESSIONE QUALQUER TECLA\n");
                if (kbhit) {
                    a = getch();
                }
                goto inicio; // redireciona para o menu
                break;

            case 3: // sair do jogo
                system("cls");
                printf("Voce pediu para sair.\n");
                break;
        }
    } while (opcao != 3);

    return 0;
}
