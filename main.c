#include "main.h"

/*
* Mögliche Eingabeparameter für Help
* ./main -h
* ./main --help
*
*
* Mögliche Eingabeparameter für Ausführung
* ./main x y
* wo x, y können folgende Werte annehmen: (x, y Є {NAN, -NAN, Infinity, -Infinity}) ^ (x, y Є Z)
* Ist y rational, so wird x > 0 angennomen. Für ganzahlige y sind alle Werte (auch x < 0) zugelassen 
* Fehlerhaftes Verhalten wie z.B. char als Parameter ist nicht abgefangt.
*
*
* Mögliche Eingabeparameter für Tests
* ./main -test 1
    * Test Nummer 1 wird ausgeführt. 
    * 
    * Dabei wird eine "resource/test1.txt"-Datei erwartet, wovon die Werte für Perfomanzanalyse
    * abgelesen werden. Dieser Test erfolgt automatisch und erzeugt 3 neue Dateien.
    * 
    * In test1.txt muss für korrekte Ausführung am Ende eine leere Zeile hinzugefügt werden
    * 
    * In diesem Test werden Laufzeiten 3 verschiedener Funktionen vergliechen: 
    * - Unsere Implementierung in Assembler
    * - Unsere Implementierung in C 
    * - pow(double x, double y) aus der Standardbibliothek
    * 
    * Der Abstand beträgt jeweils mehr als eine Sekunde (sleep(1) wird aufgerufen) und alle Endergebnisse
    * sind Durchschnittswerte für 15 Wiederholungen.
* ./main -test 2
    * Test Nummer 2 wird ausgeführt. 
    * 
    * Hier werden die Analysergebnisse am Ende der Datei "resource/test2.txt" angehängt. 
    * Da die Abweichungsrate stets konstant bleibt, gibt es keine Wiederholungen.
    * 
    * In test2-Datei sind verschieden Werte für Anzahl der Taylorschritte zu finden. 
    * Alledings kann man diese Werte im Test automatisch nicht ändern, weil die Anzahle der Tazlor Schritte 
    * im Assemblercode festgeschrieben sind.
* ./main -test 3
    * Test Nummer 3 wird ausgeführt. 
    * 
    * Hier wird für verschiedene Anzahl an Taylorschritte Perfomazanalyse durchgeführt
    * 
    * Der Abstand beträgt jeweils mehr als eine Sekunde (sleep(1) wird aufgerufen) und alle Endergebnisse
    * sind Durchschnittswerte für 15 Wiederholungen. Allerdings erfolgt es auch nicht automatisch.
* ./main -test 4
    * Dabei wird eine "resource/test4.txt"-Datei erwartet, wovon die Werte für Perfomanzanalyse
    * abgelesen werden. Dieser Test erfolgt automatisch und erzeugt 3 neue Dateien.
    * 
    * Bitte beachten Sie, dass für Test 4 auch alternative Implementierungen aus dem Verzeichnis "Alternativer Ansatz" kompiliert werden müssen
    * 
    * In test4 muss für korrekte Ausführung am Ende eine leere Zeile hinzugefügt werden
    * 
    * In diesem Test werden Laufzeiten 3 verschiedener Funktionen vergliechen: 
    * - Alternative Implementierung in Assebmler
    * - Implementierung in Assembler
    * - Implementierung in C 
    * - pow(double x, double y) aus der Standardbibliothek
    * 
    * Der Abstand beträgt jeweils mehr als eine Sekunde (sleep(1) wird aufgerufen) und alle Endergebnisse
    * sind Durchschnittswerte für 15 Wiederholungen.
* ./main -test -all
    * Alle Tests werden nacheinander ausgeführt.
    * Wegen der benötigen Abstand in 1 Sekunde zwischen Messungen, kann die Ausführung 
    * dauern, da jeder Wert 15 Mal berechnet wird.
    * 
* Mehr als 4 oder keine Paramter führen zur Fehlermeldung und Terminierung
*/
int main(int argc, char *argv[]) {
    // 0 false
    // 1 true
    if(argc <= 1 ||  argc >= 4) {
        incorectParameters();
    }

    if(isHelp(argc, argv)) {
        printHelp();
    } else if(isTest(argc, argv)) {
        execute_tests(isTest(argc, argv));
    } else {
        double x = atof(argv[1]);
        double y = atof(argv[2]);

        printf("f = %.15lf\n", f(x, y));

        return f(x, y);
    }

    return 1;    
}

int isHelp(int argc, char *argv[]) {
    return (strcmp(argv[1], "-h") || strcmp(argv[1], "-help")) && (argc <= 2);
}

int isTest(int argc, char *argv[]) {
    int testNumber = (!strcmp(argv[2], "1")) || (!strcmp(argv[2], "2")) || (!strcmp(argv[2], "3")) || (!strcmp(argv[2], "4"));
    int t1 = !strcmp(argv[1], "-test");
    int t2 = (testNumber || (!strcmp(argv[2], "-all")));
    int isTestVar = t1 && t2;
    
    if(isTestVar && testNumber) {        
        return strtod(argv[2], NULL);
    } else if (isTestVar) {
        return 'a';
    } else {
        return 0;
    }
    
}

void printHelp() {
    int c;
    FILE *file;

    file = fopen("help.txt", "r");

    if(file) {
        while ((c = getc(file)) != EOF)
            putchar(c);

        fclose(file);
    }
}

void incorectParameters() {
    printf("Falsche Parameter übergegen\nVersuchen Sie \"./main -h\" für Hilfe\n");
    exit(EXIT_FAILURE);
}