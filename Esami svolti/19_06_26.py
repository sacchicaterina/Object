import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import solve_ivp


# dpsi = v
# dv = -psi + lam * psi^3


class QuantumSystem:

    def __init__(self, lam, k=1.):

        self.k = k
        self.lam = lam

        self.psi0 = None
        self.dpsi0 = None
        self.x = None
        self.psi = None
        self.dpsi = None
        self.stopped_early = None
        self.energy_error = None


    def initialize_system(self, psi0, dpsi0):

        self.psi0 = psi0
        self.dpsi0 = dpsi0


    def equations(self, x, y):

        psi, v = y

        dpsi_dx = v
        dv_dx = - self.k**2 * psi + self.lam * psi**3

        return [dpsi_dx, dv_dx]


    def integration(self, x_max, method, n_points):

        # funzione di stop locale: serve solo a questa integrazione,
        # non dipende da self (la soglia 100 e' fissa) quindi non serve self
        def stop_event(x, y):
            # solve_ivp cerca lo zero di questa funzione; si annulla quando psi^2 raggiunge 100.
            return y[0]**2 - 100.

        stop_event.terminal = True
        stop_event.direction = 1  # attiva solo quando psi^2 sta crescendo oltre 100

        solution = solve_ivp(fun = self.equations,
                             t_span = (0, x_max),
                             y0 = [self.psi0, self.dpsi0],
                             method = method,
                             t_eval = np.linspace(0, x_max, n_points),
                             events = stop_event,
                             rtol = 1e-9,
                             atol = 1e-12,
                            )

        self.x = solution.t                                  #sono tutti vettori al variare di x!
        self.psi = solution.y[0]
        self.dpsi = solution.y[1]
        self.stopped_early = len(solution.t_events[0]) > 0
        self.energy_error = self.compute_energy_error()

        return solution


    def compute_energy_error(self):

        E = 0.5 * self.dpsi**2 + 0.5 * self.k**2 * self.psi**2   # usa dpsi, psi GIA' calcolati
        E0 = E[0]
        self.energy_error = (E - E0) / E0                          # salva il risultato sull'oggetto
        return self.energy_error


    def visualize_results(self, title_suffix):

        fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(8, 6))

        ax1.plot(self.x, self.psi, color="tab:blue", lw=1.8)
        ax1.set_xlabel("x")
        ax1.set_ylabel(r"$\psi$")
        ax1.set_title(r"$\psi$ vs x" + title_suffix)
        ax1.grid(alpha=0.3)


        ax2.plot(self.x, self.dpsi, color="tab:blue", lw=1.8)
        ax2.set_xlabel("x")
        ax2.set_ylabel(r"$d\psi$")
        ax2.set_title(r"$d\psi$ vs x" + title_suffix)
        ax2.grid(alpha=0.3)


        ax3.plot(self.psi, self.dpsi, color="tab:blue", lw=1.8)
        ax3.set_xlabel(r"$\psi$")
        ax3.set_ylabel(r"$d\psi/dx$")
        ax3.set_title(r"$d\psi/dx$ vs $\psi$" + title_suffix)
        ax3.grid(alpha=0.3)

        fig.tight_layout()

        filename = f"simulation_lam{self.lam}_psi0{self.psi0}_dpsi0{self.dpsi0}.png"
        plt.savefig(filename, dpi = 150)
        plt.show()

        return fig


# ======================================================================
# Funzioni esterne alla classe: operano su piu' istanze di QuantumSystem,
# non sullo stato di un singolo oggetto -> non appartengono alla classe.
# ======================================================================

def is_stable(psi0_test, lam=0.05, dpsi0=0.1, x_max=50.0, method="DOP853", n_points=500):
    """Ritorna True se il sistema resta 'legato' (non supera |psi|^2 > 100
    entro x_max), False se diverge (stato aperto/instabile)."""

    system = QuantumSystem(lam)
    system.initialize_system(psi0_test, dpsi0)
    system.integration(x_max, method, n_points)

    # stopped_early=True -> e' diventato instabile -> not True = False -> non stabile
    # stopped_early=False -> non ha mai superato la soglia -> not False = True -> stabile
    return not system.stopped_early


