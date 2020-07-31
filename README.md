projeto de EA871 (roteiro 9)

enunciado:

Modificar o programa do cronômetro do Roteiro 8 para que agora:

- O comando para reiniciar (reset) o cronômetro seja feito pelo botão de pressão S1, utilizando a interrupção NMI, substituindo a funcionalidade anteriormente associada ao caractere 'r';
- O comando para parar o cronômetro seja feito por interrupção disparada pelo botão de pressão S3, associado ao bit 12 da Porta A, substituindo funcionalidade anteriormente associada ao caractere 's';
- A recepção dos caracteres de comando ('d', 'l' e ESC) pela UART0 seja feita por interrupção e não por polling.
