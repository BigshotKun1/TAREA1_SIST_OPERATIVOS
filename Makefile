# El nombre del archivo ejecutable
TARGET = carrera

# El compilador
CC = g++

# Las opciones del compilador
CXXFLAGS = -std=c++11 -pthread

# Los archivos de código fuente
SRCS = prueba.cpp

# Reglas para la compilación
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CXXFLAGS) $(SRCS) -o $(TARGET)

# Regla para limpiar los archivos generados
clean:
	rm -f $(TARGET)