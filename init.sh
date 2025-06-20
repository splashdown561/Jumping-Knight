#!/bin/bash

# Limpia archivos temporales
echo "Limpiando archivos..."
make clean

# Compilar código
echo "Compilando el juego..."
make

# Verificar si la compilación fue exitosa
if [ $? -eq 0 ]; then
    echo "Ejecutando el juego..."
    ./bin/game
else
    echo "Error en la compilación. Revisa los logs."
    exit 1
fi
