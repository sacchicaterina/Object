#ARRAY BOOL + BINCOUNT/UNIQUE
import numpy as np
import matplotlib.pyplot as plt
import random
import time
from scipy.optimize import curve_fit
from scipy.special import gammaln


class Die:

    def __init__(self, faces=(1, 2, 3, 4, 5, 6)):      
        self.faces = tuple(faces)

    def roll(self):
        return random.choice(self.faces)


class KniffelGame:

    COMBINATIONS = np.array([
        "straight_low",     # 1,2,3,4,5
        "straight_high",    # 2,3,4,5,6
        "kniffel",          # tutti uguali
        "three_of_a_kind",  # esattamente 3 uguali
        "four_of_a_kind",   # esattamente 4 uguali
        "full_house",       # 3 + 2
    ])

    IDX_STRAIGHT_LOW = 0
    IDX_STRAIGHT_HIGH = 1
    IDX_KNIFFEL = 2
    IDX_THREE_OF_A_KIND = 3
    IDX_FOUR_OF_A_KIND = 4
    IDX_FULL_HOUSE = 5

    N_COMBINATIONS = 6

    def __init__(self, n_dice=5):
        
        self.n_dice = n_dice
        self.die = Die()
        self.n_rounds = 0
        self.found = np.zeros(self.N_COMBINATIONS, dtype=bool)   #array di 6 False
        

    def __str__(self):
        
        """Riassunto leggibile dello stato della partita, utile per il debug."""
        parts = []
        for name, ok in zip(self.COMBINATIONS, self.found):
            parts.append(f"{name}={'YES' if ok else 'no'}")
        status = ", ".join(parts)
        
        return f"KniffelGame(rounds={self.n_rounds}, {status})"
    

    def gameSimulation(self):
        
        while not self.found.all():
            values = np.array([self.die.roll() for _ in range(self.n_dice)])
            # values è l'array di partenza contenente i dati grezzi (es. categorie, etichette, numeri)

            # Step 1: np.unique(values, return_counts=True)
            # Trova tutti i valori DISTINTI presenti in "values" e, grazie a return_counts=True,
            # restituisce anche quante volte ciascun valore compare.
            # Il risultato è una tupla: (array_dei_valori_unici, array_dei_conteggi)

            # Step 2: [1]
            # Prendo solo il secondo elemento della tupla, cioè l'array dei conteggi
            # (scarto l'array dei valori unici, mi interessa solo "quante volte" compare ognuno)

            # Step 3: np.sort(...)
            # Ordino i conteggi in ordine crescente (dal più piccolo al più grande)

            # Step 4: [::-1]
            # Inverto l'array ordinato, ottenendo l'ordine decrescente
            # (dal valore più frequente al meno frequente)
            freq = np.sort(np.unique(values, return_counts=True)[1])[::-1]

            # freq contiene ora le frequenze assolute dei valori unici in "values",
            # ordinate dalla più alta alla più bassa.
            # NB: si perde l'associazione con il valore originale a cui ogni frequenza si riferisce,
            # si ottengono solo i numeri (conteggi) ordinati.

            
            #array_equal crea confronto tra array di diversa lunghezza generando un unico bool
            if np.array_equal(freq, [1, 1, 1, 1, 1]) and 6 not in values:     
                self.found[self.IDX_STRAIGHT_LOW] = True
            elif np.array_equal(freq, [1, 1, 1, 1, 1]) and 1 not in values:
                self.found[self.IDX_STRAIGHT_HIGH] = True
            elif np.array_equal(freq, [3, 1, 1]):
                self.found[self.IDX_THREE_OF_A_KIND] = True
            elif np.array_equal(freq, [3, 2]):
                self.found[self.IDX_FULL_HOUSE] = True
            elif np.array_equal(freq, [4, 1]):
                self.found[self.IDX_FOUR_OF_A_KIND] = True
            elif np.array_equal(freq, [5]):
                self.found[self.IDX_KNIFFEL] = True

            self.n_rounds += 1

        return self.n_rounds

        # --- Versione alternativa con np.bincount (tenuta per studio) ---
        # freq0 = np.bincount(values, minlength=7)
        # freq = np.sort(freq0[freq0 > 0])[::-1]
        # == crea confronto elemento per elemento generando un array di bool
        # if np.all(freq0[1:6] == 1):
        #     self.found[self.IDX_STRAIGHT_LOW] = True
        # elif np.all(freq0[2:7] == 1):
        #     self.found[self.IDX_STRAIGHT_HIGH] = True
        # elif np.array_equal(freq, [3, 1, 1]):
        #     self.found[self.IDX_THREE_OF_A_KIND] = True
        # elif np.array_equal(freq, [3, 2]):
        #     self.found[self.IDX_FULL_HOUSE] = True
        # elif np.array_equal(freq, [4, 1]):
        #     self.found[self.IDX_FOUR_OF_A_KIND] = True
        # elif np.array_equal(freq, [5]):
        #     self.found[self.IDX_KNIFFEL] = True


def poisson(k, lam):
    """Probabilita' di Poisson, calcolata in scala logaritmica per evitare
    overflow numerico quando lambda e' grande (es. ~1300, come nel nostro
    caso: il kniffel e' molto raro, quindi servono in media molti round).

    Formula originale (probabilita' diretta):
        P(k) = (lam**k) * exp(-lam) / factorial(k)

    Problema: con lam e k grandi, lam**k e factorial(k) diventano numeri
    talmente enormi che Python non riesce piu' a rappresentarli (overflow),
    dando come risultato 'inf' o errori nel fit.

    Soluzione: calcolare il LOGARITMO della probabilita' invece della
    probabilita' diretta. Il logaritmo di un numero enorme resta un
    numero piccolo e gestibile, quindi non c'e' overflow:
        log(P(k)) = k*log(lam) - lam - log(k!)
    Poi si torna alla probabilita' vera solo alla fine, con exp().
    """
    log_pmf = k * np.log(lam) - lam - gammaln(k + 1)
    return np.exp(log_pmf)


