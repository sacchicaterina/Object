import pickle
import numpy as np

# 1. Carico il file
with open("nomefile.pickle", "rb") as f:
    d = pickle.load(f)

# 2. Controllo che tipo di oggetto è
print(type(d))

# 3. Se è un dizionario, guardo le chiavi
print(d.keys())

# 4. Per ogni chiave, guardo il tipo del valore associato
for k, v in d.items():
    print(k, type(v))

# 5. Per ogni chiave, converto in array e guardo forma/range
for k, v in d.items():
    v = np.array(v)
    print(k, v.shape, v.dtype, v.min(), v.max())
