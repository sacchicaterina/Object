import numpy as np
import matplotlib.pyplot as plt
import random
import time
from scipy.optimize import curve_fit
from scipy.stats import norm


class Die:
    """Represents a single six-faced CLR die.

    The die has six faces: one C, one L, one R and three dots (blanks).
    Rolling the die returns one of these faces at random with equal
    probability, using the built-in `random` module (faster than
    `numpy.random` for single-value draws).
    """

    def __init__(self, faces=("C", "L", "R", ".", ".", ".")):
        """Store the tuple of face values for this die.

        Parameters
        ----------
        faces : tuple of str
            The six face values of the die. Defaults to the standard CLR
            die: one C, one L, one R and three blank ('.') faces.
        """
        self.faces = tuple(faces)

    def roll(self):
        """Return the result of a single fair roll of the die.

        Returns
        -------
        str
            One of the face values stored in `self.faces`, chosen with
            uniform probability.
        """
        return random.choice(self.faces)


# --- Alternativa: dado con facce numerate 1-6 e mapping esplicito a simboli CLR ---
# Questa versione rappresenta un dado "fisico" con facce numerate, più una
# mappa separata numero -> simbolo. Comportamentalmente equivalente alla
# versione sopra (roll() restituisce comunque "C"/"L"/"R"/"."), solo con un
# passaggio di lookup in più.
#
# class Die:
#     FACE_SYMBOLS = {1: "C", 2: "L", 3: "R", 4: ".", 5: ".", 6: "."}
#
#     def __init__(self, faces=(1, 2, 3, 4, 5, 6)):
#         self.faces = tuple(faces)
#
#     def roll(self):
#         face_number = random.choice(self.faces)
#         return self.FACE_SYMBOLS[face_number]
#
#     def __str__(self):
#         return f"Die with faces {self.faces} -> {self.FACE_SYMBOLS}"


class CLRGame:
    """Simulates a single game of CLR with n players and c starting coins."""

    def __init__(self, n_players, c_coins):
        """Set up a new CLR game.

        Parameters
        ----------
        n_players : int
            Number of players in the game (must be > 2).
        c_coins : int
            Number of coins each player starts with (must be > 0).
        """
        if n_players <= 2:
            raise ValueError("n_players must be greater than 2")
        if c_coins <= 0:
            raise ValueError("c_coins must be greater than 0")

        self.n_players = n_players
        self.c_coins = c_coins
        self.die = Die()
        self.player_coins = np.full(n_players, c_coins, dtype=int)   #n_players celle con c_coins ognuno
        self.n_rounds = 0

    def __str__(self):
        """Return a human-readable snapshot of the current game state."""
        return (f"CLRGame(n_players={self.n_players}, c_coins={self.c_coins}, "
                f"round={self.n_rounds}, coins={self.player_coins.tolist()})")

    def has_winner(self):
        """Check whether the game has reached a winning state.

        Returns
        -------
        bool
            True if at most one player still holds coins, False otherwise.
        """
        #conta quanti elementi sono diversi da zero
        return np.count_nonzero(self.player_coins) <= 1

    def roll_dice_for_player(self, player):
        """Roll the appropriate number of dice for a given player.

        Parameters
        ----------
        player : int
            Index of the player who is rolling.

        Returns
        -------
        list of str
            The outcomes of the dice rolled (one die per coin held, up to
            a maximum of 3 dice).
        """
        n_dice = min(self.player_coins[player], 3)
        return [self.die.roll() for _ in range(n_dice)]    #lista outcomes 

    def process_roll(self, player, outcomes):
        """Apply the effects of a set of dice outcomes for a player.

        Parameters
        ----------
        player : int
            Index of the player who rolled.
        outcomes : list of str
            The face results ('C', 'L', 'R' or '.') to process.
        """
        left = (player - 1) % self.n_players
        right = (player + 1) % self.n_players

        #uso count poichè ho a che fare con le liste python
        n_left = outcomes.count("L")     #numero monete left
        n_right = outcomes.count("R")    #numero monete right
        n_center = outcomes.count("C")   #numero monete centro 

        # --- Alternativa equivalente con numpy, solo a scopo di confronto ---
        # outcomes_arr = np.array(outcomes)          # es. array(['L', 'C', 'L'])
        #
        # outcomes_arr == "L"                        # array([True, False, True])
        # outcomes_arr == "R"                        # array([False, False, False])
        # outcomes_arr == "C"                        # array([False, True, False])
        #
        # np.count_nonzero conta quanti True (cioè "diversi da zero") ci sono
        # in ciascuno di questi array booleani intermedi:
        # n_left = np.count_nonzero(outcomes_arr == "L")     # -> 2
        # n_right = np.count_nonzero(outcomes_arr == "R")    # -> 0
        # n_center = np.count_nonzero(outcomes_arr == "C")   # -> 1
        #
        # (richiede prima la conversione a array numpy; per liste piccole come
        # questa, .count() sulla lista Python resta più diretto e leggero)

        self.player_coins[player] -= (n_left + n_right + n_center)
        self.player_coins[left] += n_left
        self.player_coins[right] += n_right

    def play_round(self):
        """Play a single round: every player with coins rolls once."""
        for player in range(self.n_players):
            if self.player_coins[player] > 0:
                outcomes = self.roll_dice_for_player(player)
                self.process_roll(player, outcomes)

    def play_game(self):
        """Play the full game until a single winner remains.

        Returns
        -------
        int
            The total number of rounds played.
        """
        while not self.has_winner():
            self.play_round()
            self.n_rounds += 1
        return self.n_rounds


