#include <iostream>
#include "massa.h"

int main() {
    std::cout << "=== TEST CLASSE MASSA (BINARI COMPATTI) ===" << std::endl << std::endl;

    // 1. Test Costruttore di Default
    std::cout << "1. Test Costruttore Default (1.4 - 1.4 M_sun):" << std::endl;
    massa binario_default;
    binario_default.print();
    std::cout << std::endl;

    // 2. Test Costruttore Regolare (Dati tipo GW150914)
    // Parametri approssimativi: Mc ~ 28.0, v ~ 0.247
    // Spin: proviamo ad assegnare componenti casuali
    std::cout << "2. Test Costruttore Regolare (Sistema Massiccio):" << std::endl;
    massa gw150914(0.1, 0.0, 0.2,   // Spin 1
                   0.0, 0.1, -0.1,  // Spin 2
                   28.1, 0.247);    // Mc e v
    
    std::cout << "Output tramite operatore <<: " << gw150914 << std::endl;
    std::cout << "Precessione (chi_p): " << gw150914.calcolo_spin_precession() << std::endl;
    std::cout << std::endl;

    // 3. Test Validazione e Coerenza (Setter)
    std::cout << "3. Test Setter e Coerenza Interna:" << std::endl;
    massa test_coerenza;
    
    std::cout << "Massa iniziale m1: " << test_coerenza.m1value() << std::endl;
    std::cout << "Modifico m1 a 30.0..." << std::endl;
    test_coerenza.set_m1(30.0);
    
    std::cout << "Nuova m1: " << test_coerenza.m1value() << std::endl;
    std::cout << "Nuova Mc (auto-aggiornata): " << test_coerenza.Mcvalue() << std::endl;
    std::cout << "Nuovo v  (auto-aggiornato): " << test_coerenza.vvalue() << std::endl;
    std::cout << std::endl;

    // 4. Test Errori (Input non validi)
    std::cout << "4. Test Sicurezza (Valori illegali):" << std::endl;
    
    std::cout << "Provo a impostare uno spin X1x = 5.0 (dovrebbe fallire):" << std::endl;
    test_coerenza.set_X1x(5.0); 
    
    std::cout << "Provo a impostare v = 0.3 (fuori range 0.25):" << std::endl;
    test_coerenza.set_v(0.3);

    std::cout << "\n=== FINE TEST ===" << std::endl;

    return 0;
}
