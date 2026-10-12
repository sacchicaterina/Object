# %% [markdown]
# # Computing Methods for Physics — Esame 14 Giugno 2023
# ## Parte 2 — Python
#
# **Consegna:**
# 1. Caricare il dizionario contenuto in `data.pickle`
# 2. Plottare il contenuto dei dati
# 3. Impostare un Monte Carlo con `emcee` per fittare i dati
# 4. Mostrare e commentare i risultati del fit

# %%
import pickle
import numpy as np
import matplotlib.pyplot as plt
import emcee
import corner
from scipy.optimize import minimize

# %% [markdown]
# ## 1. Caricamento dei dati

# %%
with open("data_20230614.pickle", "rb") as f:
    data = pickle.load(f)

x, y, yerr = data["x"], data["y"], data["yerr"]

print(f"Numero di punti: {len(x)}")
print(f"Range x: [{x.min()}, {x.max()}]")
print(f"Range y: [{y.min():.2f}, {y.max():.2f}]")

# %% [markdown]
# ## 2. Plot dei dati

# %%
fig, ax = plt.subplots(figsize=(10, 6))
ax.errorbar(x, y, yerr=yerr, fmt=".", color="steelblue", ecolor="lightgray",
            alpha=0.6, markersize=3, label="Dati sperimentali")
ax.set_xlabel("x")
ax.set_ylabel("y")
ax.set_title("Dati caricati da data.pickle")
ax.legend()
fig.tight_layout()
plt.show()

# %% [markdown]
# I dati mostrano chiaramente un andamento **oscillante** attorno a un valore medio
# pressoché nullo, con errori (`yerr`) di entità paragonabile all'ampiezza del segnale.

# %% [markdown]
# ## 3. Setup del fit MCMC con `emcee`
#
# Si assume un modello sinusoidale con offset:
#
# $$ y(x) = A \sin\left(\frac{2\pi x}{P} + \varphi\right) + C $$
#
# dove $A$ è l'ampiezza, $P$ il periodo, $\varphi$ la fase e $C$ un offset costante.
#
# Il fit bayesiano richiede la definizione di:
# - una **log-likelihood** gaussiana (assumendo errori indipendenti su ogni punto),
# - un **log-prior** uniforme (non informativo) entro range fisicamente ragionevoli,
# - la conseguente **log-probability** a posteriori.


# %%
def model(theta, x):
    """Modello sinusoidale con offset: y = A*sin(2*pi*x/P + phi) + C"""
    A, P, phi, C = theta
    return A * np.sin(2 * np.pi * x / P + phi) + C


def log_likelihood(theta, x, y, yerr):
    """Log-verosimiglianza gaussiana, assumendo errori indipendenti.

    Formula di partenza (probabilita' gaussiana per ogni punto, poi prodotto
    su tutti i punti assumendo errori indipendenti):

        P_i = 1/sqrt(2*pi*sigma_i^2) * exp(-(y_i - y_model_i)^2 / (2*sigma_i^2))
        L = prod_i P_i

    Logaritmo (trasforma il prodotto in una somma, numericamente stabile):

        log(L) = sum_i log(P_i)
               = -0.5 * sum_i [ (y_i - y_model_i)^2 / sigma_i^2 + log(2*pi*sigma_i^2) ]

    Questa e' esattamente la riga di return sottostante.

    Collegamento col chi-quadro: il primo termine della somma,
    sum_i[(y_i-y_model_i)^2/sigma_i^2], E' ESATTAMENTE il chi-quadro
    classico. Il secondo termine, sum_i[log(2*pi*sigma_i^2)], dipende SOLO
    da sigma (yerr), che e' fisso e non dipende da theta: quindi, quando si
    cerca il theta che massimizza log(L), quel termine e' una costante
    additiva che non sposta la posizione del massimo. Ne consegue che
    MASSIMIZZARE log(L) e MINIMIZZARE il chi-quadro sono lo stesso identico
    problema di ottimizzazione (verificato numericamente: i due approcci
    danno lo stesso risultato fino alla precisione numerica). Il pre-fit
    con scipy.optimize piu' avanti sta quindi gia' facendo, di fatto, un
    fit ai minimi quadrati, anche se non chiama esplicitamente il
    chi-quadro. Il chi-quadro esplicito, calcolato invece alla fine dello
    script, non serve a trovare i parametri (gia' trovati dall'MCMC), ma
    solo come controllo diagnostico finale sulla bonta' del fit.
    """
    y_model = model(theta, x)
    sigma2 = yerr ** 2
    return -0.5 * np.sum((y - y_model) ** 2 / sigma2 + np.log(2 * np.pi * sigma2))


