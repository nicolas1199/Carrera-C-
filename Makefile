# Nombre del compilador
CXX = g++

# Bandera de compilación
CXXFLAGS = -std=c++11 -pthread

# Nombre del ejecutable
TARGET = Carrera

# Archivos fuente
SRC = Carrera.cpp

# Reglas para compilar el programa
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Regla para limpiar archivos objeto y el ejecutable
clean:
	rm -f $(TARGET)

.PHONY: all clean
