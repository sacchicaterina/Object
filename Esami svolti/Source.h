#ifndef SOURCE_H
#define SOURCE_H

#include <string>
#include <vector>
#include "Mesh.h"

// Rappresenta una sorgente di campo scalare definita su una Mesh:
// un valore numerico per ciascun punto della mesh (es. una carica puntiforme).
//
// Perché la mesh delle cariche sta dentro Source (e non passata a parte a Voltage):
//
// Un Source non è solo un elenco di numeri (le cariche) - è la combinazione
// di "dove sono le cariche" (la mesh, salvata come attributo "grid") e
// "quanto valgono le cariche" (i valori, salvati in "values"). Le due
// informazioni sono inseparabili: un valore di carica senza sapere dove
// si trova non avrebbe senso fisico.
//
// Il costruttore di Voltage riceve "Mesh grid" (dove calcolare il potenziale)
// e "Source charges" (le cariche, complete di posizione e valore) - non
// riceve due mesh separate. Quindi, quando Voltage ha bisogno della posizione
// delle cariche, deve andarla a prendere dentro l'oggetto Source tramite il
// getter sourceGrid() - è l'unico modo per accedere a quell'informazione,
// che è incapsulata (privata) dentro Source.
class Source {

private:
  Mesh grid;
  std::vector<double> values;

public:
  Source(Mesh g, std::string field);

  const Mesh& sourceGrid() const;   // mesh dove sono posizionate le cariche
  size_t sourceSize() const;
  const std::vector<double>& valuesVec() const;
  double value(int i) const;
};

#endif // SOURCE_H
