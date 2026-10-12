#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cmath>   

// Classe astratta DataSet: rappresenta un insieme di N numeri reali
class DataSet {
protected:
  std::vector<double> numbers;

public:
  // Costruttore: inizializza il dataset a partire da un vettore di double
  DataSet(const std::vector<double>& num) : numbers(num) {}

  // Distruttore virtuale: necessario per il corretto uso polimorfico della classe
  virtual ~DataSet() = default;

  // Getter: restituisce i dati (per riferimento costante, nessuna copia)
  const std::vector<double>& getData() const { return numbers; }

  // Getter: restituisce la dimensione N del dataset
  size_t getDataDim() const { return numbers.size(); }

  // Metodi virtuali puri: la loro implementazione dipende dalla sottoclasse
  virtual double calculateMean() const = 0;
  virtual double calculateStandardDeviation() const = 0;

  // Calcola la mediana: uguale per ogni sottoclasse, quindi implementata qui
  double calculateMedian() const {
    std::vector<double> sorted = numbers;
    std::sort(sorted.begin(), sorted.end());
    size_t n = sorted.size();
    double med;

    if ((n % 2) == 0) {  // caso pari: media dei due valori centrali
      double pos1 = sorted[(n / 2) - 1];
      double pos2 = sorted[n / 2];
      med = (pos1 + pos2) / 2;
    }
    else {                // caso dispari: valore centrale
      med = sorted[n / 2];
    }
    return med;
  }

  // Stampa a video mediana, media e deviazione standard
  void print() const {
    std::cout << "La mediana è " << calculateMedian() << std::endl;
    std::cout << "La media è " << calculateMean() << std::endl;
    std::cout << "La deviazione standard è " << calculateStandardDeviation() << std::endl;
  }
};

// Classe concreta Arithmetics: implementa media e deviazione standard aritmetiche
class Arithmetics : public DataSet {
public:
  // Costruttore: richiama semplicemente il costruttore della classe base
  Arithmetics(const std::vector<double>& num) : DataSet(num) {}

  // Calcola la media aritmetica: mu = (1/N) * sum_i x_i
  double calculateMean() const {
    double sum = 0.;
    for (size_t i = 0; i < numbers.size(); i++) {
      sum += numbers[i];
    }
    double mean = sum / numbers.size();
    return mean;
  }

  // Calcola la deviazione standard: sigma = sqrt( (1/N) * sum_i (x_i - mu)^2 )
  double calculateStandardDeviation() const {
    double sum = 0.;
    for (size_t i = 0; i < numbers.size(); i++) {
      sum += std::pow((numbers[i] - calculateMean()), 2);
    }
    double standard_deviation = std::sqrt(sum / numbers.size());
    return standard_deviation;
  }

  // Overloading dell'operatore (*): prodotto elemento per elemento (z_i = x_i * y_i)
  Arithmetics operator*(const Arithmetics& other) const {

    // Controllo di sicurezza: i due dataset devono avere la stessa dimensione
    if (numbers.size() != other.numbers.size()) {
      throw std::invalid_argument("Errore: I DataSet devono avere la stessa dimensione per essere moltiplicati.");
    }
    size_t N = numbers.size();

    // Vettore temporaneo per salvare i risultati della moltiplicazione
    std::vector<double> result_vec(N);

    // Moltiplicazione elemento per elemento
    for (size_t i = 0; i < N; i++) {
      result_vec[i] = numbers[i] * other.numbers[i];
    }

    // Costruisce e restituisce un nuovo oggetto Arithmetics con il risultato
    return Arithmetics(result_vec);
  }
};

