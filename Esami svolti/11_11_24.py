"""
galton_board.py

Implementazione della classe GaltonBoard per il compito di
Computing Methods for Physics 1 - 11 Novembre 2024 (Parte 2 - Python).

Un Galton board con N righe viene rappresentato come un array 1D di
N*(N+1)/2 elementi. La riga r (r = 0, ..., N-1) contiene r+1 bin e viene
memorizzata "appiattita" nell'array a partire dall'indice r*(r+1)/2.

Una pallina parte dalla riga 0 (bin 0) e ad ogni riga successiva si sposta
di una posizione a destra con probabilita' 1/2, oppure resta nella stessa
posizione (equivalente a "andare a sinistra") con probabilita' 1/2. Questo
riproduce la classica distribuzione binomiale/gaussiana del Galton board.

La simulazione di Nexp palline e' completamente vettorizzata: i passi
casuali di tutte le palline vengono generati in un'unica matrice e le
posizioni riga per riga si ottengono con un cumsum lungo le colonne,
senza alcun ciclo Python sulle palline o sulle righe.
"""

import pickle

import numpy as np
import matplotlib.pyplot as plt


class GaltonBoard:
    """Rappresenta un Galton board verticale con N righe."""

    def __init__(self, n_rows):
        if n_rows < 1:
            raise ValueError("n_rows deve essere un intero positivo.")

        self.n_rows = n_rows
        # numero totale di bin nell'intero triangolo: la riga r ha r+1
        # bin (r=0,...,n_rows-1), quindi la somma su tutte le righe e'
        # 1+2+...+n_rows = n_rows*(n_rows+1)/2 (somma dei primi n_rows
        # interi). E' la lunghezza dell'array piatto che rappresenta
        # l'intero board.
        # Uso // (divisione intera) e non / (divisione normale): // 
        # restituisce sempre un int, / restituisce sempre un float
        # (anche quando il risultato e' un numero esatto, es. 55.0
        # invece di 55). n_bins deve essere un int perche' viene passato
        # subito dopo a np.zeros(n_bins, ...), che richiede un intero
        # come dimensione e solleva un errore se gli si passa un float.
        n_bins = n_rows * (n_rows + 1) // 2
        # self.board e' un array 1D: NON e' mai una struttura
        # triangolare vera in memoria - il "triangolo" del Galton board
        # e' solo il modo in cui lo interpretiamo concettualmente (con
        # l'aiuto di _bin_index, che traduce riga/colonna in posizione
        # nell'array piatto). self.board ti dice "quante palline sono
        # passate per ciascun bin" - un conteggio, aggregato nel tempo
        # (somma di tutte le palline di tutte le run). Non ti dice
        # "quale pallina specifica" - solo il totale per ciascun bin.
        # E' persistente: si accumula fra chiamate diverse a
        # run_experiments, si azzera solo con reset().
        self.board = np.zeros(n_bins, dtype=int)
        # Contatore del numero totale di palline simulate finora (NON
        # del numero di chiamate a run_experiments: se chiami
        # run_experiments(100) e poi run_experiments(50), n_experiments
        # diventa 150, non 2). E' persistente: si accumula fra chiamate
        # diverse, si azzera solo con reset().
        self.n_experiments = 0
        # Storico del contenuto della riga finale: una fotografia del
        # contenuto della riga finale dopo aver lanciato ogni pallina
        # (quindi dopo la prima, dopo la seconda, dopo la terza, ...) in
        # maniera cumulativa (non per ogni chiamata a run_experiments).
        # E' una lista Python, non un array numpy: si accumula fra
        # chiamate diverse a run_experiments (se non viene fatto reset()
        # nel mezzo), e una lista puo' crescere in coda in modo
        # economico con .extend(); un array numpy invece ha una
        # dimensione fissa decisa alla creazione e "allungarlo" fra una
        # chiamata e l'altra richiederebbe ricrearlo da capo ogni volta
        # (con vstack/concatenate), copiando via via tutti i dati vecchi.
        self.history = []

    # ------------------------------------------------------------------
    # Utilita' interne
    # ------------------------------------------------------------------
    def _bin_index(self, row, col):
        """Traduce una coppia (riga, colonna) nell'indice assoluto nell'array piatto.

        `row` e `col` possono essere interi singoli oppure array numpy
        (anche di shape diversa fra loro, purche' compatibili per
        broadcasting): in questo caso il metodo ritorna un array di
        indici, utile per vettorizzare il conteggio su molte palline.

        L'indice di partenza della riga e' la somma 1 + 2 + ... + row =
        row*(row+1)/2, cioe' il numero di elementi che precedono la riga.
        Aggiungendo l'offset col (la posizione dentro quella riga), si
        ottiene l'indice assoluto nell'intero array piatto. col riparte
        sempre da 0 ad ogni riga (e' una posizione RELATIVA alla riga
        corrente, non assoluta nell'array): e' row_start a "spostare"
        quel valore relativo nella posizione giusta dell'array piatto.
        """
        row_start = row * (row + 1) // 2
        return row_start + col

    @property
    def bottom_row(self):
        """Contenuto attuale della riga finale (bottom row) del board.

        La riga finale (indice N-1) e' l'ultimo pezzo dell'array piatto:
        parte dall'indice (N-1)*N//2 e arriva fino alla fine dell'array.

        Non e' un dato calcolato o salvato a parte: legge sempre e
        direttamente da self.board, quindi accumula fra chiamate diverse
        a run_experiments esattamente come self.board (si azzera solo
        con reset()).
        """
        # --- VERSIONE 1: slicing diretto sull'array piatto ---
        # self.board e' un unico array 1D (l'array piatto): non esiste
        # mai una struttura triangolare vera in memoria. La riga finale
        # (indice N-1) e' l'ultimo pezzo di quell'array piatto, quindi
        # basta uno slicing "da dove inizia la riga N-1 fino alla fine
        # dell'array" per estrarla in un colpo solo.
        # n = self.n_rows
        # start = (n - 1) * n // 2
        # return self.board[start:]

        # --- VERSIONE 2: con _bin_index e fancy indexing (equivalente) ---
        # cols e' un array con tutte le colonne della riga finale:
        # [0, 1, ..., last_row]. _bin_index(last_row, cols) applica la
        # formula riga/colonna a OGNI elemento di cols in un colpo solo
        # (broadcasting: last_row e' uno scalare, cols e' un array, la
        # formula si applica elemento per elemento), restituendo un
        # array di indici assoluti - uno per ogni bin della riga finale.
        # self.board[array_di_indici] (fancy indexing, non slicing) usa
        # poi quell'array per estrarre esattamente quei bin.
        # last_row = self.n_rows - 1
        # cols = np.arange(last_row + 1)
        # return self.board[self._bin_index(last_row, cols)]

        # --- VERSIONE 3 (usata qui): con _bin_index e slicing ---
        # _bin_index(last_row, 0) calcola l'indice assoluto del primo
        # bin (colonna 0) della riga finale, cioe' esattamente il punto
        # di partenza che nella Versione 1 calcolavo a mano con la
        # formula (n-1)*n//2. Da li' in poi lo slicing prende tutto il
        # resto dell'array, che coincide con la riga finale perche' e'
        # l'ultima riga del board (non c'e' nient'altro dopo).
        last_row = self.n_rows - 1
        return self.board[self._bin_index(last_row, 0):]

    # ------------------------------------------------------------------
    # Simulazione (vettorizzata)
    # ------------------------------------------------------------------
    # ------------------------------------------------------------------
    # (A) METODO SEPARATO: simulazione di UNA singola pallina
    # ------------------------------------------------------------------
    # NOTA: questo metodo NON e' utilizzato da nessun'altra parte del
    # codice (run_experiments non lo chiama, e' completamente
    # indipendente). E' lasciato qui a scopo didattico/di studio: mostra
    # la logica di simulazione di una singola pallina in modo esplicito
    # e leggibile, per confronto diretto con la versione vettorizzata su
    # n_exp palline in run_experiments piu' sotto.
    # ------------------------------------------------------------------
    def drop_bead(self):
        """Simula la caduta di UNA singola pallina e aggiorna il board.

        Ritorna la colonna finale raggiunta nella riga di fondo.
        """
        # N-1 lanci di moneta (0 = resta, 1 = sposta a destra), uno per
        # ogni transizione fra righe consecutive
        random_steps = np.random.randint(0, 2, size=self.n_rows - 1)

        # la somma cumulativa restituisce gli indici di col per ogni riga
        cumulative = np.cumsum(random_steps)

        # antepongo col = 0, la colonna di partenza alla riga 0
        col_per_row = np.concatenate(([0], cumulative))  # shape (n_rows,)

        # traduco (riga, colonna) -> indice assoluto e segno il passaggio
        # in ogni bin attraversato, una riga alla volta (qui n_rows e'
        # piccolo, quindi il costo e' trascurabile anche restando "espliciti")
        rows = np.arange(self.n_rows)
        indices = self._bin_index(rows, col_per_row)
        self.board[indices] += 1

        self.n_experiments += 1
        return col_per_row[-1]  # colonna finale raggiunta

    # ------------------------------------------------------------------
    # (B) simulazione vettorizzata di N palline insieme
    # ------------------------------------------------------------------
    # E' la generalizzazione di drop_bead a n_exp palline simultanee:
    # invece di un vettore di N-1 lanci per una pallina, si genera una
    # MATRICE (n_exp, N-1) di lanci, una riga per pallina; il cumsum si
    # applica lungo le colonne (axis=1) cosi' ogni riga della matrice
    # calcola la propria traiettoria in parallelo alle altre.
    # ------------------------------------------------------------------
    def run_experiments(self, n_exp):
        """Esegue la simulazione di `n_exp` palline in modo vettorizzato.

        Concettualmente equivale a chiamare drop_bead() n_exp volte in un
        ciclo Python, ma senza mai scrivere quel ciclo: le traiettorie di
        tutte le palline vengono generate in un'unica matrice, e il
        conteggio nel board avviene con np.add.at, che gestisce
        correttamente gli indici ripetuti (piu' palline che finiscono
        nello stesso bin nella stessa chiamata).
        """
        # un passo casuale (0 = sinistra, 1 = destra) per ogni pallina e
        # per ognuna delle n_rows - 1 transizioni fra righe consecutive
        steps = np.random.randint(0, 2, size=(n_exp, self.n_rows - 1))

        # posizione cumulativa raggiunta ad ogni riga successiva alla prima
        cumulative = np.cumsum(steps, axis=1)

        # antepongo la colonna di zeri corrispondente alla riga 0.
        # start_column e' un array 1D (non serve dichiararlo come
        # matrice (n_exp, 1)): column_stack lo tratta automaticamente
        # come una singola colonna da affiancare alle colonne di
        # cumulative (che e' gia' 2D).
        start_column = np.zeros(n_exp, dtype=int)
        positions = np.column_stack((start_column, cumulative))  # shape (n_exp, n_rows)
        # equivalente con concatenate, ma li' start_column deve essere
        # dichiarato esplicitamente come matrice (n_exp, 1), non 1D:
        # start_column_2d = np.zeros((n_exp, 1), dtype=int)
        # positions = np.concatenate((start_column_2d, cumulative), axis=1)
        # equivalente anche con hstack (per array 2D, hstack e' un alias
        # di concatenate(axis=1)):
        # positions = np.hstack((start_column_2d, cumulative))
        #
        # positions ti dice "in quale bin si trova ciascuna pallina", riga
        # per riga - e' l'informazione INDIVIDUALE, non aggregata, di dove
        # va a finire ciascuna pallina lungo il suo percorso. E' un dato
        # temporaneo, locale a questa chiamata (a differenza di
        # self.board, vedi sotto, che invece e' un conteggio aggregato e
        # persistente).
        #
        # ATTENZIONE: positions contiene COLONNE RELATIVE (col), non
        # indici assoluti nell'array piatto self.board. col riparte
        # sempre da 0 ad ogni riga (vedi _bin_index): lo stesso valore
        # di col (es. 0) corrisponde a indici assoluti diversi a seconda
        # della riga (es. indice 0 alla riga 0, indice 1 alla riga 1,
        # ecc.). Per ottenere gli indici assoluti serve combinare
        # positions con il numero di riga tramite _bin_index - e' quello
        # che fa flat_indices piu' sotto.

        # salvo lo stato della riga finale PRIMA di aggiornare il board:
        # serve al METODO 2 (attivo piu' sotto) per costruire
        # self.history facendo partire il cumsum gia' dal valore
        # corretto, invece di calcolarlo isolato e correggerlo dopo.
        previous_bottom_row = self.bottom_row.copy()

        # indice assoluto, nell'array piatto, del bin raggiunto in ogni
        # riga da ogni pallina: traduco (riga, colonna) -> indice con
        # _bin_index, sfruttando il broadcasting fra righe (array 1D) e
        # colonne (matrice n_exp x n_rows). flat_indices e' ancora una
        # matrice 2D a questo punto, nonostante il nome "flat" - diventa
        # davvero 1D solo dopo .ravel() piu' sotto.
        rows = np.arange(self.n_rows)  # shape (n_rows,)
        flat_indices = self._bin_index(rows, positions)  # shape (n_exp, n_rows)

        # accumulo i conteggi nel board. Poiche' piu' palline possono
        # cadere nello stesso bin nella stessa chiamata, un semplice
        # self.board[flat_indices] += 1 darebbe risultati sbagliati: gli
        # indici ripetuti si sovrascriverebbero a vicenda invece di
        # sommarsi (legge tutti i vecchi valori in blocco, poi scrive
        # tutti i nuovi valori in blocco: le scritture verso la stessa
        # cella si sovrascrivono, sopravvive solo l'ultima).
        #
        # flat_indices e' una matrice 2D (n_exp, n_rows). np.add.at e
        # np.unique appiattiscono l'input automaticamente, quindi per
        # loro .ravel() e' facoltativo (lo scrivo comunque per chiarezza,
        # per rendere esplicito che si lavora su un elenco piatto di
        # posizioni, non su una griglia pallina/riga). np.bincount invece
        # RICHIEDE un array 1D e solleva un errore se gli si passa una
        # matrice 2D: li' .ravel() e' obbligatorio, non solo stilistico.

        # --- OPZIONE 1 (equivalente): np.add.at ---
        # Incrementa di 1 le caselle di self.board negli indici indicati
        # in flat_indices. Processa un indice alla volta: legge il
        # valore attuale, somma, scrive subito, prima di passare al
        # successivo. Se lo stesso indice ricompare, vede gia' il
        # valore aggiornato dal giro precedente, quindi accumula
        # correttamente.
        # (.ravel() qui e' facoltativo: add.at appiattisce da solo)
        # np.add.at(self.board, flat_indices.ravel(), 1)

        # --- OPZIONE 2 (usata qui): np.bincount ---
        # bincount conta quante volte compare ciascun valore possibile
        # (da 0 a minlength-1) nell'array di indici, restituendo un
        # array di conteggi per ogni indice, lungo quanto self.board;
        # sommandolo poi con un normale += non c'e' alcun indice
        # ripetuto coinvolto (bincount ha gia' fatto il conteggio),
        # quindi += funziona senza problemi.
        # (.ravel() qui e' OBBLIGATORIO: bincount richiede un array 1D e
        # solleva "object too deep for desired array" su una matrice 2D)
        self.board += np.bincount(flat_indices.ravel(), minlength=len(self.board))

        # --- OPZIONE 3 (equivalente): np.unique + indicizzazione ---
        # unique(..., return_counts=True) restituisce i valori distinti
        # presenti nell'array e quante volte compare ciascuno. Usando i
        # valori distinti (senza ripetizioni) come indici, += funziona
        # bene perche' ogni indice compare una sola volta in questo
        # array "compattato".
        # (.ravel() qui e' facoltativo: unique appiattisce da solo)
        #
        # valori_unici, conteggi = np.unique(flat_indices.ravel(), return_counts=True)
        # self.board[valori_unici] += conteggi

        # aggiorna il contatore totale delle palline simulate, sommando
        # le n_exp nuove di questa chiamata a quelle gia' accumulate
        # nelle run precedenti (si azzera solo con reset(), come
        # self.board e self.history)
        self.n_experiments += n_exp

        # ------------------------------------------------------------------
        # Storico del contenuto della riga finale, una fotografia per
        # ciascuna pallina appena simulata (per rispondere a "come era
        # la riga finale dopo la k-esima simulazione", dove per
        # "simulazione" si intende UNA singola pallina, non l'intera
        # chiamata a run_experiments).
        # ------------------------------------------------------------------

        # colonna finale (nella riga di fondo) raggiunta da ciascuna
        # pallina di QUESTA run: un numero per pallina.
        # final_cols contiene solo le colonne RELATIVE dell'ultima riga
        # per ogni pallina (eredita la stessa natura di positions, di
        # cui e' un pezzo). A differenza di flat_indices, qui NON serve
        # tradurre con _bin_index: non c'e' ambiguita' perche' bead_hits
        # (piu' sotto) rappresenta SOLO la riga finale, non tutte le
        # righe del board mescolate insieme come in self.board - quindi
        # "colonna X" identifica gia' univocamente un bin, senza bisogno
        # di specificare anche la riga.
        final_cols = positions[:, -1]  # shape (n_exp,)

        # Matrice "one-hot": una riga per ciascuna pallina di questa run,
        # con un 1 nella colonna dove e' atterrata (final_cols) e 0 in
        # tutte le altre colonne. bead_rows = [0,1,...,n_exp-1] abbina
        # ogni riga della matrice alla pallina corrispondente;
        # bead_hits[bead_rows, final_cols] seleziona quindi, per ogni
        # pallina, esattamente la sua cella di atterraggio, e la accende
        # con =1.
        #
        # ATTENZIONE: la "riga" di bead_hits NON e' una riga reale del
        # Galton board - e' solo un modo di visualizzare nello spazio
        # della matrice l'1 della pallina, per distinguere una pallina
        # dall'altra. Ogni riga di bead_hits rappresenta una pallina
        # diversa, ma tutte quante si riferiscono alla STESSA riga
        # fisica del board (l'ultima, quella finale) - non c'e' nessuna
        # corrispondenza fra "riga di bead_hits" e "riga del triangolo".
        # Per questo la variabile si chiama bead_rows e non rows: per
        # distinguerla da rows piu' sopra, che invece indicizza le righe
        # FISICHE del board (usata in flat_indices).
        #
        # Uso = e non += perche' ogni cella di bead_hits viene toccata
        # AL MASSIMO UNA VOLTA: bead_rows = [0,1,...,n_exp-1] non ha mai
        # ripetizioni, quindi ogni coppia (riga, colonna) e' unica anche
        # se piu' palline condividono la stessa colonna (final_cols puo'
        # ripetersi, ma abbinato a righe sempre diverse). A differenza
        # di self.board (dove serviva np.add.at perche' la STESSA cella
        # poteva essere toccata piu' volte nella stessa chiamata), qui
        # non c'e' alcun rischio di sovrascrittura da gestire.
        #
        # IMPORTANTE: bead_hits contiene SOLO le nuove palline di questa
        # run, isolate l'una dall'altra (un solo 1 per riga, nessun
        # accumulo fra righe). Non sa nulla ne' delle palline di run
        # precedenti, ne' del fatto che piu' palline di questa run
        # possano condividere lo stesso bin: quell'accumulo avviene solo
        # dopo, con il cumsum piu' sotto.
        bead_hits = np.zeros((n_exp, self.n_rows), dtype=int)  # shape (n_exp, n_rows)
        bead_rows = np.arange(n_exp)  # shape (n_exp,)
        bead_hits[bead_rows, final_cols] = 1  # bead_hits resta shape (n_exp, n_rows)

        # ------------------------------------------------------------------
        # Sia il Metodo 1 che il Metodo 2 servono per UNIRE PIU' RUN: il
        # loro scopo e' costruire fotografie corrette per self.history
        # anche quando run_experiments viene chiamato piu' volte senza
        # reset() nel mezzo, tenendo conto delle palline gia' presenti
        # dalle run precedenti, non solo di quelle di questa run.
        # ------------------------------------------------------------------

        # --- METODO 1 (equivalente): cumsum isolato da zero, poi correggo con baseline ---
        # Prima sommo cumulativamente bead_hits da solo (axis=0, lungo le
        # palline): la riga k del risultato conta le prime k+1 palline
        # di QUESTA run soltanto, come se il board fosse vuoto — il
        # cumsum parte sempre da zero, non sa nulla di eventuali run
        # precedenti (run_experiments NON azzera il board fra chiamate
        # diverse: si accumula, si azzera solo con reset(), vedi sopra).
        #
        # bottom_row_history = np.cumsum(bead_hits, axis=0)
        #
        # self.bottom_row, letto QUI (dopo che il board e' gia' stato
        # aggiornato con np.add.at poche righe sopra), e' il totale
        # reale e completo (vecchie run + questa run). Sottraendo il
        # contributo isolato di questa run (l'ultima riga del cumsum)
        # dal totale reale, isolo esattamente quante palline c'erano
        # GIA' prima che questa run iniziasse:
        #
        # baseline = self.bottom_row - bottom_row_history[-1]
        #
        # Sommando poi baseline a OGNI riga del cumsum (broadcasting:
        # baseline e' un array 1D, il cumsum una matrice 2D, la somma si
        # applica riga per riga), ogni fotografia isolata diventa una
        # fotografia reale e completa:
        #
        # self.history.extend(baseline + bottom_row_history)

        # --- METODO 2 (usato qui): antepongo lo stato di partenza PRIMA del cumsum ---
        # Invece di calcolare il cumsum isolato e correggerlo dopo con
        # una sottrazione, faccio partire il cumsum gia' dal punto
        # giusto fin dall'inizio. np.cumsum non accetta un valore di
        # partenza esplicito: l'unico modo per farlo "partire da un
        # certo valore" e' anteporgli quel valore come riga fittizia
        # iniziale (esattamente come si e' fatto con positions, dove si
        # anteponeva uno 0 prima del cumsum dei passi casuali).
        #
        # previous_bottom_row (lo stato del board PRIMA di questa run,
        # salvato con .copy() a inizio metodo, PRIMA di aggiornare il
        # board con np.add.at) diventa quella riga fittizia iniziale:
        # [previous_bottom_row] lo trasforma da array 1D a matrice con
        # una sola riga, cosi' concatenate puo' incollarlo sopra
        # bead_hits lungo axis=0 (una riga sopra l'altra).
        combined = np.concatenate(([previous_bottom_row], bead_hits), axis=0)  # shape (n_exp+1, n_rows)
        # equivalente con vstack (vertical stack): non richiede nemmeno
        # le parentesi quadre attorno a previous_bottom_row, perche'
        # vstack converte automaticamente un array 1D in riga singola:
        # combined = np.vstack((previous_bottom_row, bead_hits))

        # Il cumsum su combined parte gia' da previous_bottom_row (riga
        # 0 del risultato = solo previous_bottom_row, nessuna pallina
        # nuova ancora aggiunta), e via via accumula i contributi delle
        # nuove palline, una alla volta. [1:] scarta quella prima riga
        # fittizia: non rappresenta nessuna pallina reale, solo il punto
        # di partenza da cui le vere palline di questa run iniziano ad
        # accumularsi. Il risultato ha esattamente n_exp righe, una per
        # pallina, gia' complete e corrette (vecchie+nuove) senza
        # bisogno di ulteriori aggiustamenti.
        bottom_row_history = np.cumsum(combined, axis=0)[1:]  # shape (n_exp, n_rows)

        # trasferisco le n_exp fotografie in self.history (che persiste
        # nel tempo, a differenza delle variabili locali di questo
        # metodo): extend le aggiunge una per una in coda, cosi' lo
        # storico resta corretto anche attraverso piu' chiamate a
        # run_experiments senza reset() nel mezzo.
        self.history.extend(bottom_row_history)

    def reset(self):
        """Azzera tutti i bin del board e cancella lo storico delle simulazioni.

        E' l'UNICO modo per far ripartire il conteggio da zero: le
        chiamate a run_experiments NON si azzerano automaticamente fra
        loro, si accumulano (come un vero Galton board a cui continui
        ad aggiungere palline senza svuotarlo). reset() serve appunto a
        svuotare esplicitamente il board fisico (self.board), il
        contatore delle palline simulate (self.n_experiments) e lo
        storico delle fotografie (self.history), cosi' la run
        successiva a questo reset riparte da un board vuoto.
        """
        self.board[:] = 0
        self.n_experiments = 0
        self.history.clear()

    # ------------------------------------------------------------------
    # Ispezione / output
    # ------------------------------------------------------------------
    def print_bottom_row_at(self, k):
        """Stampa a schermo il contenuto della riga finale dopo la k-esima simulazione.

        `k` e' l'indice (1-based) della simulazione di cui si vuole
        conoscere lo stato della riga finale.
        """
        if not (1 <= k <= len(self.history)):
            raise IndexError(
                f"k={k} non valido: sono state eseguite {len(self.history)} simulazioni."
            )
        print(f"Riga finale dopo la {k}-esima simulazione: {self.history[k - 1]}")

    def plot_distribution(self):
        """Disegna, salva e mostra un istogramma a barre della distribuzione
        sulla riga finale.

        Ritorna la figura completa (utile per ulteriori personalizzazioni
        dopo la chiamata).
        """
        fig, ax = plt.subplots(figsize=(8, 5))

        bins = np.arange(self.n_rows)
        counts = self.bottom_row
        # self.bottom_row coincide sempre con self.history[-1] (l'ultima
        # riga/fotografia di self.history): sono lo stesso dato letto in
        # due modi diversi, non serve rappresentare tutto self.history
        # qui - basta lo stato finale, che e' quello che il grafico deve
        # mostrare (la distribuzione risultante da Nexp esperimenti).

        ax.bar(bins, counts, color="steelblue", edgecolor="black",
               label=f"$N_{{exp}}$ = {self.n_experiments}")
        ax.set_xlabel("Indice del bin nella riga finale")
        ax.set_ylabel("Numero di palline")
        ax.set_title(f"Distribuzione del Galton board (N = {self.n_rows} righe)")
        ax.set_xticks(bins)
        ax.legend()
        ax.grid(axis="y", linestyle="--", alpha=0.6)

        fig.tight_layout()
        fig.savefig("galton_distribution.png", dpi=150)
        plt.show()

        return fig

    # ------------------------------------------------------------------
    # Persistenza su file binario
    # ------------------------------------------------------------------
    def save(self, filename):
        """Salva lo stato completo del board (board, storico, ecc.) su file binario."""
        with open(filename, "wb") as f:
            pickle.dump(self, f)

    @staticmethod
    def load(filename):
        """Carica un'istanza di GaltonBoard precedentemente salvata con `save`."""
        with open(filename, "rb") as f:
            return pickle.load(f)

    # ------------------------------------------------------------------
    def __repr__(self):
        return (f"GaltonBoard(n_rows={self.n_rows}, "
                f"n_experiments={self.n_experiments})")


# ----------------------------------------------------------------------
# Programma principale: esempio di utilizzo della classe GaltonBoard
# ----------------------------------------------------------------------
def main():
    # Creo un Galton board con 10 righe
    n_rows = 10
    board = GaltonBoard(n_rows=n_rows)

    # Eseguo 5000 esperimenti (palline), simulati in modo vettorizzato
    n_exp = 5000
    board.run_experiments(n_exp)

    # Stampo a schermo il contenuto della riga finale dopo la 1a, la
    # 100esima e l'ultima simulazione, a titolo di esempio
    for k in (1, 100, n_exp):
        board.print_bottom_row_at(k)

    # Plotto la distribuzione risultante
    fig = board.plot_distribution()

    # Salvo le simulazioni su un unico file binario
    board.save("galton_board.pkl")

    # Esempio di reset e ri-simulazione con un numero diverso di esperimenti
    board.reset()
    board.run_experiments(2000)
    print(f"\nDopo il reset e {board.n_experiments} nuovi esperimenti:")
    print(f"Riga finale: {board.bottom_row}")


if __name__ == "__main__":
    main()
