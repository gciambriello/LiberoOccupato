#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Configura le credenziali Wi-Fi
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Inizializza il server sulla porta 80
ESP8266WebServer server(80);

// Struttura per memorizzare le stanze e i loro stati
struct Room {
  String name;
  String status;
};

#define MAX_ROOMS 10
Room rooms[MAX_ROOMS];
int roomCount = 0;

// Funzione per trovare una stanza
int findRoom(const String& name) {
  for (int i = 0; i < roomCount; i++) {
    if (rooms[i].name == name) return i;
  }
  return -1;
}

// Funzione per creare o aggiornare una stanza
void setRoomStatus(const String& name, const String& status) {
  int index = findRoom(name);
  if (index >= 0) {
    rooms[index].status = status;
  } else if (roomCount < MAX_ROOMS) {
    rooms[roomCount].name = name;
    rooms[roomCount].status = status;
    roomCount++;
  }
}

// Funzione per ottenere lo stato di una stanza
String getRoomStatus(const String& name) {
  int index = findRoom(name);
  if (index >= 0) {
    return rooms[index].status;
  }
  return "NOT_FOUND";
}

// Funzione per gestire la pagina di selezione (home.html)
void handleHomePage() {
  String html = R"=====(
<!DOCTYPE html>
<html lang="it">
<head>
    <meta charset="UTF-8">
    <title>Seleziona Pagina</title>
</head>
<body>
    <h1>Benvenuto</h1>
    <a href="/index.html">Vai a Controllo Stanza</a><br>
    <a href="/status.html">Vai a Stato Stanza</a>
</body>
</html>
)=====";
  server.send(200, "text/html", html);
}

// Funzione per servire la pagina di controllo (index.html)
void handleControlPage() {
  String html = R"=====(
<!DOCTYPE html>
<html lang="it">
<head>
    <meta charset="UTF-8">
    <title>Controllo Stanza</title>
    <style>
        body {
            font-family: Arial, sans-serif; 
            text-align: center; 
            transition: background-color 0.5s;
        }
        .green { background-color: green; color: white; }
        .red { background-color: red; color: white; }
        .button { padding: 10px 20px; font-size: 20px; cursor: pointer; margin-top: 20px; }
    </style>
</head>
<body id="body">
    <h1>Controllo Stanza</h1>
    <input type="text" id="roomName" placeholder="Nome stanza">
    <button class="button" onclick="toggleStatus()">Cambia Stato</button>
    <div id="status"></div>
    <script>
       async function toggleStatus() {
        const roomName = document.getElementById('roomName').value;
        if (!roomName) return;

        // Recupera lo stato attuale della stanza
        const response = await fetch(`/get?name=${roomName}`);
        const currentStatus = await response.text();

        // Determina il nuovo stato in base a quello attuale
        let newStatus = currentStatus === 'LIBERO' ? 'OCCUPATO' : 'LIBERO';

        // Invia richiesta per cambiare lo stato
        await fetch('/set', {
            method: 'POST',
            headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
            body: `name=${roomName}&status=${newStatus}`
        });

        // Aggiorna lo stato visualizzato
        updateStatus();
    }

        async function updateStatus() {
            const roomName = document.getElementById('roomName').value;
            const response = await fetch(`/get?name=${roomName}`);
            const status = await response.text();

            const body = document.getElementById('body');
            if (status === 'LIBERO') {
                body.className = 'green';
                document.getElementById('status').textContent = 'LIBERO';
            } else if (status === 'OCCUPATO') {
                body.className = 'red';
                document.getElementById('status').textContent = 'OCCUPATO';
            } else {
                body.className = '';
                document.getElementById('status').textContent = 'Stanza non trovata';
            }
        }
    </script>
</body>
</html>
)=====";
  server.send(200, "text/html", html);
}

// Funzione per servire la pagina di stato (status.html)
void handleStatusPage() {
  String html = R"=====(
<!DOCTYPE html>
<html lang="it">
<head>
    <meta charset="UTF-8">
    <title>Stato Stanza</title>
    <style>
        body { 
            font-family: Arial, sans-serif; 
            text-align: center; 
            transition: background-color 0.5s;
        }
        .green { background-color: green; color: white; }
        .red { background-color: red; color: white; }
        #status { font-size: 48px; margin-top: 20px; }
    </style>
</head>
<body id="body">
    <h1>Stato Stanza</h1>
    <input type="text" id="roomName" placeholder="Nome stanza">
    <button onclick="showStatus()">Mostra Stato</button>
    <div id="status"></div>
    <script>
        async function showStatus() {
            const roomName = document.getElementById('roomName').value;
            if (!roomName) return;
            updateStatus();
        }

        async function updateStatus() {
            const roomName = document.getElementById('roomName').value;
            const response = await fetch(`/get?name=${roomName}`);
            const status = await response.text();

            const body = document.getElementById('body');
            if (status === 'LIBERO') {
                body.className = 'green';
                document.getElementById('status').textContent = 'LIBERO';
            } else if (status === 'OCCUPATO') {
                body.className = 'red';
                document.getElementById('status').textContent = 'OCCUPATO';
            } else {
                body.className = '';
                document.getElementById('status').textContent = 'Stanza non trovata';
            }
        }

        // Aggiorna automaticamente lo stato ogni 3 secondi
        setInterval(updateStatus, 3000);
    </script>
</body>
</html>
)=====";
  server.send(200, "text/html", html);
}

// Endpoint per cambiare lo stato
void handleSetStatus() {
  if (server.hasArg("name") && server.hasArg("status")) {
    String roomName = server.arg("name");
    String status = server.arg("status");
    setRoomStatus(roomName, status);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Missing parameters");
  }
}

// Endpoint per ottenere lo stato
void handleGetStatus() {
  if (server.hasArg("name")) {
    String roomName = server.arg("name");
    String status = getRoomStatus(roomName);
    server.send(200, "text/plain", status);
  } else {
    server.send(400, "text/plain", "Missing parameter");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(1000);
  server.on("/", handleHomePage);
  server.on("/index.html", handleControlPage);
  server.on("/status.html", handleStatusPage);
  server.on("/set", HTTP_POST, handleSetStatus);
  server.on("/get", HTTP_GET, handleGetStatus);
  server.begin();
}

void loop() {
  server.handleClient();
}
