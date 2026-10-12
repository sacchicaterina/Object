"""
Computing Methods for Physics - 14 June 2022
Part 2 - Binary Black Holes in Python

Genera una popolazione di N sistemi binari di buchi neri con parametri
casuali, calcola le grandezze derivate (chi_eff, massa totale, mass
ratio simmetrico, chirp mass, tempo in banda) e ne mostra le
distribuzioni. Infine esegue un fit Lorentziano sulla distribuzione
di chi_eff.
"""

import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

NU_10 = 4.916e-5
N_SAMPLES = 10000


def generate_distributions(n):
    """Genera n sistemi binari con parametri casuali:
    masse uniformi in [10, 100], spin uniformi in [0, 1],
    angoli distribuiti uniformemente in coseno (isotropia).
    """
    m1 = np.random.uniform(10, 100, n)
    chi1 = np.random.uniform(0, 1, n)
    u1 = np.random.uniform(-1., 1., n)
    theta1 = np.arccos(u1)

    m2 = np.random.uniform(10, 100, n)
    chi2 = np.random.uniform(0, 1, n)
    u2 = np.random.uniform(-1., 1., n)
    theta2 = np.arccos(u2)

    dictionary1 = {"mass1": m1,
                   "spin1": chi1,
                   "angle1": theta1,
                   "mass2": m2,
                   "spin2": chi2,
                   "angle2": theta2}
    return dictionary1


def compute_derived_quantities(dictionary1):
    """Calcola le grandezze derivate a partire dai parametri primari:
    massa totale M, mass ratio simmetrico eta, chirp mass, chi_eff
    e tempo in banda tc.
    """
    m1, chi1, theta1 = dictionary1["mass1"], dictionary1["spin1"], dictionary1["angle1"]
    m2, chi2, theta2 = dictionary1["mass2"], dictionary1["spin2"], dictionary1["angle2"]

    M = m1 + m2
    eta = (m1 * m2) / M**2
    chirp_mass = (m1 * m2)**(3/5) / M**(1/5)
    chi_eff = (chi1 * m1 * np.cos(theta1) + chi2 * m2 * np.cos(theta2)) / M
    r0 = (chirp_mass / np.pi)**(2/3) * (2 * NU_10)**(2/3)
    tc = (5/256) * r0**4 / (m1 * m2 * M) / NU_10

    dictionary2 = {"M": M,
                   "eta": eta,
                   "chirp_mass": chirp_mass,
                   "chi_eff": chi_eff,
                   "tc": tc}
    return dictionary2


def plot_distributions(dictionary2):
    """Plotta le distribuzioni (istogrammi) delle 5 grandezze derivate
    in una griglia 2x3 di subplot.
    """
    labels = {
        "M": r"Total mass $M$ [$M_\odot$]",
        "eta": r"Symmetric mass ratio $\eta$",
        "chirp_mass": r"Chirp mass $\mathcal{M}$ [$M_\odot$]",
        "chi_eff": r"Effective spin $\chi_{\rm eff}$",
        "tc": r"Time to coalescence $t_c$ [s]",
    }

    fig, axes = plt.subplots(2, 3, figsize=(15, 8))
    # plt.subplots(2, 3) restituisce axes come array 2D di forma (2, 3):
    # axes[0][0], axes[0][1], ..., axes[1][2]. Il ciclo sotto usa zip()
    # per accoppiare un Axes alla volta con una grandezza alla volta, ma
    # zip su un array 2D scorre le RIGHE (axes[0], poi axes[1]), non i
    # singoli subplot. flatten() appiattisce l'array in 1D, nell'ordine
    # riga per riga: [axes[0,0], axes[0,1], axes[0,2], axes[1,0], ...]
    axes = axes.flatten()

    for ax, (key, data) in zip(axes, dictionary2.items()):
        label = labels[key]
        ax.hist(data, bins=80, color="steelblue", edgecolor="black", alpha=0.8)
        ax.set_xlabel(label, fontsize=11)
        ax.set_ylabel("Counts", fontsize=11)
        ax.set_title(f"Distribution of {label}", fontsize=11)
        ax.grid(alpha=0.3)

    # il sesto subplot (2x3 = 6 slot, 5 grandezze) resta vuoto
    for ax in axes[len(dictionary2):]:
        ax.axis("off")

    fig.suptitle("Distributions of derived BBH population quantities "
                 f"(N = {N_SAMPLES:,})", fontsize=14)
    fig.tight_layout()
    fig.savefig("bbh_distributions.png", dpi=150)
    plt.show()


def lorentzian(x, x0, gamma):
    """Funzione Lorentziana normalizzata:
    L(x) = (1/pi) * gamma / ((x - x0)^2 + gamma^2)
    """
    return (1.0 / np.pi) * gamma / ((x - x0) ** 2 + gamma ** 2)