def main():
    # =============================================================================
    # DATA PRODUCTION: verifica di velocità e simulazione di n_games partite
    # =============================================================================

    # --- Verifica velocità di una singola partita (deve stare ben sotto 100 ms) ---
    kn = KniffelGame()
    t0 = time.time()
    kn.gameSimulation()
    t1 = time.time()
    print("Tempo singola partita:", (t1 - t0) * 1000, "ms")
    print(kn)

    # --- Simulazione di n_games partite ---
    n_games = 10_000
    n_rounds_history = np.zeros(n_games, dtype=int)

    t0 = time.time()
    for game in range(n_games):
        kn = KniffelGame()
        n_rounds_history[game] = kn.gameSimulation()
    t1 = time.time()

    print(f"{n_games} partite in {(t1 - t0):.1f} secondi")
    print("Min:", n_rounds_history.min(), "- Max:", n_rounds_history.max())
    print("Media campionaria:", n_rounds_history.mean())


    # =============================================================================
    # DATA PROCESSING: istogramma di densita' e fit con distribuzione di Poisson
    # =============================================================================

    # Costruisce i bordi (edges) dei bin dell'istogramma: numeri equidistanti,
    # distanziati di 100 round l'uno dall'altro, da poco prima del valore
    # minimo osservato a poco dopo il valore massimo (il -0.5 e +1.5 servono
    # ad allargare leggermente l'intervallo, cosi' anche i valori estremi
    # rientrano in un bin).
    bin_edges = np.arange(n_rounds_history.min() - 0.5, n_rounds_history.max() + 1.5, 100)

    # np.histogram conta quanti valori di n_rounds_history cadono in ciascun bin.
    # density=True normalizza il risultato come densita' di probabilita'
    # (l'area totale sotto l'istogramma somma a 1), necessario per poterlo
    # confrontare con la curva teorica di Poisson nel fit.
    # Restituisce due array: counts (altezza di ciascun bin) ed edges (bordi,
    # identici a bin_edges).
    counts, edges = np.histogram(n_rounds_history, bins=bin_edges, density=True)

    # Il fit ha bisogno di UN valore x per ciascun bin, non due bordi. Si
    # prende quindi il punto medio tra bordo sinistro (edges[:-1], tutti i
    # bordi tranne l'ultimo) e bordo destro (edges[1:], tutti tranne il
    # primo) di ciascun bin.
    bin_centers = 0.5 * (edges[:-1] + edges[1:])


    # curve_fit cerca automaticamente il valore di "lam" che fa combaciare al
    # meglio la funzione poisson() con i dati reali (bin_centers sull'asse x,
    # counts sull'asse y). p0 e' il valore di partenza per la ricerca: dare
    # come stima iniziale la media campionaria aiuta l'algoritmo a convergere
    # piu' velocemente e in modo piu' affidabile.
    #
    # curve_fit restituisce due cose:
    #   popt -> i parametri OTTIMALI trovati (qui un array con un solo
    #           elemento, dato che poisson ha un solo parametro libero: lam)
    #   pcov -> la matrice di covarianza dei parametri stimati (qui una
    #           matrice 1x1, perche' c'e' un solo parametro da stimare)
    popt, pcov = curve_fit(poisson, bin_centers, counts, p0=[n_rounds_history.mean()])

    # Estrae il valore stimato di lambda (primo, e unico, elemento di popt).
    lam_fit = popt[0]

    # pcov[0, 0] e' la varianza della stima di lam (quanto "oscillerebbe" la
    # stima ripetendo l'esperimento). La radice quadrata della varianza da'
    # la deviazione standard, cioe' l'incertezza (l'errore) sulla stima.
    lam_err = np.sqrt(pcov[0, 0])

    # Stampa il risultato del fit, con 2 cifre decimali (":.2f").
    print(f"Fit Poisson: lambda = {lam_fit:.2f} +/- {lam_err:.2f}")


    # --- Plot: istogramma + fit sovrapposti, con la media evidenziata ---
    fig = plt.figure(figsize=(8, 6))

    # disegna l'istogramma
    plt.hist(
        n_rounds_history, bins=bin_edges, density=True,
        alpha=0.6, color="steelblue", label="Dati simulati (densità)",
    )

    # disegna la curva del fit
    x_fit = np.linspace(bin_edges[0], bin_edges[-1], 500)
    plt.plot(
        x_fit, poisson(x_fit, lam_fit), "r-", lw=2,
        label=rf"Fit Poisson: $\lambda$ = {lam_fit:.1f} $\pm$ {lam_err:.1f}",
    )

    plt.axvline(lam_fit, color="black", linestyle="--", label=f"Media = {lam_fit:.1f}")

    plt.xlabel("Numero di round")
    plt.ylabel("Densità di probabilità")
    plt.title(f"Kniffel: distribuzione di {n_games} partite simulate")
    plt.legend()
    plt.tight_layout()
    plt.savefig("kniffel_distribution.png", dpi=150)
    plt.show()


if __name__ == "__main__":
    main()

    
    
   
