import numpy as np
import matplotlib.pyplot as plt

HEALTHY = 0
SICK = 1


class Epidemic:

    def __init__(self, N, L, l, pd, ph, ps, d, n_steps):

        if pd + ph >= 1:
            raise ValueError("pd + ph deve essere minore di 1")

        self.N = N
        self.L = L
        self.l = l
        self.pd = pd
        self.ph = ph
        self.ps = ps
        self.d = d
        self.n_steps = n_steps

        # array bidi(self.N, 2) ogni casella riempita con valori da 0 a self.L
        self.positions = np.random.uniform(0, self.L, size=(self.N, 2))  # array(N,2)
        self.health_status = np.full(self.N, HEALTHY, dtype=int)         # array(N,)
        self.health_status[0] = SICK

        self.history = []
        self.record_history()   # registro le condizioni iniziali (t=0), prima di qualsiasi timestep
        #self.positions_history = []

    def step_movement(self):
        is_healthy = (self.health_status == HEALTHY)  # (N,)

        directions = np.random.uniform(0, 2 * np.pi, size=self.N)
        dx = self.l * np.cos(directions)
        dy = self.l * np.sin(directions)
        displacement = np.column_stack((dx, dy))  # array(N, 2)
        #dispacement = np.vstack((dx, dy)).T
        # vstack impila come RIGHE -> (2, N)
        # .T trasposto -> (N, 2)

        # si muovono solo i sani            array(N,2)                array(N,2)
        self.positions[is_healthy] = (self.positions[is_healthy] + displacement[is_healthy]) % self.L

    def update_states(self):
        is_healthy = (self.health_status == HEALTHY)
        is_sick = (self.health_status == SICK)

        # GESTIONE CONTAGI                                  (N,1,2)                          (1,N,2)
        differences = self.positions[:, np.newaxis, :] - self.positions[np.newaxis, :, :]  # tutti vs tutti
        differences -= self.L * np.round(differences / self.L)
        distances = np.linalg.norm(differences, axis=-1)  # o axis = 2

        # is_sick: shape (N,) -> chi e' malato adesso
        # distances: shape (N, N) -> distanza tra ogni coppia di individui
        #
        # Lavoro sempre sulla matrice N x N completa (non la riduco filtrando
        # righe/colonne man mano) perche' e' piu' semplice e sicura da gestire:
        # la sua forma resta sempre fissa e prevedibile ad ogni timestep, anche
        # quando cambiano quanti sono i sani o i malati. Il costo di generare
        # qualche numero casuale "in piu'" (per coppie che poi scarto con l'AND)
        # e' trascurabile, mentre evitare shape dinamiche riduce il rischio di
        # bug legati a rimappature di indici.

        # Per ogni possibile coppia (i, j): j e' malato E i e' abbastanza vicino
        # a j da poter essere contagiato. shape (N, N)
        sick_contacts_matrix = is_sick[np.newaxis, :] & (distances <= self.d)

        # Tiro un dado (numero casuale U(0,1)) per OGNI possibile coppia (i, j).
        # shape (N, N)
        random_matrix = np.random.uniform(0, 1, size=(self.N, self.N))

        # Il contagio per la coppia (i,j) avviene solo se ENTRAMBE le condizioni
        # sono vere: c'era un contatto pericoloso E il dado e' andato a favore
        # del contagio. shape (N, N)
        contagion_events = sick_contacts_matrix & (random_matrix < self.ps)

        # Per ogni individuo (riga), voglio trovare almeno un contagiante tra
        # tutti i suoi possibili contatti (le colonne di quella riga) - axis=1
        # scorre le colonne. shape (N,)
        has_been_infected = np.any(contagion_events, axis=1)

        # Si ammalano davvero solo quelli che erano sani prima del contagio.
        # shape (N,)
        newly_infected = is_healthy & has_been_infected

        # GESTIONE MORTE/GUARIGIONE
        x = np.random.uniform(0, 1, size=self.N)
        dies = is_sick & (x < self.pd)
        recovers = is_sick & (x >= self.pd) & (x < self.pd + self.ph)

        # AGGIORNAMENTI
        self.health_status[recovers] = HEALTHY
        self.health_status[newly_infected] = SICK

        # alive: shape (N,) -- inverto i booleani di dies con ~ (dove dies era
        # True diventa False e viceversa), ottenendo True per chi NON muore in
        # questo timestep
        alive = ~dies

        # Tengo solo le righe (posizioni) dei sopravvissuti, scarto quelle dei
        # morti. L'array si accorcia: aveva N righe, ora ne ha N - (numero morti)
        self.positions = self.positions[alive]

        # Stessa identica selezione, con la STESSA maschera alive, cosi' resta
        # allineato a self.positions: la riga i di health_status corrisponde
        # sempre alla riga i di positions, cioe' allo stesso individuo
        self.health_status = self.health_status[alive]

        # Aggiorno N alla nuova dimensione reale della popolazione (dopo le
        # rimozioni), altrimenti al prossimo timestep verrebbero generati array
        # della vecchia dimensione, non piu' corrispondente alla popolazione
        # effettiva
        self.N = self.health_status.size

    def record_history(self):
        n_sick = np.sum(self.health_status == SICK)
        n_healthy = np.sum(self.health_status == HEALTHY)
        self.history.append((n_healthy, n_sick, n_healthy + n_sick))
        #self.positions_history.append(self.positions.copy())

    def run(self):
        t = 0
        while t < self.n_steps and self.health_status.size > 0:
            self.step_movement()
            self.update_states()
            self.record_history()
            t += 1

    def plot_history(self):
        history = np.array(self.history)
        t = np.arange(history.shape[0])
        healthy, sick, total = history[:, 0], history[:, 1], history[:, 2]

        fig = plt.figure(figsize=(8, 6))

        plt.plot(t, healthy, label="Sani", color="tab:blue", linewidth=2)
        plt.plot(t, sick, label="Malati", color="tab:green", linewidth=2)
        plt.plot(t, total, label="Totali", color="tab:red", linewidth=2)

        plt.xlabel("Timesteps")
        plt.ylabel("Numero di individui")
        plt.title("Evoluzione epidemia nel tempo")
        plt.legend()
        plt.grid(alpha=0.3)

        fig.tight_layout()
        plt.savefig("epidemia.png")
        plt.show()


def main():
    N = 300
    L = 50.0
    l = 1.0
    pd = 0.01
    ph = 0.03
    ps = 0.3
    d = 1.5
    n_steps = 150

    simulation = Epidemic(N, L, l, pd, ph, ps, d, n_steps)
    simulation.run()
    simulation.plot_history()


if __name__ == "__main__":
    main()
