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


def gaussian_distribution(x, amplitude, mean, sigma):
    """Gaussian (normal) probability density function, up to a scale factor.

    Parameters
    ----------
    x : array_like
        Points at which to evaluate the function.
    amplitude : float
        Peak height of the curve.
    mean : float
        Mean (expectation value) of the distribution.
    sigma : float
        Standard deviation of the distribution.

    Returns
    -------
    array_like
        The Gaussian evaluated at `x`.
    """
    return amplitude * norm.pdf(x, loc=mean, scale=sigma)


# --- Alternativa: gaussiana come semplice funzione a campana, NON come densità ---
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
# Da usare quando NON stai confrontando la curva con un istogramma a
# densità (density=True), ma con conteggi grezzi o altri dati non
# normalizzati: in quel caso "amplitude" assorbe interamente la scala
# dei tuoi dati, senza bisogno che la curva integri a 1.
#
# def gaussian_distribution(x, amplitude, mean, sigma):
#     return amplitude * np.exp(-0.5 * ((x - mean) / sigma) ** 2)


def main():
    """Run the full exam workflow: demo, timing, simulation, fit and plot."""

    # --- Verifica velocità di una singola partita (deve stare ben sotto 100 ms) ---
    n_players, c_coins = 3, 20
    clr = CLRGame(n_players, c_coins)
    t0 = time.time()
    clr.play_game()
    t1 = time.time()
    print(f"Single game runtime: {(t1 - t0) * 1e3:.3f} ms")
    print(clr)

    # --- Simulazione di n_games partite ---
    n_games = 100_000
    n_rounds_history = np.zeros(n_games, dtype=int)

    t0 = time.time()
    for game in range(n_games):
        clr = CLRGame(n_players, c_coins)
        n_rounds_history[game] = clr.play_game()
    t1 = time.time()
    print(f"{n_games} partite in {(t1 - t0):.1f} secondi")
    print(f"Media: {n_rounds_history.mean():.3f}, Std: {n_rounds_history.std():.3f}")

    # --- Istogramma di densità ---
    # Un bin per ogni valore intero possibile di round tra min e max osservati
    n_bins = int(n_rounds_history.max() - n_rounds_history.min()) + 1
    counts, bin_edges = np.histogram(n_rounds_history, bins=n_bins, density=True)
    bin_centers = 0.5 * (bin_edges[:-1] + bin_edges[1:])

    # --- Fit gaussiano ---
    p0 = (counts.max(), n_rounds_history.mean(), n_rounds_history.std())
    popt, pcov = curve_fit(gaussian_distribution, bin_centers, counts, p0=p0)
    amplitude_fit, mean_fit, sigma_fit = popt
    mean_err, sigma_err = np.sqrt(np.diag(pcov))[1:]

    print(f"Fitted expectation value : {mean_fit:.3f} +/- {mean_err:.3f} rounds")
    print(f"Fitted standard deviation: {sigma_fit:.3f} +/- {sigma_err:.3f} rounds\n")

    # --- Plot: istogramma + fit sovrapposti, con media e banda 1-sigma ---
    fig, ax = plt.subplots(figsize=(9, 6))

    ax.hist(n_rounds_history, bins=n_bins, density=True, color="steelblue",
            alpha=0.65, edgecolor="white", label=f"Simulated games (N={n_games:,})")

    x_fit = np.linspace(bin_edges[0], bin_edges[-1], 500)
    ax.plot(x_fit, gaussian_distribution(x_fit, *popt), color="crimson", lw=2.2,
            label="Gaussian fit")

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


if __name__ == "__main__":
    main()
