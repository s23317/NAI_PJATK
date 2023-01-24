1.Przygotuj repozytorium Git. 



2.Napisz program, który będzie pozwalał na policzenie jednej z 3 funkcji. Funkcja ta będzie wybierana za pomocą nazwy z linii komend. Skorzystaj z tablicy asocjacyjnej (std::map) i z funkcji lambda aby to osiągnąć.


UWAGA: Wybór funkcji nie może być zrealizowany za pomocą instrukcji warunkowych.

Programik niech działa tak:



NAZWA

       lab1 - oblicza wartość wybranej funkcji 
SKŁADNIA

       lab1 [FUNKCJA] [ARGUMENT]...
OPIS

       Liczy wartość funkcji FUNKCJA z podanymi ARGUMENT-ami. Wynik
       jest wypisywany na standardowe wyjście.
       Jeśli nie podano nazwy funkcji, program wypisze informację o sobie.
   Dostępne funkcje:
   
       
       sin   liczy wartość funkcji sinus dla jednego argumentu.
       
       add   sumuje dwie wartości
       
       mod   oblicza wartość reszty z dzielenia
          
   Kod wyjściowy:
   
       0      OK,
       1      Błąd - na przykład zła nazwa funkcji
PRZYKŁADY

       lab1 mod 10 2
            wyświetli wynik 0
       lab1 add 2 2
            wyświetli wynik 4 
