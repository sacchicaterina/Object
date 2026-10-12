"""
lorenz.py

Computing Methods for Physics 1 - 11 April 2024, Part 2 (Python)

Implements the LorenzSystem class, representing the three-dimensional
nonlinear, aperiodic, deterministic system of first-order ODEs:

    dx/dt = sigma * (y - x)
    dy/dt = x * (rho - z) - y
    dz/dt = x*y - beta*z

The class provides:
    - initialization from (sigma, rho, beta) and initial conditions (x0, y0, z0)
    - a hand-written 2nd order Runge-Kutta (RK2 / midpoint method) integrator
    - saving of the simulated trajectory {t_i, x_i, y_i, z_i} to a text file
    - plotting of the results: x-y, x-z, y-z projections and a 3D trajectory

The script then runs and saves the 6 simulations requested by the exam text:
    (rho, sigma, beta) in {(0.5, 10, 8/3), (15, 10, 8/3), (28, 10, 8/3)}
each with initial conditions (x0, y0, z0) = (0, 1, 2) and (0, 1, 2.1).
"""

import csv
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D  # noqa: F401  (needed for 3D projection)


class LorenzSystem:
    """Represents an instance of the Lorenz system of differential equations."""

    def __init__(self, sigma, rho, beta, x0, y0, z0):
        """
        Initialize the Lorenz system.

        Parameters
        ----------
        sigma, rho, beta : float
            Positive real parameters of the Lorenz system.
        x0, y0, z0 : float
            Initial conditions x(0), y(0), z(0).
        """
        self.sigma = sigma
        self.rho = rho
        self.beta = beta

        self.x0 = x0
        self.y0 = y0
        self.z0 = z0

        # Storage for the results of the last simulation performed.
        self.t = None
        self.x = None
        self.y = None
        self.z = None

    def _derivatives(self, t, state):
        """
        Evaluate f(t, u) for the Lorenz system.

        The system is autonomous (f does not actually depend on t), but the
        time argument is kept explicit to mirror the general Cauchy problem
        formulation du/dt = f(t, u) used in the exam text.

        Parameters
        ----------
        t : float
            Current time (unused, kept for interface consistency).
        state : array_like, shape (3,)
            Current (x, y, z) values.

        Returns
        -------
        np.ndarray, shape (3,)
            (dx/dt, dy/dt, dz/dt) evaluated at `state`.
        """
        x, y, z = state
        dx = self.sigma * (y - x)
        dy = x * (self.rho - z) - y
        dz = x * y - self.beta * z
        return np.array([dx, dy, dz])

    def simulate(self, T, dt):
        """
        Integrate the system from t=0 to t=T with time step dt using the
        2nd order Runge-Kutta (midpoint) method:

            t_{i+1} = t_i + dt
            K1 = f(t_i, u_i)
            K2 = f(t_i + dt/2, u_i + K1*dt/2)
            u_{i+1} = u_i + K2*dt

        Parameters
        ----------
        T : float
            Total simulation time.
        dt : float
            Integration/sampling step.

        Returns
        -------
        t, x, y, z : np.ndarray
            Arrays with the simulated trajectory.
        """
        n_steps = int(round(T / dt))

        t = np.zeros(n_steps + 1)
        u = np.zeros((n_steps + 1, 3))

        t[0] = 0.0
        u[0] = np.array([self.x0, self.y0, self.z0])

        for i in range(n_steps):
            t[i + 1] = t[i] + dt                                    # (1a)
            k1 = self._derivatives(t[i], u[i])                      # (1b)
            k2 = self._derivatives(t[i] + dt / 2.0, u[i] + k1 * dt / 2.0)  # (1c)
            u[i + 1] = u[i] + k2 * dt                                # (1d)

        self.t = t
        self.x, self.y, self.z = u[:, 0], u[:, 1], u[:, 2]

        return self.t, self.x, self.y, self.z

    def save(self, filename):
        """Save the {t, x, y, z} trajectory of the last simulation to a CSV file."""
        with open(filename, mode="w", newline="") as f:
            writer = csv.writer(f)
            writer.writerow(["t", "x", "y", "z"])
            for ti, xi, yi, zi in zip(self.t, self.x, self.y, self.z):
                writer.writerow([f"{ti:.8e}", f"{xi:.8e}", f"{yi:.8e}", f"{zi:.8e}"])

    def plot(self, savepath, title=None):
        """
        Generate the 4 required plots: x vs y, x vs z, y vs z, and a 3D
        trajectory plot of (x, y, z). The figure is always saved to
        `savepath` and shown on screen.

        Parameters
        ----------
        savepath : str
            Path where the figure is saved.
        title : str, optional
            Suptitle for the figure (e.g. describing sigma, rho, beta, ICs).
        """
        fig = plt.figure(figsize=(12, 10))

        # x vs y
        ax1 = fig.add_subplot(2, 2, 1)
        ax1.plot(self.x, self.y, lw=0.6, color="tab:blue")
        ax1.set_xlabel("x(t)")
        ax1.set_ylabel("y(t)")
        ax1.set_title("x vs y")

        # x vs z
        ax2 = fig.add_subplot(2, 2, 2)
        ax2.plot(self.x, self.z, lw=0.6, color="tab:orange")
        ax2.set_xlabel("x(t)")
        ax2.set_ylabel("z(t)")
        ax2.set_title("x vs z")

        # y vs z
        ax3 = fig.add_subplot(2, 2, 3)
        ax3.plot(self.y, self.z, lw=0.6, color="tab:green")
        ax3.set_xlabel("y(t)")
        ax3.set_ylabel("z(t)")
        ax3.set_title("y vs z")

        # 3D trajectory
        ax4 = fig.add_subplot(2, 2, 4, projection="3d")
        ax4.plot(self.x, self.y, self.z, lw=0.5, color="tab:red")
        ax4.set_xlabel("x(t)")
        ax4.set_ylabel("y(t)")
        ax4.set_zlabel("z(t)")
        ax4.set_title("3D trajectory")

        # Imposta il super titolo direttamente
        fig.suptitle(title, fontsize=13)
        # Applica il layout compatto
        fig.tight_layout()
        # Salva sempre il file
        fig.savefig(savepath, dpi=150)
        # Mostra sempre il grafico a schermo
        plt.show()
        # Chiude la figura per liberare la memoria del computer
        plt.close(fig)


