# Jogo de Texto em C - RPG por Turnos

## Descrição

Este é um jogo de RPG baseado em texto, desenvolvido em linguagem C, onde o jogador enfrenta inimigos em um sistema de combate por turnos. O jogo apresenta um modo história simples com 25 leveis (rodadas), nos quais o jogador pode explorar, lutar, coletar itens e tomar decisões que influenciam o desfecho da jornada.

## Funcionalidades

- **Sistema de combate por turnos**: O jogador pode escolher entre atacar, defender-se ou utilizar itens durante as batalhas.
- **Personagens com atributos**: Vida, ataque e defesa influenciam o desempenho no jogo.
- **Inventário de itens**: Diferentes tipos de itens que podem ser utilizados para recuperar vida ou melhorar atributos temporariamente.
- **Exploração e eventos aleatórios**: O jogador pode encontrar itens, desafios ou inimigos ao longo da jornada.
- **Sistema de decisões e múltiplos finais**: As escolhas do jogador ao longo da história afetam o desfecho do jogo.

## Estrutura do Código

- ``: Centraliza a lógica principal do jogo e gerencia a execução das 25 rodadas.
- ``: Definição e manipulação dos atributos do jogador.
- ``: Estruturas e funções para criação e gerenciamento de inimigos.
- ``: Definição dos itens e seus efeitos no jogo.
- ``: Sistema de combate, incluindo ataques, defesa e efeitos de itens.
- ``: Eventos aleatórios e exploração.
- ``: Funções auxiliares, como geração de números aleatórios com `rand()`.

## Como Jogar

1. Compile o código-fonte utilizando um compilador C (como GCC) e execute o script:
   ```sh
   gcc GameManager.C -o GameManager && "C:\caminho\do\executavel\"GameManager 
   ```
2. Durante o jogo, siga as instruções na tela e escolha suas ações conforme necessário.

## Dependências

- Compilador C (GCC recomendado)
- Sistema operacional compatível com C (Windows, Linux, MacOS)

## Notas

- O uso de `rand()` permite gerar aleatoriedade em exploração, itens encontrados e dano de combate.
- A estrutura do jogo foi projetada para ser modular, permitindo expansão futura.
- Teste diferentes decisões ao longo do jogo para desbloquear diferentes finais.

## Contribuição

Sinta-se à vontade para contribuir com melhorias! Sugestões, correções de bugs e novas funcionalidades são bem-vindas.