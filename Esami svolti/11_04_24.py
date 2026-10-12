import numpy as np
import matplotlib.pyplot as plt
import pickle


class LorenzSystem:

    def __init__(self, sigma, rho, beta, x0, y0, z0):
        if sigma <= 0 or rho <= 0 or beta <= 0:
            raise ValueError("sigma, rho e beta devono essere numeri reali positivi.")
        self.sigma = sigma
        self.rho = rho
        self.beta = beta
        self.initial_state = np.array([x0, y0, z0], dtype=float)
        self.t_eval = None
        self.results = None

    def equations(self, state):
        x, y, z = state
        dx_dt = self.sigma * (y - x)
        dy_dt = x * (self.rho - z) - y
        dz_dt = x * y - self.beta * z
        return np.array([dx_dt, dy_dt, dz_dt])

    def integration(self, T, dt):
        n_points = int(np.round(T / dt))
        self.t_eval = np.linspace(0, T, n_points)
        self.results = np.zeros((n_points, 3), dtype=float)
        self.results[0, :] = self.initial_state

        u = self.initial_state.copy()
        for i in range(n_points - 1):
            K1 = self.equations(u)
            K2 = self.equations(u + K1 * dt / 2)
            u += K2 * dt
            self.results[i + 1] = u

    def save(self, filename):
        payload = {
            "sigma": self.sigma,
            "rho": self.rho,
            "beta": self.beta,
            "initial_state": self.initial_state,
            "t_values": self.t_eval,
            "results": self.results,
        }
        with open(filename, "wb") as f:
            pickle.dump(payload, f)

    @classmethod
    def load(cls, filename):
        with open(filename, "rb") as f:
            payload = pickle.load(f)
        x0, y0, z0 = payload["initial_state"]
        system = cls(sigma=payload["sigma"], rho=payload["rho"], beta=payload["beta"], x0=x0, y0=y0, z0=z0)
        system.t_eval = payload["t_values"]
        system.results = payload["results"]
        return system

    def plot(self):
        fig = plt.figure(figsize=(5, 7))
        x0, y0, z0 = self.initial_state
        description = rf"$\sigma$={self.sigma}, $\rho$={self.rho}, $\beta$={self.beta:.3f}, $x_0$={x0}, $y_0$={y0}, $z_0$={z0}"
        fig.suptitle("Sistema di Lorenz " + description, fontsize=10)

        ax1 = fig.add_subplot(2, 2, 1)
        ax1.plot(self.results[:, 0], self.results[:, 1], color="tab:blue", lw=1.5)
        ax1.set_xlabel("x(t)")
        ax1.set_ylabel("y(t)")
        ax1.set_title("x versus y")
        ax1.grid(alpha=0.3)

        ax2 = fig.add_subplot(2, 2, 2)
        ax2.plot(self.results[:, 0], self.results[:, 2], color="tab:blue", lw=1.5)
        ax2.set_xlabel("x(t)")
        ax2.set_ylabel("z(t)")
        ax2.set_title("x versus z")
        ax2.grid(alpha=0.3)

        ax3 = fig.add_subplot(2, 2, 3)
        ax3.plot(self.results[:, 1], self.results[:, 2], color="tab:blue", lw=1.5)
        ax3.set_xlabel("y(t)")
        ax3.set_ylabel("z(t)")
        ax3.set_title("y versus z")
        ax3.grid(alpha=0.3)

        ax4 = fig.add_subplot(2, 2, 4, projection="3d")
        ax4.plot(self.results[:, 0], self.results[:, 1], self.results[:, 2], color="tab:blue", lw=1.5)
        ax4.set_xlabel("x(t)")
        ax4.set_ylabel("y(t)")
        ax4.set_zlabel("z(t)")
        ax4.set_title("x versus y versus z")

        fig.tight_layout()

        return fig


def main():
    sigma = 10
    beta = 8. / 3.
    x0 = 0
    y0 = 1
    rho_values = [0.5, 15, 28]
    z0_values = [2, 2.1]

    for rho in rho_values:
        for z0 in z0_values:
            system = LorenzSystem(sigma, rho, beta, x0, y0, z0)
            system.integration(50., 0.01)

            filename = f"sigma{sigma}_rho{rho}_beta{beta:.3f}_x0{x0}_y0{y0}_z0{z0}"
            system.save(filename + ".pkl")

            fig = system.plot()
            fig.savefig(filename + ".png")
            plt.show()
            plt.close(fig)


if __name__ == "__main__":
    main()
