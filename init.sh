#!/bin/bash

# Limpia archivos temporales
echo "Limpiando archivos..."
make clean

# Compilar código
echo "Compilando el juego..."
make

# Verificar si la compilación fue exitosa
if [ $? -eq 0 ]; then
    echo "Compilación exitosa."

    # Subir cambios a GitHub
    echo "Enviando cambios al repositorio..."
    git add .
    git commit -m "Compilación automática y limpieza de código"
    git push origin main

    # Ejecutar el juego
    echo "Ejecutando el juego..."
    ./game
else
    echo "Error en la compilación. Revisa los logs."
    exit 1
fi