def log_prior(theta):
    """Prior uniforme (non informativo) entro range fisicamente ragionevoli.

    Logica dietro ciascun limite:
    - A (0, 20): un'ampiezza deve essere positiva per definizione fisica;
      il limite superiore e' circa 4 volte la stima grezza dell'ampiezza
      osservata nei dati (escursione max-min / 2), un margine ampio ma non
      arbitrario.

    - P (10, 500): entrambi i limiti derivano da come sono strutturati i
      dati (1000 punti, uno ogni unita' di x, da 0 a 999).
        * Limite inferiore (P>10): con un campione ogni unita' di x, il
          teorema di Nyquist richiede ALMENO 2 campioni per periodo per
          poter ricostruire un'oscillazione senza ambiguita'. Un periodo
          troppo vicino a questo limite teorico (es. P=2 o P=3) rischia
          "aliasing": un'oscillazione veloce puo' apparire come una
          oscillazione piu' lenta e diversa da quella vera (lo stesso
          motivo per cui le ruote di un'auto sembrano girare al contrario
          in un video con frame rate basso). P=10 da' un margine di
          sicurezza abbondante sopra il limite critico di 2.
        * Limite superiore (P<500): se il periodo fosse vicino o superiore
          alla lunghezza totale osservata (999), vedrei MENO DI UNA
          oscillazione completa nei dati: il segnale sembrerebbe una curva
          qualsiasi, non necessariamente periodica, e non ci sarebbe modo
          di stimare un periodo in modo affidabile. Con P=500 (circa meta'
          del range) si vedono almeno ~2 cicli completi (999/500 ≈ 2),
          il minimo ragionevole per riconoscere una vera periodicita'.
        * Il periodo vero, trovato dal fit, e' ≈112: ben dentro questi
          limiti, non vicino ai bordi, quindi il prior non ha "forzato"
          in alcun modo il risultato.

    - phi (-2*pi, 2*pi): puramente matematico, non fisico. La funzione seno
      e' periodica di 2*pi, quindi qualunque fase fuori da questo intervallo
      e' equivalente (stessa curva) a un valore gia' incluso: un range piu'
      ampio sarebbe solo ridondante.

    - C (-10, 10): un offset fisicamente ragionevole non dovrebbe superare
      di molto il range osservato dei dati stessi (qui y va da -4.8 a 5.1);
      il limite e' circa il doppio di quel range, per sicurezza.
    """
    A, P, phi, C = theta
    if 0 < A < 20 and 10 < P < 500 and -2 * np.pi < phi < 2 * np.pi and -10 < C < 10:
        return 0.0
    return -np.inf


def log_probability(theta, x, y, yerr):
    """Log-probabilita' a posteriori = log-prior + log-likelihood.

    Formula di partenza (teorema di Bayes, denominatore P(dati) ignorato
    perche' costante e irrilevante per il campionamento MCMC):

        P(theta | dati) = P(dati | theta) * P(theta) / P(dati)
                         ∝ likelihood * prior

    Logaritmo (prodotto -> somma, log(a*b) = log(a) + log(b)):

        log(P(theta | dati)) = log(likelihood) + log(prior)

    Questa e' esattamente "lp + log_likelihood(...)" nel return sottostante.
    """
    lp = log_prior(theta)
    if not np.isfinite(lp):
        # se il prior e' -inf (parametri fuori dai limiti), esco subito
        # senza calcolare log_likelihood, che e' piu' costoso (richiede il
        # modello su tutti i 1000 punti dati): il risultato sarebbe -inf
        # comunque, quindi risparmio calcolo inutile
        return -np.inf
    return lp + log_likelihood(theta, x, y, yerr)


# %% [markdown]
# ### Stima del periodo tramite FFT
#
# Per ottenere una stima quantitativa del periodo (invece di indovinarlo "a
# occhio" dal grafico) si analizza lo spettro in frequenza del segnale con la
# trasformata di Fourier discreta. Si rimuove prima la componente continua
# (sottraendo la media), poi si individua la frequenza a cui corrisponde il
# picco più alto dello spettro: il periodo stimato è semplicemente il suo
# inverso.

# %%
# tolgo l'offset (media) dai dati: la FFT interpreta il valore costante
# come una componente a frequenza zero, che rischierebbe di dominare lo
# spettro e nascondere il vero picco dell'oscillazione
y_centered = y - np.mean(y)
# genero solo l'ASSE delle frequenze possibili (non calcolo ancora nulla):
# len(x)=1000 campioni, d=1 la spaziatura tra un campione e il successivo
# -> risultato: 501 frequenze da 0 a 0.5 (limite di Nyquist)
freqs = np.fft.rfftfreq(len(x), d=1)
# calcolo la trasformata di Fourier discreta (restituisce numeri complessi,
# ampiezza+fase per ciascuna frequenza); np.abs prende il modulo, cioe'
# quanto e' "forte" ciascuna frequenza nel segnale, ignorando la fase
spectrum = np.abs(np.fft.rfft(y_centered))
spectrum[0] = 0  # rimuove l'eventuale residuo di componente continua

