# MetexSpeaker - Interface głosowy do odczytu wartości z przyrządu pomiarowego METEX M-3860D

  Projekt został zbudowany w oparciu o kontroler ESP32 w wersji Wemos D32 mini , przetwornik audio I2S MAX98357A oraz mostek USB RS232 UART

![IMG_2045](https://user-images.githubusercontent.com/17962241/168257330-691578a7-1ef5-47b8-8bc7-7c52ca28de10.JPG)

![IMG_2046](https://user-images.githubusercontent.com/17962241/168257485-fea47770-559c-4119-b77a-37aefa643c98.JPG)

![IMG_2047](https://user-images.githubusercontent.com/17962241/168257552-69bd96af-bcce-4055-86a7-1970930b09b5.JPG)

![IMG_2033](https://user-images.githubusercontent.com/17962241/168257628-a994a2a2-ac3a-4992-8603-c5b0205f6a21.JPG)

# Opis budowy układu

Multimetr Metex M-3860D wyposażony został w interface szeregowy RS232 pracujący w standardzie baudrate 1200-7-2.Niestety do podłączenia konieczny jest kabel szeregowy zakończony specjalnym , bardzo nie standardowym konektorem, który nie jest dostępny na rynku. Na szczęście stworzenie własnego zamiennika nie stanowi wielkiego wyzwania. ![IMG_2042](https://user-images.githubusercontent.com/17962241/168260033-3eba8f12-91ee-40e9-adc7-9c5a3384f749.JPG)

Połączenie pomiędzy multimetrem Metex a ESP32 zrealizowane zostało za pomocą konwertera USB-RS232 UART

![konwerter-rs232-uart-ze-zlaczem-db9-sp3232-33v5v-waveshare-3965](https://user-images.githubusercontent.com/17962241/168258839-9e9a6178-3391-491b-900b-57894c61c10e.jpg)
  
Szczegóły protokołu komunikacyjnego użytego w Metex M-3860D oraz pinout konektora znaleźć można -> https://www.sbprojects.net/projects/m3850/index.php

  Odebrana z miernika ramka pomiarowa zostaje odpowiednio przetworzona poprzez kontroler ESP32 i wysłania  poprzez Wifi do serwisu Google Translate Text-to-Speech, który przetwarza tekst na plik audio clip MP3 wg zadanych parametrów tj. lektor, język, prędkość wymowy, itd. Odebrany plik MP3 zostaje następnie pobrany do ESP , zdekodowany i wysłany interfacem I2S do dekodera audio MAX98357A. Do wyjścia dekodera został podłączony mały głośnik 3W 4ohm. 
  
  Schemat podłączenia ESP32 z dekoderem widoczny jest poniżej:

![I2S-ESP32-Play-from-Memory_Steckplatine](https://user-images.githubusercontent.com/17962241/168258586-f9c6710a-1b62-4a56-9ea9-05c532f4cdcd.png)

# Efekt działania układu można zobaczyć poniżej:
[pomiar prądu DC     ](https://youtu.be/sveKjSaQzWs)
[pomiar napięcia DC     ](https://youtu.be/nhY50dIIWes)
[pomiar rezystancji     ](https://youtu.be/uiNAxEi9wzU)
[komunikat powitalny     ](https://youtu.be/Lm9R9BroX0s)


