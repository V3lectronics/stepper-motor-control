

<div style='text-align: center;'> 
Wydział Mechatroniki Politechniki Warszawskiej <br>
Warszawa
<br>
<img src='https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcSaFxay4U1OoyID6XGx9bkc38p_8p54DM-uxw&s' width='60%'> 


<br>
<br>
<hr>
<h1 style='text-align: center;'>Dwuosiowe gniazdo obrotowe</h1>
<hr>
<div style='text-align: center;'> 
Jednostka modułowa:<br>
ZAP II – Zasady programowania strukturalnego
</div>  
</div> 
<br>
<br>

<div style='text-align: right;'> 
Wykonał:
Jan Waga
</div> 
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>

<div style='text-align: center;'> 05 marzec 2025 </div> 

# Spis treści
Założenia programu 1
Planowane postępy 1
Instrukcja obsługi etapu pierwszego 1

<br>

# Założenia programu 
Głównym zadaniem oprogramowania jest ustawianie gniazda w zadanym kierunku poprzez odpowiednie sterowanie silnikami krokowymi.

Dodatkowo, ma możliwość sekwencyjnego wykonywania listy poleceń wczytanej z pliku oraz zapisywania historii ruchów do pliku.

W zależności od możliwości czasowych zostaną dodane pomniejsze funkcjonalności.

# Planowane postępy 
### 1 Etap
W pierwszym etapie program powinien:
- umieć precyzyjnie poruszać silnikami krokowymi
- wczytywać listę poleceń z pliku tekstowego `commands.txt`
- zapisywać wykonane ruchy do pliku tekstowego `history.txt`

Hash commitu pierwszego etapu: `c1a1512`.

### 2 Etap
W drugim etapie program powinien:
- Wspierać regulację prędkości na zasadzie "rampy"
- Mieć możliwość poruszania dwoma silnikami na raz
- Wspierać niektóre G kody zamiennie z komendami `up`, `down`, itd.

# Instrukcja obsługi etapu pierwszego 
### Instalacja & uruchomienie
Program może działać tylko na urządzeniu wyposażonym w piny GPIO, które są niezbędne do komunikacji ze sterownikami silników.

1. 
Wykonać komendę:
```zsh
$ git clone https://github.com/V3lectronics/stepper-motor-control.git
```

Alternatywnie można pobrać lub skopiować kod z [repozytorium](https://github.com/V3lectronics/stepper-motor-control).

2. 
Nadać uprawnienia i uruchomić program
```zsh
$ chmod +x stepper-motor-control
$ ./stepper-motor-control
```

W razie potrzeby, program można skompilować przy użyciu komendy:
```zsh
$ g++ main.cpp linked-list.cpp -o stepper-motor-control -l
wiringPi
```

Wygodnym sposobem na uruchamianie programu jest korzystanie ze skryptu `automate-testing.sh`. Pobiera on najnowszą wersję repozytorium, podejmuję próbę kompilacji a następnie uruchamia program. W przypadku niepowodzenia kompilacji, uruchamia poprzednią skompilowaną wersję programu.

> [!danger] Ostrożnie 
>  Przerwanie działania programu (e.g. przez CTRL+C) może spowodować pozostawienie niektórych pinów GPIO w stanie wysokim, co powoduje ciągłe doprowadzanie zasilania do cewek, co z kolei może przegrzać silnik.

### Konfiguracja
Zachowanie programu może być konfigurowane w pliku `motor.conf` o następującym formacie:

```txt
command_file_dir
<ścieżka_do_pliku_z_komendami>

history_file_directory
<ścieżka_do_pliku_historii>

name <nazwa_pierwszego_silnika>
degrees_per_full_step <ilość_stopni_na_1_krok>

name <nazwa_drugiego_silnika>
degrees_per_full_step <ilość_stopni_na_1_krok>
```

### Zadawanie poleceń
Po uruchomieniu program zacznie sekwencyjnie wykonywać polecenia.

Możliwe jest wykonanie następujących poleceń:
-  `up <stopnie>`
-  `down <stopnie>`
-  `left <stopnie>`
-  `right <stopnie>`
-  `sleep <czas w ms>`

Dodatkowo, istnieje polecenie `end 0` - sygnalizuje ono zakończenie pliku z komendami.

Program wczytuje listę poleceń z pliku `commands.txt` określonego na początku pliku  `motor.conf`. Format pliku z poleceniami:

```
<polecenie> <argument>
```

W przypadku poleceń ruchu, argumentem jest ilość kroków, które mają zostać wykonane, zaś w przypadku polecenia `sleep`, argumentem jest ilość milisekund, do odczekania przed wykonaniem następnej komendy.

Przykład pliku `commands.txt`:

```
sleep 1000
up 5
sleep 750
right 5
sleep 500
down 5
sleep 250
left 5
end 0
```







