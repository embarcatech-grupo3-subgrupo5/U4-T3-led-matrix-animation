# Geração de Animações em uma Matriz de LEDs 5x5

Este projeto faz parte da terceira atividade obrigatória da unidade 4 do curso Embarcatech em sistemas embarcados e tem como objetivo utilizar o simulador Wokwi, o microcontrolador Raspberry Pi Pico W, e um teclado matricial 4x4 para controlar animações em uma matriz de LEDs coloridos 5x5 WS2812.

## Descrição do Projeto

O sistema permite a geração de diferentes animações na matriz de LEDs a partir da interação com o teclado matricial. Cada tecla acionada executa uma funcionalidade específica, conforme detalhado abaixo:

### Funcionalidades Principais:

1. **Teclas de 0 a 9:** Executam diferentes animações na matriz de LEDs. Cada animação possui pelo menos 5 frames e FPS definido para garantir fluidez.
2. **Tecla A:** Desliga todos os LEDs.
3. **Tecla B:** Liga todos os LEDs na cor azul com 100% de luminosidade.
4. **Tecla C:** Liga todos os LEDs na cor vermelha com 80% de luminosidade.
5. **Tecla D:** Liga todos os LEDs na cor verde com 50% de luminosidade.
6. **Tecla #:** Liga todos os LEDs na cor branca com 20% de luminosidade.
7. **Tecla *:** Habilita o modo de gravação via software (reboot).

### Especificações Opcionais:

- Geração de um sinal sonoro utilizando um buzzer para uma das animações.
- Ativação do modo de gravação via software.
---

## Componentes Utilizados

- **Teclado Matricial 4x4**: Dispositivo de entrada para seleção das animações e comandos.
- **Microcontrolador Raspberry Pi Pico W**: Responsável pelo processamento e controle do sistema.
- **Matriz de LEDs Coloridos (5x5 WS2812)**: Exibe as animações definidas.
- **Buzzer Passivo Eletromagnético MLT-8530 (Opcional)**: Utilizado para a geração de sons.

---

## Pré-requisitos

- **Ambiente de Desenvolvimento:** VS Code configurado com o Kit de Desenvolvimento Pico SDK.
- **Simulador:** [Wokwi](https://wokwi.com) para simulação dos componentes.
- **Linguagem:** C.
- **Controle de Versão:** Git e GitHub para versionamento do código.

---

## Instruções de Uso

1. Clone este repositório:
   ```bash
   git clone <[URL do repositório](https://github.com/embarcatech-grupo3-subgrupo5/U4-T3-led-matrix-animation)>
   ```

2. Abra o projeto no VS Code.

3. Configure o Pico SDK e o simulador Wokwi seguindo [esta documentação](https://wokwi.com/docs).

4. Compile e execute o código no simulador Wokwi.

5. Utilize o teclado matricial para testar as funcionalidades:
   - **0-9:** Animações.
   - **A-D, #, *:** Comandos adicionais conforme descrito acima.

---

## Estrutura do Projeto

- **src/**: Código-fonte do projeto.
- **include/**: Arquivos de cabeçalho.
- **tests/**: Testes básicos para validação das funcionalidades.
- **README.md**: Documentação do projeto.

---

## Contribuição

1. Crie um branch para suas alterações:
   ```bash
   git checkout -b minha-feature
   ```

2. Faça commits regulares:
   ```bash
   git commit -m "Descrição da alteração"
   ```

3. Envie as alterações para o repositório:
   ```bash
   git push origin minha-feature
   ```

4. Abra um Pull Request para revisão.

---

## Vídeo Apresentação

Confira o vídeo demonstrativo do projeto: [Link para o vídeo](#)

---

## Autores

Este projeto foi desenvolvido pela equipe do curso Embarcatech, com a colaboração dos seguintes integrantes:

- Líder: Geison de Oliveira
- Desenvolvedores: [Lista de nomes dos integrantes]

---
