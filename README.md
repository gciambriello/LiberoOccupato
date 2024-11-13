# Controllo Stato Stanze con ESP8266

## Descrizione del Progetto
Questo progetto sfrutta un **ESP8266** per creare un sistema di gestione dello stato delle stanze tramite un'interfaccia web. L'ESP8266 funge da server che ospita le pagine web, permettendo agli utenti di controllare e monitorare in tempo reale se una stanza è "LIBERO" o "OCCUPATO". Tutto è gestito in memoria sul microcontroller, senza necessità di un database esterno.

## Funzionalità
- **Controllo dello stato**: L'utente può impostare una stanza come "LIBERO" o "OCCUPATO" tramite una semplice interfaccia web.
- **Visualizzazione in tempo reale**: Un'altra pagina consente di monitorare lo stato attuale delle stanze, aggiornandosi automaticamente ogni 3 secondi.
- **Cambiamento di colore**: Il colore di sfondo delle pagine cambia dinamicamente in base allo stato:
  - **Verde** per "LIBERO".
  - **Rosso** per "OCCUPATO".

## Struttura del Progetto
Il progetto è composto da tre pagine web integrate direttamente nel codice Arduino:
1. **home.html**: Pagina principale con link per accedere al controllo e alla visualizzazione dello stato delle stanze.
2. **index.html**: Pagina per impostare lo stato di una stanza. L'utente può inserire il nome della stanza e alternare tra "LIBERO" e "OCCUPATO".
3. **status.html**: Pagina che mostra lo stato corrente di una stanza, con aggiornamento automatico ogni 3 secondi.

## Dettagli Tecnici
- Il server web è gestito utilizzando la libreria `ESP8266WebServer`.
- Le pagine HTML sono integrate direttamente nel codice C++ dell'ESP8266.
- Tutte le informazioni sulle stanze vengono memorizzate in memoria sul microcontroller.
- Il sistema è progettato per funzionare su una rete locale.

## Requisiti
- **ESP8266**
- **Arduino IDE**
- Libreria `ESP8266WebServer`

## Configurazione
1. **Configurare le credenziali Wi-Fi** nel codice (`ssid` e `password`).
2. **Caricare il codice** sull'ESP8266 utilizzando l'IDE di Arduino.
3. **Accedere all'indirizzo IP** assegnato all'ESP8266 su un browser web (es: `http://192.168.1.100/`). E' possibile leggere l'indirizzo ip dal monitor di Arduino.

## Utilizzo
### Pagina di Controllo (`/index.html`)
- Inserisci il nome di una stanza e premi "Cambia Stato".
- Lo stato della stanza verrà invertito tra "LIBERO" e "OCCUPATO".
- Il colore dello sfondo cambierà per riflettere il nuovo stato.

### Pagina di Stato (`/status.html`)
- Inserisci il nome di una stanza e premi "Mostra Stato".
- La pagina si aggiornerà automaticamente ogni 3 secondi per mostrare lo stato attuale.
- Il colore dello sfondo indicherà se la stanza è "LIBERO" (verde) o "OCCUPATO" (rosso).

## Esempio di Utilizzo
1. **Accedi alla pagina principale** (`/`) per selezionare il tipo di operazione.
2. **Imposta lo stato** di una stanza tramite la pagina `/index.html`.
3. **Monitora lo stato** in tempo reale dalla pagina `/status.html`.

## Limitazioni
- Tutti i dati sono memorizzati in memoria volatile sull'ESP8266, quindi verranno persi in caso di riavvio del dispositivo.
- Il sistema funziona solo su una rete locale a meno che non venga configurato il port forwarding sul router per l'accesso remoto.

## Possibili Estensioni
- Implementare un sistema di autenticazione per limitare l'accesso.
- Utilizzare un database esterno (ad es. Firebase) per memorizzare i dati in modo persistente.
- Aggiungere notifiche via email o Telegram per segnalare i cambiamenti di stato.

## Autore
Questo progetto è stato sviluppato come esempio di utilizzo dell'ESP8266 per applicazioni IoT semplici ma efficaci.

