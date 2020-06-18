Este projeto corre num terminal Linux e o simulador é programado em C. É utilizado o cenário dos labirintos e do rato que o tenta completar.

Utilizando os sockets implementa-se uma arquitetura cliente-servidor onde o cliente é responsável por enviar os comandos de movimento ao servidor que atua como simulador.

Também são utilizadas duas threads, tendo uma o papel de produtor e outra com o papel de consumidor.

Este programa funciona mostrando no servidor o labirinto com o cursor e mandando informações do cliente para o servidor movendo o cursor no labirinto.

Tem o ficheiro test.txt que apresenta o labirinto que irá ser utilizado, um ficheiro server datagram que irá receber informações do cliente e utilizando as funções do jogo.c, irá mover o cursor no labirinto.

Tambem existe um ficheiro Jogo.c que irá buscar o labirinto utilizando o reading_a_file podendo também retornar a posição e direção do cursor e mover o cursor utilizando o ficheiro moving_in_screen.

Com isto tudo o programa tem dois modos de ser utilizado, o modo de quatro teclas onde ao tocar no W o cursor irá para cima, no S para baixo, no D para a direita e no A para a esquerda.

O outro modo é o de três teclas, onde o cursor apresenta direção, sendo que para se movimentar para a direita é necessário tocar na tecla D até que o cursor se vire nessa direção, só depois clicando na tecla W para se movimentar.

Video do Projeto:
https://youtu.be/3fwbdjJQMI4
