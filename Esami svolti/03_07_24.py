"""
Computing Methods for Physics 1 -- 3 luglio 2024
Parte 2 (Python)

Gerarchia di classi per rappresentare solidi rotanti a densita' uniforme.

Classe base:
    Solid(mass, length1=0, length2=0, length3=0)
        - mass          : massa M del corpo rotante
        - length1/2/3   : tre scale di lunghezza caratteristiche (alcuni
                           solidi ne avranno una o due poste a zero)
        - omega         : frequenza angolare di rotazione
        - d             : distanza tra l'asse di rotazione e un asse
                           parallelo passante per il centro di massa (CoM)
        - I             : momento d'inerzia rispetto all'asse di rotazione
                           effettivo (tramite il teorema dell'asse parallelo
                           I = I_CoM + M*d**2)
        - K             : energia cinetica, K = 1/2 * I * omega**2

Il momento d'inerzia dipende dalla forma specifica, quindi non puo' essere
calcolato da Solid stessa: le sottoclassi concrete (Sphere, Rod, Rectangle)
devono fornirlo implementando _inertia_about_com.

I e K hanno senso solo dopo che il solido e' stato effettivamente messo in
rotazione, cioe' dopo che Spin(d, omega) e' stato chiamato: prima di allora
valgono None invece di restituire silenziosamente un valore sbagliato.
"""

# ABC (Abstract Base Class) e abstractmethod vengono dal modulo standard
# `abc` di Python. Sono l'equivalente pythonico di una funzione virtuale
# pura in C++: permettono a una classe base di definire un'interfaccia
# comune obbligando ogni sottoclasse a fornire la propria implementazione
# di determinati metodi.
from abc import ABC, abstractmethod


class Solid(ABC):
    """Classe base per un solido rigido rotante a densita' uniforme.

    Ereditare da ABC rende questa classe astratta: Python impedisce di
    creare direttamente un oggetto Solid(...) (es. Solid(mass=1.0) solleva
    un TypeError). Questo ha senso perche' "un generico solido rotante"
    non e' qualcosa per cui possiamo effettivamente calcolare nulla: senza
    conoscere la forma specifica (sfera, asta, rettangolo, ...) non sappiamo
    come calcolare il momento d'inerzia. Solid esiste solo per essere
    ereditata.
    """

    def __init__(self, mass, length1=0.0, length2=0.0, length3=0.0):
        self.mass = mass
        self.length1 = length1
        self.length2 = length2
        self.length3 = length3

        # Non note finche' il solido non viene effettivamente messo in
        # rotazione tramite Spin
        self._omega = None
        self._d = None

    # ------------------------------------------------------------------
    # Getter / setter (pythonici, tramite property)
    # ------------------------------------------------------------------
    @property
    def mass(self):
        return self._mass

    @mass.setter
    def mass(self, value):
        if value <= 0:
            raise ValueError("mass must be positive")
        self._mass = float(value)

    @property
    def length1(self):
        return self._length1

    @length1.setter
    def length1(self, value):
        if value < 0:
            raise ValueError("a characteristic length cannot be negative")
        self._length1 = float(value)

    @property
    def length2(self):
        return self._length2

    @length2.setter
    def length2(self, value):
        if value < 0:
            raise ValueError("a characteristic length cannot be negative")
        self._length2 = float(value)

    @property
    def length3(self):
        return self._length3

    @length3.setter
    def length3(self, value):
        if value < 0:
            raise ValueError("a characteristic length cannot be negative")
        self._length3 = float(value)

    @property
    def omega(self):
        return self._omega

    @property
    def d(self):
        return self._d

    @property
    def I(self):
        """Momento d'inerzia rispetto all'asse di rotazione effettivo,
        tramite il teorema dell'asse parallelo. None finche' il solido
        non e' in rotazione."""
        if self._d is None:
            return None
        return self._inertia_about_com() + self.mass * self._d ** 2

    @property
    def K(self):
        """Energia cinetica, ricalcolata al volo a partire dagli attuali
        I e omega. None se il solido non e' ancora in rotazione."""
        if self._omega is None or self.I is None:
            return None
        return 0.5 * self.I * self._omega ** 2

    # ------------------------------------------------------------------
    # Momento d'inerzia (dipendente dalla forma) rispetto a un asse
    # passante per il CoM. Ogni sottoclasse concreta deve implementarlo.
    #
    # @abstractmethod segna questo metodo come obbligatorio per ogni
    # sottoclasse. Se una sottoclasse (es. una futura Cube(Solid))
    # dimenticasse di sovrascrivere _inertia_about_com, Python solleverebbe
    # un TypeError non appena si provasse a istanziarla (Cube(...)),
    # invece di far emergere il bug piu' tardi come un confuso
    # NotImplementedError quando qualcuno chiama .I o .K. Il corpo qui
    # sotto (raise NotImplementedError) non viene mai effettivamente
    # eseguito in pratica: ABC + @abstractmethod bloccano gia'
    # l'istanziazione prima che questo codice possa girare; e' tenuto
    # solo come rete di sicurezza / documentazione dell'intento.
    # ------------------------------------------------------------------
    @abstractmethod
    def _inertia_about_com(self):
        """Restituisce I_CoM, il momento d'inerzia rispetto a un asse
        passante per il centro di massa, parallelo all'asse di rotazione
        effettivo."""
        raise NotImplementedError

    # ------------------------------------------------------------------
    # Comportamento pubblico
    # ------------------------------------------------------------------
    def Spin(self, d, omega):
        """Mette il solido in rotazione.

        Parametri
        ---------
        d : distanza tra l'asse di rotazione e l'asse parallelo passante
            per il centro di massa.
        omega : frequenza angolare di rotazione.

        I e K non vengono memorizzati direttamente: diventano disponibili
        (calcolati al volo) tramite le property I e K non appena d e omega
        sono impostati.
        """
        if d < 0:
            raise ValueError("d must be non-negative")

        self._d = float(d)
        self._omega = float(omega)

        return self.I, self.K

    def __str__(self):
        state = (
            f"omega={self._omega}, d={self._d}, I={self.I}, K={self.K}"
            if self._d is not None
            else "not spinning yet"
        )
        # self.__class__.__name__ restituisce il nome della classe reale
        # dell'oggetto come stringa (es. "Sphere", "Rod", "Rectangle") e
        # non "Solid", anche se questo metodo __str__ e' definito qui
        # nella classe base. In questo modo un solo __str__ funziona per
        # tutte le sottoclassi senza doverlo riscrivere in ognuna.
        return f"{self.__class__.__name__}(M={self.mass}) [{state}]"

    def __repr__(self):
        return self.__str__()


