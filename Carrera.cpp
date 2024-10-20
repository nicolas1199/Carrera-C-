#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <mutex>
#include <algorithm>
#include <chrono>

using namespace std;

// Representa un auto en la AutoCarrera
class Auto
{
public:
  int id;
  int distanciaViaje;
  thread auto_thread; // Cambié el nombre de la variable thread a auto_thread
  chrono::high_resolution_clock::time_point tiempoInicio;
  chrono::high_resolution_clock::time_point tiempoFinal;

  Auto(int id) : id(id), distanciaViaje(0) {}

  void inicioAutoCarrera(int distancia_total)
  {
    tiempoInicio = chrono::high_resolution_clock::now();
    // Generamos numeros aleatorios para distancia y tiempo
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distanceDistribution(1, 10);
    uniform_int_distribution<> tiempo(100, 500);

    while (distanciaViaje < distancia_total)
    {
      // Cada auto avanza una distancia aleatoria
      int avance = distanceDistribution(gen);
      distanciaViaje += avance;

      if (distanciaViaje > distancia_total)
      {
        distanciaViaje = distancia_total;
      }

      cout << "Auto " << id << " avanzo " << avance << " metros (distancia total: " << distanciaViaje << " )" << endl;

      // El auto espera un tiempo random
      this_thread::sleep_for(chrono::milliseconds(tiempo(gen)));
    }
    tiempoFinal = chrono::high_resolution_clock::now();
    cout << "Auto " << id << " finalizo la Carrera!" << endl;
  }
};

// Representa la AutoCarrera
class AutoCarrera
{
public:
  int distancia_total;
  int numero_autos;
  vector<Auto> Autos;
  mutex mtx; // Mutex para proteger el acceso al vector de autos

  AutoCarrera(int distancia_total, int numero_autos) : distancia_total(distancia_total), numero_autos(numero_autos)
  {
    // Inicializa un vector de autos
    for (int i = 0; i < numero_autos; ++i)
    {
      Autos.emplace_back(i);
    }
  }

  void inicioAutoCarrera()
  {
    // Inicia un hilo para cada auto
    for (auto &Auto : Autos)
    {
      Auto.auto_thread = thread(&Auto::inicioAutoCarrera, &Auto, distancia_total);
    }

    // Espera a que todos los hilos terminen
    for (auto &Auto : Autos)
    {
      if (Auto.auto_thread.joinable())
      {
        Auto.auto_thread.join();
      }
    }

    // Ordena los autos por el tiempo que tardaron en finalizar
    sort(Autos.begin(), Autos.end(), [](const Auto &a, const Auto &b)
         {
            auto tiempoA = chrono::duration_cast<chrono::milliseconds>(a.tiempoFinal - a.tiempoInicio).count();
            auto tiempoB = chrono::duration_cast<chrono::milliseconds>(b.tiempoFinal - b.tiempoInicio).count();
            return tiempoA < tiempoB; });

    // Muestra los resultados de la AutoCarrera
    cout << "Rank\tAuto" << endl;
    cout << "----\t----" << endl;
    for (int i = 0; i < numero_autos; ++i)
    {
      cout << i + 1 << "\tAuto" << Autos[i].id << endl;
    }
  }
};

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    cerr << "Uso: " << argv[0] << "<num_Autos> <total_distance>" << endl;
    return 1;
  }
  /*Ingresar autos*/
  int numero_autos = atoi(argv[1]);

  /*Ingresar distancia total*/
  int distancia_total = atoi(argv[2]);

  AutoCarrera AutoCarrera(distancia_total, numero_autos);
  AutoCarrera.inicioAutoCarrera();

  return 0;
}
