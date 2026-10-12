"""
Computing Methods for Physics 1 - Exam 31 January 2024
Part 2 - Python

Simulation of the decay of a particle into two lighter particles.

The decaying particle (rest mass M, lifetime tau) travels along the x-axis
of the laboratory frame with velocity v0. At a randomly drawn decay time
t_bar it decays into two products; their momenta are computed in the
center-of-mass frame and then boosted into the laboratory frame.

Physical case studied: B0 -> K+ + pi-
"""

import numpy as np
import matplotlib.pyplot as plt


# Speed of light, expressed in units compatible with the MeV/c^2 masses
# (we work in "natural-ish" units: c = 1, so momenta/energies are in MeV,
#  masses in MeV/c^2, and velocities are given as fractions of c).
C_LIGHT = 1.0


class Decay:
    """
    Represents the decay of a particle into a set of lighter decay products.

    Attributes
    ----------
    name : str
        Human-readable name of the particle (e.g. "B^0", "K+", "pi-"),
        used to make print_info() output easier to read.
    mass : float
        Rest mass of the decaying particle (M), in MeV/c^2.
    tau : float
        Mean lifetime of the decaying particle, in the same time units
        used throughout the simulation (here: picoseconds/nanoseconds
        are converted to a common unit, seconds, before being stored).
    products : list of Decay
        Collection of decay products. Each product is itself an instance
        of the Decay class (so that, in principle, a product could later
        be decayed further). Initially empty; filled in by the user or by
        the momentum-assignment method.
    decay_time : float or None
        The (randomly drawn) proper decay time t_bar. None until drawn.
    momentum : numpy.ndarray or None
        4-momentum (E/c, px, py, pz) of this particle in the laboratory
        frame. None until assigned.
    position : numpy.ndarray or None
        3-position (x, y, z) of this particle at the moment it was
        produced (or, for the mother, at t = 0). None until assigned.
    """

    def __init__(self, name, mass, tau):
        """
        Create a new Decay instance (a single particle: the mother of
        the whole chain, or one of its decay products - the class does
        not distinguish between the two roles, the same blueprint is
        used for both).

        self is the specific instance being created. Every attribute
        assigned below with "self." (self.name, self.mass, self.tau,
        self.products, self.decay_time, self.decay_time_lab,
        self.momentum, self.position) belongs ONLY to this particular
        instance, not to the Decay class as a whole: creating B0, Kp and
        pi_minus as three separate Decay objects gives each of them its
        own independent copy of every one of these attributes. Setting
        B0.position later on does not touch Kp.position or
        pi_minus.position - they are three distinct pieces of memory,
        even though the attribute name written in the code (".position")
        is the same for all three, because it is defined once here, in
        the constructor that runs for every instance.
        """

        self.name = name
        self.mass = mass
        self.tau = tau
        self.products = []
        self.decay_time = None       # proper decay time (particle's own frame)
        self.decay_time_lab = None   # decay time in the laboratory frame
        self.momentum = None
        self.position = None

    def set_initial_mother_state(self, v0, position_array):  # per la particella madre
        """
        Assign this particle's initial 4-momentum (E/c, px, py, pz) and
        initial 3-position (x, y, z) in the laboratory frame, assuming
        it moves with velocity v0 (as a fraction of c) along the x-axis.
        The starting position must be given explicitly by the caller
        (e.g. (0.0, 0.0, 0.0) for a particle starting at the origin).

        This is meant to be called only on a particle that is NOT itself
        a decay product (i.e. the mother of the whole decay chain, whose
        initial conditions in the lab are known directly). Decay products
        instead get their momentum and position assigned by the mother's
        decayment() method, at the point and instant where they are born.

        This uses the standard relativistic energy-momentum relations
        for a free particle of mass M = self.mass:

            E  = gamma * M * c^2
            px = gamma * M * v0 * c
            py = pz = 0

        with gamma = 1 / sqrt(1 - v0^2/c^2).

        This is completely general: it applies to any mother particle
        whose velocity in the lab frame is known directly (as opposed
        to the decay products, whose momenta are first computed in the
        center-of-mass frame and then boosted to the lab frame).
        """
        M = self.mass
        beta = v0 / C_LIGHT
        gamma = 1.0 / np.sqrt(1.0 - beta**2)

        E = gamma * M * C_LIGHT**2
        px = gamma * M * v0 * C_LIGHT

        self.momentum = np.array([E, px, 0.0, 0.0])
        # np.asarray converte position_array in un vero array numpy (serve
        # per poterlo sommare/moltiplicare vettorialmente piu' avanti): se
        # position_array e' gia' un array numpy col tipo giusto lo riusa
        # senza copiarlo, se e' una tupla o lista (come nella chiamata da
        # main()) lo converte. A differenza di np.array, non copia mai
        # inutilmente quando non serve.
        self.position = np.asarray(position_array, dtype=float)
        return self.momentum

    def draw_decay_time(self):
        """
        Draw the proper decay time t_bar from an exponential distribution
        with mean tau, using numpy.random.exponential.
        """
        self.decay_time = np.random.exponential(scale=self.tau)
        return self.decay_time

    def draw_decay_time_lab(self, v0):
        """
        Draw the proper decay time from Exp(tau) (as draw_decay_time
        does, which also stores it in self.decay_time), then convert it
        into the corresponding elapsed time in the laboratory frame via
        time dilation, given this particle's velocity v0 (as a fraction
        of c) in the lab frame:

            t_lab = gamma * t_proper,   gamma = 1 / sqrt(1 - v0^2/c^2)

        Returns t_lab. The proper time remains available separately via
        self.decay_time.
        """
        t_proper = self.draw_decay_time()
        beta = v0 / C_LIGHT
        gamma = 1.0 / np.sqrt(1.0 - beta**2)
        t_lab = gamma * t_proper
        return t_lab

    # product1 e product2 sono istanze della classe Decay: oggetti Decay
    # a tutti gli effetti, con i propri attributi (mass, tau, products,
    # momentum, position, ...) gia' pronti per essere riempiti qui sotto.
    def decayment(self, product1, product2, v0):
        """
        Assign 4-momenta (in the laboratory frame) to two decay products,
        given their rest masses (already stored in product1.mass and
        product2.mass) and the velocity v0 (as a fraction of c, along x)
        of the mother particle in the laboratory frame.

        The direction of the decay products in the CM frame is drawn
        isotropically at random; the resulting 4-momenta are then boosted
        into the laboratory frame.
        """
        m1, m2 = product1.mass, product2.mass
        p_star = self.two_body_momentum(m1, m2)

        # Random isotropic direction in the CM frame: draw a Gaussian
        # 3-vector (isotropic by symmetry) and normalize it, keeping
        # only its direction. The result is uniformly distributed on
        # the unit sphere.
        random_vec = np.random.normal(size=3)
        direction = random_vec / np.linalg.norm(random_vec)

        # CM-frame energies and 3-momenta of the two products
        E1_star = np.sqrt(p_star**2 + m1**2) * C_LIGHT
        E2_star = np.sqrt(p_star**2 + m2**2) * C_LIGHT
        p1_star_vec = p_star * direction
        p2_star_vec = -p_star * direction  # back to back in the CM frame

        four_mom_1_star = np.array([E1_star, *p1_star_vec])
        four_mom_2_star = np.array([E2_star, *p2_star_vec])

        # Boost both 4-momenta from the CM frame to the laboratory frame.
        # The mother moves with velocity v0 along +x in the lab frame, so
        # to go from CM -> lab we boost with velocity +v0 along x.
        product1.momentum = self._boost_along_x(four_mom_1_star, v0)
        product2.momentum = self._boost_along_x(four_mom_2_star, v0)

        # Decay vertex: where the mother was when it decayed. Both
        # products are born at this same point in space.
        #     x_vertex = x0 + v * t_lab   (uniform rectilinear motion)
        #
        # Shapes: self.position is (3,), velocity_from_momentum() is (3,),
        # self.decay_time_lab is a scalar. (3,) * scalar -> (3,); (3,) +
        # (3,) -> (3,). So vertex is a plain 1D vector of 3 elements
        # [x, y, z] - NOT a (1,3) matrix. This matters because position
        # is later unpacked elsewhere (e.g. E, px, py, pz = self.momentum
        # style unpacking, or plotted as traj[:, 0]/[:, 1]/[:, 2]), which
        # requires a (3,) vector, not a (1,3) row.
        vertex = self.position + self.velocity_from_momentum() * self.decay_time_lab
        product1.position = vertex
        product2.position = vertex.copy()  # independent copy, not an alias

        self.products = [product1, product2]

    def two_body_momentum(self, m1, m2):
        """
        Return the magnitude of the momentum p* of each decay product,
        as measured in the center-of-mass frame of the decaying particle,
        for a two-body decay M -> m1 + m2.

        p* = sqrt(M^4 + m1^4 + m2^4 - 2 M^2 m1^2 - 2 M^2 m2^2 - 2 m1^2 m2^2)
             / (2 M)   [times c, set to 1 here]
        """
        M = self.mass
        if M < m1 + m2:
            # Note: the sign of the radicand alone is not a sufficient
            # test, because the radicand (a quartic polynomial in
            # M, m1, m2) can remain positive even when M < m1 + m2.
            raise ValueError(
                "Kinematically forbidden decay: M is too small for the "
                "given product masses m1, m2."
            )
        radicand = (
            M**4 + m1**4 + m2**4
            - 2 * M**2 * m1**2
            - 2 * M**2 * m2**2
            - 2 * m1**2 * m2**2
        )
        return np.sqrt(radicand) / (2 * M) * C_LIGHT

    @staticmethod
    def _boost_along_x(four_momentum, v0):
        """
        Boost a 4-momentum (E/c, px, py, pz) along the x-axis by velocity
        v0 (fraction of c), using the standard Lorentz transformation.
        """
        E, px, py, pz = four_momentum
        beta = v0 / C_LIGHT
        gamma = 1.0 / np.sqrt(1.0 - beta**2)

        E_new = gamma * (E + beta * px * C_LIGHT)
        px_new = gamma * (px + beta * E / C_LIGHT)

        return np.array([E_new, px_new, py, pz])

    def velocity_from_momentum(self):
        """
        Return the 3-velocity (vx, vy, vz), as a fraction of c, of this
        particle in the laboratory frame, computed from its 4-momentum.
        """
        E, px, py, pz = self.momentum
        return np.array([px, py, pz]) * C_LIGHT**2 / E

    def trajectory(self, t_array):
        """
        Return the 3-position of this particle at each time in t_array,
        assuming uniform rectilinear motion starting from self.position
        with the velocity given by velocity_from_momentum().

        This is generic: it applies to any free particle (mother or
        decay product) whose position and momentum are already known.

        Returns an array of shape (len(t_array), 3): one row per instant.
        """
        # Shapes: t_array is (N,) - N time instants. t_array[:, np.newaxis]
        # reshapes it to (N,1) - a column. velocity_from_momentum() is
        # (3,). Multiplying (N,1) * (3,) broadcasts to (N,3): each of the
        # N rows gets the same (3,) velocity vector scaled by its own
        # time value. Adding self.position, which is (3,), broadcasts
        # again: the same starting point is added to every one of the N
        # rows. Final result: (N,3) - one row [x,y,z] per time instant,
        # NOT a single (3,) vector like vertex was (there N=1 would still
        # give (1,3), not (3,) - see the note in decayment about why the
        # vertex is instead computed with a plain scalar formula).
        return self.position + t_array[:, np.newaxis] * self.velocity_from_momentum()

    def plot_decay_3d(self, v0):
        """
        Generate a 3D plot of the simulated decay: the trajectory of this
        particle (assumed to be the mother) from t = 0 to its lab-frame
        decay time t_bar, followed by the trajectories of its two decay
        products from t_bar to t = 2*t_bar.

        Note: unlike the other methods of this class, this one is NOT
        fully generic - it assumes self is a mother with exactly two
        decay products already assigned in self.products.

        The figure is both saved to disk and shown on screen here (the
        two go together as a single "produce the plot" action); main()
        does not repeat either step.
        """
        t_bar = self.decay_time_lab
        n_points = 200

        # --- mother trajectory: t in [0, t_bar] ---
        # t_mother: shape (200,); mother_traj: shape (200, 3)
        t_mother = np.linspace(0.0, t_bar, n_points)
        mother_traj = self.trajectory(t_mother)

        # --- decay products trajectories: t in [t_bar, 2*t_bar] ---
        # the time that matters for each product is the one elapsed since
        # its own birth (t_products - t_bar), not the absolute lab time
        # t_products: shape (200,); Kp_traj, pi_traj: shape (200, 3) each
        t_products = np.linspace(t_bar, 2 * t_bar, n_points)
        Kp, pi_minus = self.products

        Kp_traj = Kp.trajectory(t_products - t_bar)
        pi_traj = pi_minus.trajectory(t_products - t_bar)

        fig = plt.figure(figsize=(9, 7))
        ax = fig.add_subplot(111, projection="3d")
        # -- Alternativa equivalente, disattivata: si puo' ottenere lo
        # stesso asse 3D anche con plt.subplots() invece di
        # fig.add_subplot(), passando la proiezione tramite subplot_kw.
        # Utile se in futuro servissero piu' sottografici (es. una
        # griglia di assi), perche' plt.subplots() li genera tutti in
        # un colpo solo; qui, con un solo asse, add_subplot resta la
        # scelta piu' diretta e non richiede disimballare una tupla.
        # fig, ax = plt.subplots(subplot_kw={"projection": "3d"}, figsize=(9, 7))

        ax.plot(mother_traj[:, 0], mother_traj[:, 1], mother_traj[:, 2],
                label=r"$B^0$", color="tab:blue", linewidth=2)
        ax.plot(Kp_traj[:, 0], Kp_traj[:, 1], Kp_traj[:, 2],
                label=r"$K^+$", color="tab:orange", linewidth=2)
        ax.plot(pi_traj[:, 0], pi_traj[:, 1], pi_traj[:, 2],
                label=r"$\pi^-$", color="tab:green", linewidth=2)

        # Mark the decay vertex
        ax.scatter(*self.products[0].position, color="black", s=40,
                   label="decay vertex")

        ax.set_xlabel("x [light-seconds]")
        ax.set_ylabel("y [light-seconds]")
        ax.set_zlabel("z [light-seconds]")
        ax.set_title(r"Simulated decay $B^0 \rightarrow K^+ + \pi^-$"
                     f"\n(v0 = {v0:.3f} c, "
                     f"t_bar (lab) = {t_bar:.3e} s)")
        ax.legend()
        # View angle chosen explicitly: matplotlib's default angle tends
        # to make the three trajectories look nearly collinear in
        # projection, hiding the branching of the decay products.
        ax.view_init(elev=20, azim=45)
        fig.tight_layout()

        fig.savefig("decay_plot.png", dpi=150)
        plt.show()

        return fig

    def print_info(self):
        """Print a human-readable summary of this decaying particle."""
        print(f"{self.name}: M = {self.mass} MeV/c^2, tau = {self.tau} s")
        if self.momentum is not None:
            print(f"  4-momentum (E, px, py, pz) = {self.momentum}")
        if self.position is not None:
            print(f"  production position (x, y, z) = {self.position}")
        if self.decay_time is not None:
            print(f"  proper decay time t_bar = {self.decay_time:.6e} s")
        if self.decay_time_lab is not None:
            print(f"  lab-frame decay time t_bar = {self.decay_time_lab:.6e} s")
        if self.products:
            print(f"  decays into {len(self.products)} product(s):")
            for i, p in enumerate(self.products):
                print(f"    product {i+1}: {p.name}, mass = {p.mass} MeV/c^2")
        else:
            print("  no decay products assigned yet")