def chi_eff_fit(chi_eff):
    """Istogramma normalizzato di chi_eff con fit Lorentziano
    tramite scipy.optimize.curve_fit, e plot di dati + fit.
    """
    # bins in input puo' avere diverse strutture:
    #  - un intero (come qui, bins=80): numero di intervalli di uguale
    #    larghezza, calcolati automaticamente su [chi_eff.min(), chi_eff.max()]
    #  - una stringa (es. "auto", "fd", "sturges"): numero di bin scelto
    #    automaticamente da una regola statistica
    #  - una sequenza di numeri (es. [0, 0.2, 0.5, 1]): bordi espliciti,
    #    anche di larghezza diversa tra loro, definiti a mano
    # Parametri di np.histogram(a, bins, density) usati qui:
    #  - a = chi_eff: l'array di dati da istogrammare
    #  - bins = 80: numero di intervalli (vedi sopra le forme alternative)
    #  - density = True: normalizza le altezze ad area totale 1
    # Restituisce due array: counts (altezze) e bin_edges (bordi).
    counts, bin_edges = np.histogram(chi_eff, bins=80, density=True)
    # bins=80 -> 80 intervalli, quindi bin_edges ha 81 bordi (e0..e80),
    # uno in piu' di counts: il bin i e' l'intervallo [e_i, e_{i+1}).
    # counts ha 80 valori, uno per bin: con density=True e' l'altezza
    # che rende l'area del bin (altezza * larghezza) pari alla frazione
    # di dati caduti li', cosi' l'area totale dell'istogramma vale 1.
    #
    # density=True normalizza l'istogramma a densita' di probabilita'
    # (area totale = 1), invece di lasciare conteggi grezzi che
    # dipendono da N_SAMPLES e dal numero di bin. La funzione lorentzian
    # e' gia' normalizzata (il fattore 1/pi la rende area=1), quindi
    # serve confrontare dati e modello sulla stessa scala: altrimenti
    # curve_fit userebbe gamma per compensare lo scarto di scala,
    # restituendo una stima priva di significato fisico.
    bin_centers = 0.5 * (bin_edges[:-1] + bin_edges[1:])
    # bin_edges[:-1] = bordi sinistri (e0..e79), bin_edges[1:] = bordi
    # destri (e1..e80): la media da' il punto medio di ciascun bin,
    # cosi' bin_centers ha 80 valori (come counts) e puo' essere
    # accoppiato a counts come coppie (x, y) per curve_fit.

    # Stima iniziale dei parametri: mediana come centro,
    # meta' dell'IQR come stima grezza della larghezza gamma
    median = np.median(chi_eff)
    q75, q25 = np.percentile(chi_eff, [75, 25])
    iqr = q75 - q25  # interquartile range
    p0 = [median, iqr / 2.0]

    popt, pcov = curve_fit(lorentzian, bin_centers, counts, p0=p0)
    x0_fit, gamma_fit = popt
    x0_err, gamma_err = np.sqrt(np.diag(pcov))  # pcov e' la matrice 2x2 di covarianza

    print("Fit Lorentziano su chi_eff:")
    print(f"  x0    = {x0_fit:.4f} +/- {x0_err:.4f}")
    print(f"  gamma = {gamma_fit:.4f} +/- {gamma_err:.4f}")

    # Plot: dati (istogramma) + curva di fit
    x_smooth = np.linspace(chi_eff.min(), chi_eff.max(), 500)
    y_smooth = lorentzian(x_smooth, *popt)   # valori y della Lorentziana fittata, calcolati su x_smooth

    fig, ax = plt.subplots(figsize=(8, 6))
    ax.hist(chi_eff, bins=80, density=True, color="lightgray",
            edgecolor="black", alpha=0.7, label="Data (histogram)")
    ax.plot(x_smooth, y_smooth, color="crimson", linewidth=2,
            label=rf"Lorentzian fit: $x_0$={x0_fit:.3f}, $\gamma$={gamma_fit:.3f}")
    ax.set_xlabel(r"$\chi_{\rm eff}$", fontsize=12)
    ax.set_ylabel("Probability density", fontsize=12)
    ax.set_title(r"Lorentzian fit of the $\chi_{\rm eff}$ distribution", fontsize=13)
    ax.legend(fontsize=11)
    ax.grid(alpha=0.3)
    fig.tight_layout()
    fig.savefig("chi_eff_lorentzian_fit.png", dpi=150)
    plt.show()


def main():
    pop = generate_distributions(N_SAMPLES)
    derived = compute_derived_quantities(pop)

    plot_distributions(derived)
    chi_eff_fit(derived["chi_eff"])


if __name__ == "__main__":
    main()








    
