* Je nach Fenster- oder Bildschirmausrichtung wird eine horizontale bzw. vertikale Darstellung verwendet.
* Das Spiel ist auch bei kleinen Fenstern bzw. Bildschirmen bedienbar.
* Spielsteine können zum Zielfeld gezogen, oder durch drücken des Zielfeldes dorthin bewegt werden.
* Ein Aufbau Modus, um Spielsteine ohne Regelprüfung auf Felder zu setzen, ist vorhanden.
* Eine Züge Liste kann eingeblendet werden. Je nach Darstellung erscheint sie rechts oder unten.
* Für jeden Spieler kann ein Computer Gegner aktiviert werden.


# Sichtbare Hinweise

* Auswählbare Spielsteine sind größer als die anderen.
* Das Startfeld des ausgewählten Spielsteins ist grün.
* Mögliche Zielfelder eines ausgewählten Spielsteins zeigen eine leicht durchsichtige Vorschau des Spielsteins in der passenden Farbe.
* Ein Spielstein, der geschlagen werden kann, hat eine rote Hintergrundfarbe.


# Bedienknöpfe am Spielfeld

![refresh](../data/refresh.svg) Neues Spiel

: Die weißen Spielsteine landen in der horizontalen Darstellung auf ihren
Startfeldern links vom Spielfeld. In der vertikalen Darstellung sind diese
Felder über dem Spielfeld. Die schwarzen Steine werden entsprechend rechts bzw.
darunter platziert. Bei veränderter Züge Liste ist eine weitere Bestätigung
nötig, da die Liste durch Beginn eines neues Spiels geleert wird. Ein eventuell
aktiver Aufbau Modus wird beendet. Ein eventuell aktiver Computer Gegner für
Weiß wird deaktiviert.

![list](../data/list.svg) Züge Liste

: Hier erscheinen alle bisher getätigten Züge.

![setup](../data/setup.svg) Aufbau Modus

: Im Aufbau Modus kann jeder Spielstein jeder Farbe ausgewählt werden und auf
jedes mögliche freie Feld mit Ausnahme der Gefängnis Felder der anderen Farbe
bewegt werden. Bei veränderter Züge Liste ist eine weitere Bestätigung nötig,
da die Liste durch Betreten des Aufbau Modus geleert wird. Starten des Aufbau
Modus deaktiviert eventuell aktive Computer Gegner. Im Aufbau Modus werden
keine Spielregeln geprüft. Dadurch sind Stellungen möglich, die in einem
regulären Spiel nicht erreicht werden können. Achten Sie daher genau auf das,
was Sie tun. Der Aufbau Modus bleibt aktiv, bis er durch einen weiteren Druck
auf den Knopf beendet wird oder ein Computer Gegner aktiviert wird. Der Spieler
der zuletzt ausgewählten Spielsteinfarbe ist dann am Zug. Durch Betätigen des
"Neues Spiel" Knopfes wird der Aufbau Modus ebenfalls beendet.

![engine](../data/engine.svg) Computer Gegner

: Für jeden Spieler existiert je ein Knopf mit der dazugehörigen Farbe. Ein
aktivierter Computer Gegner, erkennbar an einem grünen Rahmen um den Knopf,
lässt den Computer den nächsten Zug für die entsprechende Farbe berechnen,
währenddessen der Rahmen leicht pulsiert. Das Aktivieren beider Knöpfe ist
möglich und lässt den Computer die Partie alleine weiterspielen. Deaktivieren
eines Knopfes, während der Computer gerade für diese Seite rechnet, führt zum
Abbruch der Berechnung und der sofortigen Ausführung des bis dahin besten
gefundenen Zuges. Betreten des Aufbau Modus oder auswählen eines Zuges aus der
Züge Liste deaktiviert Computer Gegner. Umgekehrt wird beim Aktivieren eines
Computer Gegners der Aufbau Modus beendet. Ein eventuell aktiver Computer
Gegner für Spieler weiß (linker bzw. oberer der beiden Knöpfe) wird
deaktiviert, wenn ein neues Spiel begonnen wird.


# Züge Liste

