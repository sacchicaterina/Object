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

        self.positions = np.random.uniform(0, self.L, size=(self.N, 2))
        self.health_status = np.full(self.N, HEALTHY, dtype=int)
        self.health_status[0] = SICK

        self.history = []
        self.positions_history = []
        self._record_history()

    def _record_history(self):
        n_sick = int(np.sum(self.health_status == SICK))
        n_healthy = int(np.sum(self.health_status == HEALTHY))
        self.history.append((n_healthy, n_sick, n_healthy + n_sick))
        self.positions_history.append(self.positions.copy())

    def step_movement(self):
        is_healthy = (self.health_status == HEALTHY)
        indici_sani = np.where(is_healthy)[0]

        n_sani = indici_sani.size
        directions = np.random.uniform(0, 2 * np.pi, size=n_sani)
        dx = self.l * np.cos(directions)
        dy = self.l * np.sin(directions)
        displacement = np.column_stack((dx, dy))  # array(n_sani, 2)

        posizioni_sani_aggiornate = (self.positions[indici_sani] + displacement) % self.L
        self.positions[indici_sani] = posizioni_sani_aggiornate

    def update_states(self):
        is_healthy = (self.health_status == HEALTHY)
        is_sick = (self.health_status == SICK)
        indici_sani_originali = np.where(is_healthy)[0]
        indici_malati_originali = np.where(is_sick)[0]

        # GESTIONE CONTAGI
        differences = self.positions[:, np.newaxis, :] - self.positions[np.newaxis, :, :]
        differences -= self.L * np.round(differences / self.L)
        distances = np.linalg.norm(differences, axis=-1)  # o axis = 2

        # Matrice RETTANGOLARE: solo righe sani, solo colonne malati
        distances_sani_malati = distances[is_healthy, :][:, is_sick]
        sick_contacts_matrix = distances_sani_malati <= self.d

        # Dadi casuali solo sulla griglia piu' piccola (n_sani x n_malati)
        random_matrix = np.random.uniform(0, 1, size=sick_contacts_matrix.shape)
        contagion_events = sick_contacts_matrix & (random_matrix < self.ps)

        # Per ogni SANO (riga di questa matrice piu' piccola), almeno un contagio?
        has_been_infected = np.any(contagion_events, axis=1)  # shape: (n_sani,)

        # RIMAPPATURA: traduco "riga i della matrice piccola" in "indice originale"
        indici_nuovi_malati = indici_sani_originali[has_been_infected]

        # GESTIONE MORTE/GUARIGIONE (lavora solo sui malati, matrice piu' piccola)
        n_malati = indici_malati_originali.size
        x = np.random.uniform(0, 1, size=n_malati)
        dies_relativo = x < self.pd
        recovers_relativo = (x >= self.pd) & (x < self.pd + self.ph)

        # RIMAPPATURA: traduco "posizione i tra i malati" in "indice originale"
        indici_morti = indici_malati_originali[dies_relativo]
        indici_guariti = indici_malati_originali[recovers_relativo]

        self.health_status[indici_guariti] = HEALTHY
        self.health_status[indici_nuovi_malati] = SICK

        # RIMOZIONE MORTI
        alive = np.ones(self.N, dtype=bool)
        alive[indici_morti] = False

        self.positions = self.positions[alive]
        self.health_status = self.health_status[alive]
        self.N = self.health_status.size

    def run(self):
        t = 0
        while t < self.n_steps and self.health_status.size > 0:
            self.step_movement()
            self.update_states()
            self._record_history()
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
