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
    printf("- quantidade de poçoes de cura: %d\n", p->quantidade_cura);
    if (strlen(p->item.nome) > 0) {
        printf("- Equipamento: %c\n", p->item);
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

    while(p->vida > 0 && i->vida > 0) {
        int danoPlayer = rand() % (p->ataque + 1);
        if (danoPlayer == 0) {
            printf("Voce tentou atacar o inimigo mas ele desviou! Nenhum dano causado.\n");
        }
        else {
            i->vida -= danoPlayer;
            if (i->vida < 0) i->vida = 0;
            printf("Voce atacou causando %d de dano! Vida do inimigo: %d\n", danoPlayer, i->vida);
            if (i->vida <= 0) {
                printf("O inimigo foi derrotado!\n");
                break;
            }
        }
        Sleep(1200);

        int danoInimigo = rand() % (i->ataque + 1);
        if (danoInimigo == 0) {
            printf("O inimigo tentou te atacar mas voce desviou! Nenhum dano sofrido.\n");
        }
        else {
            p->vida -= danoInimigo;
            if (p->vida < 0) p->vida = 0;
            printf("O inimigo te atacou causando %d de dano! Sau vida: %d\n", danoInimigo, p->vida);
            if (p->vida <= 0) {
                printf("O jogador foi derrotado!\n");
            }
        }
        Sleep(1200);
    }
}

int main()
{
    Player meuPlayer = inicializaPlayer(100, 20, 10, 3);

    for (int i = 0; i < 5; i++) {
        if (meuPlayer.vida <= 0) {
            printf("O jogador morreu! Fim de jogo.\n");
            return 0;
        }

        exibePropriedadesDoPlayer(&meuPlayer);

        char nextLevel;
        printf("\nPronto para a proxima etapa? (S/N)\n");
        scanf("%c\n", &nextLevel);

        if (toupper(nextLevel) == 'N') {
            printf("\nJogo encerrado!");
            return 0;
        }

        Inimigo meuInimigo = inicializaInimigo(40, 8);
        luta(&meuPlayer, &meuInimigo);
    }

    if (meuPlayer.vida > 0) {
        printf("\nParabens! Voce venceu o jogo!\n");
    }


    return 0;
}