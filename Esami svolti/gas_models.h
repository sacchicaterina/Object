// =====================================================================
// Object Oriented Programming for Data Processing
// 14 September 2026
// Michela De Angelis 1797244
//
// Interfacce (dichiarazioni) delle classi che modellano il calcolo
// della variazione di energia interna (DeltaU) per diversi modelli
// di gas: gas ideale, gas con capacita' termica dipendente dalla
// temperatura, gas di van der Waals.
//
// L'implementazione dei metodi si trova in gas_models.cc.
// =====================================================================

#ifndef GAS_MODELS_H
#define GAS_MODELS_H

#include <string>

// ---------------------------------------------------------------------
// Classe base astratta GasModel
//
// Contiene i dati comuni a tutti i modelli di gas: la capacita' termica
// molare a volume costante (Cv0, in J/(mol*K)), il numero di moli (n)
// e un'etichetta testuale che identifica il gas. Definisce l'interfaccia
// comune tramite il metodo virtuale puro computeDeltaU().
// ---------------------------------------------------------------------
class GasModel
{
protected:
    double Cv0;         // capacita' termica molare a volume costante [J/(mol*K)]
    double n;            // numero di moli [mol]
    std::string label;   // nome/etichetta del gas

public:
    GasModel(double Cv0, double n, const std::string& label);

    virtual ~GasModel() = default;

    // --- Getters ---
    double getCv0() const { return Cv0; }
    double getMoles() const { return n; }
    const std::string& getLabel() const { return label; }

    // --- Setters ---
    void setCv0(double value);
    void setMoles(double value);
    void setLabel(const std::string& value) { label = value; }

    // Interfaccia comune: calcola la variazione di energia interna [J]
    virtual double computeDeltaU() const = 0;
};

// ---------------------------------------------------------------------
// IdealGasModel
//
// Gas ideale: DeltaU = n * Cv0 * (T2 - T1)
// ---------------------------------------------------------------------
class IdealGasModel : public GasModel
{
private:
    double T1;  // temperatura iniziale [K]
    double T2;  // temperatura finale   [K]

public:
    IdealGasModel(double Cv0, double n, const std::string& label,
                  double T1, double T2);

    ~IdealGasModel() override = default;

    // --- Getters ---
    double getT1() const { return T1; }
    double getT2() const { return T2; }

    // --- Setters ---
    void setT1(double value);
    void setT2(double value);

    double computeDeltaU() const override;
};

// ---------------------------------------------------------------------
// TemperatureDependentGas
//
// Gas con capacita' termica che varia linearmente con la temperatura
// (eccitazione vibrazionale ad alte temperature):
//
// DeltaU = n * [ Cv0*(T2 - T1) + (beta/2)*(T2^2 - T1^2) ]
//
// beta e' il coefficiente termico [J/(mol*K^2)].
// ---------------------------------------------------------------------
class TemperatureDependentGas : public GasModel
{
protected:
    double T1;    // temperatura iniziale [K]
    double T2;    // temperatura finale   [K]
    double beta;  // coefficiente termico [J/(mol*K^2)]

public:
    TemperatureDependentGas(double Cv0, double n, const std::string& label,
                             double T1, double T2, double beta);

    ~TemperatureDependentGas() override = default;

    // --- Getters ---
    double getT1() const { return T1; }
    double getT2() const { return T2; }
    double getBeta() const { return beta; }

    // --- Setters ---
    void setT1(double value);
    void setT2(double value);
    void setBeta(double value) { beta = value; }

    double computeDeltaU() const override;
};

// ---------------------------------------------------------------------
// VanDerWaalsGas
//
// Eredita da TemperatureDependentGas e aggiunge il termine correttivo
// dovuto alle forze intermolecolari (parametro di van der Waals a,
// volumi molari Vm1 e Vm2):
//
// DeltaU = n*[Cv0*(T2-T1) + (beta/2)*(T2^2-T1^2)] + n^2*a*(1/Vm1 - 1/Vm2)
// ---------------------------------------------------------------------
class VanDerWaalsGas : public TemperatureDependentGas
{
private:
    double a;    // parametro di van der Waals [Pa*m^6/mol^2]
    double Vm1;  // volume molare iniziale [m^3/mol]
    double Vm2;  // volume molare finale   [m^3/mol]

public:
    VanDerWaalsGas(double Cv0, double n, const std::string& label,
                   double T1, double T2, double beta,
                   double a, double Vm1, double Vm2);

    ~VanDerWaalsGas() override = default;

    // --- Getters ---
    double getA() const { return a; }
    double getVm1() const { return Vm1; }
    double getVm2() const { return Vm2; }

    // --- Setters ---
    void setA(double value);
    void setVm1(double value);
    void setVm2(double value);

    double computeDeltaU() const override;
};

#endif // GAS_MODELS_H
