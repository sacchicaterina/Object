import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import solve_ivp


#du = v
#dv = -u + 3Mu^2


class BlackHole :
# Costanti fisiche in unita' SI, usate solo nella parte di evaporazione
# (dove non lavoriamo in unita' geometriche perche' i tempi/masse fisiche
# sono quelli di interesse).
    G_SI = 6.67430e-11        # m^3 kg^-1 s^-2
    C_SI = 2.99792458e8       # m/s
    HBAR_SI = 1.054571817e-34 # J s
    KB_SI = 1.380649e-23      # J/K
    M_SUN = 1.98892e30        # kg

    def __init__(self, M, label):    #riferito al blackhole
        """
        M: il numero che l'utente passa, usato direttamente per il fotone
           (es. M=1 vuol dire massa = 1 in unita' geometriche, M=40 vuol
           dire massa = 40 in unita' geometriche: qualunque numero scrivi,
           viene usato cosi' com'e' in equations()).

        Per l'evaporazione serve invece la massa fisica in kg: quello
        stesso numero M, se interpretato come "masse solari", va
        moltiplicato per M_SUN per ottenere i kg veri, usati nei
        metodi di evaporazione. Questa conversione e' fatta qui, una
        volta sola, e salvata in self.M_kg.
        """
        self.label = label
        self.M = M                      # usato direttamente da photon_equations() (fotone)
        self.M_kg = M * self.M_SUN      # M interpretato come masse solari, convertito in kg

        self.photon_result = None
        self.evaporation_result = None

    @property   #utilizzare un metodo come attributo
    def Schwarschild_radius_geometric(self):
        return 2 * self.M

    @property
    def Schwarschild_radius(self):
        """Rs = 2 G M / c^2, in metri, usando la massa fisica in kg (self.M_kg)."""
        return 2 * self.G_SI * self.M_kg / self.C_SI**2

    def photon_equations(self, phi, y):
        u, v = y
        du_dphi = v
        dv_dphi = -u + 3 * self.M * u**2
        return [du_dphi, dv_dphi]

    def photon_integration(self, r0, du_dphi0, phi_max, method, n_points):
        u0 = 1. / r0

        # ------------------------------------------------------------- #
        # Le due sentinelle sotto NON distinguono due "tipi" di fotone:
        # per ogni singolo fotone lanciato puo' succedere una delle due
        # cose (mai entrambe) - o cade dentro (cattura), o scappa per
        # sempre (fuga). Sono passate insieme a solve_ivp, che si ferma
        # al primo dei due eventi che si verifica per quella traiettoria.
        # ------------------------------------------------------------- #

        # EVENTO 1: cattura del fotone (r scende fino all'orizzonte Rs)
        #
        # u = 1/r, quindi r = Rs equivale a u = 1/Rs. Questa funzione
        # vale zero esattamente in quell'istante; solve_ivp interpola
        # per trovare il phi preciso in cui succede e ferma li' la
        # traiettoria (terminal=True), invece di continuare a integrare
        # dentro l'orizzonte, dove il fotone e' comunque perso per sempre
        # e la traiettoria non avrebbe piu' significato fisico.
        def event_capture(phi, y):
            return y[0] - 1. / self.Schwarschild_radius_geometric
        event_capture.terminal = True
        # direction = +1: scatta solo quando l'espressione CRESCE
        # attraverso lo zero, cioe' quando u sta aumentando verso 1/Rs.
        # u che cresce significa r = 1/u che DIMINUISCE: e' esattamente
        # il caso "il fotone si sta avvicinando e sta per cadere dentro".
        # Senza specificare direction, l'evento scatterebbe anche nel
        # verso opposto (u che diminuisce attraverso 1/Rs, come un
        # fotone che uscisse da dietro l'orizzonte - fisicamente
        # impossibile, ma il segno da solo non lo escluderebbe).
        event_capture.direction = +1

        # EVENTO 2: fuga del fotone all'infinito (r -> infinito, u -> 0)
        #
        # Qui restituisco direttamente y[0] = u: la funzione vale zero
        # quando u = 0, cioe' quando r = 1/u tende a infinito. Non serve
        # sottrarre nulla, a differenza dell'evento sopra, perche' il
        # bersaglio da individuare (u = 0) e' gia' il valore naturale
        # della funzione stessa.
        def event_escape(phi, y):
            return y[0]
        event_escape.terminal = True
        # direction = -1: scatta solo quando u sta DIMINUENDO attraverso
        # lo zero (fotone che si allontana sempre di piu', mai il
        # contrario). Necessario perche', mano a mano che il fotone si
        # allontana, u si avvicina a zero e per errori di arrotondamento
        # numerico potrebbe diventare leggermente negativo: un valore
        # privo di senso fisico (corrisponderebbe a un raggio r
        # negativo). Fermare l'integrazione appena u tocca lo zero
        # garantisce che self.photon_result["r"] resti sempre un numero
        # positivo e fisicamente valido su tutta la traiettoria.
        event_escape.direction = -1

        solution = solve_ivp(fun=self.photon_equations,
                             t_span=(0, phi_max),
                             y0=[u0, du_dphi0],
                             method=method,
                             t_eval=np.linspace(0, phi_max, n_points),
                             events=(event_capture, event_escape),
                             rtol=1e-9,
                             atol=1e-12,
                             max_step=phi_max / 2000
                             )

        self.photon_result = {
            "phi": solution.t,
            "u": solution.y[0],
            "r": 1. / solution.y[0],
            "du_dphi": solution.y[1],
            "captured": len(solution.t_events[0]) > 0,   # evento 1 scattato
            "escaped": len(solution.t_events[1]) > 0,     # evento 2 scattato
        }

        # ================================================================= #
        # SEZIONE DIMOSTRATIVA (commentata): come estrarre t_events e       #
        # y_events da un oggetto solve_ivp quando sono definiti PIU' eventi #
        # ================================================================= #
        #
        # Quando si passano piu' funzioni evento a solve_ivp tramite la
        # tupla events=(event_capture, event_escape), l'oggetto restituito
        # (qui chiamato "solution") contiene DUE liste parallele, una per
        # ciascun evento, nello stesso ordine in cui gli eventi sono stati
        # passati:
        #
        #   solution.t_events -> lista lunga quanto gli eventi passati
        #       solution.t_events[0]  -> array dei phi in cui e' scattato
        #                                 event_capture (cattura)
        #       solution.t_events[1]  -> array dei phi in cui e' scattato
        #                                 event_escape (fuga)
        #
        #   solution.y_events -> stessa struttura, ma con lo STATO
        #                         completo [u, du/dphi] in quell'istante
        #       solution.y_events[0]  -> array (n_eventi_capture x 2)
        #       solution.y_events[1]  -> array (n_eventi_escape x 2)
        #
        # NOTA: con terminal=True su entrambi gli eventi, l'integrazione si
        # ferma al primo che scatta, quindi in pratica UNA delle due liste
        # sara' vuota (array vuoto, shape (0,) o (0,2)) e l'altra conterra'
        # al massimo un elemento. Il codice sotto e' comunque scritto in
        # modo generale (funziona anche se in futuro si rimuove terminal
        # o si aggiungono altri eventi non terminali).
        #
        # --- Accesso diretto ai due eventi definiti in questo metodo ---
        #
        # t_capture = solution.t_events[0]   # array dei phi di cattura (vuoto se non catturato)
        # y_capture = solution.y_events[0]   # array [[u, du/dphi], ...] di cattura
        #
        # t_escape = solution.t_events[1]    # array dei phi di fuga (vuoto se non scappato)
        # y_escape = solution.y_events[1]    # array [[u, du/dphi], ...] di fuga
        #
        # --- Tempo (phi) ed evento preciso scattato per QUESTA traiettoria ---
        #
        # Dato che event_capture ed event_escape sono entrambi terminal=True,
        # per una singola chiamata a photon_integration scatta SEMPRE al
        # massimo uno dei due (mai entrambi: l'integrazione si ferma al
        # primo). Quindi per sapere qual e' e a quale phi e' successo, basta
        # controllare quale dei due array t_events non e' vuoto:
        #
        # Lettura dei tre indici di y_events[i_evento][i_occorrenza][i_componente]:
        #   i_evento      -> QUALE evento (0 = capture, 1 = escape)
        #   i_occorrenza  -> QUALE VOLTA e' scattato quell'evento (qui sempre
        #                    0, perche' terminal=True impedisce che scatti
        #                    piu' di una volta nella stessa traiettoria; NON
        #                    e' uno spacchettamento, e' comunque la selezione
        #                    della riga - la prima e unica occorrenza)
        #   i_componente  -> QUALE variabile dello stato in quella riga
        #                    (0 = u, 1 = du/dphi): questo e' il vero
        #                    spacchettamento dello stato [u, du/dphi]
        #
        # if len(solution.t_events[0]) > 0:
        #     evento_scattato = "capture"
        #     phi_evento = solution.t_events[0][0]         # phi preciso dell'evento
        #     u_evento = solution.y_events[0][0][0]        # componente u dello stato
        #     dudphi_evento = solution.y_events[0][0][1]   # componente du/dphi dello stato
        # elif len(solution.t_events[1]) > 0:
        #     evento_scattato = "escape"
        #     phi_evento = solution.t_events[1][0]         # phi preciso dell'evento
        #     u_evento = solution.y_events[1][0][0]        # componente u dello stato
        #     dudphi_evento = solution.y_events[1][0][1]   # componente du/dphi dello stato
        # else:
        #     evento_scattato = None                       # ne' cattura ne' fuga entro phi_max
        #     phi_evento = None
        #
        # print(f"Evento scattato: {evento_scattato}, a phi = {phi_evento}")
        #
        # ================================================================= #

        return solution
    

    def photon_trajectory(self):
        
        fig, ax = plt.subplots(subplot_kw={'projection': 'polar'}, figsize=(8, 6))
        
        ax.plot(self.photon_result["phi"], self.photon_result["r"], color="tab:blue", lw=1.8, label="photon path")
        
        theta = np.linspace(0, 2 * np.pi, 200)
        r_circle = np.full_like(theta, self.Schwarschild_radius_geometric)
        ax.fill(theta, r_circle, color="black", label="event horizon")
        
        ax.set_rmax(4 * np.min(self.photon_result["r"]))  #zoom
        
        status = "captured" if self.photon_result["captured"] else "escaped"
        r0 = self.photon_result["r"][0]
        du_dphi0 = self.photon_result["du_dphi"][0]
        
        ax.set_title(f"Trajectory of {status} photon with M = {self.M}, r_0 = {r0}, du/dphi_0 = {du_dphi0}")
        ax.set_xlabel(r"$\phi$")
        ax.set_ylabel("r")
        ax.legend(loc="upper right", bbox_to_anchor=(1.25, 1.1))
        fig.tight_layout()
        
        filename = f"photon_trajectory_M{self.M}_r0{r0}_dudphi0{du_dphi0}.png"
        fig.savefig(filename, dpi = 150)
        plt.show()
        
        return fig

    def find_photon_sphere(self, r0=50.0, phi_max=20*np.pi, du_lo=0.15, du_hi=0.25, tolerance=1e-10, max_iterations=100):
        """Trova il raggio della sfera dei fotoni con bisezione scritta a mano,
        senza usare scipy.optimize.brentq: stesso principio, ma il ciclo
        di dimezzamento dell'intervallo lo scriviamo esplicitamente noi.

        du_lo deve dare FUGA, du_hi deve dare CATTURA: se non lo sono
        gia', vengono corretti automaticamente prima di iniziare la
        bisezione vera e propria.
        """
        lo = du_lo    #fuga
        hi = du_hi    #cattura

        # Se lo non desse fuga, lo dimezzo finche' non ci riesce.
        self.photon_integration(r0, lo, phi_max, "DOP853", 2000)
        while not self.photon_result["escaped"]:
            lo = lo / 2
            self.photon_integration(r0, lo, phi_max, "DOP853", 2000)

        # Se hi non desse cattura, lo raddoppio finche' non ci riesce.
        self.photon_integration(r0, hi, phi_max, "DOP853", 2000)
        while not self.photon_result["captured"]:
            hi = hi * 2
            self.photon_integration(r0, hi, phi_max, "DOP853", 2000)

        iteration = 0
        while (hi - lo) > tolerance and iteration < max_iterations:
            mid = 0.5 * (lo + hi)
            self.photon_integration(r0, mid, phi_max, "DOP853", 2000)
            if self.photon_result["captured"]:
                hi = mid   # mid catturato -> il confine sta prima -> restringo da destra
            else:
                lo = mid   # mid scappato -> il confine sta oltre -> restringo da sinistra
            iteration += 1

        du_critical = 0.5 * (lo + hi)
        # NOTA: uso lo (ultimo valore SICURAMENTE in fuga), non du_critical
        # o hi: la traiettoria a lo e' quella che si avvicina di piu' al
        # buco nero SENZA cadere dentro, quindi il suo minimo di r e' la
        # stima piu' vicina al vero raggio della sfera dei fotoni. Usare
        # invece un valore anche solo leggermente catturato farebbe
        # cadere il fotone fin dentro l'orizzonte (r_min~2M), sbagliando
        # completamente la stima.
        self.photon_integration(r0, lo, phi_max, "DOP853", 3000)
        # punto di massimo avvicinamento al buco nero lungo questa traiettoria: stima del raggio della sfera dei fotoni
        return float(np.min(self.photon_result["r"]))

    def evaporation_equations(self, t, y):
        M = y[0]
        dM_dt = -self.HBAR_SI * self.C_SI**4 / (15360 * np.pi * self.G_SI**2 * M**2)
        return [dM_dt]

    def evaporation_integration(self, method, m_stop_fraction=1e-4):
        # Stima ANALITICA (non numerica) del tempo di vita del buco nero,
        # ottenuta integrando a mano dM/dt = -k/M^2 (separazione di variabili):
        #   M^2 dM = -k dt
        #   [M^3/3] da M0 a 0 = -k * t_life
        #   t_life = M0^3 / (3*k),  con k = hbar*c^4 / (15360*pi*G^2)
        # Usata per impostare t_max, senza doverlo far scegliere all'utente:
        # non e' una scelta fisica interessante, e sbagliarla (es. troppo
        # piccola) farebbe fermare l'integrazione prima che l'evaporazione
        # sia completa, senza nessun errore evidente.
        k = self.HBAR_SI * self.C_SI**4 / (15360 * np.pi * self.G_SI**2)
        t_life_estimate = self.M_kg**3 / (3 * k)
        t_max = 1.5 * t_life_estimate

        m_stop = self.M_kg * m_stop_fraction

        def mass_depleted(t, y):
            return y[0] - m_stop
        mass_depleted.terminal = True
        mass_depleted.direction = -1

        # NIENTE t_eval: la massa scende lentissimamente per quasi tutto
        # il tempo di vita, poi crolla rapidamente negli ultimi istanti.
        # Un t_eval scelto a priori (equispaziato o logaritmico) rischia
        # di "mancare" quel crollo finale. Lasciando che solve_ivp scelga
        # da solo i suoi passi (adattivi), li infittisce automaticamente
        # proprio dove il fenomeno cambia piu' velocemente.
        #
        # NIENTE dense_output per lo stesso motivo: dense_output=True
        # servirebbe solo se dopo volessi ricampionare la soluzione con
        # solution.sol(t) su una griglia scelta a priori (es. log-uniforme).
        # Ma qui e' proprio l'opposto di quello che serve al grafico: i
        # passi adattivi del solver sono GIA' piu' fitti esattamente dove
        # M(t) crolla velocemente (fine vita), quindi ricampionare su una
        # griglia uniforme non aggiungerebbe dettaglio dove serve, anzi
        # rischierebbe di "lisciare via" quella densita' informativa -
        # oltre a costare il calcolo extra dei polinomi di interpolazione
        # ad ogni step, per un beneficio nullo nel nostro caso. Per questo
        # uso direttamente solution.t e solution.y nei plot, senza sol().
        solution = solve_ivp(fun=self.evaporation_equations,
                             t_span=(0, t_max),
                             y0=[self.M_kg],
                             method=method,
                             events=mass_depleted,
                             rtol=1e-9,
                             atol=1e-12
                             )

        # ================================================================= #
        # SEZIONE DIMOSTRATIVA (commentata): estrazione di t_events/y_events #
        # nel caso di UN SOLO evento (mass_depleted), passato come funzione  #
        # singola e non come tupla.                                          #
        # ================================================================= #
        #
        # Anche passando un solo evento (non una tupla), solve_ivp
        # restituisce comunque LISTE (di lunghezza 1):
        #
        #   solution.t_events -> [array_dei_t_in_cui_e'_scattato_l'evento]
        #   solution.y_events -> [array_degli_stati_[M]_in_quei_t]
        #
        # quindi per accedere ai valori bisogna comunque indicizzare [0]:
        #
        # t_mass_depleted = solution.t_events[0]   # array dei tempi (vuoto se mai raggiunto m_stop)
        # y_mass_depleted = solution.y_events[0]    # array [[M], ...] in quei tempi
        #
        # if len(t_mass_depleted) > 0:
        #     t_finale = t_mass_depleted[0]         # istante (scalare) in cui M = m_stop
        #     M_finale = y_mass_depleted[0][0]      # massa (scalare) in quell'istante
        #     print(f"Massa scesa a m_stop dopo t = {t_finale:.3e} s, M = {M_finale:.3e} kg")
        # else:
        #     print("L'evento mass_depleted non e' mai scattato entro t_max.")
        #
        # Nota sugli indici, per coerenza con photon_integration: qui c'e'
        # UN SOLO evento e UNA SOLA variabile di stato (M), quindi partendo
        # direttamente da solution (senza passare per la variabile
        # intermedia y_mass_depleted) servono TRE indici, non due:
        #
        #   solution.y_events[0][0][0]
        #                     |  |  |
        #                     |  |  +-- i_componente: quale variabile dello
        #                     |  |      stato (0 = M, l'unica che c'e')
        #                     |  +----- i_occorrenza: quale volta e' scattato
        #                     |         l'evento (sempre 0, terminal=True)
        #                     +-------- i_evento: quale evento (sempre 0,
        #                               qui ce n'e' uno solo: mass_depleted)
        #
        # M_finale = solution.y_events[0][0][0]   # equivalente a y_mass_depleted[0][0]
        #
        # --- Perche' [0][0] e [0][0][0] coincidono numericamente qui ---
        #
        # y_mass_depleted[0][0] e solution.y_events[0][0][0] danno LO STESSO
        # numero, perche' y_mass_depleted e' gia' solution.y_events[0]: il
        # primo indice e' stato "consumato" quando si e' definita la
        # variabile intermedia. Non e' una coincidenza, e' la stessa
        # posizione raggiunta partendo da due punti diversi:
        #
        #   shape a ogni livello (evaporazione: 1 sola variabile di stato M)
        #   -------------------------------------------------------------
        #   solution.y_events[0]        -> shape (n_occorrenze, 1)   es. [[M_val]]
        #   solution.y_events[0][0]     -> shape (1,)                es. [M_val]    (array di 1 elemento, non scalare)
        #   solution.y_events[0][0][0]  -> scalare                   es. M_val
        #
        # Confronto con il fotone (2 variabili di stato: u, du/dphi), dove
        # invece [0][0] NON basta per arrivare a uno scalare:
        #
        #   solution.y_events[0]        -> shape (n_occorrenze, 2)   es. [[u_val, dudphi_val]]
        #   solution.y_events[0][0]     -> shape (2,)                es. [u_val, dudphi_val]  (coppia, non scalare!)
        #   solution.y_events[0][0][0]  -> scalare                   es. u_val (solo la componente u)
        #
        # La differenza e' quante variabili di stato ha il sistema: con 1
        # sola variabile, fermarsi al secondo indice da' gia' uno scalare
        # "nascosto" in un array di lunghezza 1; con 2 variabili, il
        # secondo indice da' ancora una coppia, e serve il terzo per
        # separare le due componenti.
        #
        # ================================================================= #

        # temperatura di Hawking calcolata UNA VOLTA SOLA, sull'intero
        # array di M gia' completo (numpy applica la formula elemento
        # per elemento automaticamente)
        T_Hawking = self.HBAR_SI * self.C_SI**3 / (8 * np.pi * self.G_SI * solution.y[0] * self.KB_SI)

        self.evaporation_result = {
            "t": solution.t,
            "M": solution.y[0],
            "T": T_Hawking,
            "depleted": len(solution.t_events[0]) > 0,
        }

        return solution

    def evaporation_plots(self):
        
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(8, 6))

        lifetime = self.evaporation_result["t"][-1]

        ax1.plot(self.evaporation_result["t"][1:], self.evaporation_result["M"][1:], color="tab:blue", lw=1.8)
        ax1.set_title(self.label + ": mass vs time")
        ax1.set_xscale("log")
        ax1.set_yscale("log")
        ax1.set_xlabel("time [s]")
        ax1.set_ylabel(r"mass [$M_\odot$]")
        ax1.grid(True, which="both", alpha=0.3)

        ax2.plot(self.evaporation_result["t"][1:], self.evaporation_result["T"][1:], color="tab:green", lw=1.8)
        ax2.set_title(self.label + ": temperature vs time")
        ax2.set_xscale("log")
        ax2.set_yscale("log")
        ax2.set_xlabel("time [s]")
        ax2.set_ylabel(r"temperature [K]")
        ax2.grid(True, which="both", alpha=0.3)

        fig.suptitle(rf"{self.label} (M = {self.M} $M_\odot$): lifetime $\approx$ {lifetime:.3e} s")
        fig.tight_layout()
        
        filename = f"evaporation_{self.label}_M{self.M}.png"
        fig.savefig(filename, dpi = 150)
        plt.show()
        
        return fig


def main():

    orbiting_photon = BlackHole(M=1, label="M = 1 black hole")
    orbiting_photon.photon_integration(r0=50.0, du_dphi0=0.191, phi_max=20*np.pi, method="DOP853", n_points=3000)
    orbiting_photon.photon_trajectory()
    r_photon_sphere = orbiting_photon.find_photon_sphere()
    print(f"Estimated photon sphere radius: {r_photon_sphere:.3f} M (theoretical value: 3 M)")

    black_holes = [
        BlackHole(M=10, label="stellare"),
        BlackHole(M=1e-18, label="asteroide"),
        BlackHole(M=1e-25, label="micro"),
    ]

    for bh in black_holes:
        bh.evaporation_integration(method="DOP853")
        bh.evaporation_plots()
        print(f"{bh.label}: depleted={bh.evaporation_result['depleted']}  lifetime={bh.evaporation_result['t'][-1]:.3e} s")


if __name__ == "__main__":
    main()