class Sphere(Solid):
    """Una sfera solida uniforme di raggio R: I_CoM = 2/5 * M * R**2."""

    def __init__(self, mass, R):
        # super() da' accesso alla classe genitore (Solid). Qui chiamiamo
        # il suo __init__ per far fare a Solid tutto il lavoro "comune"
        # gia' scritto una volta sola: validare mass, creare gli attributi
        # length1/length2/length3 (con i controlli nei setter) e inizializzare
        # lo stato di rotazione (_omega, _d a None). Passiamo R come
        # length1, cioe' usiamo lo slot generico "length1" della classe
        # base per rappresentare il raggio, cosi' non serve riscrivere la
        # logica di validazione/salvataggio gia' presente in Solid.
        super().__init__(mass, length1=R)

    @property
    def R(self):
        return self.length1

    @R.setter
    def R(self, value):
        self.length1 = value

    def _inertia_about_com(self):
        return (2.0 / 5.0) * self.mass * self.R ** 2

    def __str__(self):
        # super().__str__() chiama il metodo __str__ della classe base
        # Solid, che gia' costruisce una stringa tipo
        # "Sphere(M=2.0) [not spinning yet]" (usando self.__class__.__name__,
        # come visto prima). Qui la riutilizziamo com'e' e ci inseriamo
        # dentro anche il raggio R, invece di riscrivere da zero tutta la
        # formattazione della massa e dello stato di rotazione.
        base = super().__str__()
        return base.replace(
            f"Sphere(M={self.mass})", f"Sphere(M={self.mass}, R={self.R})"
        )


class Rod(Solid):
    """Un'asta solida sottile di lunghezza L, che ruota attorno a un
    asse perpendicolare all'asta: I_CoM = 1/12 * M * L**2."""

    def __init__(self, mass, L):
        super().__init__(mass, length1=L)

    @property
    def L(self):
        return self.length1

    @L.setter
    def L(self, value):
        self.length1 = value

    def _inertia_about_com(self):
        return (1.0 / 12.0) * self.mass * self.L ** 2

    def __str__(self):
        base = super().__str__()
        return base.replace(
            f"Rod(M={self.mass})", f"Rod(M={self.mass}, L={self.L})"
        )


class Rectangle(Solid):
    """Una lastra rettangolare sottile L x W, che ruota attorno a un asse
    perpendicolare al piano che la contiene:
    I_CoM = 1/12 * M * (L**2 + W**2)."""

    def __init__(self, mass, L, W):
        super().__init__(mass, length1=L, length2=W)

    @property
    def L(self):
        return self.length1

    @L.setter
    def L(self, value):
        self.length1 = value

    @property
    def W(self):
        return self.length2

    @W.setter
    def W(self, value):
        self.length2 = value

    def _inertia_about_com(self):
        return (1.0 / 12.0) * self.mass * (self.L ** 2 + self.W ** 2)

    def __str__(self):
        base = super().__str__()
        return base.replace(
            f"Rectangle(M={self.mass})",
            f"Rectangle(M={self.mass}, L={self.L}, W={self.W})",
        )


if __name__ == "__main__":
    # Semplice demo / controllo di sanita'
    sphere = Sphere(mass=2.0, R=0.5)
    rod = Rod(mass=1.5, L=2.0)
    rect = Rectangle(mass=3.0, L=1.0, W=0.4)

    print(sphere)
    print(rod)
    print(rect)

    for solid in (sphere, rod, rect):
        solid.Spin(d=0.3, omega=10.0)
        print(solid)
