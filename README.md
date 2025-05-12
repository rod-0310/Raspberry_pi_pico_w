# Raspberry_pi_pico_w
Microros2_perifericos_rasp

https://www.electromaker.io/blog/article/getting-started-with-the-raspberry-pi-pico-w-cc-sdk?srsltid=AfmBOooYdcMjbeYhbagSr1kPjeFgmpMNlTypQouSHA1PA0v-Z1_Q2aXj
en el link explica como compilar para la raspberri pi pico_w 
crear una carpeta build a lado del archivo .c y CMakeLists.txt
mkdir build
cd build
export PICO_SDK_PATH=../../pico-sdk/
cmake -DPICO_BOARD=pico_w .. 
(esto se realiza una sola vez y sis se corrije el archivo .c solo se debe hacer make )
make
Copiar el archivo que se genero en formato .uf2 a la placa raspberry pi pico_w
