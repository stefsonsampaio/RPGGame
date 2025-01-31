#include <stdio.h>
#include <string.h>

typedef struct {
    int vida;
    int ataque;
    int defesa;
    int quantidade_cura;
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

    return p;
}

void exibePropriedadesDoPlayer(Player *p) {
    printf("vida atual do player: %d\n", p->vida);
}

int main()
{
    Player meuPlayer = inicializaPlayer(100, 10, 10, 3);

    exibePropriedadesDoPlayer(&meuPlayer);

    return 0;
}