def find_critical_psi0(lam=0.05, dpsi0=0.1, x_max=50.0, lo_start=0.1, hi_start=10.0, tolerance=1e-8, max_iterations=100):
    """Cerca per bisezione il valore critico di psi(0) che separa
    il comportamento stabile da quello instabile.

    Ritorna una tupla (psi0_critical, lo, hi):
    - psi0_critical: stima del valore di soglia
    - lo: ultimo valore testato risultato STABILE
    - hi: ultimo valore testato risultato INSTABILE
    """

    lo = lo_start
    hi = hi_start

    # Verifico (non assumo) che lo sia davvero stabile
    while not is_stable(lo, lam, dpsi0, x_max):
        lo = lo / 2

    # Verifico (non assumo) che hi sia davvero instabile
    while is_stable(hi, lam, dpsi0, x_max):
        hi = hi * 2

    # Bisezione vera e propria, con criterio di convergenza esplicito
    iteration = 0
    while (hi - lo) > tolerance and iteration < max_iterations:
        mid = 0.5 * (lo + hi)
        if is_stable(mid, lam, dpsi0, x_max):
            lo = mid   # mid stabile -> la soglia sta oltre -> restringo da sinistra
        else:
            hi = mid   # mid instabile -> la soglia sta prima -> restringo da destra
        iteration += 1

    psi0_critical = 0.5 * (lo + hi)

    print(f"Valore critico stimato di psi(0): {psi0_critical:.8f}")
    print(f"Raggiunto in {iteration} iterazioni, intervallo residuo: {hi - lo:.2e}")

    return psi0_critical, lo, hi


def plot_stability_boundary(lo, hi, psi0_critical):
    # Confronta le traiettorie psi(x) per i due valori di psi0 appena sotto e
    # appena sopra la soglia critica (lo=stabile, hi=instabile), mostrando su un
    # unico grafico come divergono rispetto alla soglia |psi|=10.

    fig, ax = plt.subplots(figsize=(7, 5))

    for psi0_test, colore in [(lo, "tab:blue"), (hi, "tab:red")]:

        system = QuantumSystem(0.05)
        system.initialize_system(psi0_test, 0.1)
        system.integration(30.0, "DOP853", 1500)

        stato = "stabile" if not system.stopped_early else "instabile"
        ax.plot(system.x, system.psi, color=colore, lw=1.5, label=f"psi(0) = {psi0_test:.4f} ({stato})")

    ax.axhline(10, color="gray", ls="--", lw=1, label=r"soglia $|\psi|=10$")
    ax.axhline(-10, color="gray", ls="--", lw=1)
    ax.set_xlabel("x")
    ax.set_ylabel(r"$\psi(x)$")
    ax.set_title(rf"Confine di stabilita': psi(0) critico $\approx$ {psi0_critical:.4f}")
    ax.legend()
    ax.grid(alpha=0.3)
    
    fig.tight_layout()

    filename = rf"critical_boundary_lam0.05_lo{lo:.4f}_hi{hi:.4f}.png"
    plt.savefig(filename, dpi=150, bbox_inches="tight")
    plt.show()


def plot_energy_conservation():

    methods = ["RK45", "RK23", "DOP853", "Radau", "BDF", "LSODA"]

    fig, ax = plt.subplots(figsize=(8, 5.5))

    for method in methods:
        # Creo un sistema con lambda=0: in questo caso l'equazione e' quella
        # dell'oscillatore armonico semplice, per cui l'energia DEVE conservarsi
        system = QuantumSystem(0.0)
        system.initialize_system(1.0, 0.1)
        system.integration(50.0, method, 2000)

        ax.plot(system.x, system.energy_error, lw=1.5, label=method)

        max_err = np.max(np.abs(system.energy_error))
        print(f"{method:8s}: errore massimo |dE/E0| = {max_err:.3e}")

    ax.set_yscale("symlog", linthresh=1e-12)
    ax.set_xlabel("x")
    ax.set_ylabel(r"$\Delta E / E_0$")
    ax.set_title("Errore relativo di energia (oscillatore armonico, lambda = 0)")
    ax.legend()
    ax.grid(alpha=0.3)
    
    fig.tight_layout()

    filename = "energy_conservation_comparison.png"
    plt.savefig(filename, dpi=150, bbox_inches="tight")
    plt.show()


# ======================================================================
# main
# ======================================================================

def main():

    # --- Simulazione principale: lambda=0.05, psi(0)=1.0, psi'(0)=0.1 ---
    print("=== Simulazione principale: lambda = 0.05, psi(0) = 1.0, psi'(0) = 0.1 ===")

    qs = QuantumSystem(0.05)
    qs.initialize_system(1.0, 0.1)
    qs.integration(50.0, "DOP853", 2000)

    print(f"Punti calcolati: {len(qs.x)}")
    print(f"Arresto anticipato per |psi|^2 > 100: {qs.stopped_early}")
    if qs.stopped_early:
        print(f"  -> avvenuto a x = {qs.x[-1]:.4f}")
    qs.visualize_results(rf" ($\lambda={qs.lam}$, $\psi(0)={qs.psi0}$, $\psi'(0)={qs.dpsi0}$)")

    # --- Ricerca del valore critico di psi(0) per bisezione ---
    psi0_critical, lo, hi = find_critical_psi0()
    plot_stability_boundary(lo, hi, psi0_critical)

    # --- Controllo di conservazione dell'energia: lambda = 0, vari integratori ---
    plot_energy_conservation()


if __name__ == "__main__":
    main()
