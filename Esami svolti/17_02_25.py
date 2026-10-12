import numpy as np
import matplotlib.pyplot as plt
import pickle


class IsotropicParetoRandomWalk:

    def __init__(self, shape, Nexp, Nsteps, dim):
        if shape <= 0:
            raise ValueError("shape parameter 'a' must be > 0")
        if dim not in (2, 3):
            raise ValueError("dim must be 2 or 3")
        if Nexp < 1 or Nsteps < 1:
            raise ValueError("Nexp and Nsteps must be >= 1")

        self.shape = shape
        self.Nexp = Nexp
        self.Nsteps = Nsteps
        self.dim = dim

        self.steps = None
        self.directions = None
        self.trajectories = None

    def generateRandomWalk(self):
        initial_steps = np.zeros((self.Nexp, 1, self.dim))

        # Generiamo un vettore casuale per ogni passo di ogni walker.
        # Ogni componente (x, y, e z se in 3D) viene pescata da una gaussiana
        # standard N(0,1), indipendentemente dalle altre.
        # Risultato: array di forma (Nexp, Nsteps, dim) -- non ancora normalizzato,
        # quindi le lunghezze di questi vettori sono ancora casuali e non ci interessano.
        vecs = np.random.normal(size=(self.Nexp, self.Nsteps, self.dim))

        # Calcoliamo la norma (lunghezza) di ciascun vettore.
        # axis=-1 significa "l'ultimo asse", cioè quello delle componenti (x,y,[z]):
        # per ogni walker e ogni passo, riduciamo le sue "dim" componenti a un solo
        # numero (la lunghezza del vettore). Usare -1 invece di un numero fisso
        # rende il codice corretto automaticamente sia in 2D che in 3D.
        # keepdims=True mantiene quell'asse come dimensione 1 (invece di eliminarlo),
        # così la divisione sotto funziona correttamente via broadcasting.
        norms = np.linalg.norm(vecs, axis=-1, keepdims=True)

        # Normalizziamo: dividiamo ogni vettore per la propria norma, ottenendo
        # vettori di lunghezza esattamente 1 -- solo la direzione, nessuna lunghezza.
        # La gaussiana multivariata standard è isotropa (densità dipendente solo
        # dalla distanza dall'origine, mai dall'angolo), quindi il vettore
        # normalizzato è uniformemente distribuito sul cerchio (2D) o sulla
        # sfera (3D) unitari.
        self.directions = vecs / norms

        # Lunghezze dei passi dalla distribuzione di Pareto II (supporto r >= 1).
        self.steps = np.random.pareto(self.shape, size=(self.Nexp, self.Nsteps)) + 1

        # Spostamento di ogni passo = direzione (unitaria) * lunghezza (Pareto),
        # sommati cumulativamente nel tempo per ottenere le posizioni.
        positions = np.cumsum(self.directions * self.steps[:, :, np.newaxis], axis=1)

        # L'origine (zero) viene concatenata davanti alle posizioni cumulate,
        # cosi' ogni traiettoria include esplicitamente il punto di partenza.
        self.trajectories = np.concatenate((initial_steps, positions), axis=1)

        return self.trajectories

    def save(self, filename):
        """
        Save the simulation (parameters + trajectories) to a single pickle
        file, so it can be reloaded later without re-running the simulation.
        """
        payload = {
            "shape": self.shape,
            "Nexp": self.Nexp,
            "Nsteps": self.Nsteps,
            "dim": self.dim,
            "trajectories": self.trajectories,
        }

        with open(filename, "wb") as f:
            pickle.dump(payload, f)

    @classmethod
    def load(cls, filename):
        """
        Rebuild an IsotropicParetoRandomWalk (with its trajectories already
        filled in) from a pickle file produced by save().
        """
        with open(filename, "rb") as f:
            payload = pickle.load(f)

        obj = cls(
            shape=payload["shape"],
            Nexp=payload["Nexp"],
            Nsteps=payload["Nsteps"],
            dim=payload["dim"],
        )

        obj.trajectories = payload["trajectories"]

        return obj

    def display(self):
        fig = plt.figure(figsize=(7, 6))
        ax = fig.add_subplot(111, projection="3d" if self.dim == 3 else None)

        for w in range(self.Nexp):
            traj = self.trajectories[w]
            if self.dim == 2:
                ax.plot(traj[:, 0], traj[:, 1], alpha=0.75, linewidth=1.2, label=f"walker {w}")
            else:
                ax.plot(traj[:, 0], traj[:, 1], traj[:, 2], alpha=0.75, linewidth=1.2, label=f"walker {w}")

        origin = [0.0] * self.dim
        ax.scatter(*origin, color="black", marker="x", s=60, zorder=5, label="start")

        ax.set_xlabel("x")
        ax.set_ylabel("y")
        if self.dim == 3:
            ax.set_zlabel("z")

        ax.set_title(f"{self.dim}D random walk (a={self.shape}, Nexp={self.Nexp}, Nsteps={self.Nsteps})")
        ax.legend(loc="best", fontsize=8)
        ax.set_aspect("equal" if self.dim == 2 else "auto")
        plt.tight_layout()

        plt.savefig(f"walk_{self.dim}D_a{self.shape}_Nexp{self.Nexp}_Nsteps{self.Nsteps}.png", dpi=80)

        return fig

    # Versione alternativa di display: usa la trasposta (traj.T) e lo
    # spacchettamento con l'asterisco (*) al posto dell'if esplicito
    # self.dim == 2 / else. ax.plot(*traj.T) passa automaticamente 2 o 3
    # argomenti a seconda di quante righe ha traj.T, quindi funziona
    # identica in 2D e 3D senza bisogno di distinguere i due casi.
    # def display(self):
    #     fig = plt.figure(figsize=(7, 6))
    #     ax = fig.add_subplot(111, projection="3d" if self.dim == 3 else None)
    #
    #     for w in range(self.Nexp):
    #         traj = self.trajectories[w]
    #         ax.plot(*traj.T, alpha=0.75, linewidth=1.2, label=f"walker {w}")
    #
    #     origin = [0.0] * self.dim
    #     ax.scatter(*origin, color="black", marker="x", s=60, zorder=5, label="start")
    #
    #     ax.set_xlabel("x")
    #     ax.set_ylabel("y")
    #     if self.dim == 3:
    #         ax.set_zlabel("z")
    #
    #     ax.set_title(f"{self.dim}D random walk (a={self.shape}, Nexp={self.Nexp}, Nsteps={self.Nsteps})")
    #     ax.legend(loc="best", fontsize=8)
    #     ax.set_aspect("equal" if self.dim == 2 else "auto")
    #     plt.tight_layout()
    #
    #     plt.savefig(f"walk_{self.dim}D_a{self.shape}_Nexp{self.Nexp}_Nsteps{self.Nsteps}.png", dpi=80)
    #
    #     return fig


def main():
    # Esempio 2D con 10 walker
    sim2d = IsotropicParetoRandomWalk(shape=1.5, Nexp=10, Nsteps=300, dim=2)
    sim2d.generateRandomWalk()
    sim2d.save("walk2d.pkl")
    sim2d.display()

    # Esempio 3D con 10 walker
    sim3d = IsotropicParetoRandomWalk(shape=1.5, Nexp=10, Nsteps=300, dim=3)
    sim3d.generateRandomWalk()
    sim3d.save("walk3d.pkl")
    sim3d.display()


if __name__ == "__main__":
    main()    
 
   
       
    

       

     