# np.argmax trova l'INDICE del valore piu' alto in spectrum (la frequenza
# piu' presente nel segnale); freqs[...] recupera la frequenza vera e
# propria corrispondente a quell'indice
peak_freq = freqs[np.argmax(spectrum)]
# la frequenza indica "oscillazioni per unita' di x"; il periodo e' il suo
# reciproco, "unita' di x per un'oscillazione completa"
P0_fft = 1 / peak_freq

print(f"Frequenza dominante individuata dalla FFT: {peak_freq:.4f}")
print(f"Periodo stimato (1/frequenza): {P0_fft:.2f}")

# %% [markdown]
# ### Stime iniziali dei parametri
#
# Per inizializzare i walkers vicino alla soluzione (e velocizzare la convergenza),
# si usano stime rapide ricavate direttamente dai dati:
# - $A_0$: metà dell'escursione (max − min) di $y$ — una sinusoide oscilla tra
#   $-A$ e $+A$ (attorno al suo offset), quindi l'escursione totale dei dati
#   è circa $2A$; è una stima grezza perché include anche il rumore (`yerr`)
#   nei valori estremi, non solo l'oscillazione pura.
# - $P_0$: periodo individuato dalla FFT appena calcolata.
# - $\varphi_0 = 0$: nessuna informazione a priori sulla fase — non c'è un
#   modo semplice di stimarla "a occhio" dai dati, quindi si parte da zero
#   e si lascia che sia il pre-fit (scipy) a trovarla.
# - $C_0$: media di $y$ — una sinusoide pura oscilla simmetricamente tra
#   $-1$ e $+1$, quindi ha valore medio nullo; sommando l'offset $C$, il
#   valore medio dell'intera curva diventa esattamente $C$ (la parte
#   oscillante si annulla mediando su un numero sufficiente di cicli).
#   La media dei dati osservati è quindi una stima diretta e naturale di
#   $C$, tanto più precisa quante più oscillazioni complete sono presenti
#   nel dataset (qui ce ne sono circa 9, quindi la stima è già buona).

# %%
# escursione dei dati / 2: stima grezza dell'ampiezza (sovrastimata, include
# anche il rumore nei valori estremi di y)
A0 = (y.max() - y.min()) / 2
# periodo dal picco dello spettro FFT calcolato sopra
P0 = P0_fft
# nessuna informazione a priori sulla fase: parto da zero
phi0 = 0.0
# media di y: stima diretta di C, sfruttando il fatto che una sinusoide
# pura ha valore medio nullo (vedi spiegazione sopra)
C0 = np.mean(y)

theta0 = np.array([A0, P0, phi0, C0])
labels = ["A", "P", "phi", "C"]

print("Parametri iniziali (stima):")
for lab, val in zip(labels, theta0):
    print(f"  {lab} = {val:.3f}")

# %% [markdown]
# ### Ottimizzazione preliminare (pre-fit)
#
# Il modello sinusoidale presenta minimi secondari nella likelihood, dovuti alla
# periodicità di $\varphi$ e $P$: combinazioni di fase/periodo leggermente diverse
# da quella vera possono comunque riprodurre abbastanza bene i dati. Se i walker
# partissero da una stima grezza e dispersa, alcuni di essi rischierebbero di
# restare intrappolati in queste soluzioni sub-ottimali per l'intera catena.
#
# Per ridurre drasticamente questo rischio, si usa `scipy.optimize.minimize` per
# rifinire la stima iniziale trovando il vero massimo (locale) della likelihood
# più vicino, partendo dalla stima grezza già ragionevole (`theta0`). I walker
# vengono poi inizializzati in una sfera molto stretta attorno a questo punto
# ottimizzato: partendo tutti vicinissimi al massimo globale, la probabilità che
# qualcuno finisca in un minimo secondario si riduce fortemente.

# %%
# scipy.optimize.minimize MINIMIZZA una funzione, ma io voglio MASSIMIZZARE
# la likelihood: minimizzare il suo opposto equivale a massimizzare
# l'originale, quindi creo una funzione "gemella" col segno invertito
neg_log_likelihood = lambda theta, x, y, yerr: -log_likelihood(theta, x, y, yerr)

# cerco, partendo dalla stima grezza theta0, il punto che minimizza
# neg_log_likelihood (cioe' massimizza log_likelihood); args passa i dati
# fissi (x,y,yerr) che NON vanno ottimizzati, solo theta viene variato
opt_result = minimize(neg_log_likelihood, theta0, args=(x, y, yerr))
# opt_result e' un oggetto con vari attributi (.fun, .success, .nit, ...):
# .x e' quello che serve qui, contiene i 4 parametri ottimizzati trovati
# (il punto di minimo raggiunto), non il dato x dei nostri punti sperimentali
theta_start = opt_result.x