def time_single_game(n_players, c_coins):
    """Play one game and report how long it took, for a quick sanity check.

    Parameters
    ----------
    n_players : int
        Number of players in the game.
    c_coins : int
        Number of coins each player starts with.

    Returns
    -------
    CLRGame
        The finished game instance (useful for printing its final state).
    """
    clr = CLRGame(n_players, c_coins)
    t0 = time.time()
    clr.play_game()
    t1 = time.time()
    print(f"Single game runtime: {(t1 - t0) * 1e3:.3f} ms")
    print(clr)
    return clr


def simulate_games(n_players, c_coins, n_games):
    """Simulate many independent CLR games and collect their durations.

    Parameters
    ----------
    n_players : int
        Number of players in each game.
    c_coins : int
        Number of coins each player starts with.
    n_games : int
        Number of independent games to simulate.

    Returns
    -------
    numpy.ndarray
        Array of length `n_games` with the number of rounds each game took.
    """
    n_rounds_history = np.zeros(n_games, dtype=int)

    t0 = time.time()
    for game in range(n_games):
        clr = CLRGame(n_players, c_coins)
        n_rounds_history[game] = clr.play_game()
    t1 = time.time()

    print(f"{n_games} partite in {(t1 - t0):.1f} secondi")
    print(f"Media: {n_rounds_history.mean():.3f}, Std: {n_rounds_history.std():.3f}")

    return n_rounds_history


def fit_gaussian_to_rounds(n_rounds_history):
    """Build a density histogram of game durations and fit it with a Gaussian.

    Parameters
    ----------
    n_rounds_history : numpy.ndarray
        Number of rounds taken by each simulated game.

    Returns
    -------
    tuple
        (mean_fit, mean_err, sigma_fit, sigma_err): the fitted mean and
        standard deviation, together with their uncertainties from the
        fit's covariance matrix.
    """
    # Un bin per ogni valore intero possibile di round tra min e max osservati.
    #
    # Il +1 qui NON riguarda gli edges dell'istogramma (quelli sono sempre
    # n_bins + 1, gestiti automaticamente da np.histogram). Riguarda invece
    # quanti bin voglio: sto semplicemente contando quanti interi ci sono
    # in un intervallo chiuso [min, max].
    #
    # Esempio: se min=20 e max=22, i round possibili sono 20, 21, 22 -> 3 valori.
    # Ma max - min = 22 - 20 = 2, quindi serve +1 per ottenere il conto giusto (3).
    # Senza il +1 l'ultimo valore intero (max) non avrebbe un bin dedicato tutto
    # suo e finirebbe schiacciato insieme al penultimo.
    n_bins = int(n_rounds_history.max() - n_rounds_history.min()) + 1
    counts, bin_edges = np.histogram(n_rounds_history, bins=n_bins, density=True)
    bin_centers = 0.5 * (bin_edges[:-1] + bin_edges[1:])

    # Fittiamo direttamente norm.pdf (la densità di probabilità gaussiana),
    # senza un parametro di ampiezza libero. Questo è corretto proprio
    # perché stiamo fittando una DENSITÀ (density=True nell'istogramma):
    # una gaussiana, in quanto distribuzione di probabilità, ha per
    # definizione area unitaria sotto la curva (integra sempre a 1).
    # Se avessimo dati non normalizzati (es. conteggi grezzi invece di
    # densità), un parametro di ampiezza libero sarebbe stato necessario
    # per assorbire la scala dei dati. Qui invece i dati sono già una
    # densità, quindi imporre ampiezza = 1 è la scelta fisicamente corretta,
    # non solo una semplificazione tecnica.
    p0 = (n_rounds_history.mean(), n_rounds_history.std())
    # curve_fit chiama norm.pdf(x, *params) ad ogni iterazione, passando
    # i parametri liberi come argomenti posizionali dopo x. Con p0 nell'ordine
    # (mean, std), il primo parametro libero viene passato come "loc" e il
    # secondo come "scale" (norm.pdf(x, loc, scale)) -- non li scriviamo
    # esplicitamente qui perché è curve_fit a fornirli automaticamente,
    # aggiornandoli ad ogni iterazione dell'ottimizzazione.
    popt, pcov = curve_fit(norm.pdf, bin_centers, counts, p0=p0)

    mean_fit, sigma_fit = popt
    mean_err, sigma_err = np.sqrt(np.diag(pcov))

    print(f"Fitted expectation value : {mean_fit:.3f} +/- {mean_err:.3f} rounds")
    print(f"Fitted standard deviation: {sigma_fit:.3f} +/- {sigma_err:.3f} rounds\n")

    return mean_fit, mean_err, sigma_fit, sigma_err