def main():
    """Run and save the 6 simulations requested by the exam text."""

    # Simulation settings
    T = 50.0     # total integration time
    DT = 0.01    # sampling / integration step

    # (rho, sigma, beta) triples requested in the exam text
    rho_sigma_beta = np.array([
        [0.5, 10.0, 8.0 / 3.0],
        [15.0, 10.0, 8.0 / 3.0],
        [28.0, 10.0, 8.0 / 3.0],
    ])

    # (x0, y0, z0) initial conditions requested in the exam text
    initial_conditions = np.array([
        [0.0, 1.0, 2.0],
        [0.0, 1.0, 2.1],
    ])

    # Build all 6 combinations (3 parameter sets x 2 initial conditions) as a
    # single array: each row is (rho, sigma, beta, x0, y0, z0).
    n_params, n_ics = len(rho_sigma_beta), len(initial_conditions)
    params_grid = np.repeat(rho_sigma_beta, n_ics, axis=0)
    ics_grid = np.tile(initial_conditions, (n_params, 1))
    combinations = np.hstack([params_grid, ics_grid])

    for rho, sigma, beta, x0, y0, z0 in combinations:
        tag = f"rho{rho}_sigma{sigma:.2f}_beta{beta:.3f}_z0{z0}"

        system = LorenzSystem(sigma=sigma, rho=rho, beta=beta, x0=x0, y0=y0, z0=z0)
        system.simulate(T=T, dt=DT)
        system.save(f"{tag}.csv")

        title = (
            rf"$\rho={rho}$, $\sigma={sigma}$, $\beta={beta:.3f}$, "
            rf"IC: $x_0={x0}, y_0={y0}, z_0={z0}$"
        )
        system.plot(savepath=f"{tag}.png", title=title)


if __name__ == "__main__":
    main()
