import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import solve_ivp


class Pendulum:


  def __init__(self, omega_0, gamma, amplitude, Omega, theta_init, omega_init):

      self.omega_0 = omega_0
      self.gamma = gamma
      self.amplitude = amplitude
      self.Omega = Omega
      self.theta_init = theta_init
      self.omega_init = omega_init


      self.t_prime = None
      self.t_real = None
      self.theta = None
      self.omega = None




  def equations(self, t_prime, y):
      
     theta, omega = y

     dtheta_dt_prime = omega
     domega_dt_prime = -np.sin(theta) - (self.gamma/self.omega_0) * omega + (self.amplitude / self.omega_0 ** 2) * np.cos((self.Omega / self.omega_0) * t_prime)

     return [dtheta_dt_prime, domega_dt_prime]



  def integration(self, t_max = 100, n_points = 2000):


      solution = solve_ivp(
                 fun = self.equations,
                 t_span = (0., t_max),
                 y0 = [self.theta_init, self.omega_init],
                 method = 'RK45',
                 t_eval = np.linspace(0., t_max, n_points),
                 rtol = 1e-9,
                 atol = 1e-9
                 )


      self.t_prime = solution.t
      self.t_real = self.t_prime / self.omega_0
      self.theta = solution.y[0]
      self.omega = solution.y[1]


      return solution




  def plot_temporal_evolution(self):

    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize = (8,6), sharex = 'col')
    fig.suptitle(r"Angular displacement $\theta$ and angular velocity $\omega$ vs time\n$" + self._label(), fontsize=9, weight = 'bold')

    #theta vs t'
    ax1.plot(self.t_prime, self.theta, color = "tab:blue", lw = 1.5)
    ax1.set_xlabel(r"$t^\prime")
    ax1.set_ylabel(r"$\theta$")
    ax1.title(r"$\theta vs t^\prime$")
    ax1.grid(True, alpha = 0.6)

    #theta vs t
    ax2.plot(self.t_real, self.theta, color = "tab:blue", lw = 1.5)
    ax2.set_xlabel(r"$t$")
    ax2.set_ylabel(r"$\theta$")
    ax2.title(r"$\theta vs t$")
    ax2.grid(True, alpha = 0.6)

    #omega vs t'
    ax3.plot(self.t_prime, self.theta, color = "tab:blue", lw = 1.5)
    ax3.set_xlabel(r"$t^\prime")
    ax3.set_ylabel(r"$\omega$")
    ax3.title(r"$\omega vs t^\prime$")
    ax3.grid(True, alpha = 0.6)
    
    #omega vs t
    ax4.plot(self.t_real, self.theta, color = "tab:blue", lw = 1.5)
    ax4.set_xlabel(r"$t$")
    ax4.set_ylabel(r"$\omega$")
    ax4.title(r"$\omega vs t$")
    ax4.grid(True, alpha = 0.6)

    fig.tight_layout()

    return fig



 def plot_phase_space(self):

   fig, ax = plt.subplots(1, 1, figsize = (6, 6))

   ax.plot(self.theta, self.omega, color = "tab:blue", lw = 1.5)
   ax.set_xlabel(r"$\theta$")
   ax.y_label(r"$\omega$")
   ax.set_title("Phase space: " + r"$\omega$ vs $\theta$" + "\n" + self._label(), fontsize=9)
   plt.grid(True, alpha = 0.6)

   plt.scatter(self.theta_init, self.omega_init, color = 'red', zorder = 5, label = f"Inizio ({self.theta_init}, {self.omega_init})")
   plt.legend(loc = 'upper right')
   
   fig.tight_layout()

   return fig

    

      
                 
