# TFG

El proyecto contiene a el código necesario para crear un dispositivo con la capacidad de medir la actividad física,
utilizando un acelerómetro y un electromiograma que posteriormente enviará los datos utilizando un NodeMCU ESP8266 como módulo wifi.

-La carpeta tfg-api contiene el servidor Rest empleado con la configuración necesaria para el proyecto 

-La carpeta client contiene la configuración de la página web donde se mostrarán los datos de los sensores

-En la carpeta podómetro se encuentra el codigo necesario para convertir los ejes de un acelerómetro en un podometro

-La carpeta main contiene el código que se programará en el módulo wifi para enviar los datos del podómetro y 
del electromiograma al servidor Rest
