"""
Bat Echolocation FM-Sweep Signal Processing
=============================================

Michela De Angelis - 1797244

Simula un impulso di eco-localizzazione di un pipistrello (downward FM sweep),
lo inserisce in una finestra temporale piu' ampia con rumore, applica un filtro
adattato (matched filter) e ne calcola la densita' spettrale di potenza (PSD).

Corso: OOP for Data Processing - 14/09/2026
"""

import numpy as np
import matplotlib.pyplot as plt
from scipy.signal.windows import tukey
from scipy.fft import fft, fftfreq


class BatEcholocationSignal:
    """Modella, elabora e visualizza un impulso di eco-localizzazione di un pipistrello."""

    def __init__(self, amplitude_0=1.0, peak_time=0.05, sigma=0.015,
                 start_frequency=80e3, chirp_rate=1.2e6,
                 sampling_rate=250e3, pulse_duration=0.1,
                 tukey_alpha=0.1, padding_duration=0.2, noise_sigma=1.5):
        # --- parametri dell'impulso h(t) ---
        self.amplitude_0 = amplitude_0            # A0, ampiezza di picco [adimensionale]
        self.peak_time = peak_time                # t0, istante di picco dell'inviluppo [s]
        self.sigma = sigma                        # sigma, larghezza dell'inviluppo gaussiano [s]
        self.start_frequency = start_frequency    # f0, frequenza iniziale del chirp [Hz]
        self.chirp_rate = chirp_rate              # k, tasso di variazione della frequenza [Hz/s]

        # --- parametri di campionamento e acquisizione ---
        self.sampling_rate = sampling_rate        # fs, frequenza di campionamento [Hz]
        self.pulse_duration = pulse_duration      # durata dell'impulso h(t) [s]
        self.tukey_alpha = tukey_alpha            # parametro di forma della finestra di Tukey
        self.padding_duration = padding_duration  # padding di zeri su ciascun lato [s]
        self.noise_sigma = noise_sigma            # deviazione standard del rumore bianco (sigma_n = 1.5 nel testo)

    @property
    def dt(self):
        """Intervallo di campionamento [s]."""
        return 1.0 / self.sampling_rate  # 1/fs

    @property
    def nyquist_frequency(self):
        """Frequenza di Nyquist [Hz]."""
        return self.sampling_rate / 2.0  # fs/2

    # ----------------------------------------------------------------- #
    #  Generazione del segnale
    # ----------------------------------------------------------------- #
    def envelope(self, t):
        """Inviluppo gaussiano di ampiezza A(t)."""
        return self.amplitude_0 * np.exp(-((t - self.peak_time) ** 2) / (2 * self.sigma ** 2))  # gaussiana centrata in t0

    def phase(self, t):
        """Fase phi(t) del chirp discendente (downward FM sweep)."""
        return 2 * np.pi * (self.start_frequency * t - 0.5 * self.chirp_rate * t ** 2)  # frequenza decresce nel tempo

    def generate_pulse(self):
        """Genera l'impulso h(t) = A(t) sin(phi(t)) nel dominio del tempo (prima della finestratura)."""
        t = np.arange(0.0, self.pulse_duration, self.dt)  # asse dei tempi [s], da 0 a pulse_duration
        return self.envelope(t) * np.sin(self.phase(t))  # h(t) = A(t) * sin(phi(t))

    def apply_tukey_window(self, signal):
        """Applica una finestra di Tukey (tapered cosine) al segnale dato."""
        window = tukey(len(signal), alpha=self.tukey_alpha)  # finestra della stessa lunghezza del segnale
        return signal * window  # smussa i bordi del segnale

    def pad_signal(self, signal):
        """Aggiunge zeri di durata `padding_duration` su entrambi i lati del segnale.

        Il segnale risultante s(t) ha durata totale pulse_duration + 2*padding_duration
        (0.1 + 0.2 + 0.2 = 0.5 s coi parametri di default).
        Ritorna l'asse dei tempi (in secondi, shiftato dal padding) e il segnale paddato.
        """
        n_pad = round(self.padding_duration * self.sampling_rate)  # secondi x campioni/secondo = numero di campioni di zero per lato
        padded_signal = np.pad(signal, pad_width=n_pad, mode="constant", constant_values=0.0)  # aggiunge n_pad campioni a zero prima e dopo il segnale
        t_padded = np.arange(len(padded_signal)) * self.dt - self.padding_duration  # asse dei tempi [s] del segnale paddato
        return t_padded, padded_signal

    def add_white_noise(self, signal, rng):
        """Aggiunge rumore bianco gaussiano di media nulla e deviazione standard sigma_n."""
        noise = rng.normal(loc=0.0, scale=self.noise_sigma, size=signal.shape)  # rumore casuale, stessa forma del segnale
        return signal + noise  # somma il rumore al segnale

    # ----------------------------------------------------------------- #
    #  Elaborazione del segnale
    # ----------------------------------------------------------------- #
    def matched_filter(self, h_windowed, d_windowed):
        """Filtro adattato: convoluzione tra l'impulso finestrato e il segnale ricevuto finestrato.

        Ritorna l'asse dei tempi (offset) e l'uscita del filtro rho(t).
        """
        rho = np.convolve(h_windowed, d_windowed, mode="full")  # convoluzione full tra i due segnali
        time_offset = np.arange(len(rho)) * self.dt  # asse dei tempi [s] corrispondente a rho
        return time_offset, rho

    def power_spectral_density(self, signal):
        """Calcola la PSD |FFT(s(t))|^2, limitata alle frequenze non negative fino a Nyquist."""
        spectrum = fft(signal)  # DFT del segnale (numeri complessi)
        freqs = fftfreq(len(signal), d=self.dt)  # frequenze [Hz] corrispondenti alle bin della FFT

        positive_mask = (freqs >= 0) & (freqs <= self.nyquist_frequency)  # tiene solo 0 <= f <= Nyquist
        freqs_positive = freqs[positive_mask]  # frequenze filtrate
        psd = np.abs(spectrum[positive_mask]) ** 2  # |FFT|^2 = potenza dello spettro
        return freqs_positive, psd

    # ----------------------------------------------------------------- #
    #  Visualizzazione
    # ----------------------------------------------------------------- #
    def plot_results(self, t_padded, s_signal, d_signal, time_offset, rho, freqs, psd):
        """Crea la figura con i tre sottografici richiesti e li annota con i picchi massimi."""
        fig, (ax_time, ax_corr, ax_psd) = plt.subplots(3, 1, figsize=(10, 11))

        # 1) Segnale pulito s(t) sovrapposto al segnale rumoroso d(t)
        ax_time.plot(t_padded, d_signal, color="lightgray", label="d(t) - segnale rumoroso ricevuto")  # curva d(t), disegnata sotto
        ax_time.plot(t_padded, s_signal, color="darkorange", linewidth=1.5, label="s(t) - impulso emesso (pulito)")  # curva s(t), sopra
        ax_time.set_xlabel("Tempo [s]")  # etichetta asse x
        ax_time.set_ylabel("Ampiezza")  # etichetta asse y
        ax_time.set_title("Impulso emesso s(t) e segnale ricevuto rumoroso d(t)")  # titolo sottografico
        ax_time.legend(loc="upper right")  # mostra la legenda

        # 2) Uscita del filtro adattato rho(t)
        peak_idx_corr = np.argmax(rho)  # indice del valore massimo di rho
        ax_corr.plot(time_offset, rho, color="teal", label="rho(t) - uscita filtro adattato")  # curva rho(t)
        ax_corr.plot(time_offset[peak_idx_corr], rho[peak_idx_corr], "rx", markersize=10,
                     label=f"Picco massimo @ {time_offset[peak_idx_corr]:.4f} s")  # marker sul picco
        ax_corr.annotate(f"{rho[peak_idx_corr]:.2f}",
                          xy=(time_offset[peak_idx_corr], rho[peak_idx_corr]),
                          xytext=(10, 10), textcoords="offset points", color="red")  # etichetta col valore del picco
        ax_corr.set_xlabel("Offset temporale [s]")  # etichetta asse x
        ax_corr.set_ylabel("Ampiezza correlazione")  # etichetta asse y
        ax_corr.set_title("Uscita del filtro adattato (matched filter)")  # titolo sottografico
        ax_corr.legend(loc="upper right")  # mostra la legenda

        # 3) Densita' spettrale di potenza |s~(f)|^2 in scala logaritmica
        freq_mask = (freqs >= 20e3) & (freqs <= 125e3)  # tiene solo l'intervallo [20, 125] kHz per il grafico
        freqs_khz = freqs[freq_mask] / 1e3  # frequenze filtrate, convertite in kHz
        psd_windowed = psd[freq_mask]  # PSD filtrata, allineata a freqs_khz
        peak_idx_psd = np.argmax(psd_windowed)  # indice del picco entro l'intervallo filtrato

        ax_psd.semilogy(freqs_khz, psd_windowed, color="purple", label="|s~(f)|^2")  # curva PSD, asse y log
        ax_psd.plot(freqs_khz[peak_idx_psd], psd_windowed[peak_idx_psd], "rx", markersize=10,
                    label=f"Picco massimo @ {freqs_khz[peak_idx_psd]:.1f} kHz")  # marker sul picco
        ax_psd.annotate(f"{freqs_khz[peak_idx_psd]:.1f} kHz",
                         xy=(freqs_khz[peak_idx_psd], psd_windowed[peak_idx_psd]),
                         xytext=(10, 10), textcoords="offset points", color="red")  # etichetta col valore del picco
        ax_psd.set_xlabel("Frequenza [kHz]")  # etichetta asse x
        ax_psd.set_ylabel("Densita' spettrale di potenza")  # etichetta asse y
        ax_psd.set_title("Densita' spettrale di potenza (PSD) di s(t)")  # titolo sottografico
        ax_psd.legend(loc="upper right")  # mostra la legenda

        fig.tight_layout()  # sistema gli spazi tra i sottografici
        fig.savefig("bat_echolocation_results.png", dpi=150)  # salva la figura su file
        plt.show()  # mostra la figura a schermo
        return fig

    # ----------------------------------------------------------------- #
    #  Pipeline completa
    # ----------------------------------------------------------------- #
    def run(self):
        """Esegue l'intera pipeline: generazione, rumore, filtro adattato, PSD e grafici."""
        rng = np.random.default_rng(seed=42)  # seed fisso per riproducibilita'

        # --- Generazione dell'impulso h(t) ---
        h_pulse = self.generate_pulse()  # impulso puro, senza finestra
        h_windowed = self.apply_tukey_window(h_pulse)  # impulso finestrato con Tukey

        # --- Costruzione del segnale emesso s(t) con padding ---
        t_padded, s_signal = self.pad_signal(h_windowed)  # h_windowed + padding = s(t), con il suo asse dei tempi

        # --- Segnale acquisito rumoroso d(t) ---
        d_signal = self.add_white_noise(s_signal, rng)  # s(t) + rumore = d(t)
        d_windowed = self.apply_tukey_window(d_signal)  # d(t) finestrato con Tukey

        # --- Filtro adattato ---
        time_offset, rho = self.matched_filter(h_windowed, d_windowed)  # rho(t) tra h_windowed e d_windowed

        # --- Densita' spettrale di potenza di s(t) ---
        freqs, psd = self.power_spectral_density(s_signal)  # PSD calcolata su s(t)

        # --- Grafici ---
        fig = self.plot_results(t_padded, s_signal, d_signal, time_offset, rho, freqs, psd)  # crea e mostra la figura


if __name__ == "__main__":
    bat_signal = BatEcholocationSignal(chirp_rate=0.0)  # crea l'oggetto con i parametri di default
    bat_signal.run()  # esegue l'intera pipeline
