Computer Vision

Wstęp
Computer Vision jest to dziedzina Informatyki zajmująca się przetwarzaniem obrazu w celu wydobycia z niego informacji. Składają się na to metody uzyskiwania, przetwarzania analizowania i 'rozumienia' danych ze świata rzeczywistego w celu przetworzenia tych danych na liczby. Na przykład mamy zdjęcie, i chcemy określić, czy na tym zdjęciu znajduje się kwadrat. My widzimy, że jest tam kwadrat, natomiast komputer 'widzi' tylko ciąg liczb (taki gupi jest). 

Machine Vision (System wizyjny) - system służący do przemysłowego przetwarzania obrazów w celu automatyzacji różnych procesów, na przykład segregacja paczek według kodów QR określających miejsce docelowe.


CV jest elementem wykorzystywanym także jako część większych systemów inteligentnych, na przykład (mniej lub bardziej poważne):

Air Hockey https://www.youtube.com/watch?v=CjzSeOg8oTs

system automatycznego rozpoznawania, namierzania i odstraszania wiewiórek za pomocą pistoletu na wodę, kamerki i systemu rozpoznawania obrazu https://www.youtube.com/watch?v=_mbvqxjFZpE

Rozpoznawanie i uczenie rozpoznawania obiektów https://www.youtube.com/watch?v=1GhNXHCQGsM.



Temat jest duży, ale spokojnie, na zajęciach skupimy się raczej na podstawach. Jeśli będzie zainteresowanie, to możemy poświęcić na to trochę więcej czasu.
Jeśli ktoś jest zainteresowany sporym zbiorem publikacji z tego tematu: http://www.vision.caltech.edu/


Istotne elementy:
	
cv::Mat - typowa struktura danych prezchowująca informację o macierzy (czyli na przykład o ramce obrazu).

cv::VideoCapture - urządzenie do przechwytywania obrazu.

cv::waitKey - sprawdzenie i pobranie wciśniętego klawisza - funkcja czeka przez zadaną liczbę milisekund.

Linki
	
http://docs.opencv.org/modules/highgui/doc/user_interface.html

http://docs.opencv.org/doc/tutorials/core/table_of_content_core/table_of_content_core.html

http://docs.opencv.org/doc/tutorials/imgproc/imgtrans/canny_detector/canny_detector.html

http://docs.opencv.org/doc/tutorials/imgproc/imgtrans/canny_detector/canny_detector.html

http://docs.opencv.org/3.0-beta/index.html

http://docs.opencv.org/3.1.0/#gsc.tab=0


Zadania


Rozpocznijmy przygodę z OpenCV. Na dziś zrobimy tylko instalację i proste demo z odbiciem lustrzanym.

Uwaga - kroki początkowe można zrobić zgodnie z https://docs.opencv.org/4.x/d3/d52/tutorial_windows_install.html albo z https://stackoverflow.com/questions/35984678/use-opencv-with-clion-ide-on-windows#answer-36422689



	
Zainstaluj u siebie biblioteki do OpenCV - w zależności od systemu operacyjnego proces będzie wyglądał trochę inaczej. Najłatwiej jest pod Linuksem (apt install libopencv-dev). Na pracowni trzeba sobie zainstalować (a właściwie to skompilować) bibliotekę OpenCV (https://github.com/opencv/opencv)
	
Przygotuj projekt wykorzystujący bibliotekę OpenCV
	
Niech program który jest w Twoim repozytorium będzie wyświetlał odbicie lustrzane tego co widzi kamerka. Jeśli nie działa okno, to niech zamiast okna, wynik pojawi się w serii plików png.
	
Niech też program się zakończy w momencie wciśnięcia klawisza ESC.
	
Jeśli się udało daj znać prowadzącemu - dostaniesz plusa.

 


Zadanie dodatkowe

To samo, ale w innym języku programowania. W obu wersjach proszę wykonać wyszukiwanie konturów i porównać czas między momentem po pobraniu ramki z kamery a wybraniem największego konturu.
