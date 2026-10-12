import pickle

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

G = 9.81  # accelerazione di gravita' [m/s^2]


class Projectile:

    def __init__(self, mass, x0, y0, speed0, angle0_deg):

        if mass <= 0:
            raise ValueError("mass deve essere positiva.")
        if y0 < 0:
            raise ValueError("y0 deve essere >= 0.")
        if speed0 < 0:
            raise ValueError("speed0 deve essere >= 0.")

        self.mass = mass
        self.x0 = x0
        self.y0 = y0
        self.vx0 = speed0 * np.cos(np.radians(angle0_deg))  # cos/sin input in radianti
        self.vy0 = speed0 * np.sin(np.radians(angle0_deg))

        self.exploded = False
        self.t_exp = None
        self.frags_mass = None
        self.frags_x0 = self.frags_y0 = None
        self.frags_vx0 = self.frags_vy0 = None

        # -- traiettorie calcolate: inizialmente None, popolate solo --
        # dopo aver chiamato mother_evolution / fragments_evolution.
        # x_mother, y_mother: forma (n_points,) - traiettoria della madre
        # x_frags, y_frags: forma (N, n_points) - traiettorie dei frammenti
        self.x_mother = self.y_mother = None
        self.x_frags = self.y_frags = None

    def print_info(self):
        """Stampa un riepilogo leggibile dello stato del proiettile:
        condizioni iniziali di lancio e, se gia' esploso, i dati di
        tutti i frammenti.
        """
        print(f"Projectile: mass = {self.mass} kg")
        print(f"  posizione iniziale: (x0, y0) = ({self.x0}, {self.y0}) m")
        print(f"  velocita' iniziale: (vx0, vy0) = ({self.vx0:.3f}, {self.vy0:.3f}) m/s")

        if not self.exploded:
            print("  non ancora esploso")
        else:
            N = len(self.frags_x0)
            print(f"  esploso a t_exp = {self.t_exp:.3f} s in N = {N} frammenti")
            print(f"  massa di ciascun frammento: {self.frags_mass[0]:.3f} kg")
            for i in range(N):
                print(
                    f"    frammento {i + 1}: posizione = "
                    f"({self.frags_x0[i]:.3f}, {self.frags_y0[i]:.3f}) m, "
                    f"velocita' = ({self.frags_vx0[i]:.3f}, {self.frags_vy0[i]:.3f}) m/s"
                )

    @staticmethod
    def calculate_position(t, t0, x0, y0, vx0, vy0):
        """Calcola la posizione (x, y) di uno o piu' oggetti, dato il
        tempo t, l'istante di nascita t0, e le condizioni iniziali x0,
        y0, vx0, vy0.

        SIGNIFICATO DI N E M USATI NEI COMMENTI DI QUESTO METODO:
            N = numero di frammenti (quanti oggetti stiamo calcolando
                insieme)
            M = numero di punti temporali per ciascun frammento
                (n_points, la "risoluzione" della griglia di tempi)
        Una forma (N, M) e' quindi una matrice con N righe (una per
        frammento) e M colonne (un istante di tempo diverso per
        colonna) - t_frags[i, j] e' "il j-esimo istante di tempo, nella
        griglia del frammento i".

        QUESTO METODO NON SA se sta calcolando per un solo oggetto (la
        madre) o per N oggetti insieme (i frammenti) - riceve solo
        numeri/array e applica la formula. E' compito di CHI CHIAMA il
        metodo preparare gli argomenti nella forma giusta PRIMA di
        passarli qui dentro.

        DUE MODI IN CUI VIENE CHIAMATO:

        (A) Per la madre (un solo oggetto): tutti gli argomenti sono
            SCALARI semplici (numeri puri). Esempio:
                calculate_position(t_exp, 0, self.x0, self.y0, self.vx0, self.vy0)

        (B) Per gli N frammenti insieme: t ha gia' forma (N, M) - una
            riga per frammento, una colonna per ciascuno degli M punti
            temporali di quel frammento. QUESTA FORMA VIENE COSTRUITA
            FUORI da questo metodo, dentro fragments_evolution, con
            np.linspace(t_exp, frags_t_ground, n_points).T - dove
            frags_t_ground e' un array di N valori (il tempo di
            atterraggio di ciascun frammento), quindi linspace produce
            N sequenze diverse in parallelo (una per frammento),
            trasposte per avere N righe. x0, y0, vx0, vy0 sono invece
            array di forma (N, 1) - preparati DA FUORI con
            [:, np.newaxis] prima di chiamare questo metodo.
            Esempio:
                calculate_position(
                    t_frags,                        # (N, M) - vedi sopra
                    self.t_exp,                     # scalare
                    self.frags_x0[:, np.newaxis],   # (N, 1) <- trasformato FUORI
                    self.frags_y0[:, np.newaxis],   # (N, 1)
                    self.frags_vx0[:, np.newaxis],  # (N, 1)
                    self.frags_vy0[:, np.newaxis],  # (N, 1)
                )
        """

        # -- garantisco che t abbia ALMENO 1 dimensione --
        # Uno scalare puro (numero Python, o array numpy 0-dimensionale,
        # forma "()") NON e' considerato un array vero da numpy - non
        # supporta l'indicizzazione (x[mask]=... piu' sotto darebbe
        # errore su un numero puro, perche' un numero puro non ha
        # "elementi" al suo interno da selezionare). Per essere trattato
        # come un array vero, uno scalare deve avere ALMENO 1 dimensione.
        # ndmin=1 fa esattamente questo: CONVERTE uno scalare puro (0
        # dimensioni) in un array di UN elemento (1 dimensione, forma
        # (1,)) - stesso valore numerico, ma ora e' "un array con dentro
        # un numero", non "un numero nudo". Se t arriva gia' come array
        # (caso B, forma (N,M)), questa riga non cambia nulla - resta
        # (N,M), perche' ha gia' almeno 1 dimensione.
        t = np.array(t, ndmin=1, dtype=float)

        # -- dt eredita SEMPRE la forma di t --
        # t0 e' sempre uno scalare, quindi "t - t0" non cambia la forma
        # di t: sottrarre uno scalare non aggiunge ne' toglie dimensioni.
        #   caso A: dt forma (1,)      caso B: dt forma (N,M)
        dt = t - t0

        # -- x nasce da un vero BROADCASTING ARITMETICO tra x0/vx0*dt --
        # x0 rappresenta una posizione: uno stato che CAMBIA nel tempo,
        # quindi la formula deve SOMMARE "dove si trovava" (x0) a
        # "quanto si e' mosso" (vx0*dt) - e' un calcolo vero, non una
        # semplice ripetizione di un valore costante.
        #   caso A: x0, vx0 scalari, dt (1,)   --> x resta (1,)
        #   caso B: x0, vx0 (N,1), dt (N,M)     --> numpy "ripete" la
        #           singola colonna di x0/vx0 lungo le M colonne di dt
        #           (broadcasting automatico) --> x diventa (N,M),
        #           una posizione per ogni coppia (frammento, istante)
        # Questa riga non richiede aiuto esplicito (ne' newaxis ne'
        # broadcast_to): +, *, - fanno broadcasting da soli quando le
        # forme sono compatibili.
        x = x0 + vx0 * dt

        # -- y, stessa identica logica di x, con il termine gravita' --
        #   caso A: y forma (1,)      caso B: y forma (N,M)
        y = y0 + vy0 * dt - 0.5 * G * dt ** 2

        # -- la maschera: perche' PUO' servire allargarla, e a cosa
        #    serve comunque il NaN --
        #
        # PERCHE' t E x POTREBBERO AVERE FORME DIVERSE (in generale):
        # Guarda le righe sopra: dt eredita SEMPRE la forma di t (t0 e'
        # scalare, sottrarlo non cambia la forma). Ma x = x0 + vx0*dt
        # e' un vero broadcasting: se x0/vx0 hanno una forma "piu'
        # grande" di dt lungo qualche dimensione (es. x0 di forma
        # (N,1) e dt di forma (M,), che si combinano in (N,M)), allora
        # x FINISCE PER AVERE UNA FORMA PIU' GRANDE DI t. Il metodo non
        # sa in anticipo se questo capitera' o no - dipende da come chi
        # lo chiama ha preparato gli argomenti.
        #
        # NEL CASO D'USO ATTUALE di questo codice (fragments_evolution),
        # t arriva GIA' con forma (N,M) grazie al trucco di np.linspace
        # con un array come destinazione (vedi fragments_evolution) -
        # quindi qui x e t hanno gia' la stessa forma, e "t < t0" ha
        # gia' da solo la forma corretta (N,M), identica a x. Ma se in
        # futuro si tornasse a costruire t come una griglia PIU' PICCOLA
        # (comune a piu' frammenti, come in una versione precedente di
        # questo esercizio dove tutti condividevano la stessa griglia
        # fino al frammento piu' longevo), "t < t0" avrebbe una forma
        # piu' piccola di x - e x[mask]=... con una maschera piu'
        # piccola NON darebbe errore, ma numpy la interpreterebbe come
        # "seleziona RIGHE intere" invece che "seleziona singole celle",
        # producendo un risultato SBAGLIATO IN SILENZIO.
        # np.broadcast_to(t < t0, x.shape) e' la garanzia che, qualunque
        # sia la forma di partenza di t, la maschera venga sempre
        # allargata esattamente alla forma di x prima di essere usata -
        # nel caso attuale non cambia nulla (le forme coincidono gia'),
        # ma protegge il metodo se venisse richiamato diversamente.
        #
        # A COSA SERVE IL NaN, IN GENERALE: anche se nella pratica
        # attuale "t < t0" e' sempre False (ogni oggetto viene valutato
        # solo su tempi che partono dalla sua nascita), questa riga
        # resta utile come rete di sicurezza contro ERRORI - per
        # esempio, se per sbaglio si passasse al metodo un tempo
        # antecedente alla nascita di un oggetto (l'oggetto non esiste
        # ancora) o, in un contesto diverso, un tempo troppo avanzato
        # rispetto a quando l'oggetto smette di esistere (es. dopo che
        # ha gia' toccato terra). In quei casi, invece di restituire un
        # numero "valido ma sbagliato" (calcolato applicando comunque la
        # formula, come se l'oggetto fosse sempre esistito), il metodo
        # restituisce NaN: un segnale esplicito che quel punto non ha
        # senso fisico, facile da individuare (sia stampando i numeri
        # sia guardando un grafico, dove i punti NaN vengono saltati
        # automaticamente da matplotlib) invece di passare inosservato.
        mask = np.broadcast_to(t < t0, x.shape)
        x[mask] = np.nan
        y[mask] = np.nan
        return x, y

    @staticmethod
    def calculate_velocity(t, t0, vx0, vy0):
        """Calcola la velocita' (vx, vy) di uno o piu' oggetti, dato il
        tempo t e l'istante di nascita t0.

        SIGNIFICATO DI N E M (identico a calculate_position):
            N = numero di frammenti, M = numero di punti temporali per
            ciascun frammento (n_points). Una forma (N, M) e' una
            matrice con N righe (una per frammento) e M colonne (un
            istante di tempo diverso per colonna).

        DUE MODI IN CUI VIENE CHIAMATO (stessi due casi di
        calculate_position):

        (A) Per la madre (un solo oggetto): t0, vx0, vy0 sono SCALARI.
            Esempio: calculate_velocity(t_exp, 0, self.vx0, self.vy0)

        (B) Per gli N frammenti insieme: t ha gia' forma (N, M) -
            costruita FUORI da questo metodo, dentro fragments_evolution
            (vedi calculate_position per i dettagli di come nasce
            questa forma) - mentre vx0/vy0 hanno forma (N, 1),
            preparati DA FUORI con [:, np.newaxis] prima di chiamare
            questo metodo.
        """

        # -- RIGA 1: garantisco che t abbia ALMENO 1 dimensione --
        # Stesso identico motivo di calculate_position: uno scalare puro
        # (0 dimensioni) non e' un array vero per numpy, e vx[mask]=...
        # piu' sotto darebbe errore su un numero puro. ndmin=1 CONVERTE
        # uno scalare puro (0 dimensioni) in un array di UN elemento (1
        # dimensione) - solo cosi' viene "visto" come un array e
        # supporta l'indicizzazione.
        #   caso A: t forma () --> convertita a (1,)
        #   caso B: t forma (N,M) --> invariata (ha gia' almeno 1 dimensione)
        t = np.array(t, ndmin=1, dtype=float)

        # -- RIGA 2: dt eredita SEMPRE la forma di t --
        # t0 e' sempre uno scalare, quindi non cambia la forma.
        #   caso A: dt forma (1,)      caso B: dt forma (N,M)
        dt = t - t0

        # -- RIGA 3: vx - QUI NON e' un broadcasting aritmetico come in
        #    calculate_position, ma un RIEMPIMENTO --
        #
        # PERCHE' LA FORMULA E' DIVERSA DA x0 + vx0*dt: vx0 rappresenta
        # la velocita' ORIZZONTALE, che per la fisica del problema NON
        # CAMBIA MAI nel tempo (nessuna forza orizzontale, solo la
        # gravita' che agisce sulla componente verticale). Quindi
        # v_x(t) = v_x0 per OGNI istante t: non c'e' nulla da "sommare"
        # o calcolare, il valore vx0 e' gia' il risultato finale per
        # qualunque t - devi solo "copiarlo" per ogni istante richiesto.
        #
        # np.full_like(dt, vx0) fa esattamente questo: prende SOLO LA
        # FORMA di dt (non il suo contenuto numerico) e la riempie
        # interamente con il valore (o i valori) di vx0.
        #
        #   caso A: dt e' (1,), vx0 e' uno scalare
        #           --> vx = full_like(dt, vx0) ha forma (1,)
        #           esempio: dt=[0.5], vx0=22.9 --> vx=[22.9]
        #
        #   caso B: dt e' (N,M), vx0 e' (N,1)
        #           --> vx = full_like(dt, vx0) ha forma (N,M)
        #           ATTENZIONE: qui full_like riempie CIASCUNA RIGA con
        #           il vx0 CORRISPONDENTE a quella riga (numpy applica
        #           vx0 riga per riga, rispettando la sua forma (N,1)),
        #           non un unico valore per tutta la matrice
        #           esempio con N=2, M=3, vx0=[[10.0],[20.0]]:
        #             vx = [[10.0, 10.0, 10.0],
        #                   [20.0, 20.0, 20.0]]
        #           ogni frammento mantiene la propria velocita'
        #           orizzontale costante, ripetuta per tutti i tempi
        vx = np.full_like(dt, vx0)

        # -- RIGA 4: vy - qui SI' e' un vero calcolo aritmetico --
        # vy0 rappresenta la velocita' VERTICALE, che INVECE cambia nel
        # tempo per via della gravita': v_y(t) = v_y0 - g*dt. E' una
        # formula analoga a quella di y in calculate_position (stessa
        # fisica), ma qui non c'e' nessun "punto di partenza + broadcasting
        # che fa crescere la forma": vy0 ha gia' la stessa forma "colonna"
        # di vx0 (scalare nel caso A, (N,1) nel caso B), e dt ha gia' la
        # forma finale che si vuole ((1,) o (N,M)) - quindi la sottrazione
        # produce direttamente vy della forma giusta, senza sorprese.
        #   caso A: vy forma (1,)      caso B: vy forma (N,M)
        vy = vy0 - G * dt

        # -- RIGA 5: la maschera - perche' QUI NON serve broadcast_to,
        #    a differenza di calculate_position --
        #
        # CONFRONTO DIRETTO CON calculate_position:
        #   in calculate_position, x nasceva da x0 + vx0*dt: x0 aveva
        #   forma (N,1), dt aveva forma (N,M) - numpy li COMBINAVA con
        #   broadcasting aritmetico, e il risultato x "cresceva" alla
        #   forma (N,M), PIU' GRANDE della sola forma di t (che era
        #   solo la base di dt, non ancora "arricchita" da x0).
        #   La maschera "t < t0" ereditava invece SOLO la forma di t,
        #   quindi poteva restare piu' PICCOLA di x - da qui il bisogno
        #   di broadcast_to per allargarla esplicitamente.
        #
        #   QUI in calculate_velocity, invece: vx nasce da full_like(dt,
        #   vx0) - che per definizione ha SEMPRE la stessa forma di dt,
        #   MAI piu' grande. vy nasce da vy0 - G*dt - anche qui la forma
        #   finale e' sempre quella di dt, senza mai "crescere" oltre.
        #   Quindi vx e vy hanno SEMPRE, per costruzione, la stessa
        #   forma di dt - che e' la stessa forma di t.
        #
        #   La maschera "mask = t < t0" eredita anch'essa la forma di t.
        #   Quindi: mask, vx e vy hanno TUTTI E TRE la stessa forma,
        #   fin dall'inizio, senza mai bisogno di espanderne una per
        #   farla combaciare con le altre - a differenza del caso di
        #   calculate_position, dove x "correva avanti" (diventava piu'
        #   grande) rispetto alla maschera.
        #
        #   caso A: mask (1,), vx (1,), vy (1,)     --> gia' identiche
        #   caso B: mask (N,M), vx (N,M), vy (N,M)  --> gia' identiche
        #
        # Come per calculate_position, resta comunque un controllo di
        # sicurezza che nella pratica attuale non e' mai attivo (t < t0
        # e' sempre False, dato che ogni oggetto viene valutato solo su
        # tempi successivi alla propria nascita - vedi spiegazione
        # completa in calculate_position).
        mask = t < t0
        vx[mask] = np.nan
        vy[mask] = np.nan
        return vx, vy

    @staticmethod
    def time_to_ground(y0, vy0, t0):
        """Istante assoluto (nel tempo della simulazione) in cui uno o
        piu' oggetti toccano terra (y=0).

        Se y0, vy0 sono SCALARI (un solo oggetto - tipicamente la madre),
        restituisce l'ISTANTE ASSOLUTO SINGOLO in cui quell'oggetto
        tocca terra.

        Se y0, vy0 sono ARRAY di N elementi (N oggetti insieme - i
        frammenti), restituisce un array di N ISTANTI ASSOLUTI, uno per
        ciascun oggetto: il risultato in posizione i e' l'istante in cui
        l'oggetto i-esimo (con la sua altezza y0[i] e velocita' vy0[i])
        tocca terra - tutti calcolati in un'unica chiamata, senza ciclo.

        In entrambi i casi, risolve l'equazione di secondo grado
            -0.5*g*dt^2 + vy0*dt + y0 = 0
        con la formula quadratica, e sceglie la soluzione fisicamente
        sensata (dt > 0, cioe' nel futuro rispetto alla nascita).
        """
        a = -0.5 * G                    # a: scalare puro (dipende solo da G)
        b = vy0                          # b: stessa forma di vy0 (scalare o (N,))
        c = y0                           # c: stessa forma di y0 (scalare o (N,))
        disc = b ** 2 - 4 * a * c        # disc: stessa forma di b/c (scalare o (N,))

        # -- controllo: il discriminante deve essere >= 0, altrimenti --
        # l'equazione non ha soluzioni reali: significherebbe che
        # l'oggetto (dati y0, vy0) non tocca MAI terra. Nel nostro
        # problema (y0 sempre >= 0, gravita' che tira sempre verso il
        # basso) questo non dovrebbe MAI succedere per motivi fisici -
        # ma un discriminante che dovrebbe essere ESATTAMENTE 0 puo'
        # risultare, per errori di arrotondamento nei calcoli in
        # virgola mobile, come un numero leggermente negativo (es.
        # -1e-15 invece di 0). Per questo la soglia non e' "disc < 0"
        # ma "disc < tolerance" (tolerance = -1e-9): un valore appena
        # sotto zero viene tollerato come "rumore numerico", mentre un
        # valore DAVVERO negativo (un problema fisico vero, non un
        # arrotondamento) fa scattare l'errore.
        # np.any(...) serve perche' disc puo' essere un array (N,) nel
        # caso dei frammenti: basta che UNO SOLO dei valori sia sotto
        # soglia perche' l'intero controllo debba fermare l'esecuzione
        # (se anche un solo frammento non toccasse mai terra, l'intera
        # simulazione non avrebbe senso).
        tolerance = -1e-9
        if np.any(disc < tolerance):
            raise ValueError(
                "Discriminante negativo: l'oggetto non tocca mai terra con i dati forniti."
            )

        # -- np.maximum(disc, 0.0): "pulisce" i minuscoli valori --
        # negativi dovuti ad arrotondamento (quelli tollerati sopra,
        # tra tolerance e 0), riportandoli esattamente a 0.0 prima di
        # calcolarne la radice quadrata. Senza questa riga, np.sqrt di
        # un numero leggermente negativo (es. -1e-12) darebbe NaN o un
        # warning, anche se il valore "vero" (fisicamente) dovrebbe
        # essere 0. E' un secondo livello di sicurezza rispetto al
        # controllo sopra: quello BLOCCA i casi davvero anomali, questo
        # ADDOMESTICA i casi limite dovuti solo a precisione numerica.
        #
        # ALTERNATIVA EQUIVALENTE: np.clip(disc, 0.0, None) darebbe
        # esattamente lo stesso risultato. np.maximum(a, b) confronta
        # DUE valori/array elemento per elemento e tiene il piu' grande
        # (pensato per confrontare due quantita' tra loro); np.clip(a,
        # minimo, massimo) invece "taglia" un array dentro un intervallo
        # [minimo, massimo] (con None per un lato senza limite). Qui
        # coincidono perche' il caso d'uso e' un caso particolare di
        # entrambi: "porta tutto cio' che e' sotto zero a zero, senza
        # limite superiore". Si preferisce np.maximum perche' comunica
        # piu' direttamente l'intento ("confronta con la soglia zero"),
        # mentre np.clip risulterebbe meno immediato da leggere per chi
        # non e' abituato a vedere None come "nessun limite" su un lato.
        disc = np.maximum(disc, 0.0)     # disc: forma invariata (scalare o (N,))

        dt1 = (-b + np.sqrt(disc)) / (2 * a)  # dt1: forma di disc (scalare o (N,))
        dt2 = (-b - np.sqrt(disc)) / (2 * a)  # dt2: forma di disc (scalare o (N,))

        # -- dt = np.maximum(dt1, dt2): scelgo la soluzione POSITIVA --
        # L'equazione di secondo grado ha sempre DUE soluzioni (dt1 e
        # dt2), ma solo UNA ha senso fisico: quella con dt > 0 (un
        # istante nel FUTURO rispetto alla nascita dell'oggetto).
        # L'altra soluzione e' negativa (o comunque minore) - corrisponde
        # matematicamente a un punto della stessa parabola, ma proiettato
        # "all'indietro nel tempo", PRIMA che l'oggetto nascesse: non ha
        # alcun significato fisico nel nostro problema.
        # Perche' np.maximum() sceglie sempre quella giusta: qui
        # a = -0.5*g e' NEGATIVO (la gravita' e' positiva). Dividere per
        # un numero negativo INVERTE l'ordine tra le due soluzioni -
        # quindi non puoi sapere "a priori" se sia dt1 o dt2 quella
        # positiva (dipende dai valori numerici di b, c). Calcolandole
        # entrambe e prendendone il massimo, ottieni SEMPRE quella
        # positiva/corretta, senza doverti preoccupare di quale delle
        # due formule (+ o -) la produca in un caso specifico.
        dt = np.maximum(dt1, dt2)        # dt: forma di dt1/dt2 (scalare o (N,))

        # -- return t0 + dt: perche' NON basta restituire dt da solo --
        #
        # dt rappresenta un tempo RELATIVO: "quanto tempo passa, contando
        # da zero, da quando l'oggetto e' nato, fino a quando tocca
        # terra". Ma il resto del codice lavora sempre in tempo ASSOLUTO
        # della simulazione (lo stesso "orologio" condiviso da madre e
        # frammenti, che parte da t=0 al lancio).
        #
        # Se l'oggetto e' la madre, t0=0, quindi dt e t0+dt coincidono -
        # la distinzione non si nota. MA se l'oggetto e' un frammento,
        # t0 = t_exp (es. 2.0s, l'istante dell'esplosione): se dt fosse,
        # ad esempio, 6.38 (il frammento impiega 6.38s dalla propria
        # nascita per toccare terra), restituire SOLO dt=6.38 sarebbe
        # SBAGLIATO - il frammento tocca terra a t=2.0+6.38=8.38 nel
        # tempo assoluto della simulazione, non a t=6.38 (che ignorerebbe
        # i 2 secondi gia' trascorsi prima che il frammento nascesse).
        #
        # Questo valore assoluto e' esattamente quello che serve dopo,
        # ad esempio in fragments_evolution, per costruire la griglia
        # np.linspace(self.t_exp, frags_t_ground, n_points): se
        # frags_t_ground contenesse i dt relativi invece dei tempi
        # assoluti, la griglia si fermerebbe troppo presto per ogni
        # frammento, tagliando via l'ultima parte della sua traiettoria.
        return t0 + dt                    # output: forma di dt (scalare o (N,))

    def explode(self, t_exp, N, E):
        # -- controllo: il testo chiede esplicitamente che il proiettile
        #    si rompa in "multiple parts", quindi N deve essere >= 2
        #    (N=1 non rappresenterebbe una vera esplosione: nessuna
        #    separazione tra frammenti, solo un impulso di velocita' in
        #    piu' sullo stesso oggetto) --
        if N < 2:
            raise ValueError("N deve essere >= 2 (il proiettile deve rompersi in piu' parti).")

        # -- controllo: E rappresenta l'energia RILASCIATA dall'esplosione
        #    - un'energia negativa non ha senso fisico (l'esplosione non
        #    puo' "assorbire" energia). Se non controllato, un E negativo
        #    farebbe si' che speed_fragments_cm = sqrt(2*E/mass) calcoli
        #    la radice quadrata di un numero negativo: np.sqrt NON
        #    solleva errore in quel caso, restituisce silenziosamente
        #    NaN, che si propagherebbe poi in tutti i calcoli successivi
        #    (velocita' e posizioni dei frammenti), rendendo l'intera
        #    simulazione inutilizzabile SENZA un messaggio chiaro sul
        #    perche'. Questo
        #    controllo intercetta il problema subito, con un errore
        #    esplicito, invece di lasciare che si manifesti piu' avanti
        #    come NaN silenziosi difficili da diagnosticare --
        if E < 0:
            raise ValueError("E deve essere >= 0 (l'energia rilasciata non puo' essere negativa).")

        self.t_exp = t_exp
        self.exploded = True

        # -- controllo: l'esplosione deve avvenire mentre la madre e'
        #    ancora in volo --
        #
        # RIGA 1: calcolo "se la madre non esplodesse mai, a che tempo
        # toccherebbe terra da sola?" - uso time_to_ground con le
        # condizioni di lancio della madre (self.y0, self.vy0) e t0=0
        # (la madre nasce al lancio). Il risultato e' uno scalare: un
        # singolo tempo assoluto ipotetico di atterraggio.
        #
        # RIGA 2-3: confronto t_exp (il tempo di esplosione scelto
        # dall'utente) con quel tempo ipotetico. Se t_exp e' MAGGIORE O
        # UGUALE, l'utente sta chiedendo un'esplosione che avverrebbe
        # DOPO (o esattamente quando) il proiettile avrebbe gia' toccato
        # terra - fisicamente impossibile: non puoi far esplodere in
        # aria qualcosa che e' gia' a terra. Il controllo usa ">="
        # (non solo ">"): se t_exp coincidesse esattamente con l'istante
        # di atterraggio, il proiettile sarebbe gia' a y=0, e "esplodere
        # a terra" non e' lo scenario descritto dal problema (che
        # presuppone l'esplosione in volo).
        #
        # Se il controllo fallisce, il codice si ferma con un messaggio
        # chiaro che riporta sia il valore scelto sia il limite massimo
        # consentito, invece di proseguire con calcoli che produrrebbero
        # frammenti "nati sottoterra" o comunque privi di senso fisico.
        t_ground_no_explosion = self.time_to_ground(self.y0, self.vy0, t0=0)  # scalare
        if t_exp >= t_ground_no_explosion:
            raise ValueError(
                f"t_exp={t_exp}s deve essere minore del tempo di volo "
                f"({t_ground_no_explosion:.2f}s), altrimenti la madre "
                "ha gia' toccato terra."
            )

        # x_exp, y_exp, vx_exp, vy_exp: forma (1,) - la madre e' un
        # singolo oggetto, valutato a un singolo istante t_exp
        x_exp, y_exp = self.calculate_position(t_exp, 0, self.x0, self.y0, self.vx0, self.vy0)
        vx_exp, vy_exp = self.calculate_velocity(t_exp, 0, self.vx0, self.vy0)

        # speed_fragments_cm: modulo della velocita' di CIASCUN
        # frammento, misurata nel sistema di riferimento del CENTRO DI
        # MASSA (CM) - cioe' vista da un osservatore che si muove
        # insieme al centro di massa dell'esplosione, non da un
        # osservatore fermo a terra. E' lo stesso identico modulo per
        # tutti gli N frammenti (formula data dal testo dell'esercizio),
        # cambia solo la DIREZIONE (data dagli angoli piu' sotto). Va
        # poi sommata alla velocita' della madre nel laboratorio
        # (vx_exp, vy_exp) per ottenere la velocita' finale dei
        # frammenti nel sistema di riferimento "vero" (il laboratorio).
        speed_fragments_cm = np.sqrt(2.0 * E / self.mass)  # scalare (E, mass sono scalari)

        i = np.arange(N)                          # i: forma (N,)
        angles = np.pi / 2 + 2 * np.pi * i / N    # angles: forma (N,)

        vx_cm = speed_fragments_cm * np.cos(angles)  # vx_cm: forma (N,)
        vy_cm = speed_fragments_cm * np.sin(angles)  # vy_cm: forma (N,)

        # ======================================================
        # PRINCIPIO FISICO: COMPOSIZIONE CLASSICA DELLE VELOCITA'
        # ======================================================
        #
        # vx_cm, vy_cm sono la velocita' di ciascun frammento misurata
        # da un osservatore che si muove insieme al centro di massa
        # (come se il proiettile, al momento dell'esplosione, fosse
        # fermo). Ma vogliamo la velocita' misurata da un osservatore
        # FERMO nel laboratorio (quello che vede l'intero volo dal
        # lancio in poi). Le due velocita' sono legate dal principio di
        # composizione classica (Galileiana) delle velocita':
        #
        #     v_frammento,LABORATORIO = v_CM,LABORATORIO + v_frammento,CM
        #
        # cioe': la velocita' "vera" (nel laboratorio) di un frammento
        # e' la velocita' del sistema che si muove (il centro di massa,
        # che nell'istante dell'esplosione si muove esattamente come si
        # muoveva la madre: vx_exp, vy_exp) PIU' la velocita' che il
        # frammento ha ALL'INTERNO di quel sistema in movimento (vx_cm,
        # vy_cm). E' lo stesso principio per cui, se lanci una pallina
        # in avanti da un treno in corsa, un osservatore fermo sui
        # binari la vede muoversi alla velocita' del treno PIU' quella
        # della pallina rispetto al treno - non solo a quella del treno,
        # ne' solo a quella della pallina da sola.
        # La somma e' componente per componente (x con x, y con y)
        # perche' moto orizzontale e verticale sono indipendenti.
        #
        # ============================================================
        # DEFINIZIONE: self.frags_vx0, self.frags_vy0 SONO LE CONDIZIONI
        # INIZIALI DI VELOCITA' DI CIASCUN FRAMMENTO, NEL SISTEMA FISSO
        # (IL LABORATORIO) - cioe' esattamente i valori (vx0, vy0) da
        # usare, per ciascun frammento, nelle formule del moto parabolico
        # (calculate_position, calculate_velocity), allo stesso modo in
        # cui self.vx0, self.vy0 lo sono per la madre.
        # ============================================================
        #
        # frags_vx0, frags_vy0: forma (N,) - vx_exp e' (1,), vx_cm e'
        # (N,) --> broadcasting a (N,)
        self.frags_vx0 = vx_exp + vx_cm
        self.frags_vy0 = vy_exp + vy_cm

        # ============================================================
        # DEFINIZIONE: self.frags_x0, self.frags_y0 SONO LE CONDIZIONI
        # INIZIALI DI POSIZIONE DI CIASCUN FRAMMENTO, NEL SISTEMA FISSO
        # (IL LABORATORIO) - il punto in cui e' avvenuta l'esplosione
        # (x_exp, y_exp), lo stesso per tutti i frammenti (nascono tutti
        # nello stesso punto dello spazio), replicato N volte con
        # np.full per avere un array della stessa lunghezza degli altri
        # attributi frags_*, cosi' da poter essere usato direttamente
        # come (x0, y0) nelle formule del moto per ciascun frammento.
        # ============================================================
        self.frags_x0 = np.full(N, x_exp)
        self.frags_y0 = np.full(N, y_exp)

        self.frags_mass = np.full(N, self.mass / N)  # forma (N,)

    def mother_evolution(self, n_points=500):
        t_mother = np.linspace(0, self.t_exp, n_points)  # forma (n_points,) = (M,)
        # x0, y0, vx0, vy0 sono scalari (un solo oggetto, la madre) -->
        # x_mother, y_mother hanno forma (M,), stessa forma di t_mother
        x_mother, y_mother = self.calculate_position(t_mother, 0, self.x0, self.y0, self.vx0, self.vy0)
        # -- salvo il risultato come attributi, cosi' l'oggetto "ricorda" --
        # la propria traiettoria una volta calcolata (a questa risoluzione)
        self.x_mother, self.y_mother = x_mother, y_mother
        return x_mother, y_mother

    def fragments_evolution(self, n_points=500):
        """Traiettoria di tutti gli N frammenti, ciascuno con la propria
        griglia di tempi ESATTA (da t_exp al proprio istante di
        atterraggio) - vettoriale, senza ciclo Python.

        IL TRUCCO DI np.linspace CON UN ARRAY COME "STOP":

        Se "fine" e' un ARRAY di N valori (invece di un numero singolo),
        numpy costruisce N sequenze IN PARALLELO, tutte che partono dallo
        stesso "inizio", ciascuna che arriva alla propria destinazione.

        Esempio con N=3 frammenti, 4 punti:
            np.linspace(2.0, [5.0, 6.0, 4.0], 4)
            # -> [[2. , 2.  , 2.  ],   <- RIGA 0 = passo 0: tutti i
            #                             frammenti partono da 2.0
            #     [3. , 3.33, 2.67],   <- RIGA 1 = passo 1: ciascun
            #                             frammento avanza verso la
            #                             PROPRIA destinazione
            #     [4. , 4.67, 3.33],   <- RIGA 2 = passo 2
            #     [5. , 6.  , 4.  ]]   <- RIGA 3 = passo 3 (ultimo):
            #                             ciascuno arriva esattamente
            #                             alla propria meta

        ATTENZIONE ALL'ORIENTAMENTO: LA PROGRESSIONE NEL TEMPO SI LEGGE
        SCENDENDO LUNGO LE RIGHE (dall'alto verso il basso), NON lungo
        le colonne. Infatti: seguendo la COLONNA 0 dall'alto in basso
        (2.0 -> 3.0 -> 4.0 -> 5.0) ottieni l'intero percorso completo
        del PRIMO frammento (quello che arriva a 5.0); seguendo la
        COLONNA 1 (2.0 -> 3.33 -> 4.67 -> 6.0) ottieni il percorso
        completo del secondo frammento (quello che arriva a 6.0). Quindi:
            ogni RIGA = un singolo istante di tempo, con tutti gli N
                        frammenti fotografati in quell'istante
            ogni COLONNA = l'intero percorso temporale di UN frammento,
                           dall'inizio alla fine
        Questo e' l'OPPOSTO della convenzione che usiamo nel resto della
        classe (dove vogliamo una riga per frammento, coerente con
        self.frags_x0 che ha forma (N,)). Serve quindi TRASPORRE con
        .T, che scambia righe con colonne: dopo la trasposizione, ogni
        RIGA diventa il percorso completo di un frammento specifico.
        """
        frags_t_ground = self.time_to_ground(self.frags_y0, self.frags_vy0, self.t_exp)  # (N,)

        # np.linspace(scalare, (N,), M) prima del .T produce forma
        # (M, N); dopo .T diventa (N, M) - una riga per frammento, con
        # M punti temporali ciascuna, dagli estremi t_exp -> proprio
        # atterraggio (diverso per ogni riga)
        t_frags = np.linspace(self.t_exp, frags_t_ground, n_points).T  # (N, M)

        # -- PERCHE' [:, np.newaxis] VIENE FATTO QUI, FUORI DA
        #    calculate_position, E NON DENTRO IL METODO STESSO --
        #
        # calculate_position e' un metodo GENERICO: non sa, guardando i
        # suoi parametri, se sta calcolando per un solo oggetto (la
        # madre, dove x0/vx0 sono scalari) o per N oggetti insieme (i
        # frammenti, dove x0/vx0 devono diventare colonne (N,1) per
        # combinarsi correttamente con una griglia di tempi (N,M)). Il
        # metodo riceve semplicemente "x0" e applica la formula - non
        # gli spetta decidere se quell'x0 vada rimodellato o no.
        #
        # CHI CHIAMA il metodo, invece, SA ESATTAMENTE con cosa ha a che
        # fare: qui, dentro fragments_evolution, sappiamo per certo che
        # self.frags_x0 rappresenta N frammenti (non un solo oggetto),
        # quindi sappiamo che serve trasformarlo in colonna PRIMA di
        # passarlo. E' per questo che il [:, np.newaxis] si scrive nella
        # CHIAMATA (qui sotto), non dentro calculate_position: la
        # decisione "questo array va rimodellato per il broadcasting"
        # dipende dal CONTESTO D'USO (madre singola vs N frammenti), che
        # solo chi chiama il metodo conosce - non il metodo stesso, che
        # deve restare valido per entrambi i casi senza dover indovinare.
        #
        # self.frags_x0[:, np.newaxis]: da (N,) a (N,1) - una colonna
        # x_frags, y_frags: risultato del broadcasting tra (N,1) e
        # (N,M) --> forma finale (N, M)
        x_frags, y_frags = self.calculate_position(
            t_frags,                        # (N, M)
            self.t_exp,                     # scalare
            self.frags_x0[:, np.newaxis],   # (N,1)
            self.frags_y0[:, np.newaxis],   # (N,1)
            self.frags_vx0[:, np.newaxis],  # (N,1)
            self.frags_vy0[:, np.newaxis],  # (N,1)
        )

        # -- salvo il risultato come attributi, cosi' l'oggetto "ricorda" --
        # la propria traiettoria una volta calcolata (a questa risoluzione)
        self.x_frags, self.y_frags = x_frags, y_frags

        return x_frags, y_frags  # entrambi forma (N, M)

    def plot_trajectories(self, filename):
        """Disegna su un unico grafico: la traiettoria della madre (prima
        dell'esplosione), le traiettorie di tutti i frammenti (dopo),
        il punto di lancio, il punto dell'esplosione, e i punti in cui
        ciascun frammento tocca terra. Salva il grafico nel file indicato
        da filename. Usa self.x_mother, self.y_mother, self.x_frags,
        self.y_frags - devono essere gia' stati calcolati chiamando
        prima mother_evolution() e fragments_evolution().
        """
        if self.x_mother is None or self.x_frags is None:
            raise ValueError(
                "Traiettorie non ancora calcolate: chiama prima "
                "mother_evolution() e fragments_evolution()."
            )

        # -- uso direttamente gli attributi, non parametri --
        x_mother, y_mother = self.x_mother, self.y_mother
        x_frags, y_frags = self.x_frags, self.y_frags

        fig, ax = plt.subplots(figsize=(9, 6))

        # -- traiettoria della madre --
        ax.plot(x_mother, y_mother, color="black", lw=2, label="Proiettile (pre-esplosione)")

        # -- traiettorie dei frammenti: x_frags, y_frags hanno forma
        #    (N, n_points) - una riga per frammento. Trasponendo (.T) si
        #    ottiene (n_points, N): matplotlib legge le COLONNE come
        #    serie separate, quindi una sola chiamata a plot() disegna
        #    automaticamente tutti gli N frammenti, senza ciclo --
        ax.plot(x_frags.T, y_frags.T, lw=1.5, ls="--")

        # -- punto di partenza: il PRIMO punto della traiettoria della
        #    madre, cioe' il momento del lancio (t=0) --
        ax.scatter(
            x_mother[0], y_mother[0], color="blue", marker="o", s=80,
            label="Lancio (t=0)", zorder=5,
        )

        # -- punto dell'esplosione: e' l'ultimo punto della traiettoria
        #    della madre (il momento in cui "diventa" i frammenti) --
        ax.scatter(
            x_mother[-1], y_mother[-1], color="red", marker="*", s=200,
            label=f"Esplosione (t={self.t_exp:.2f} s)", zorder=5,
        )

        # -- punti di atterraggio: l'ULTIMA colonna di x_frags/y_frags
        #    (indice -1), una per frammento, disegnati tutti insieme --
        ax.scatter(
            x_frags[:, -1], y_frags[:, -1], color="black", marker="x", s=80,
            label="Atterraggio frammenti", zorder=5,
        )

        ax.axhline(0, color="gray", lw=0.8)
        ax.set_xlabel("x [m]")
        ax.set_ylabel("y [m]")
        ax.set_title("Moto parabolico del proiettile con esplosione in N frammenti")
        ax.legend(loc="best", fontsize=9)
        ax.grid(alpha=0.3)
        ax.set_aspect("equal", adjustable="datalim")

        fig.tight_layout()
        fig.savefig(filename, dpi=150)
        plt.show()
        return fig

    # -- animate_trajectories: DISATTIVATA (commentata) --
    # Il testo dell'esame dice esplicitamente che animare non e'
    # obbligatorio ("animating may be time consuming, so it is not
    # mandatory") - era una bonus, qui disattivata per intero.
    #
    # def animate_trajectories(self, filename, n_points=100):
    #     """Anima il moto del proiettile e dei frammenti nel tempo
    #     (opzionale: il testo dell'esame dice esplicitamente che animare
    #     non e' obbligatorio, ma aggiunta come bonus). Salva l'animazione
    #     nel file indicato da filename.
    #
    #     Calcola da sola le traiettorie (con un n_points piu' basso di
    #     default rispetto al plot statico, per un file piu' leggero e
    #     un'animazione piu' scorrevole), senza bisogno di riceverle gia'
    #     pronte da fuori.
    #
    #     Prima la madre avanza lungo la sua traiettoria, poi, arrivata al
    #     punto dell'esplosione, tutti gli N frammenti avanzano insieme
    #     lungo le proprie traiettorie fino al proprio atterraggio.
    #
    #     ATTENZIONE: mother_evolution/fragments_evolution salvano il loro
    #     risultato anche in self.x_mother/self.y_mother/self.x_frags/
    #     self.y_frags (vedi __init__). Per evitare che l'animazione (che
    #     usa un n_points piu' basso) SOVRASCRIVA quegli attributi con una
    #     risoluzione peggiore rispetto a quella usata da plot_trajectories
    #     (n_points=500 di default), qui salvo prima gli attributi
    #     correnti, li ripristino subito dopo aver ottenuto x_mother, ecc.
    #     come variabili locali per l'animazione.
    #     """
    #     # -- salvo lo stato attuale degli attributi (es. quello a
    #     #    risoluzione piu' alta calcolato per il plot statico) --
    #     x_mother_backup, y_mother_backup = self.x_mother, self.y_mother
    #     x_frags_backup, y_frags_backup = self.x_frags, self.y_frags
    #
    #     x_mother, y_mother = self.mother_evolution(n_points)
    #     x_frags, y_frags = self.fragments_evolution(n_points)
    #
    #     # -- ripristino gli attributi originali: l'animazione usa solo --
    #     # le variabili locali x_mother, y_mother, x_frags, y_frags qui
    #     # sotto, senza intaccare lo stato "ufficiale" dell'oggetto
    #     self.x_mother, self.y_mother = x_mother_backup, y_mother_backup
    #     self.x_frags, self.y_frags = x_frags_backup, y_frags_backup
    #
    #     N, M = x_frags.shape
    #
    #     fig, ax = plt.subplots(figsize=(9, 6))
    #     ax.set_xlim(x_mother.min() - 5, np.nanmax(x_frags) + 10)
    #     ax.set_ylim(-5, max(y_mother.max(), np.nanmax(y_frags)) + 10)
    #     ax.axhline(0, color="gray", lw=0.8)
    #     ax.set_xlabel("x [m]")
    #     ax.set_ylabel("y [m]")
    #     ax.set_title("Animazione: moto del proiettile e dei frammenti")
    #     ax.grid(alpha=0.3)
    #
    #     # -- elementi grafici da aggiornare ad ogni frame: una linea (il
    #     #    percorso gia' fatto) e un punto (la posizione attuale) per
    #     #    la madre, e lo stesso per ciascuno degli N frammenti --
    #     mother_line, = ax.plot([], [], color="black", lw=2, label="Proiettile")
    #     mother_point, = ax.plot([], [], "o", color="black", markersize=8)
    #
    #     cmap = plt.get_cmap("tab10")
    #     frag_lines = [ax.plot([], [], lw=1.5, ls="--", color=cmap(i % 10))[0] for i in range(N)]
    #     frag_points = [ax.plot([], [], "o", color=cmap(i % 10), markersize=6)[0] for i in range(N)]
    #
    #     ax.legend(loc="best", fontsize=9)
    #
    #     n_mother_frames = len(x_mother)
    #     n_total_frames = n_mother_frames + M  # prima la madre, poi i frammenti
    #
    #     def update(frame):
    #         """Funzione richiamata da FuncAnimation per ciascun frame:
    #         aggiorna i dati di linee e punti mostrando solo i valori
    #         fino all'istante corrente (frame)."""
    #         if frame < n_mother_frames:
    #             # fase 1: la madre e' ancora in volo
    #             mother_line.set_data(x_mother[:frame + 1], y_mother[:frame + 1])
    #             mother_point.set_data([x_mother[frame]], [y_mother[frame]])
    #         else:
    #             # fase 2: la madre e' arrivata all'esplosione (resta ferma
    #             # li'), i frammenti avanzano lungo le proprie traiettorie
    #             mother_line.set_data(x_mother, y_mother)
    #             mother_point.set_data([x_mother[-1]], [y_mother[-1]])
    #             j = frame - n_mother_frames
    #             for i in range(N):
    #                 frag_lines[i].set_data(x_frags[i, :j + 1], y_frags[i, :j + 1])
    #                 frag_points[i].set_data([x_frags[i, j]], [y_frags[i, j]])
    #         return [mother_line, mother_point] + frag_lines + frag_points
    #
    #     ani = animation.FuncAnimation(
    #         fig, update, frames=n_total_frames, interval=30, blit=True
    #     )
    #     ani.save(filename, writer="pillow", fps=30)
    #     plt.close(fig)
    #     print(f"Animazione salvata in: {filename}")
    #     return ani

    def save_results(self, results_filename, projectile_filename):
        """Salva i risultati con pickle: un file (results_filename) con
        i quattro array della simulazione (nella loro forma originale,
        presi da self.x_mother, self.y_mother, self.x_frags, self.y_frags
        - devono essere gia' stati calcolati), e un file
        (projectile_filename) con l'intero oggetto Projectile
        (salvataggio di self attualmente disattivato, vedi sotto).

        Il ricaricamento NON avviene piu' qui dentro: e' stato spostato
        nei due metodi separati load_results (staticmethod) e load
        (classmethod), qui sotto - entrambi attualmente disattivati.
        """
        if self.x_mother is None or self.x_frags is None:
            raise ValueError(
                "Traiettorie non ancora calcolate: chiama prima "
                "mother_evolution() e fragments_evolution()."
            )

        results = {
            "x_mother": self.x_mother, "y_mother": self.y_mother,
            "x_frags": self.x_frags, "y_frags": self.y_frags,
        }
        with open(results_filename, "wb") as f:
            pickle.dump(results, f)
        print(f"Risultati salvati in: {results_filename}")

        # -- salvataggio dell'intero oggetto Projectile (self) --
        # disattivato: non serve salvare anche l'oggetto se bastano gli
        # array di risultati per l'uso attuale
        # with open(projectile_filename, "wb") as f:
        #     pickle.dump(self, f)
        # print(f"Oggetto Projectile salvato in: {projectile_filename}")

    # -- @staticmethod load_results: NON usa ne' self ne' cls --
    # Ricarica solo il dizionario "results" (i quattro array x_mother,
    # y_mother, x_frags, y_frags) da un file pickle. Il risultato non
    # e' un'istanza di Projectile, ma un semplice dict di array - non
    # c'e' quindi nessun bisogno di "cls" per costruire l'oggetto
    # giusto: la funzione fa solo "apri il file, leggi, restituisci",
    # esattamente come farebbe una funzione libera fuori dalla classe.
    # E' raggruppata qui dentro solo per tenerla vicina, concettualmente,
    # a save_results.
    # @staticmethod
    # def load_results(results_filename):
    #     """Ricarica il dizionario dei risultati (x_mother, y_mother,
    #     x_frags, y_frags) da file pickle, per verifica/riuso.
    #     """
    #     with open(results_filename, "rb") as f:
    #         results_reloaded = pickle.load(f)
    #     print(f"Verifica: results ricaricato ha chiavi {list(results_reloaded.keys())}")
    #     print(f"Verifica: x_frags ricaricato ha forma {results_reloaded['x_frags'].shape}")
    #     return results_reloaded

    # -- @classmethod load: usa cls, non self --
    # Ricarica l'intero oggetto Projectile da file pickle. Qui cls ha
    # senso reale: il metodo RESTITUISCE un'istanza della classe (un
    # "factory method"), quindi puo' controllare con isinstance(obj,
    # cls) che il file contenga davvero un oggetto compatibile, e
    # stampare cls.__name__ invece di scrivere "Projectile" a mano - se
    # in futuro esistesse una sottoclasse (es. ExplodingProjectile),
    # cls si adatterebbe da solo, senza bisogno di riscrivere il metodo.
    # @classmethod
    # def load(cls, projectile_filename):
    #     """Ricarica l'intero oggetto Projectile da file pickle, per
    #     verifica/riuso.
    #     """
    #     with open(projectile_filename, "rb") as f:
    #         projectile_reloaded = pickle.load(f)
    #     if not isinstance(projectile_reloaded, cls):
    #         raise TypeError(f"Il file non contiene un'istanza di {cls.__name__}.")
    #     print(f"Verifica: {cls.__name__} ricaricato ha t_exp = {projectile_reloaded.t_exp}, "
    #           f"N frammenti = {len(projectile_reloaded.frags_x0)}")
    #     return projectile_reloaded


