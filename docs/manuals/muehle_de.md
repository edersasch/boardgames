# Mühle

Spielregeln aus [Wikipedia](https://de.wikipedia.org/wiki/M%C3%BChle_(Spiel)#Spielablauf), Version vom 29. Juli 2018:

> Das Spiel läuft in drei Phasen ab:
>
> - Setzphase: Die Spieler setzen abwechselnd je einen Stein, insgesamt je
> neun, auf Kreuzungs- oder Eckpunkte des Brettes
>
> - Zugphase: Die Spielsteine werden gezogen, das heißt, pro Runde darf jeder
> Spieler einen Stein auf einen angrenzenden, freien Punkt bewegen. Kann ein
> Spieler keinen Stein bewegen, so hat er verloren.
>
> - Endphase: Sobald ein Spieler nur noch drei Steine hat, darf er mit seinen
> Steinen springen, das heißt, er darf nun pro Runde mit einem Stein an einen
> beliebigen freien Punkt springen. Sobald ihm ein weiterer Stein abgenommen
> wird, hat er das Spiel verloren.
>
> Drei Steine einer Farbe, die in einer Geraden auf Feldern nebeneinander
> liegen, nennt man eine „Mühle“. Wenn ein Spieler eine Mühle schließt, darf er
> einen beliebigen Stein des Gegners aus dem Spiel nehmen, sofern dieser Stein
> nicht ebenfalls Bestandteil einer Mühle ist. Die offiziellen Turnierregeln
> erlauben seit 2010 das Schlagen eines Steines aus einer geschlossenen Mühle,
> wenn der Gegner nur noch Steine in geschlossenen Mühlen hat.

Die offiziellen Regeln: <http://www.muehlespiel.eu/images/pdf/WMD_Spielregeln.pdf>


## Merkmale des Spiels

- Je nach Fenster- oder Bildschirmausrichtung wird eine horizontale bzw. vertikale Darstellung verwendet.
- Das Spiel ist auch bei kleinen Fenstern bzw. Bildschirmen bedienbar.
- Spielsteine können zum Zielfeld gezogen, oder durch drücken des Zielfeldes dorthin bewegt werden.
- Ein Aufbau Modus, um Spielsteine ohne Regelprüfung auf Felder zu setzen, ist vorhanden.
- Eine Züge Liste kann eingeblendet werden. Je nach Darstellung erscheint sie rechts oder unten.
- Züge Listen können importiert und exportiert werden.
- Für jeden Spieler kann ein Computer Gegner aktiviert werden.
- Beim Beenden wird die aktuelle Sitzung gespeichert und bei Start wiederhergestellt.


## Installation


### Linux

Auf einem mit dem Internet verbundenen System wird die aktuelle Version des
Spiels aus der Kommandozeile mit folgendem Befehl installiert oder
aktualisiert:

```bash
wget -O - https://raw.githubusercontent.com/edersasch/boardgames/master/tools/muehle_installer.sh | bash
```


## Bedienung des Spiels

Beim Starten des Spiels werden folgende Zustände wiederhergestellt:

- [Züge Liste](#züge-liste), deren [Sichtbarkeit](#ein--und-ausblenden-der-züge-liste) und letzte Stellung
- [Aufbau Modus](#aufbau-modus)
- [Sichtbarkeit der Spielinformationen](#ein--und-ausblenden-von-spielinformationen) ([Zeit pro Spieler](#aktuell-verbrauchte-zeit-pro-spieler) beginnt bei `0:00`.)
- [Computer Gegner Einstellungen](#computer-gegner-einstellungen) ([Computer Gegner](#computer-gegner) sind aus.)
- [Farben](#schieberegler-für-farbeinstellung)

Sind die Spielsteine nicht in Ausgangsstellung, gilt die Züge Liste als
verändert und die Aktionen

- [Neues Spiel](#neues-spiel)
- [Aufbau Modus](#aufbau-modus)
- [Import einer Züge Liste](#import-einer-züge-liste)

müssen von Anfang an [bestätigt](#gewählte-aktion-bestätigen--abbrechen)
werden.


### Einen Zug ausführen

Auswählbare Spielsteine werden etwas größer dargestellt als die anderen. Der
ausgewählte Stein ist an der grünen Farbe seines Ursprungsfeldes erkennbar. Ist
bereits ein Feld grün, war nur ein Spielstein auswählbar. Mögliche Zielfelder
zeigen eine Vorschau des Spielsteins. Einen Zug führt man aus, in dem man einen
Spielstein durch anklicken auswählt und ihn dann entweder auf ein Zielfeld
zieht, oder auf ein Zielfeld klickt. Ist eine Mühle entstanden, werden
entfernbare gegnerische Spielsteine durch eine rote Hintergrundfarbe markiert.
Klickt man einen solchen Stein an, wandert er vom Spielfeld ins Gefängnis.

Hat ein Spieler gewonnen, beginnt die Farbe seiner Spielsteine zu pulsieren.
Bei unentschieden durch 50 Züge ohne schlagen oder dreimalige
Stellungswiederholung werden alle Spielsteine verkleinert.


### Bedienknöpfe am Spielfeld


#### Neues Spiel

![refresh](../../data/refresh.svg)&nbsp;

Die Spielsteine des ersten Spielers landen in der horizontalen Darstellung auf
ihren Startfeldern links vom Spielfeld. In der vertikalen Darstellung sind
diese Felder über dem Spielfeld. Die Steine des zweiten Spielers werden
entsprechend rechts bzw. darunter platziert. Bei veränderter Züge Liste ist
eine [Bestätigung](#gewählte-aktion-bestätigen--abbrechen) nötig, da die Liste durch Beginn
eines neues Spiels geleert wird. Ein eventuell aktiver
[Aufbau Modus](#aufbau-modus) wird beendet. Ein eventuell aktiver
[Computer Gegner](#computer-gegner) für den ersten Spieler wird deaktiviert.


#### Computer Gegner

![engine](../../data/engine.svg)&nbsp;

Für jeden Spieler existiert je ein Knopf mit der dazugehörigen Farbe. Ein
aktivierter Computer Gegner, erkennbar an einem grünen Rahmen um den Knopf,
lässt den Computer den nächsten Zug für die entsprechende Farbe berechnen.
Während der Berechnung pulsiert die Farbe des Knopfes. Das Aktivieren beider
Knöpfe lässt den Computer die Partie alleine weiterspielen. Deaktivieren eines
Knopfes, während der Computer gerade für diese Seite rechnet, führt zum Abbruch
der Berechnung und der sofortigen Ausführung des bis dahin besten gefundenen
Zuges. Betreten des [Aufbau Modus](#aufbau-modus) oder Auswählen eines Zuges
aus der [Züge Liste](#züge-liste) deaktiviert Computer Gegner. Umgekehrt wird
beim Aktivieren eines Computer Gegners der [Aufbau Modus](#aufbau-modus)
beendet. Ein eventuell aktiver Computer Gegner für den ersten Spieler (linker
bzw. oberer der beiden Knöpfe) wird deaktiviert, wenn ein
[neues Spiel](#neues-spiel) begonnen wird.


#### Ein- und Ausblenden der Züge Liste

![list](../../data/list.svg)&nbsp;

Steuert die Sichtbarkeit der [Züge Liste](#züge-liste)


#### Aufbau Modus

![setup](../../data/setup.svg)&nbsp;

Im Aufbau Modus kann jeder Spielstein jeder Farbe ausgewählt werden und auf
jedes mögliche freie Feld mit Ausnahme der Start- und Gefängnisfelder der
anderen Farbe bewegt werden. Bei veränderter Züge Liste ist eine
[Bestätigung](#gewählte-aktion-bestätigen--abbrechen) nötig, da die Liste durch
Betreten des Aufbau Modus geleert wird. Starten des Aufbau Modus deaktiviert
[Computer Gegner](#computer-gegner). Im Aufbau Modus werden keine Spielregeln
geprüft. Dadurch sind Stellungen möglich, die in einem regulären Spiel nicht
erreicht werden können. Achten Sie daher genau auf das, was Sie tun. Der Aufbau
Modus endet bei einem weiteren Druck auf den Knopf oder durch Aktivieren eines
[Computer Gegners](#computer-gegner). Bei Start eines
[neuen Spiels](#neues-spiel) oder [Import](#import-einer-züge-liste) einer Züge
Liste ist eine [Bestätigung](#gewählte-aktion-bestätigen--abbrechen) nötig,
bevor der Aufbau Modus verlassen wird, da die aufgebaute Stellung durch diese
Aktionen verloren geht. Der Spieler der zuletzt ausgewählten Spielsteinfarbe
ist dann am Zug.


#### Ein- und Ausblenden von Spielinformationen

![gameinfo](../../data/info.svg)&nbsp;

Am oberen Rand werden folgende Informationen angezeigt:


##### Die Anzahl der bis zur aktuellen Stellung getätigten Züge

Ein Zug besteht aus den zwei Halbzügen des ersten und zweiten Spielers. Die
Zahl wechselt bei abwechselndem Spiel also nur nach jeder zweiten Aktion. Ein
Doppelkreuz `#` ist der Zugnummer vorangestellt. Die Beschriftung `# 12` z.B.
zeigt an, dass bereits zwölf Züge gespielt wurden.


##### Aktuell verbrauchte Zeit pro Spieler

In jeweils einer farblich passenden Box pro Spieler wird die verbrauchte Zeit
in Sekunden angezeigt. Solange abwechselnd wie in einer Turniersituation
gezogen wird, sagt die Zeit aus, wer wie lange überlegt hat, um zur aktuellen
Stellung zu gelangen. Diese spieltechnische Bedeutung der Zeitanzeige geht in
folgenden Fällen verloren, was durch einen Warnhinweis
![warning](../../data/warning.svg) vor den Uhren angezeigt wird:

- In der [Züge Liste](#züge-liste) wird eine frühere Stellung ausgewählt.
- Der [Aufbau Modus](#aufbau-modus) wird betreten.
- Das Spiel startet nicht in der Grundstellung. Die Zeit pro Spieler beginnt trotzdem bei `0:00`.

Durch Beginn eines [neuen Spiels](#neues-spiel) wird die Zeit für beide
Spieler auf `0:00` gesetzt und ein eventuell sichtbarer Warnhinweis
verschwindet.


##### Vorschau der Computer Gegner Berechnung

Wenn der Computer Gegner aktiv wird, erscheint nach spätestens einer Sekunde
hinter der Box mit der Zeit noch eine Box mit Einblicken in die Berechnung.
Rechts neben dem Computer Gegner Icon ![engine](../../data/engine.svg) folgt
ein Kästchen mit der Bewertung der aktuellen Stellung:

- Die Hintergrundfarbe entspricht der des besser bewerteten Spielers.
- Der Text ist ein Pluszeichen und die Bewertung als Zahl.

Die Bewertung ergibt sich aus dem Unterschied der Anzahl der möglichen
Zielfelder für Spielsteine multipliziert mit 16.

`+64` z.B. bedeutet, dass der entsprechende Spieler vier Felder mehr zur
Verfügung hat als sein Gegner, um einen Stein darauf zu ziehen.

Dann folgt das Symbol für die Rechentiefe ![depth](../../data/depth.svg) und
die erreichte Tiefe als Zahl dahinter. Die letzte Information sind drei Felder
mit jeweils einem Zug in der passenden Spielerfarbe:

- Der beste nächste Zug, den der Computer bis jetzt gefunden hat
- Die Antwort des Gegners darauf
- Die Antwort darauf vom aktuellen Spieler


#### Ein- und Ausblenden der Einstellungen

![settings](../../data/settings.svg)&nbsp;

Steuert die Sichtbarkeit der am oberen Rand angezeigten
[Einstellungen](#einstellungen).


#### Anleitung anzeigen

![help](../../data/help.svg)&nbsp;

Ein Druck auf den Knopf zeigt diese Anleitung an. Tritt dabei ein Problem auf,
wird oberhalb des Spielfeldes eine Versionsinformation gezeigt, bis der Knopf
erneut betätigt wird.


#### Gewählte Aktion bestätigen / abbrechen

![cancel](../../data/cancel.svg)&nbsp;

Wurde die Züge Liste verändert, ist für die Aktionen
[Neues Spiel](#neues-spiel), [Aufbau Modus](#aufbau-modus) und
[Import](#import-einer-züge-liste) eine Bestätigung nötig. Für das
[Löschen einer Variante](#variante-löschen) ist immer eine Bestätigung nötig.
Der gerade gedrückte Knopf wird, je nach Anordnung seiner Nachbarknöpfe, nach
rechts bzw. unten geschoben und dabei rot gefärbt. An der ursprünglichen
Stelle des Knopfes ist jetzt der "Abbrechen" Knopf sichtbar. Zum Ausführen der
Aktion muss der verschobene Knopf erneut gedrückt werden. Betätigen des
"Abbrechen" Knopfes oder dreisekündiges Warten rücken den Bedienknopf mit
seiner normalen Farbe wieder auf seinen angestammten Platz, ohne eine Aktion
auszulösen.


### Züge Liste

- Normales Spiel fügt Knöpfe zur Liste hinzu. Der zuletzt hinzugefügte Knopf ist automatisch ausgewählt.
- Auswählen eines gelisteten Zuges bringt die Spielsteine in die dazugehörige Stellung. Eventuell aktive [Computer Gegner](#computer-gegner) werden deaktiviert.
- Wird ein Zug getätigt, obwohl an der ausgewählten Stelle bereits ein oder mehrere Züge folgen, entsteht eine Variante, falls die resultierende Stellung sich von allen Folgezügen unterscheidet. Ansonsten wird der bereits existierende Zug ausgewählt.
- Varianten werden in einer zur übergeordneten Variante leicht eingerückten neuen Zeile angezeigt.
- Varianten können nach [Bestätigung](#gewählte-aktion-bestätigen--abbrechen) wieder [gelöscht](#variante-löschen) werden.
- Ist nach einem Löschvorgang nur noch eine Variante übrig, werden die Züge zur übergeordneten Variante hinzugefügt.
- Die Liste kann [importiert](#import-einer-züge-liste) und [exportiert](#export-der-züge-liste) werden.

Die Beschriftung der Knöpfe entspricht der [WMD-Notation](http://muehlespieler.de/x_uebungen/index.php?page=begriffe_notation):
In der Setzphase steht im Knopf lediglich das Zielfeld, z.B. `B4`. In der Zug-
und Endphase zeigt der Text das Start- und Zielfeld durch einen Bindestrich
getrennt, z.B. `D1-D2`. Wird ein Stein geschlagen, endet die Beschriftung mit
einem `x` und dem Feld, von dem der Stein genommen wurde, z.B. in der Setzphase
`A1 xD2` oder in der Zug- bzw. Endphase `F4-F6 xD7`.


#### Bedienknöpfe an der Züge Liste

##### Ausgangsstellung

![home](../../data/home.svg)&nbsp;

Die Spielsteine kehren auf ihre Startfelder bzw. auf die im
[Aufbau Modus](#aufbau-modus) gesetzten Felder zurück. Eventuell aktive
[Computer Gegner](#computer-gegner) werden deaktiviert.


##### Rückwärts / Vorwärts

![back](../../data/back.svg) ![forward](../../data/forward.svg)

Züge werden vor und zurück geblättert, ohne die gerade ausgewählte Variante
zu verlieren, selbst wenn eine übergeordnete Variante betreten und wieder
verlassen wird. Eventuell aktive [Computer Gegner](#computer-gegner) werden
deaktiviert.


##### Import einer Züge Liste

![import](../../data/import.svg)&nbsp;

Eine in einem Datei Auswahl Dialog gewählte Züge Liste wird importiert. Bei
veränderter Züge Liste oder aktivem [Aufbau Modus](#aufbau-modus) ist eine
[Bestätigung](#gewählte-aktion-bestätigen--abbrechen) nötig, da die bestehende
Liste bzw. aufgebaute Stellung durch einen Import verloren geht.


##### Export der Züge Liste

![export](../../data/export.svg)&nbsp;

Die Züge Liste wird als in einem Datei Auswahl Dialog gewählte Datei
exportiert.


#### Bedienknöpfe für Varianten in der Züge Liste

##### Variante löschen

![delete](../../data/delete.svg)&nbsp;

Vor Ausführung des Löschvorgangs ist eine
[Bestätigung](#gewählte-aktion-bestätigen--abbrechen) nötig. Beim Löschen der Variante mit
dem aktuell ausgewählten Zug wird die [Ausgangsstellung](#ausgangsstellung)
aktiv.


##### Folgezüge ein- und ausblenden

![show](../../data/show.svg) ![hide](../../data/hide.svg)

Ein Auge ist bei Varianten mit mehr als einem Zug sichtbar. Ein Druck auf das
offene Auge verbirgt die Züge. Der Knopf erhält daraufhin das Bild des
geschlossenen Auges. Ein Druck darauf oder auf den dauerhaft sichtbaren ersten
Zug der Variante blendet alle Züge wieder ein und das offene Auge erscheint
wieder auf dem Knopf.


### Einstellungen

#### Computer Gegner Einstellungen

![engine](../../data/engine.svg) ![depth](../../data/depth.svg)

Hier kann mit Hilfe der Plus- und Minustasten die Rechentiefe des Computer
Gegners eingestellt werden. Je größer die Zahl ist, desto besser spielt der
Computer, wofür er aber auch mehr Zeit benötigt. Die Rechenzeit kann von einer
Zahl zur nächsten stark ansteigen. Liefert das Spiel bei z.B. Tiefe 12 noch
zufriedenstellend schnell Antwort, kann bei Tiefe 13 die Wartezeit unakzeptabel
lang sein. Die Rechenzeit hängt vom verwendeten Computer und der aktuellen
Spielstellung ab.

![engine](../../data/engine.svg) ![time](../../data/time.svg)

Die Plus- und Minustasten stellen die maximale Rechenzeit in Sekunden ein,
erkennbar am "s" hinter der Zahl im Kästchen. Wie gut der Zug ist, den der
Computer in der Zeit findet, hängt von der bis dahin erreichten Rechentiefe und
somit vom verwendeten Computer und der aktuellen Spielstellung ab.


#### Schieberegler für Farbeinstellung

Der obere Regler stellt den hellen Farbton für den ersten Spieler ein, der
untere Regler den dunklen Farbton für den zweiten Spieler. Die Farben werden
sofort im ganzen Spiel angepasst.


## Links

- Infos über das Spiel und Turniere, mit Beispielen und Übungen: <https://muehlespieler.de/>
- Beschreibung der computergestützten vollständigen Lösung: <http://library.msri.org/books/Book29/files/gasser.pdf>


----

![Creative Commons Lizenzvertrag](https://i.creativecommons.org/l/by-sa/4.0/88x31.png)

Dieses Werk ist lizenziert unter einer [Creative Commons Namensnennung - Weitergabe unter gleichen Bedingungen 4.0 International Lizenz](https://creativecommons.org/licenses/by-sa/4.0/).