def main():
    """
    Entry point: asks the user for v0, sets up and runs the B0 -> K+ + pi-
    decay simulation, prints a summary of the mother and its decay
    products, generates, saves and shows the 3D plot.
    """
    print("Simulation of B0 -> K+ + pi- decay")
    print("-----------------------------------")
    # input() restituisce SEMPRE una stringa (es. "0.3"), qualunque cosa
    # digiti l'utente - anche se scrive un numero, Python lo vede come
    # testo, non ancora come un valore numerico utilizzabile nei calcoli.
    # float(...) converte quella stringa in un vero numero a virgola
    # mobile (es. "0.3" -> 0.3): senza questa conversione, v0 sarebbe una
    # stringa e operazioni come v0**2 o v0 / C_LIGHT più avanti nel
    # codice darebbero errore (non si puo' elevare al quadrato del testo).
    # Se l'utente digita qualcosa che non è un numero valido (es. "ciao"),
    # float() solleva un ValueError, che qui non viene catturato - il
    # programma si interrompe con un messaggio d'errore esplicito.
    v0 = float(input("Enter the B0 velocity v0 (as a fraction of c, 0 < v0 < 1): "))
    while not (0.0 < v0 < 1.0):
        print("v0 must satisfy 0 < v0 < 1. Try again.")
        v0 = float(input("Enter the B0 velocity v0 (as a fraction of c, 0 < v0 < 1): "))

    # Masses in MeV/c^2
    m_B0 = 5280.0
    m_Kp = 494.0
    m_pi = 140.0

    # Lifetimes converted to seconds
    tau_B0 = 1.5e-12   # 1.5 ps
    tau_Kp = 12e-9     # 12 ns
    tau_pi = 26e-9     # 26 ns

    B0 = Decay("B^0", m_B0, tau_B0)
    Kp = Decay("K+", m_Kp, tau_Kp)
    pi_minus = Decay("pi-", m_pi, tau_pi)

    # B0 starts at the origin at t = 0, moving with v0 along x in the lab.
    B0.set_initial_mother_state(v0, position_array=(0.0, 0.0, 0.0))

    # Draw the proper decay time and convert it to a lab-frame elapsed
    # time using time dilation (this also stores t_proper on B0.decay_time)
    t_bar_lab = B0.draw_decay_time_lab(v0)
    B0.decay_time_lab = t_bar_lab

    # Assign momenta and positions to the decay products (this also
    # boosts to the lab frame, and computes the decay vertex)
    B0.decayment(Kp, pi_minus, v0)

    print()
    B0.print_info()
    print()
    for i, p in enumerate(B0.products):
        print(f"--- product {i+1} ---")
        p.print_info()
        print()

    B0.plot_decay_3d(v0)


if __name__ == "__main__":
    main()