# .success (bool) indica se scipy ritiene di aver soddisfatto i suoi
# criteri INTERNI di tolleranza numerica per dichiararsi "convergente".
# Nel nostro caso e' spesso False (messaggio: "precision loss"), ma questo
# NON significa che il risultato sia sbagliato: .x resta comunque un'ottima
# stima (verificabile confrontandola col risultato finale dell'MCMC).
# Il motivo per cui possiamo permetterci di ignorare un eventuale False:
# scipy.optimize qui serve SOLO a trovare un buon punto di PARTENZA per i
# walker, non e' la fonte del risultato finale del fit. Il vero risultato
# arriva dall'intero campionamento MCMC successivo (migliaia di passi, 32
# walker), un processo molto piu' robusto che esplora a fondo lo spazio dei
# parametri. Stampare .success resta comunque utile come controllo
# diagnostico: se scipy fosse finito MOLTO lontano dalla vera soluzione,
# l'inizializzazione stretta dei walker (piu' avanti, 1e-5) rischierebbe di
# farli partire tutti insieme in una zona sbagliata (un minimo secondario).
print("Esito ottimizzazione scipy:", opt_result.success)
print("Punto di partenza per i walker (dopo pre-fit):")
for lab, val in zip(labels, theta_start):
    print(f"  {lab} = {val:.5f}")

# %% [markdown]
# ### Verifica: procedimento equivalente minimizzando direttamente il chi-quadro
#
# Come spiegato nel docstring di `log_likelihood`, massimizzare la
# log-verosimiglianza gaussiana equivale matematicamente a minimizzare il
# chi-quadro classico:
#
# $$ \chi^2(\theta) = \sum_{i=1}^{N} \frac{(y_i - y_{model,i}(\theta))^2}{\sigma_i^2} $$
#
# Qui lo si verifica esplicitamente: si definisce il chi-quadro come
# funzione separata, lo si minimizza con lo stesso algoritmo (scipy,
# partendo dalla stessa stima grezza `theta0`), e si confronta il risultato
# con quello ottenuto sopra massimizzando la likelihood. Ci si aspetta che
# i due procedimenti diano lo stesso punto di minimo/massimo.

# %%
def chi_square(theta, x, y, yerr):
    """Chi-quadro classico: somma degli scarti al quadrato, pesati per
    l'incertezza su ciascun punto."""
    y_model = model(theta, x)
    return np.sum((y - y_model) ** 2 / yerr ** 2)

# minimizzo DIRETTAMENTE il chi-quadro (non il suo negativo: qui va gia'
# minimizzato, a differenza della likelihood che andava massimizzata)
opt_result_chi2 = minimize(chi_square, theta0, args=(x, y, yerr))
theta_start_chi2 = opt_result_chi2.x

print("Punto trovato minimizzando la likelihood: ", theta_start)
print("Punto trovato minimizzando il chi-quadro: ", theta_start_chi2)
print("Sono uguali (entro la precisione numerica)?",
      np.allclose(theta_start, theta_start_chi2, atol=1e-3))

# %% [markdown]
# I due procedimenti convergono, come previsto, praticamente allo stesso
# punto: e' la conferma pratica che, con errori gaussiani, i due approcci
# (massimizzare la likelihood o minimizzare il chi-quadro) sono
# equivalenti. Nel resto dello script si continua a usare `theta_start`
# (dalla likelihood), perche' e' la stessa quantita' che serve
# concettualmente all'MCMC successivo (che lavora in termini di
# probabilita' a posteriori, non di chi-quadro).

# %% [markdown]
# ## Esecuzione del sampler MCMC
#
# I walker partono in una sfera molto stretta attorno al punto ottimizzato da
# scipy (`theta_start`), invece che attorno alla stima grezza iniziale: questo
# minimizza la probabilità che qualche walker si allontani verso un minimo
# secondario durante il campionamento. Una catena di 6000 step è sufficiente
# per una stima affidabile del tempo di autocorrelazione, grazie alla buona
# inizializzazione.

# %%
nwalkers = 32
ndim = len(theta0)
nsteps = 6000

# i 32 walker partono TUTTI vicinissimi a theta_start (il punto ottimizzato
# da scipy): np.random.randn genera rumore gaussiano standard (media 0,
# dev.std 1) per ciascun walker e parametro, moltiplicato per 1e-5 lo rende
# minuscolo, cosi' la dispersione iniziale resta piccola ma non nulla
# (emcee richiede walker leggermente diversi tra loro per funzionare)
pos = theta_start + 1e-5 * np.random.randn(nwalkers, ndim)