def main():
    # -- parametri della simulazione (modificabili) --
    mass = 5.0          # massa del proiettile [kg]
    x0, y0 = 0.0, 0.0   # posizione iniziale [m]
    speed0 = 40.0        # velocita' iniziale [m/s]
    angle0_deg = 55.0    # angolo di lancio [gradi]

    t_exp = 2.0          # istante dell'esplosione [s]
    N = 4                # numero di frammenti (deve essere >= 2)
    E = 300.0            # energia rilasciata nell'esplosione [J]

    # -- nomi dei file di output (modificabili) --
    plot_filename = "trajectories.png"
    animation_filename = "animation.gif"
    results_filename = "simulation_results.pkl"
    projectile_filename = "projectile.pkl"

    # -- 1. creo il proiettile e lo faccio esplodere --
    projectile = Projectile(mass, x0, y0, speed0, angle0_deg)
    projectile.explode(t_exp, N, E)

    # -- 2. calcolo le traiettorie di madre e frammenti --
    # (i risultati vengono salvati anche come attributi dell'oggetto:
    # projectile.x_mother, projectile.y_mother, projectile.x_frags,
    # projectile.y_frags - vedi mother_evolution/fragments_evolution)
    projectile.mother_evolution()
    projectile.fragments_evolution()

    # -- 3. disegno il grafico e salvo i risultati --
    # (plot_trajectories e save_results usano direttamente gli attributi
    # salvati sopra, non serve piu' passarli come parametri)
    projectile.plot_trajectories(plot_filename)
    projectile.save_results(results_filename, projectile_filename)

    # -- 3bis. animazione (opzionale, il testo dice "not mandatory") --
    # animate_trajectories e' stata disattivata (commentata) per intero
    # sopra, quindi anche la sua chiamata qui e' disattivata
    # projectile.animate_trajectories(animation_filename)

    # -- 4. riepilogo a schermo --
    print()
    projectile.print_info()


if __name__ == "__main__":
    main()
