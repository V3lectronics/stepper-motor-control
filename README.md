# stepper-motor-control


<div style='text-align: center;'> 
Wydział Mechatroniki Politechniki Warszawskiej <br>
Warszawa
<br>
<br>

<img src='https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcSaFxay4U1OoyID6XGx9bkc38p_8p54DM-uxw&s' width='60%'> 

<br>
<br>

<h1 style='text-align: center;'>Dwuosiowe gniazdo obrotowe</h1>

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

<div style='text-align: center;'> 05 marzec 2025 </div> 

<br>
<br>

# Spis treści
Założenia programu 1
<br>
Planowane postępy 1
<br>
Instrukcja obsługi etapu pierwszego 1
<br>

<br>
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


# Instrukcja obsługi etapu pierwszego 
### Instalacja
Wykonać komendę:
```bash
$ git clone https://github.com/V3lectronics/stepper-motor-control.git
```

Alternatywnie można pobrać lub skopiować pliki z [repozytorium](https://github.com/V3lectronics/stepper-motor-control).


### Konfiguracja
Zachowanie programu może być konfigurowane w pliku `motor.conf` o następującym formacie:

```txt
command_file_dir <scieżka_do_pliku>

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
-  `set_pos <stopnie w OX i stopnie w OY>`

Program wczytuje listę poleceń z pliku `commands.txt` określonego na początku pliku  `motor.conf`. Format pliku z poleceniami:

```
<polecenie> <argument>
```

Przykład:

```
position 000000
sleep 1000
up 5
sleep 750
right 5
sleep 500
down 5
sleep 250
left 5
```






