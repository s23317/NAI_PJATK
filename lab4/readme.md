Wstęp do G.A.
    
Dzisiaj zajmiemy się wstępem do Algorytmu genetycznego. Przygotujemy podstawowe elementy, a na następnych zajęciach zajmiemy się samym algorytmem. Pracujemy w kontekście klasycznego podejścia do Algorytmów Genetycznych. Mamy genotypy, fenotypy, allele, funkcje dekodujące i tak dalej (to co było na wykładzie).



Zadanie 1



Wybierz jedno z zadań optymalizacyjnych. Ważne aby miało taką właściwość, że istnieje tam wiele lokalnych optimów i jedno optimum globalne.


Zadanie 2



Przygotuj funkcję dekodującą genotyp w fenotyp. Niech genotyp będzie ciągiem bitów (vector<int> albo vector<bool>) długości (100+(twój numer indeksu modulo 10)*2 ). Rozwiązanie zadania to będzie wektor liczb zmiennoprzecinkowych. W zależności od wybranego zadania będzie to X,Y, albo X0, X1, ... Xn.


BONUS (na dodatkowy punkt): Zastosuj kod Graya. Pewnie będzie wtedy część ułamkowa, część całkowita i to wszystko razy liczba współrzędnych wektora rozwiązania.


Zadanie 3



Przygotuj funkcję fitness. Wybierz wygodniejsze dla Ciebie rozwiązanie: albo funkcja fitness przyjmuje fenotyp, albo przyjmuje genotyp i wywołuje funkcję dekodującą.

Uwaga - fitness to funkcja którą maksymalizujemy. Pamiętaj o przekształcenia zadani minimalizacyjnego w zadanie maksymalizacyjne.

Oczywiście przetestuj dla jakichś wybranych punktów oraz dla punktu optymalnego.
