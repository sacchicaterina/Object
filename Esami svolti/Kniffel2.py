#COUNTER + DICT/SET
import numpy as np
import matplotlib.pyplot as plt
import random
from collections import Counter


class Die:
    def __init__(self, faces=(1, 2, 3, 4, 5, 6)):
        self.faces = tuple(faces)

    def roll(self):
        return random.choice(self.faces)


class KniffelGame:
    
    COMBINATIONS = (                                     # tupla python
        "straight_low",     # 1,2,3,4,5
        "straight_high",    # 2,3,4,5,6
        "kniffel",          # tutti uguali
        "three_of_a_kind",  # esattamente 3 uguali
        "four_of_a_kind",   # esattamente 4 uguali
        "full_house",       # 3 + 2
    )

    N_COMBINATIONS = 6

    def __init__(self, n_dice=5):
        
        self.n_dice = n_dice
        self.die = Die()       # creazione oggetto dado
        self.n_rounds = 0

        # Dizionario: chiave = nome combinazione, valore = trovata o no
        self.found = dict.fromkeys(self.COMBINATIONS, False)
        # equivale a scrivere a mano:
        # self.found = {
        #     "straight_low": False,
        #     "straight_high": False,
        #     "kniffel": False,
        #     "three_of_a_kind": False,
        #     "four_of_a_kind": False,
        #     "full_house": False,
        # }

    def gameSimulation(self):
        
        values = np.array([self.die.roll() for _ in range(self.n_dice)])
        counts = Counter(values)                       # es. Counter({2: 3, 5: 2})
        freq = sorted(counts.values(), reverse=True)    # es. [3, 2] (lista Python)

        #uso di liste python len() + ==
        if len(freq) == 5 and 6 not in values:
            self.found["straight_low"] = True
        elif len(freq) == 5 and 1 not in values:
            self.found["straight_high"] = True
        elif freq == [3, 1, 1]:
            self.found["three_of_a_kind"] = True
        elif freq == [3, 2]:
            self.found["full_house"] = True
        elif freq == [4, 1]:
            self.found["four_of_a_kind"] = True
        elif freq == [5]:
            self.found["kniffel"] = True

# ---------------------------------------------------------------------------
# APPUNTI DI STUDIO: versione alternativa con `found` come SET invece che
# come dizionario. Tenuta commentata solo come riferimento didattico.
# ---------------------------------------------------------------------------
#     def __init__(self, n_dice=5):
#         self.n_dice = n_dice
#         self.die = Die()
#  
#         # Set: contiene i NOMI delle combinazioni già trovate.
#         # Inizialmente vuoto: nessuna combinazione ancora vista.
#         self.found = set()
#
#     def gameSimulation(self):
#         values = np.array([self.die.roll() for _ in range(self.n_dice)])
#         counts = Counter(values)
#         freq = sorted(counts.values(), reverse=True)
#
          #.add() metodo dei set
#         if len(freq) == 5 and 6 not in values:
#             self.found.add("straight_low")
#         elif len(freq) == 5 and 1 not in values:
#             self.found.add("straight_high")
#         elif freq == [3, 1, 1]:
#             self.found.add("three_of_a_kind")
#         elif freq == [3, 2]:
#             self.found.add("full_house")
#         elif freq == [4, 1]:
#             self.found.add("four_of_a_kind")
#         elif freq == [5]:
#             self.found.add("kniffel")



    




    