# --- Alternativa: gaussiana come semplice funzione a campana, NON come densità ---
#
# Se invece di fittare un istogramma a densità (density=True) avessimo
# dovuto fittare conteggi grezzi o dati non normalizzati, norm.pdf da sola
# non sarebbe bastata: servirebbe un parametro di ampiezza esplicito, dato
# che una gaussiana "pura" ha sempre area unitaria e non potrebbe adattarsi
# a dati con una scala diversa da 1.
#
# Formula matematica:
#   f(x) = amplitude * exp( -0.5 * ((x - mean) / sigma)^2 )
#
# Differenza rispetto a norm.pdf: qui manca il fattore di normalizzazione
# 1 / (sigma * sqrt(2*pi)) davanti all'esponenziale. Questo significa che
# il picco della curva, quando x = mean, vale esattamente "amplitude"
# (perché l'esponente si annulla ed exp(0) = 1) -- non più il valore di
# densità di norm.pdf (che al picco vale 1 / (sigma * sqrt(2*pi))).
#
# def gaussian_distribution(x, amplitude, mean, sigma):
#     return amplitude * np.exp(-0.5 * ((x - mean) / sigma) ** 2)
#
# In quel caso il fit sarebbe stato:
# p0 = (counts.max(), n_rounds_history.mean(), n_rounds_history.std())
# popt, pcov = curve_fit(gaussian_distribution, bin_centers, counts, p0=p0)
# amplitude_fit, mean_fit, sigma_fit = popt


def plot_results(n_rounds_history, mean_fit, mean_err, sigma_fit, sigma_err,
                  n_players, c_coins, n_games):
    """Plot the rounds-to-finish histogram together with the Gaussian fit.

    Parameters
    ----------
    n_rounds_history : numpy.ndarray
        Number of rounds taken by each simulated game.
    mean_fit, mean_err : float
        Fitted mean and its uncertainty.
    sigma_fit, sigma_err : float
        Fitted standard deviation and its uncertainty.
    n_players : int
        Number of players (used for labelling the plot).
    c_coins : int
        Number of starting coins (used for labelling the plot).
    n_games : int
        Number of simulated games (used for labelling the plot).
    """
    # Stesso criterio di fit_gaussian_to_rounds: un bin per ogni intero
    # possibile tra min e max osservati (vedi commento dettagliato lì).
    n_bins = int(n_rounds_history.max() - n_rounds_history.min()) + 1

    fig, ax = plt.subplots(figsize=(9, 6))

    ax.hist(n_rounds_history, bins=n_bins, density=True, color="steelblue",
            alpha=0.65, edgecolor="white", label=f"Simulated games (N={n_games:,})")

    x_fit = np.linspace(n_rounds_history.min(), n_rounds_history.max(), 500)
    y_fit = norm.pdf(x_fit, loc=mean_fit, scale=sigma_fit)
    ax.plot(x_fit, y_fit, color="crimson", lw=2.2, label="Gaussian fit")

    ax.axvline(mean_fit, color="black", ls="--", lw=1.5,
               label=fr"$\mu = {mean_fit:.2f} \pm {mean_err:.2f}$")
    ax.axvspan(mean_fit - sigma_fit, mean_fit + sigma_fit, color="crimson",
               alpha=0.12, label=fr"$\sigma = {sigma_fit:.2f} \pm {sigma_err:.2f}$")

    ax.set_xlabel("Number of rounds to end the game")
    ax.set_ylabel("Probability density")
    ax.set_title(f"CLR game duration -- $n={n_players}$ players, $c={c_coins}$ coins each")
    ax.legend()
    ax.grid(alpha=0.25)

    fig.tight_layout()
    fig.savefig(f"clr_rounds_distribution_n{n_players}_c{c_coins}_games{n_games}.png", dpi=150)
    plt.show()


def main():
    """Run the full exam workflow: demo, timing, simulation, fit and plot."""
    n_players, c_coins = 3, 20
    n_games = 100_000

    # --- Verifica velocità di una singola partita (deve stare ben sotto 100 ms) ---
    time_single_game(n_players, c_coins)

    # --- Simulazione di n_games partite ---
    n_rounds_history = simulate_games(n_players, c_coins, n_games)

    # --- Istogramma di densità + fit gaussiano ---
    mean_fit, mean_err, sigma_fit, sigma_err = fit_gaussian_to_rounds(n_rounds_history)

    # --- Plot: istogramma + fit sovrapposti, con media e banda 1-sigma ---
    plot_results(n_rounds_history, mean_fit, mean_err, sigma_fit, sigma_err,
                 n_players, c_coins, n_games)


if __name__ == "__main__":
    main()