* Normales Spiel fügt Knöpfe zur Liste hinzu. Der zuletzt hinzugefügte Knopf ist automatisch ausgewählt.
* Die Beschriftung der Knöpfe entspricht der [WMD-Notation](http://muehlespieler.de/x_uebungen/index.php?page=begriffe_notation)
* Auswählen eines gelisteten Zuges bringt die Spielsteine in die dazugehörige Stellung. Eventuell aktive Computer Gegner werden deaktiviert.
* Wird ein Zug getätigt, obwohl an der ausgewählten Stelle bereits ein oder mehrere Züge folgen, entsteht eine Variante, falls die resultierende Stellung sich von allen Folgezügen unterscheidet. Ansonsten wird der bereits existierende Zug ausgewählt.
* Varianten werden in einer zur übergeordneten Variante leicht eingerückten neuen Zeile angezeigt.
* Varianten können nach Bestätigung wieder gelöscht werden.
* Ist nach einem Löschvorgang nur noch eine Variante übrig, werden die Züge zur übergeordneten Variante hinzugefügt.
* Die Liste kann importiert und exportiert werden.


## Bedienknöpfe an der Züge Liste

![home](../data/home.svg) Ausgangsstellung

: Die Spielsteine kehren auf ihre Startfelder bzw. auf die im Aufbau Modus
gesetzten Felder zurück. Eventuell aktive Computer Gegner werden deaktiviert.

![back](../data/back.svg) ![forward](../data/forward.svg) Rückwärts / Vorwärts

: Züge werden vor und zurück geblättert, ohne die gerade ausgewählte Variante
zu verlieren, selbst wenn eine übergeordnete Variante betreten und wieder
verlassen wird. Eventuell aktive Computer Gegner werden deaktiviert.

![import](../data/import.svg) Import

: Eine in einem Datei Auswahl Dialog gewählte Züge Liste wird importiert. Bei
veränderter Züge Liste ist eine weitere Bestätigung nötig, da die bestehende
Liste durch einen Import verloren geht.

![export](../data/export.svg) Export

: Die Züge Liste wird als in einem Datei Auswahl Dialog gewählte Datei
exportiert.


## Bedienknöpfe für Varianten in der Züge Liste

![delete](../data/delete.svg) Variante löschen

: Vor Ausführung des Löschvorgangs ist eine weitere Bestätigung nötig. Beim
Löschen der Variante mit dem aktuell ausgewählten Zug wird die Ausgangsstellung
aktiv.

![show](../data/show.svg) ![hide](../data/hide.svg) Folgezüge anzeigen / verbergen

: Ein Auge ist bei Varianten mit mehr als einem Zug sichtbar. Ein Druck auf das
offene Auge verbirgt die Züge. Der Knopf erhält daraufhin das Bild des
geschlossenen Auges. Ein Druck darauf oder auf den dauerhaft sichtbaren ersten
Zug der Variante blendet alle Züge wieder ein und das offene Auge erscheint
wieder auf dem Knopf.


# Spielregeln

Aus [Wikipedia](https://de.wikipedia.org/wiki/M%C3%BChle_(Spiel)#Spielablauf), Version vom 29. Juli 2018:

> Das Spiel läuft in drei Phasen ab:
>
> * Setzphase: Die Spieler setzen abwechselnd je einen Stein, insgesamt je
> neun, auf Kreuzungs- oder Eckpunkte des Brettes
>
> * Zugphase: Die Spielsteine werden gezogen, das heißt, pro Runde darf jeder
> Spieler einen Stein auf einen angrenzenden, freien Punkt bewegen. Kann ein
> Spieler keinen Stein bewegen, so hat er verloren.
>
> * Endphase: Sobald ein Spieler nur noch drei Steine hat, darf er mit seinen
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


# Links

* Infos über das Spiel und Turniere, mit Beispielen und Übungen: <http://muehlespieler.de/>
* Beschreibung der computergestützten vollständigen Lösung: <http://library.msri.org/books/Book29/files/gasser.pdf>

----

![Creative Commons Lizenzvertrag](https://i.creativecommons.org/l/by-sa/4.0/88x31.png)

Dieses Werk ist lizenziert unter einer [Creative Commons Namensnennung - Weitergabe unter gleichen Bedingungen 4.0 International Lizenz](http://creativecommons.org/licenses/by-sa/4.0/).
