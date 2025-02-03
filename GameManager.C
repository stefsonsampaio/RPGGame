//cd "c:\vs.code\RPGGame" && gcc GameManager.C -o GameManager && "C:\vs.code\RPGGame\"GameManager comando para executar script no CMD

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>

typedef struct {
    char nome[50];
    //TODO: trocar tipo de int para ENUM
    int tipo; //se tipo 1, item de ataque, se tipo 3, item de defesa
    int valor;
} Item;

typedef struct {
    int vida;
    int ataque;
    int defesa;
    int quantidade_cura;
    Item item;
} Player;

typedef struct {
    int vida;
    int ataque;
} Inimigo;

Player inicializaPlayer(int vida, int ataque, int defesa, int quantidade_cura) {
    Player p;
    p.vida = vida;
    p.ataque = ataque;
    p.defesa = defesa;
    p.quantidade_cura = quantidade_cura;

    strcpy(p.item.nome, "");
    p.item.tipo = 0;
    p.item.valor = 0;

    return p;
}

Inimigo inicializaInimigo(int vida, int ataque) {
    Inimigo i;
    i.vida = vida;
    i.ataque = ataque;

    return i;
}

void adicionaEquipamento(Player *p, char nome[], int tipo, int valor) {
    strcpy(p->item.nome, nome);
    p->item.tipo = tipo;
    p->item.valor = valor;
}

void exibePropriedadesDoPlayer(Player *p) {
    printf("\nSuas propriedades:\n");
    printf("- vida: %d\n", p->vida);
    printf("- quantidade de pocoes de cura: %d\n", p->quantidade_cura);
    if (strlen(p->item.nome) > 0) {
        printf("- Equipamento: %c\n", p->item.nome);
        if (p->item.tipo == 1) {
            printf("- Dano do equipamento: %d\n", p->item.valor);
        }
        else {
            printf("- Defesa do equipamento: %d\n", p->item.valor);
        }
    }
    Sleep(1200);
}

void luta(Player *p, Inimigo *i) {
    srand(time(NULL));

    printf("\nInicio de batalha!\n");
    Sleep(1200);
    
    printf("Voce deu de cara com um goblin level 2, de %d pontos de vida!\n", i->vida);

    int rodadasComEscudoAtivo;

    while(p->vida > 0 && i->vida > 0) {
        Sleep(1200);
        printf("\n1. Atacar o inimigo?\n");
        printf("2. Estabelecer defesa (Duracaoo de 3 rodadas)?\n");
        printf("3. Utilizar item de cura?\n");

        int escolha;
        printf("\nComo voce vai prosseguir? (1, 2, 3)\n ");
        scanf("%d", &escolha);

        while (escolha < 1 || escolha > 3) {
            printf("Opcao invalida! Digite um numero de 1 a 3.\n ");
            scanf("%d", &escolha);
        }

        if (escolha == 1) {
            int danoPlayer = rand() % (p->ataque + 1);
            if (danoPlayer == 0) {
                Sleep(1200);
                printf("\nVoce tentou atacar o inimigo mas ele desviou! Nenhum dano causado.\n");
            }
            else {
                i->vida -= danoPlayer;
                if (i->vida < 0) i->vida = 0;
                Sleep(1200);
                printf("\nVoce atacou causando %d de dano! Vida do inimigo: %d\n", danoPlayer, i->vida);
                if (i->vida <= 0) {
                    Sleep(1200);
                    printf("\nO inimigo foi derrotado!\n");
                    break;
                }
            }
        }
        else if (escolha == 2) {
            Sleep(1200);
            printf("Defesa reforcada por 3 rodadas!\n");
            rodadasComEscudoAtivo = 3;
        }
        else if (escolha == 3) {
            if (p->quantidade_cura > 0) {
                p->vida += 20;
                p->quantidade_cura -= 1;

                Sleep(1200);
                printf("Voce usou uma pocao de cura. Sua vida agora e %d. Quantidade de itens de cura restante: %d\n", p->vida, p->quantidade_cura);
            }
            else {
                Sleep(1200);
                printf("Voce nao tem mais curas! Sua rodada foi desperdicada.");
            }
        }

        int danoInimigo = rand() % (i->ataque + 1);

        if (rodadasComEscudoAtivo > 0) {
            //TODO
            //lógica para diminuir o dano do inimigo de acordo com a defesa do player
            //a defesa do player aumenta se ele estiver equipado com um item do tipo escudo
        }

        if (danoInimigo == 0) {
            Sleep(1200);
            printf("O inimigo tentou te atacar mas voce desviou! Nenhum dano sofrido.\n");
        }
        else {
            p->vida -= danoInimigo;
            if (p->vida < 0) p->vida = 0;
            Sleep(1200);
            printf("O inimigo te atacou causando %d de dano! Sau vida: %d\n", danoInimigo, p->vida);
            if (p->vida <= 0) {
                Sleep(1200);
                printf("O jogador foi derrotado!\n");
            }
        }

        if (rodadasComEscudoAtivo > 0) {
            rodadasComEscudoAtivo -= 1;
        }
    }
}

int main()
{
    Player meuPlayer = inicializaPlayer(100, 20, 10, 3);

    for (int i = 0; i < 5; i++) {
        char nextLevel;

        if (meuPlayer.vida <= 0) {
            Sleep(1200);
            printf("O jogador morreu! Fim de jogo.\n");
            return 0;
        }

        Sleep(1200);
        exibePropriedadesDoPlayer(&meuPlayer);

        printf("\nPronto para a proxima etapa? (S/N)\n");
        fflush(stdin);
        scanf(" %c", &nextLevel);

        if (toupper(nextLevel) == 'N') {
            printf("\nJogo encerrado!");
            return 0;
        }

        Inimigo meuInimigo = inicializaInimigo(40, 8);
        luta(&meuPlayer, &meuInimigo);
    }

    if (meuPlayer.vida > 0) {
        Sleep(1200);
        printf("\nParabens! Voce venceu o jogo!\n");
    }


    return 0;
}