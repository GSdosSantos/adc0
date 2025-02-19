# Atividade: Controle de LED e Display OLED com Raspberry Pi Pico

Este projeto faz parte da atividade do Moodle CEPeDI Embarcatech e foi desenvolvido por **Gabriel Santos do Santos**. Ele utiliza uma Raspberry Pi Pico para controlar LEDs RGB, ler entradas analógicas de um joystick e exibir informações em um display OLED, além de enviar mensagens via UART.

## Enunciado da Atividade

Com o objetivo de consolidar os conceitos sobre o uso de conversores analógico-digitais (ADC) no RP2040 e explorar as funcionalidades da placa de desenvolvimento BitDogLab, propõe-se a realização da seguinte atividade prática individual.

### Objetivos

- Compreender o funcionamento do conversor analógico-digital (ADC) no RP2040.
- Utilizar o PWM para controlar a intensidade de dois LEDs RGB com base nos valores do joystick.
- Representar a posição do joystick no display SSD1306 por meio de um quadrado móvel.
- Aplicar o protocolo de comunicação I2C na integração com o display.

### Descrição do Projeto

O joystick fornecerá valores analógicos correspondentes aos eixos X e Y, que serão utilizados para:

- **Controle de LEDs RGB**:
  - O LED Azul terá seu brilho ajustado conforme o valor do eixo Y. Quando o joystick estiver solto (posição central - valor 2048), o LED permanecerá apagado. Movendo o joystick para cima (valores menores) ou para baixo (valores maiores), o brilho aumentará gradualmente, atingindo a intensidade máxima nos extremos (0 e 4095).
  - O LED Vermelho seguirá o mesmo princípio, mas com o eixo X. Quando o joystick estiver solto, o LED estará apagado. Movendo o joystick para a esquerda ou direita, o brilho aumentará gradualmente até os extremos.
  - Os LEDs serão controlados via PWM para permitir variação suave da intensidade luminosa.

- **Display SSD1306**: Exibir um quadrado de 8x8 pixels, inicialmente centralizado, que se moverá proporcionalmente aos valores capturados pelo joystick.

- **Botões**:
  - Botão do Joystick (GPIO 22):
    - Alternar o estado do LED Verde a cada acionamento.
    - Modificar a borda do display, alternando entre diferentes estilos a cada novo acionamento.
  - Botão A (GPIO 5): Ativar ou desativar os LEDs controlados por PWM a cada acionamento.

### Requisitos do Projeto

1. **Uso de interrupções**: Todas as funcionalidades relacionadas aos botões devem ser implementadas utilizando rotinas de interrupção (IRQ).
2. **Debouncing**: Implementar o tratamento de bouncing dos botões via software.
3. **Utilização do Display 128 x 64**: Demonstrar o entendimento do funcionamento do display e a utilização do protocolo I2C.
4. **Organização do código**: O código deve estar bem estruturado e comentado para facilitar o entendimento.

---

## Funcionalidades

- Controle de LEDs RGB usando PWM com base em entradas analógicas do joystick.
- Exibição da posição do joystick em um display OLED via I2C.
- Controle de borda e LEDs via botões com tratamento de debounce.

## Diagrama de Conexões

- **Display OLED**: 
  - SDA: GPIO 14
  - SCL: GPIO 15
- **LED RGB**: 
  - Vermelho: GPIO 13
  - Verde: GPIO 11
  - Azul: GPIO 12
- **Botões**: 
  - Botão A: GPIO 5
  - Botão do Joystick: GPIO 22
- **Entradas analógicas (Joystick)**: 
  - Eixo X: GPIO 27
  - Eixo Y: GPIO 26

## Código

O código está incluído no arquivo principal, configurando e inicializando os periféricos e gerenciando as funções de exibição e controle de LEDs.

## Demonstração

Assista ao vídeo da atividade no YouTube para ver o projeto em ação:

[Assista ao vídeo](https://drive.google.com/file/d/1QnRhcXEWsiltPTyzXGw2C1MJ7tDZSqtL/view?usp=sharing)

## Como Executar

1. Clone o repositório ou copie o código-fonte para o ambiente de desenvolvimento.
2. Conecte o hardware conforme o diagrama de conexões.
3. Compile e carregue o código na Raspberry Pi Pico.
4. Acompanhe a execução via monitor serial e display OLED.

## Autor

**Gabriel Santos do Santos**

---

Se tiver dúvidas ou sugestões, entre em contato!