# costruisco il sampler: gli passo quanti walker, quante dimensioni, quale
# funzione valutare a ogni proposta di posizione (log_probability), e i
# dati fissi (args) che le vengono passati automaticamente a ogni chiamata
sampler = emcee.EnsembleSampler(nwalkers, ndim, log_probability, args=(x, y, yerr))
# lancio il campionamento vero e proprio: ogni walker fa nsteps passi,
# partendo dalle posizioni "pos"; progress=True mostra una barra di
# avanzamento (utile con 32*6000 = 192000 valutazioni della funzione)
sampler.run_mcmc(pos, nsteps, progress=True)

# %% [markdown]
# ### Andamento delle catene (trace plot)

# %%
# 4 sotto-grafici impilati verticalmente (uno per parametro), stesso asse x
fig, axes = plt.subplots(ndim, figsize=(10, 8), sharex=True)
# catena GREZZA e completa (non ripulita): voglio vedere anche l'eventuale
# transitorio iniziale, shape (nsteps, nwalkers, ndim)
chain = sampler.get_chain()
for i in range(ndim):
    ax = axes[i]
    # chain[:, :, i] fissa il parametro i, restituendo una matrice
    # (nsteps, nwalkers): passata a plot(), disegna una linea per ogni
    # walker (le colonne), mostrando come si e' mosso nel tempo.
    # alpha=0.3 rende le linee semi-trasparenti, cosi' si vede dove i
    # walker si sovrappongono (zone piu' scure = piu' accordo tra loro)
    ax.plot(chain[:, :, i], alpha=0.3, color="steelblue")
    ax.set_ylabel(labels[i])
axes[-1].set_xlabel("step")
fig.suptitle("Catene MCMC (tutti i walkers)")
fig.tight_layout()
plt.show()

# %% [markdown]
# Le catene mostrano un ottimo mescolamento (*mixing*) fin dai primi step,
# senza transitorio visibile né walker anomali: merito dell'inizializzazione
# vicina al massimo di likelihood trovato con `scipy.optimize.minimize`.

# %%
# Stima del tempo di autocorrelazione per definire burn-in e thinning

# emcee richiede, per fidarsi di questa stima, che la catena sia LUNGA
# ALMENO 50 VOLTE tau (nsteps > 50*tau): con nsteps=6000 e tau~45-50 tipici
# qui, servirebbero solo ~2250-2500 step, quindi la condizione e' ampiamente
# soddisfatta. Se non lo fosse, per default get_autocorr_time() solleva un
# errore (AutocorrError) e blocca lo script, per avvisare che la stima di
# tau (e quindi di burnin/thin sotto) sarebbe inaffidabile. quiet=True
# sopprime quell'errore, restituendo comunque la stima calcolata invece di
# interrompere l'esecuzione: qui e' un parametro puramente precauzionale,
# dato che con nsteps=6000 la condizione dei 50*tau e' gia' verificata, non
# sta quindi "nascondendo" un vero problema in questo script specifico.
tau = sampler.get_autocorr_time(quiet=True)
# regola empirica standard di emcee: dopo 3*tau step la catena ha
# sicuramente "dimenticato" la posizione di partenza -> scarto quei passi
burnin = int(3 * np.max(tau))
# quanti step servono, in media, prima che due campioni della catena
# diventino statisticamente indipendenti tra loro (un valore per parametro):
# tengo un campione ogni "thin" passi, per ridurre questa correlazione
# residua tra campioni consecutivi (uso il tau piu' piccolo, il parametro
# che decorrela piu' in fretta, dimezzato per sicurezza).
# Il max(1, ...) e' una precauzione difensiva: nel nostro caso tau e'
# sempre ~45-50, quindi thin risulta ben sopra 1 e il max() non scatta mai
# concretamente qui. Ma se tau fosse molto piccolo (<2, catena
# straordinariamente ben mescolata), 0.5*tau darebbe thin=0, un valore
# illegale per l'indicizzazione (get_chain solleverebbe ValueError:
# "slice step cannot be zero"): il max() mantiene lo script robusto
# anche in quel caso limite, senza cambiare nulla nel comportamento
# pratico con questi dati.
thin = max(1, int(0.5 * np.min(tau)))

# estraggo la catena scartando il burn-in, applicando il thinning, e
# appiattendo i 32 walker in un'unica lista di campioni (flat=True).
# get_chain() restituisce di base (senza flat) una matrice ANCORA 3D, di
# forma (nsteps_rimasti, nwalkers, ndim) = (nsteps_rimasti, 32, 4) — la
# stessa struttura a 3 dimensioni gia' vista con "chain" nel trace plot,
# solo con meno step (per via di discard e thin). Con flat=True, questa
# matrice 3D viene "spiattellata" in una matrice 2D di forma
# (nsteps_rimasti*32, 4): una riga per ciascun campione, senza piu'
# distinguere da quale walker o da quale istante provenga. Serve perche'
# da qui in poi non interessa piu' la provenienza dei campioni, ma solo
# trattarli come un'unica grande collezione di valori plausibili dal
# posteriore (per calcolare percentili, per il corner plot, per il
# fancy indexing piu' avanti).
flat_samples = sampler.get_chain(discard=burnin, thin=thin, flat=True)

