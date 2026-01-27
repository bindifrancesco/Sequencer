```txt
1) settings.json

Scopo: configurazioni generali di VS Code e delle estensioni.

    - Imposta compilatore e debugger predefiniti (gcc, g++, gdb).
    - Abilita i warnings e decide quali considerare come errori.
    - Imposta include/exclude per IntelliSense e ricerca (excludeSearch)
    - Controlla altre opzioni, ad esempio sanitizers, link-time optimization, ecc.
    - In breve: dice a VS Code e all’estensione C++ come comportarsi mentre modifichi, analizzi e fai l’IntelliSense.



2) launch.json

Scopo: configurare il debug.

    - Dice a VS Code quale eseguibile lanciare (program) e da quale cartella (cwd).
    
    - Imposta il debugger da usare (gdb) e opzioni come pretty printing.
    
    - Puoi aggiungere breakpoint iniziali, argomenti da linea di comando e variabili ambiente.
    
    - 💡 In breve: serve a VS Code per avviare e controllare il programma in modalità debug. /notes non ha impatto qui.


3) c_cpp_properties.json

Scopo: configurare l'IntelliSense.

    - Imposta includePath → quali cartelle VS Code deve scansionare per suggerimenti, header, completamento automatico.

    - Imposta compilerPath → per capire sintassi, standard e flag.

    - Imposta standard C/C++ e modalità IntelliSense.

    - 💡 In breve: dice a VS Code come comprendere il codice mentre lo scrivi, senza compilare nulla.

```
