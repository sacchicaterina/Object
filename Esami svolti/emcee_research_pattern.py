"""
COME ISPEZIONARE UN FILE PICKLE SENZA SAPERE COSA CONTIENE
=============================================================

Quando ricevi un file .pickle e non sai come è strutturato dentro,
questo script ti aiuta a "esplorarlo" passo dopo passo, invece di
stampare tutto il contenuto (che potrebbe essere enorme e illeggibile).
"""

import pickle

# -------------------------------------------------------------------
# 1) APRIRE E LEGGERE IL FILE
# -------------------------------------------------------------------
# "rb" = read binary. I file pickle sono binari, non testo, quindi
# vanno sempre aperti così (mai con "r" semplice).
#
# pickle.load(f) legge i byte e ricostruisce l'oggetto Python
# originale (dizionario, lista, array numpy, oggetto custom...)
# dentro la variabile "data".

with open("nomefile.pickle", "rb") as f:
    data = pickle.load(f)


# -------------------------------------------------------------------
# 2) CAPIRE CHE TIPO DI OGGETTO E'
# -------------------------------------------------------------------
# Questa è SEMPRE la prima cosa da fare quando non conosci il
# contenuto: ti dice se è un dict, una list, un array numpy, ecc.
# In base al tipo deciderai come continuare a esplorarlo.

print(type(data))


# -------------------------------------------------------------------
# 3) ESPLORARE IN BASE AL TIPO
# -------------------------------------------------------------------

if isinstance(data, dict):
    # Caso: DIZIONARIO
    # Stampo tutte le chiavi presenti, per sapere quali "campi" ci sono.
    print(data.keys())

    # Poi ciclo su ogni coppia chiave-valore e stampo, per ciascuna
    # chiave, il TIPO del valore associato (non il valore intero,
    # che potrebbe essere lunghissimo, es. un array con migliaia
    # di numeri).
    for k, v in data.items():
        print(k, type(v))

elif isinstance(data, (list, tuple)):
    # Caso: LISTA o TUPLA
    # Stampo quanti elementi contiene...
    print(len(data))
    # ...e mostro solo il primo elemento come "esempio", per farmi
    # un'idea della struttura senza stampare l'intera sequenza.
    print(data[0])

else:
    # Caso: QUALSIASI ALTRO TIPO
    # (array numpy, oggetto custom, numero, stringa, ecc.)
    # Di solito questi si possono stampare direttamente perché
    # sono già leggibili così come sono.
    print(data)


"""
RIASSUNTO / LOGICA DELLO SCRIPT
=============================================================
Questo è uno script di "ricognizione": non sai cosa c'è dentro il
pickle, quindi:

  1. Prima guardi il tipo generale dell'oggetto (type(data))
  2. In base al tipo, scegli il modo più sensato per dare
     un'occhiata al contenuto SENZA stampare tutto:
       - dict       -> chiavi + tipo di ogni valore
       - list/tuple -> lunghezza + primo elemento come esempio
       - altro      -> stampa diretta (di solito già leggibile)

Una volta capita la struttura (es. "è un dizionario con chiavi
x, y, yerr, tutti array numpy"), puoi passare a caricarlo in modo
mirato, tipo:

    x = data["x"]
    y = data["y"]
    yerr = data["yerr"]

senza dover più "esplorare" ogni volta.
"""