print(f"Tempo di autocorrelazione stimato per parametro: {tau}")
print(f"Burn-in scartato: {burnin} step")
print(f"Thinning: {thin}")
print(f"Numero di campioni utilizzati: {len(flat_samples)}")

# %% [markdown]
# ## 4. Risultati del fit

# %%
# Il percentile p di una distribuzione di numeri e' il valore sotto il
# quale cade il p% dei campioni: es. il 50simo percentile (la mediana)
# divide i dati esattamente a meta' (50% sotto, 50% sopra). Si usano
# 16-50-84 (non a caso): per una distribuzione gaussiana, l'intervallo
# tra il 16simo e l'84simo percentile copre esattamente il 68% centrale,
# lo stesso significato di "+-1 sigma" nella statistica classica.
# axis=0 calcola i 3 percentili per TUTTI i 4 parametri insieme (una
# colonna per parametro), invece di farlo uno alla volta con un loop.
percentiles = np.percentile(flat_samples, [16, 50, 84], axis=0)  # shape (3, ndim)
# medians: il 50simo percentile (riga di indice 1) per ciascun parametro,
# cioe' la stima "migliore" (il valore centrale della distribuzione)
medians = percentiles[1]
# lower_err: quanto scende l'incertezza verso il basso (mediana - 16simo)
lower_err = medians - percentiles[0]
# upper_err: quanto sale l'incertezza verso l'alto (84simo - mediana)
# (lower_err e upper_err sono in generale leggermente diversi tra loro,
# perche' la distribuzione a posteriori non e' perfettamente simmetrica:
# per questo si riportano come "+/-" separati, non un singolo numero)
upper_err = percentiles[2] - medians

print("Risultati del fit (mediana e intervallo di credibilita' 68%):\n")
for i, lab in enumerate(labels):
    print(f"  {lab} = {medians[i]:.4f}  (+{upper_err[i]:.4f} / -{lower_err[i]:.4f})")

# best_theta e' la MIGLIORE STIMA dei 4 parametri, risultato finale
# dell'intero percorso fatto finora:
#   1. stima grezza (theta0): punto di partenza approssimativo dai dati
#   2. pre-fit (scipy.optimize): rifinisce theta0 fino al vero
#      massimo di likelihood, ma da' solo UN numero, senza incertezze
#   3. MCMC (emcee): 32 walker esplorano l'intera distribuzione di
#      probabilita' attorno a quel punto, producendo migliaia di
#      campioni plausibili (non un solo numero)
#   4. ripulitura (burn-in, thin, flat): isola i campioni davvero
#      indipendenti e statisticamente validi (flat_samples)
#   5. percentili (16-50-84): riassumono quei campioni in un numero
#      con incertezza per ciascun parametro
best_theta = medians

# %% [markdown]
# ### Distribuzioni a posteriori (corner plot)
#
# Quando disegna il grafico, `corner.corner` deve decidere quanto "zoomare"
# su ciascun asse. Di default usa il range EFFETTIVAMENTE esplorato dai
# campioni (min-max di `flat_samples`), non il range del prior — quindi,
# a fit gia' pulito (nessun walker bloccato in minimi secondari, come
# verificato in precedenza), il grafico risulterebbe gia' leggibile anche
# senza specificare nulla. Si calcola comunque un range "su misura" come
# rete di sicurezza: se anche un solo campione residuo finisse fuori dalla
# vera zona di interesse (es. per un walker che si allontana solo
# temporaneamente durante il campionamento), baserebbe ad allargare il
# range effettivo dei dati e schiacciare l'istogramma contro un bordo. Il
# range calcolato sui percentili 0.5%-99.5% scarta automaticamente questi
# eventuali outlier, garantendo un grafico leggibile in ogni caso.

# %%
# Passo 1: per ciascuno dei 4 parametri, trovo due soglie con
# np.percentile: il valore sotto cui cade lo 0.5% dei campioni, e quello
# sotto cui cade il 99.5%. E' il range che contiene il 99% centrale dei
# campioni, scartando solo l'1% piu' estremo (gli eventuali outlier).
# axis=0 calcola questo per tutti e 4 i parametri insieme, senza loop.
# Il risultato ha pero' shape (2, ndim): riga 0 = tutte le soglie basse,
# riga 1 = tutte le soglie alte. corner.corner richiede invece il formato
# [min,max] per ciascun parametro (shape (ndim, 2)): .T (trasposizione)
# scambia righe e colonne per ottenere quel formato, senza cambiare i
# valori, solo la loro disposizione nella tabella.
range_custom = np.percentile(flat_samples, [0.5, 99.5], axis=0).T  # shape (ndim, 2)

