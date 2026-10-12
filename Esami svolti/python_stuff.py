import numpy as np

# ============================================
# ESEMPIO: valori e frequenze ordinati insieme
# ============================================

# Dati di partenza
values = np.array(['a', 'b', 'a', 'c', 'b', 'a'])

# Step 1: trovo i valori unici e quante volte compare ciascuno
# vals   = i valori distinti (ordinati alfabeticamente da np.unique)
# counts = quante volte compare ciascun valore (stessa posizione di vals)
vals, counts = np.unique(values, return_counts=True)
# vals   = ['a', 'b', 'c']
# counts = [3, 2, 1]
#  indice:   0    1    2

# Step 2: np.argsort(counts)
# argsort NON ordina i numeri, ordina le POSIZIONI (indici).
# Restituisce l'ordine di indici che, se applicato a counts,
# lo leggerebbe dal valore più PICCOLO al più GRANDE (ordine crescente).
#
# counts = [3, 2, 1]
# il valore più piccolo (1) sta all'indice 2  -> primo
# poi il valore 2 sta all'indice 1            -> secondo
# poi il valore 3 sta all'indice 0            -> terzo
# quindi:
indici_crescenti = np.argsort(counts)
# indici_crescenti = [2, 1, 0]
# verifica: counts[[2,1,0]] = [1, 2, 3]  -> crescente, corretto

# Step 3: [::-1]
# Inverto la SEQUENZA di indici (non i valori, gli indici stessi).
# Percorrere al contrario una fila di indici che porta al crescente
# equivale, per definizione, a percorrerla in ordine decrescente:
# la prima posizione che incontro ora è quella che PRIMA era l'ultima,
# cioè quella col conteggio più alto.
order = indici_crescenti[::-1]
# order = [0, 1, 2]
# (in questo esempio coincide con l'ordine "naturale" perché i conteggi
#  erano già disposti in ordine decrescente nei dati originali - è un caso,
#  non una regola generale)

# In sintesi, tutto in una riga:
order = np.argsort(counts)[::-1]

# Step 4: fancy indexing
# Uso la STESSA sequenza di indici "order" per riordinare ENTRAMBI gli array.
# Siccome vals[i] e counts[i] sono allineati (stessa posizione = stesso elemento
# originale), applicare lo stesso ordine ad entrambi mantiene l'abbinamento corretto.
vals_ordinati = vals[order]      # ['a', 'b', 'c']
freq_ordinata = counts[order]    # [3, 2, 1]

# Risultato: vals_ordinati[i] e freq_ordinata[i] si corrispondono sempre,
# e sono ordinati dal valore più frequente al meno frequente.

print(list(zip(vals_ordinati, freq_ordinata)))
# [('a', 3), ('b', 2), ('c', 1)]


# ============================================
# ESEMPIO 2: caso in cui l'inversione si "vede" meglio
# (i conteggi NON sono già in ordine decrescente in origine)
# ============================================

values2 = np.array(['x', 'y', 'y', 'z', 'z', 'z'])
vals2, counts2 = np.unique(values2, return_counts=True)
# vals2   = ['x', 'y', 'z']
# counts2 = [1, 2, 3]
#  indice:    0    1    2

order2 = np.argsort(counts2)[::-1]
# argsort(counts2) crescente = [0, 1, 2]  (già crescente così com'è)
# invertito                  = [2, 1, 0]  <- qui si vede chiaramente l'inversione

vals2_ordinati = vals2[order2]      # ['z', 'y', 'x']
freq2_ordinata = counts2[order2]    # [3, 2, 1]

print(list(zip(vals2_ordinati, freq2_ordinata)))
# [('z', 3), ('y', 2), ('x', 1)]


# ============================================
# CONCETTO CHIAVE DA RICORDARE
# ============================================
# - np.sort() riordina i VALORI di un array, perdendo il collegamento con altri array
# - np.argsort() riordina gli INDICI (le "posizioni"), NON i valori
# - Quegli stessi indici possono essere riapplicati a PIÙ array paralleli,
#   mantenendoli sincronizzati tra loro (fancy indexing: array[indici])
# - [::-1] su un ordine crescente lo rende decrescente, perché percorre
#   la stessa fila di indici partendo dall'ultimo invece che dal primo




# ============================================
# DIFFERENZA TRA == SU LISTE E == SU ARRAY NUMPY
# ============================================

# --- LISTE PYTHON ---
# Su una lista normale, == confronta l'intera lista in un colpo solo
# e restituisce UN SOLO booleano (True/False), quindi si può usare
# direttamente in un if senza problemi.

a = [1, 2, 3]
b = [1, 2, 3]

print(a == b)   # True  -> un solo booleano

if a == b:
    print("le liste sono uguali")   # funziona correttamente


# --- ARRAY NUMPY ---
# Su un array NumPy, == è invece VETTORIZZATO: confronta gli array
# ELEMENTO PER ELEMENTO e restituisce un ARRAY DI BOOLEANI,
# uno per ogni posizione, non un singolo True/False.

import numpy as np

x = np.array([1, 2, 3])
y = np.array([1, 2, 3])

print(x == y)
# → array([True, True, True])   <- NON è un booleano singolo, è un array!

# Per questo motivo, usarlo direttamente in un if genera un ERRORE,
# perché Python non sa se considerare "vero" il fatto che TUTTI gli
# elementi siano True, o che ALMENO UNO lo sia:

# if x == y:              # ❌ ValueError: the truth value of an array
#                          #    with more than one element is ambiguous


# --- LA SOLUZIONE: np.array_equal() ---
# np.array_equal(a1, a2) fa il lavoro che == fa "gratis" sulle liste:
# confronta shape E valori di due array, e restituisce SEMPRE
# un solo booleano (True/False), utilizzabile direttamente in un if.
# In pratica "ripristina" il comportamento a cui siamo abituati con le liste.

print(np.array_equal(x, y))   # True -> un solo booleano

if np.array_equal(x, y):
    print("gli array sono uguali")   # funziona correttamente


# ============================================
# RIASSUNTO
# ============================================
# - Liste:        ==  ->  confronto diretto, un solo booleano, usabile in if
# - Array NumPy:   ==  ->  confronto elemento per elemento, array di booleani,
#                          NON usabile direttamente in if
# - Per ottenere un singolo booleano con NumPy si usa:
#     np.array_equal(a, b)   -> shape e valori devono coincidere esattamente
#     (a == b).all()         -> alternativa equivalente, True se TUTTI True
#     (a == b).any()         -> True se ALMENO UNO è True
