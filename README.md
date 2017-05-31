## ArtNet Node based on ESP8266  ##

Die erste Verson basierte auf einem arduino mit dem Funkmodul nrf24L01.
Vorteil ist ein geringer Stromverbrauch sowie eine properitäre Funkverbindung, die nicht so leicht geknackt werden kann.
Für die Show werden 20 Module gebraucht. Der Aufwand für die Verdrahtung für das Funkmodul war mir einfach zu hoch.

Der folgende Ansatz basiert nun auf dem ESP8266. Die Hardware ist nun wesentlich schneller aufgebaut, da ich nur noch die Treiberstufe für die LED-Stripes hinzufügen muss. Das Modul hat nun einen Web-Server und sämtliche Einstellungen können hierüber vorgenommen werden. Weiterhin lässt sich die Firmware einfach über OTA (Over The Air) aktualisieren. Weiterhin schicken die Module Nachrichten an einen Node-Server. Über diesen Server kann man sehen, ob die Module alle Daten empfangen. Für mich sehr beruhigend, da unmittelbar vor der Show eine gewisse Hektik ausbricht und man keine Zeit alle 20 Module zu prüfen.



Node Server
Für den Node Server gibt es Web-Interface, um den Status zu kontrollieren.



Web-Interface
Das Web-Interface basiert auf dieser Vorlage http://www.john-lassen.de/en/projects/esp-8266-arduino-ide-webconfig



DMX Settings

Die Module sind alle identisch aufgebaut – also keine Schalter oder DIP-Switches um eine Kodierung vorzunehmen.
Hier kann man die Rolle des Moduls bestimmen bzw. welche DMX-Kanäle verwendet werden.



Der DMX Kanal wird nicht direkt bestimmt, sondern man wählt hier z.B. den Fahrer aus.
Das Modul ermittelt dann selber den Kanal und generiert eine Namen für das Modul.
Dieser Name ist nachher auf dem node-Server und WLAN-Router sichtbar.



Gamma-Korrektur

Über die Gamma-Korrektur lässt sich die Helligkeit reduzieren.

DMX output

Das DMX-Paket wird dann über  Serial1 ausgegeben. Somit kann man beliebige Geräte mit DMX-Eingang anbinden.

LED Pin

Ich habe mit mehreren Modulen experimentiert (NodeLUA, ESP12E) und unterschiedlichen Layouts.
Hier kann man die Zuordnung der LED Treiber zum PWM Pin auswählen, so dass das Layout flexibel ist.



DMX-Sender

Backstage oder bei Proben, möchte ich in der Lage sein die Module zu testen.
Jedes Modul kann eine Testsequenz per Multicast schicken.
Somit kann ich z.B. einen Fahrer auswählen und sehen, ob dessen drei Module aktiv sind.



 

Hardware

Grundsätzlich verwende ich ein NodeLua-Modul, welches auf dem ESP12E basiert.

Die erste Programmierung kann über USB erfolgen
Ein Spannungsregler ist bereits integriert
Achtung: es gibt Module, die für Vin nur 5V statt 12V haben
Die Treiberstufe befindet sich auf einer Platine, die auf der Unterseite verlötet wird.
Damit ist das Modul kompakt und schnell aufgebaut.

Als Treiber verwende ich den IRLR7843 mit der Bauform D-PAK. Diese SMD Bauform ist klein (aber nicht zu klein) und lässt sich gut auf eine Lochrasterplatine verlöten.
Wichtig sind der R DS (on) von 3.3m Ohm und V GS (th) von 1,5-2,3V, da der ESP mit 3,3V arbeitet.

 


