# WOLF (Word-List-Filter)

## Info

Es ist eine Datenstruktur in Form einer Liste (Vektor) mit unterschiedlichen Wörtern gegeben.
Ein Suchalgorithmus soll eine Liste mit Wörtern aus dieser Wortliste ermitteln, deren erste Zeichen mit einem Suchstring übereinstimmen.
Der Algorithmus soll moderne Multi-Core-Prozessoren ausnutzen, die Suche also auf möglichst viele Cores verteilen.

---

## Aufgabe

Entwerfen Sie eine geeignete Datenstruktur für die Wortliste.
Wählen Sie einen geeigneten Such-Algorithmus und implementieren Sie ihn in Java, C# oder C++ auf einer Windows-Plattform.

Es darf davon ausgegangen werden, dass die Wortliste dem Suchalgorithmus während der Suche exklusiv zur Verfügung steht.
Implementieren Sie einen Testclient, über den das Suchmuster eingegeben werden kann, der die Laufzeit einer Suche ermittelt und das Suchergebnis ausgibt.
Wie würden Sie vorgehen, wenn die Suche inkrementell durchzuführen wäre (d.h., die Suche beginnt bereits, wenn das erste Zeichen eingegeben wird)? Wie könnte das GUI in diesem Fall aussehen?

Zu Demonstrationszwecken soll eine Wortliste verwendet werden, die aus allen möglichen Kombinationen von 4 Großbuchstaben besteht (AAAA bis ZZZZ). Die Reihenfolge der Wörter in de r Liste ist zufällig.
