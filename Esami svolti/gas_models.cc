// =====================================================================
// Object Oriented Programming for Data Processing
// 14 September 2026
// Michela De Angelis 1797244
//
// Implementazione dei metodi delle classi dichiarate in gas_models.h.
// =====================================================================

#include "gas_models.h"
#include <stdexcept>

// ---------------------------------------------------------------------
// GasModel
// ---------------------------------------------------------------------
GasModel::GasModel(double Cv0, double n, const std::string& label)
    : Cv0(Cv0), n(n), label(label)
{
    // Il numero di moli e' una quantita' di sostanza: non ha senso fisico
    // che sia negativo.
    if (n < 0.0)
    {
        throw std::invalid_argument(
            "Il numero di moli n non puo' essere negativo.");
    }
    // La capacita' termica molare a volume costante e' sempre positiva
    // per una sostanza reale (per un gas monoatomico ideale vale gia'
    // 3/2*R > 0); un valore nullo o negativo non e' fisicamente valido.
    // Valori tipici per gas reali: Cv0 ~ 10-40 J/(mol*K), n ~ 0.1-10 mol.
    if (Cv0 <= 0.0)
    {
        throw std::invalid_argument(
            "Cv0 deve essere strettamente positivo.");
    }
}

void GasModel::setCv0(double value)
{
    if (value <= 0.0)
    {
        throw std::invalid_argument(
            "Cv0 deve essere strettamente positivo.");
    }
    Cv0 = value;
}

void GasModel::setMoles(double value)
{
    if (value < 0.0)
    {
        throw std::invalid_argument(
            "Il numero di moli n non puo' essere negativo.");
    }
    n = value;
}

// ---------------------------------------------------------------------
// IdealGasModel
// ---------------------------------------------------------------------
IdealGasModel::IdealGasModel(double Cv0, double n, const std::string& label,
                              double T1, double T2)
    : GasModel(Cv0, n, label), T1(T1), T2(T2)
{
    // Le temperature sono espresse in Kelvin (scala assoluta):
    // per definizione devono essere strettamente positive.
    // Valori tipici ragionevoli: T1, T2 ~ 200-1000 K.
    if (T1 <= 0.0 || T2 <= 0.0)
    {
        throw std::invalid_argument(
            "Le temperature T1 e T2 devono essere espresse in Kelvin "
            "e quindi strettamente positive.");
    }
}

void IdealGasModel::setT1(double value)
{
    if (value <= 0.0)
    {
        throw std::invalid_argument(
            "La temperatura T1 deve essere strettamente positiva.");
    }
    T1 = value;
}

void IdealGasModel::setT2(double value)
{
    if (value <= 0.0)
    {
        throw std::invalid_argument(
            "La temperatura T2 deve essere strettamente positiva.");
    }
    T2 = value;
}

double IdealGasModel::computeDeltaU() const
{
    return n * Cv0 * (T2 - T1);
}

// ---------------------------------------------------------------------
// TemperatureDependentGas
// ---------------------------------------------------------------------
TemperatureDependentGas::TemperatureDependentGas(double Cv0, double n,
                                                   const std::string& label,
                                                   double T1, double T2,
                                                   double beta)
    : GasModel(Cv0, n, label), T1(T1), T2(T2), beta(beta)
{
    // Le temperature sono espresse in Kelvin (scala assoluta):
    // per definizione devono essere strettamente positive.
    // Valori tipici ragionevoli: T1, T2 ~ 200-1000 K.
    // beta (coefficiente termico) e' una correzione piccola rispetto a Cv0:
    // valori tipici beta ~ 0.001-0.05 J/(mol*K^2).
    if (T1 <= 0.0 || T2 <= 0.0)
    {
        throw std::invalid_argument(
            "Le temperature T1 e T2 devono essere espresse in Kelvin "
            "e quindi strettamente positive.");
    }
}

void TemperatureDependentGas::setT1(double value)
{
    if (value <= 0.0)
    {
        throw std::invalid_argument(
            "La temperatura T1 deve essere strettamente positiva.");
    }
    T1 = value;
}

void TemperatureDependentGas::setT2(double value)
{
    if (value <= 0.0)
    {
        throw std::invalid_argument(
            "La temperatura T2 deve essere strettamente positiva.");
    }
    T2 = value;
}

double TemperatureDependentGas::computeDeltaU() const
{
    return n * (Cv0 * (T2 - T1)
                + 0.5 * beta * (T2 * T2 - T1 * T1));
}

// ---------------------------------------------------------------------
// VanDerWaalsGas
// ---------------------------------------------------------------------
VanDerWaalsGas::VanDerWaalsGas(double Cv0, double n, const std::string& label,
                                double T1, double T2, double beta,
                                double a, double Vm1, double Vm2)
    : TemperatureDependentGas(Cv0, n, label, T1, T2, beta),
      a(a), Vm1(Vm1), Vm2(Vm2)
{
    // Il parametro di van der Waals a e' sempre positivo per un gas reale.
    // Valori tipici: a ~ 0.01-6 Pa*m^6/mol^2 (es. acqua ~0.56, N2 ~0.14).
    if (a <= 0.0)
    {
        throw std::invalid_argument(
            "Il parametro di van der Waals a deve essere strettamente positivo.");
    }
    // I volumi molari sono grandezze fisiche che non possono essere
    // nulle (compaiono anche al denominatore) ne' negative.
    // Valori tipici: Vm1, Vm2 ~ 1e-5 - 1e-2 m^3/mol.
    if (Vm1 <= 0.0 || Vm2 <= 0.0)
    {
        throw std::invalid_argument(
            "I volumi molari Vm1 e Vm2 devono essere strettamente positivi.");
    }
}

void VanDerWaalsGas::setA(double value)
{
    if (value <= 0.0)
    {
        throw std::invalid_argument(
            "Il parametro di van der Waals a deve essere strettamente positivo.");
    }
    a = value;
}

void VanDerWaalsGas::setVm1(double value)
{
    if (value <= 0.0)
    {
        throw std::invalid_argument(
            "Il volume molare Vm1 deve essere strettamente positivo.");
    }
    Vm1 = value;
}

void VanDerWaalsGas::setVm2(double value)
{
    if (value <= 0.0)
    {
        throw std::invalid_argument(
            "Il volume molare Vm2 deve essere strettamente positivo.");
    }
    Vm2 = value;
}

double VanDerWaalsGas::computeDeltaU() const
{
    double deltaUBase = TemperatureDependentGas::computeDeltaU();
    double correction = n * n * a * (1.0 / Vm1 - 1.0 / Vm2);
    return deltaUBase + correction;
}
