#include <iostream>
#include <vector>
#include <utility>   // std::pair, std::swap
#include <cmath>     // std::sqrt
#include <algorithm> // std::min, std::max
#include <stdexcept> // std::invalid_argument

class PointList {

  protected:
    // Vettore di coppie (x, y): coords[i].first = x, coords[i].second = y
    std::vector<std::pair<double, double>> coords;

  public:
    // Costruttore: alloca spazio per N coppie e legge le coordinate da tastiera
    PointList(int N) {
        coords.reserve(N);

        for (int i = 0; i < N; ++i) {
            double x, y;
            std::cin >> x >> y;
            coords.push_back({x, y});
        }
    }

    // Distruttore virtuale: necessario perche' Print() e' virtual e la
    // classe puo' essere usata polimorficamente tramite puntatore/riferimento
    // alla base (evita undefined behavior nella distruzione di un derivato).
    virtual ~PointList() = default;

    // Scambia i punti in posizione i e j.
    // Void perche' modifica direttamente il vettore coords (side effect),
    // non deve restituire nulla al chiamante.
    void SwapPoints(int i, int j) {
        std::swap(coords[i], coords[j]);

        // --- Versione manuale (equivalente, senza std::swap) ---
        // std::pair<double, double> temp = coords[i];  // salvo il valore in i
        // coords[i] = coords[j];                        // in i metto il valore di j
        // coords[j] = temp;                              // in j metto il valore salvato
    }

    // --- getters ---
    int GetN() const { return coords.size(); }
    std::pair<double, double> GetPoint(int i) const { return coords[i]; }
    double GetX(int i) const { return coords[i].first; }
    double GetY(int i) const { return coords[i].second; }

    // Stampa a schermo le coordinate di tutti i punti.
    // Virtual per permettere l'override in classi derivate (es. Polygon).
    virtual void Print() const {
        int N = GetN();

        for (int i = 0; i < N; ++i) {
            std::cout << "Point " << i << ": (" << coords[i].first << ", " << coords[i].second << ")" << std::endl;
        }
    }

};


class Polygon : public PointList {

  public:
    // Costruttore: passa N alla base PointList (che legge le coordinate
    // da tastiera), poi verifica che il numero di vertici sia valido
    // per rappresentare un poligono (almeno 3).
    Polygon(int N) : PointList(N) {
        if (N < 3) {
            throw std::invalid_argument("Polygon: sono necessari almeno 3 vertici");
        }
    }

    // Stampa a schermo le coordinate di tutti i vertici, e il loro numero totale.
    // Override di PointList::Print() con etichette specifiche per un poligono.
    void Print() const override {
        int N = GetN();

        for (int i = 0; i < N; ++i) {
            std::cout << "Vertex " << i << ": (" << coords[i].first << ", " << coords[i].second << ")" << std::endl;
        }

        std::cout << "Number of vertices: " << GetN() << std::endl;
    }

    // Calcola il perimetro del poligono sommando le lunghezze dei lati.
    // Ogni lato e' la distanza euclidea tra un vertice e il successivo,
    // con l'ultimo vertice che si ricollega al primo (wrap-around con %N).
    double Perimeter() const {
        int N = GetN();
        double perimeter = 0.0;

        for (int i = 0; i < N; ++i) {
            int next = (i + 1) % N;

            double dx = coords[i].first  - coords[next].first;
            double dy = coords[i].second - coords[next].second;

            perimeter += std::sqrt(dx * dx + dy * dy);
        }

        return perimeter;
    }

    // Scambia due vertici (riusa SwapPoints della base con un nome piu'
    // coerente col dominio "poligono").
    void SwapVertices(int i, int j) {
        SwapPoints(i, j);
    }

    // Aggiorna le coordinate dell'i-esimo vertice con i nuovi valori x, y.
    // Non e' const: modifica coords, quindi lo stato dell'oggetto cambia.
    void UpdateVertex(int i, double x, double y) {
        coords[i] = {x, y};

        // --- Versione alternativa (equivalente, assegnazione separata) ---
        // coords[i].first = x;
        // coords[i].second = y;
    }

    // Stampa la distanza tra il vertice i e tutti gli altri vertici.
    void PrintDistancesFrom(int i) const {
        int N = GetN();

        for (int j = 0; j < N; ++j) {
            if (j != i) {
                double dx = coords[i].first  - coords[j].first;
                double dy = coords[i].second - coords[j].second;
                double dist = std::sqrt(dx * dx + dy * dy);

                std::cout << dist << std::endl;
            }
        }
    }

    // Calcola l'area del rettangolo minimo, con lati paralleli agli assi x e y,
    // che contiene tutti i vertici del poligono.
    double BoundingBoxArea() const {
        int N = GetN();

        double xMin = coords[0].first,  xMax = coords[0].first;
        double yMin = coords[0].second, yMax = coords[0].second;

        for (int i = 1; i < N; ++i) {
            xMin = std::min(xMin, coords[i].first);
            xMax = std::max(xMax, coords[i].first);
            yMin = std::min(yMin, coords[i].second);
            yMax = std::max(yMax, coords[i].second);
        }

        double area = (xMax - xMin) * (yMax - yMin);
        return area;
    }

};



int main() {

    std::cout << "=== PointList demo ===" << std::endl;
    int nPts;
    std::cout << "How many points? ";
    std::cin >> nPts;

    PointList points(nPts);
    points.Print();

    if (nPts >= 2) {
        std::cout << std::endl << "Swapping points 0 and 1..." << std::endl;
        points.SwapPoints(0, 1);
        points.Print();
    }

    std::cout << std::endl << "=== Polygon demo ===" << std::endl;
    int nVert;
    std::cout << "How many vertices (>=3)? ";
    std::cin >> nVert;

    Polygon poly(nVert);
    poly.Print();

    std::cout << std::endl << "Perimeter = " << poly.Perimeter() << std::endl;
    std::cout << "Bounding box area = " << poly.BoundingBoxArea() << std::endl;

    std::cout << std::endl << "Swapping vertices 0 and 1..." << std::endl;
    poly.SwapVertices(0, 1);
    poly.Print();

    std::cout << std::endl << "Updating vertex 0 to (0.0, 0.0)..." << std::endl;
    poly.UpdateVertex(0, 0.0, 0.0);
    poly.Print();

    std::cout << std::endl;
    poly.PrintDistancesFrom(0);

 
}