# corner.corner costruisce automaticamente una griglia NxN (N=4 qui):
# sulla diagonale, l'istogramma di ciascun parametro preso singolarmente
# (quanto e' probabile ciascun valore); fuori diagonale, uno scatter/
# contour 2D per ogni coppia di parametri, per mostrare le eventuali
# correlazioni tra loro (es. se P e phi si muovono insieme).
fig = corner.corner(
    flat_samples,          # i dati veri: righe=campioni, colonne=parametri
    labels=labels,         # nomi da scrivere sugli assi: "A", "P", "phi", "C"
    truths=best_theta,     # le 4 mediane: disegna una linea blu di
                            # riferimento su ogni grafico, per vedere subito
                            # dove cade il valore migliore
    quantiles=[0.16, 0.5, 0.84],  # aggiunge linee tratteggiate verticali
                                    # sugli istogrammi, agli stessi 3
                                    # percentili gia' usati per calcolare
                                    # medians/lower_err/upper_err: mostra
                                    # visivamente la stessa informazione
                                    # numerica gia' calcolata prima
    show_titles=True,      # stampa sopra ogni istogramma il valore con
                            # la sua incertezza, es. "A = 1.498 +0.006/-0.006"
    range=range_custom,    # applica lo zoom "su misura" calcolato sopra,
                            # invece del range enorme del prior
)
plt.show()

# %% [markdown]
# Le distribuzioni a posteriori sono unimodali e ben definite. Si osserva una
# correlazione positiva attesa tra periodo, fase e offset: una lieve variazione
# del periodo viene compensata da un aggiustamento di fase (e in minima parte
# dell'offset) per continuare a descrivere bene gli stessi dati.

# %% [markdown]
# ### Dati e curva di best fit

# %%
# asse x piu' fitto dei dati originali (2000 punti vs 1000), per disegnare
# una curva liscia invece che leggermente "spezzata"
x_fine = np.linspace(x.min(), x.max(), 2000)
# curva "centrale" del modello, calcolata con i parametri di best fit (mediane)
y_fit = model(best_theta, x_fine)

# banda di incertezza al 68% campionando dal posteriore (calcolo vettorizzato,
# senza loop Python: sfrutto il broadcasting di numpy sulle 200 combinazioni
# di parametri estratte a caso dal posteriore)

# 200 indici casuali tra tutti i campioni MCMC disponibili
inds = np.random.randint(len(flat_samples), size=200)
# fancy indexing: seleziono quelle 200 righe da flat_samples (200 set
# completi di parametri). Ogni riga contiene i valori (A, P, phi, C) che
# un walker ha davvero visitato durante il suo movimento nel campionamento
# MCMC: non sono numeri inventati qui, ma posizioni gia' esplorate e
# salvate dal sampler. Ogni colonna corrisponde a un parametro specifico
# (colonna 0 = A, colonna 1 = P, colonna 2 = phi, colonna 3 = C).
theta_batch = flat_samples[inds]                     # shape (200, ndim)
# trasposizione + unpacking: separo i 4 parametri in 4 array indipendenti da 200 valori ciascuno
A_b, P_b, phi_b, C_b = theta_batch.T                 # 4 array di shape (200,)
# [:, None] trasforma ogni parametro in colonna (200,1), x_fine[None,:] resta
# riga (1,2000): il broadcasting numpy calcola tutte le 200x2000 combinazioni
# in un colpo solo, senza bisogno di un loop Python esplicito
# Riepilogo concettuale: 200 e' quante COMBINAZIONI diverse di (A, P, phi,
# C) scelgo a caso tra quelle gia' esplorate dai walker MCMC. Ogni
# combinazione, applicata al modello, genera una CURVA sinusoidale
# completa (con la sua specifica ampiezza, periodo, fase, offset). Ogni
# curva viene poi VALUTATA sugli stessi 2000 punti di x_fine. Il risultato
# finale (y_samples) e' quindi una matrice con 200 righe (le curve) e 2000
# colonne (i punti x): 400000 valori di y in totale, non piu' i 4
# parametri di partenza.
y_samples = (A_b[:, None] * np.sin(2 * np.pi * x_fine[None, :] / P_b[:, None]
             + phi_b[:, None]) + C_b[:, None])         # shape (200, 2000)
