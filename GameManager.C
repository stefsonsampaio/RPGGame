//cd "c:\vs.code\RPGGame" && gcc GameManager.C -o GameManager && "C:\vs.code\RPGGame\"GameManager comando para executar script no CMD

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include <windows.h>

typedef enum {
    Nenhum = -1,
    Defesa,
    Ataque,
    Magia
} TipoItem;

typedef struct {
    char nome[50];
    TipoItem tipo;
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
    int level;
} Inimigo;

Player inicializaPlayer(int vida, int ataque, int defesa, int quantidade_cura) {
    Player p;
    p.vida = vida;
    p.ataque = ataque;
    p.defesa = defesa;
    p.quantidade_cura = quantidade_cura;

    strcpy(p.item.nome, "");
    p.item.tipo = Nenhum;
    p.item.valor = 0;

    return p;
}

void equiparItem(Player *p, const char *nome, TipoItem tipo, int valor) {
    strcpy(p->item.nome, nome);
    p->item.tipo = tipo;
    p->item.valor = valor;
}

Inimigo inicializaInimigo(int nivel) {
    Inimigo i;
    i.vida = (int)ceil(40 * nivel);
    i.ataque = (int)ceil(11 * nivel);
    i.level = nivel;

    return i;
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
    
    printf("Voce deu de cara com um goblin level %d, de %d pontos de vida!\n", i->level, i->vida);

    int rodadasComEscudoAtivo;

    while(p->vida > 0 && i->vida > 0) {
        if (rodadasComEscudoAtivo > 0) {
            Sleep(1200);
            printf("Seu escudo ainda esta ativo por %d rodadas!\n", rodadasComEscudoAtivo);
        }

        Sleep(1200);
        int escolha;
        do {
            printf("\n1. Atacar\n2. Defender\n3. Usar Cura\nEscolha: ");
            if (scanf("%d", &escolha) != 1) {
                while (getchar() != '\n'); 
                escolha = 0; 
            }
        } while (escolha < 1 || escolha > 3);

        if (escolha == 1) {
            int danoPlayer = (p->ataque - 5) + (rand() % 6);
            if (danoPlayer < 1) {
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

        int danoInimigo = ((i->ataque - 5) + (rand() % 6)) - p->defesa;
        if (p->item.tipo == Defesa && rodadasComEscudoAtivo > 0) {
            danoInimigo -= p->item.valor;
        }
        
        if (danoInimigo < 1) {
            Sleep(1200);
            printf("O inimigo tentou te atacar mas voce desviou! Nenhum dano sofrido.\n");
        }
        else {
            p->vida -= danoInimigo;
            if (p->vida < 0) p->vida = 0;
            Sleep(1200);
            printf("O inimigo te atacou causando %d de dano! Sua vida: %d\n", danoInimigo, p->vida);
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
    Player meuPlayer = inicializaPlayer(100, 20, 4, 3);

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

        Inimigo meuInimigo = inicializaInimigo(i + 1);
        luta(&meuPlayer, &meuInimigo);
    }

    if (meuPlayer.vida > 0) {
        Sleep(1200);
        printf("\nParabens! Voce venceu o jogo!\n");
    }


    return 0;
}