# per ogni punto x (ogni colonna), calcolo il 16° e 84° percentile tra le 200
# curve: ottengo il bordo inferiore/superiore della banda di incertezza al 68%.
# np.percentile con [16, 84] restituirebbe di per se' una matrice (2, 2000)
# (riga 0 = tutti i percentili bassi, riga 1 = tutti quelli alti); l'assegnazione
# multipla "y_lo, y_hi = ..." la spacchetta in due array separati, ciascuno
# di shape (2000,) — uno per ogni punto di x_fine, non piu' una matrice unica
y_lo, y_hi = np.percentile(y_samples, [16, 84], axis=0)

fig, ax = plt.subplots(figsize=(10, 6))
ax.errorbar(x, y, yerr=yerr, fmt=".", color="gray", ecolor="lightgray",
            alpha=0.4, markersize=3, label="Dati")
ax.plot(x_fine, y_fit, color="crimson", lw=2, label="Best fit (mediana)")
ax.fill_between(x_fine, y_lo, y_hi, color="crimson", alpha=0.25, label="Banda 68%")
ax.set_xlabel("x")
ax.set_ylabel("y")
ax.set_title("Fit sinusoidale con MCMC (emcee)")
ax.legend()
fig.tight_layout()
plt.show()

# %% [markdown]
# ### Bontà del fit: $\chi^2$ ridotto
#
# La "bontà del fit" e' un controllo, complementare a tutto il resto
# (bayesiano) fatto finora, che risponde alla domanda: per ciascun punto
# dato, il modello ci passa vicino quanto ci si aspetterebbe, dato il
# rumore dichiarato (yerr)? Si calcola lo scarto tra dato osservato e
# valore previsto dal modello, lo si eleva al quadrato, lo si pesa in
# base a quanto ci si fidava di quella misura (yerr), e si somma su tutti
# i punti: e' il chi-quadro. Dividendolo per i gradi di liberta' (punti
# meno parametri) si ottiene il chi-quadro RIDOTTO, un numero facilmente
# interpretabile:
#   - chi2_ridotto ≈ 1: il modello descrive i dati esattamente come ci si
#     aspetterebbe dal solo rumore statistico -- un buon fit
#   - chi2_ridotto >> 1: il modello NON descrive bene i dati (residui
#     troppo grandi rispetto agli errori dichiarati) -- forse il modello
#     e' sbagliato, o gli errori sono sottostimati
#   - chi2_ridotto << 1: i residui sono piu' piccoli di quanto atteso --
#     spesso indica che gli errori dichiarati erano sovrastimati

# %%
A_fit, P_fit, phi_fit, C_fit = best_theta
chi2 = np.sum((y - model(best_theta, x)) ** 2 / yerr ** 2)
dof = len(x) - ndim
chi2_red = chi2 / dof

print(f"Ampiezza:  A   = {A_fit:.3f}")
print(f"Periodo:   P   = {P_fit:.3f}")
print(f"Fase:      phi = {phi_fit:.3f} rad")
print(f"Offset:    C   = {C_fit:.3f}")
print(f"\nChi-quadro: {chi2:.1f}, gradi di liberta': {dof}")
print(f"Chi-quadro ridotto: {chi2_red:.3f}")

# %% [markdown]
# ## Commento ai risultati
#
# Il modello sinusoidale $y = A\sin(2\pi x/P + \varphi) + C$ descrive molto bene
# l'andamento dei dati: la curva di best fit segue fedelmente le oscillazioni
# osservate e la banda di incertezza al 68% è stretta attorno alla curva, segno
# che i parametri sono ben vincolati dai dati disponibili (1000 punti).
#
# Il modello sinusoidale presenta minimi secondari nella likelihood, dovuti alla
# periodicità di fase e periodo: senza precauzioni, alcuni walker rischierebbero
# di restare intrappolati in soluzioni sub-ottimali per l'intera catena. Per
# evitarlo si è usato `scipy.optimize.minimize` per rifinire la stima iniziale
# fino al vero massimo di likelihood, e poi inizializzato l'intero ensemble di
# walker in una sfera molto stretta attorno a questo punto: tutti i walker
# partono così già nella regione corretta, e la catena converge in modo pulito
# fin dal primo step, come si osserva nel trace plot.
#
# Le distribuzioni a posteriori sono unimodali e ben definite, con una
# correlazione positiva attesa tra periodo, fase e offset: una lieve variazione
# del periodo viene compensata da un aggiustamento di fase (e in minima parte
# dell'offset) per continuare a descrivere bene gli stessi dati.
#
# Il $\chi^2$ ridotto ottenuto è significativamente inferiore a 1. Questo indica
# che gli errori sperimentali `yerr` forniti nel dataset sono probabilmente
# sovrastimati rispetto alla dispersione reale dei dati attorno al modello: il
# fit è comunque corretto e i parametri stimati (in particolare periodo e
# ampiezza) sono affidabili, ma l'incertezza statistica associata è
# verosimilmente più piccola di quanto suggerito da un $\chi^2$ ridotto pari a 1.
