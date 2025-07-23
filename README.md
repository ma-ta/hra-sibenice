# Hra&nbsp;Š&nbsp;I&nbsp;\_&nbsp;E&nbsp;_&nbsp;I&nbsp;C&nbsp;E

> Multiplatformní implementace známé slovní hry Šibenice pro terminál

![Windows](https://img.shields.io/badge/OS-Windows-lightgrey?logo=pcgamingwiki&logoColor=white)
![DOS](https://img.shields.io/badge/OS-DOS-lightgrey?logo=d&logoColor=white)
![macOS](https://img.shields.io/badge/OS-macOS-black?logo=apple&logoColor=white)
![Linux](https://img.shields.io/badge/OS-Linux-black?logo=linux&logoColor=white)
![FreeBSD](https://img.shields.io/badge/OS-FreeBSD-black?logo=freebsd&logoColor=white)
![Android](https://img.shields.io/badge/OS-Android-3DDC84?logo=android&logoColor=white)
![WebAssembly](https://img.shields.io/badge/Web-Assembly-654FF0?logo=webassembly&logoColor=white)

![GitHub release (latest by date including pre-releases)](https://img.shields.io/github/v/release/ma-ta/hra-sibenice?include_prereleases)
![GitHub](https://img.shields.io/github/license/ma-ta/hra-sibenice)

![Šibenice – ikona](/res/github.png)


### Binárky ke stažení

- **⬇️&nbsp;[Stáhnout instalátor pro Windows&nbsp;🪟](//github.com/ma-ta/hra-sibenice/releases/download/v1.3.1/sibenice-setupwin.exe)**
- ⬇️&nbsp;Stáhnout All-in-1 balíček (všechny binárky): 
  [TAR.GZ](//github.com/ma-ta/hra-sibenice/releases/download/v1.3.1/sibenice-bin.tar.gz)&nbsp;📦, 
  [ZIP](//github.com/ma-ta/hra-sibenice/releases/download/v1.3.1/sibenice-bin.zip)&nbsp;📦&nbsp;&nbsp;|&nbsp;&nbsp;*[v1.1.0 pro macOS&nbsp;🍎](https://github.com/ma-ta/hra-sibenice/releases/tag/v1.1.0)*<br>
  (rozbalte stažený archiv a zvolte spustitelný [soubor](https://github.com/ma-ta/hra-sibenice/blob/v1.3.1/bin/#readme) dle vaší platformy):

| Platforma       | <span title="Instalátor">💽</span> | x86-64          | x86<br>32-bit  | x86<br>16-bit      | ARM64<br>(AArch64) | ARM 32-bit<br>(armhf) | RISC-V<br>(riscv64) | <span title="WebAssembly">Wasm<br>(wasm32)</span>
|-----------------|------------------------------------|-----------------|----------------|--------------------|--------------------|-----------------------|---------------------|--------------------------------------------------
| 🪟&nbsp;Windows | 🆕                                 | 🆕             | 🆕             |                    | 🆕                 |                       |                     |
| 🍎&nbsp;macOS   |                                    | ✅&nbsp;v1.1.0  |                |                    | ✅&nbsp;v1.1.0     |                       |                     |
| 🐧&nbsp;Linux   |                                    | 🆕              | 🆕            |                    | 🆕                 | 🆕                   | 🆕                  |
| 🤖&nbsp;Android |                                    |                 |                |                    | 🆕                 |                       |                     |
| 😈&nbsp;FreeBSD |                                    | 🆕              | 🆕            |                    | 🆕                 |                       |                     |
| 💾&nbsp;DOS     |                                    |                 | 🆕             | 🆕                 |                    |                       |                     |
| 🌐&nbsp;Web     |                                    |                 |                |                    |                    |                       |                     | 🆕

> **Legenda:**  
🆕&nbsp;–&nbsp;aktuální sestavení: **v1.3.1 (2025-07-10)**,  
✅&nbsp;–&nbsp;starší sestavení,  
💽&nbsp;–&nbsp;dostupný user-friendly (🐤 BFU) instalátor ve složce [bin-setup](https://github.com/ma-ta/hra-sibenice/tree/v1.3.1/bin-setup).

**Další odkazy:**
- [Přehled verzí &amp; changelog](//github.com/ma-ta/hra-sibenice/releases)
- [Adresář s binárkami](https://github.com/ma-ta/hra-sibenice/tree/v1.3.1/bin)


### Popis a stručné instrukce
Známá <a title="Wikipedie" href="//cs.wikipedia.org/wiki/%C5%A0ibenice_(hra)">slovní hra</a> (alias Oběšenec, Kat, Hangman) založená na hádání jednotlivých písmen tajného slova.
Program je napsán v jazyce C a pro jeho spuštění či sestavení by neměly být potřeba žádné dodatečné komponenty.

- **[Základní instrukce k sestavení](https://github.com/ma-ta/hra-sibenice/blob/v1.3.1/how_make.md)**
- [Nápověda ke hře](https://github.com/ma-ta/hra-sibenice/blob/v1.3.1/res/napoveda.md)
- [Známé chyby a nápady na další vývoj](https://github.com/ma-ta/hra-sibenice/blob/v1.3.1/res/poznamky.md)

#### Pozn. pro systém 🍎&nbsp;macOS
Stažené binární soubory nejsou podepsané, proto je pravděpodobně bude možné spustit až po odebrání příslušného příznaku.<br>
Např. příkazem: [xattr -d com.apple.quarantine sibenice-macUni].

#### Pozn. pro systém 🤖&nbsp;Android
Hra byla sestavena a otestována v emulátoru terminálu [Termux](https://termux.dev/en/), který je možné
nainstalovat přes alternativní obchod [F-Droid](https://f-droid.org/en/packages/com.termux/).

#### Pozn. pro systémy typu 💾&nbsp;DOS
U 32-bit binárky v případě zobrazení hlášky *"Load error: no DPMI - Get csdpmi\*.zip"* umístěte soubor [CWSDPMI.EXE](/bin/CWSDPMI.EXE) do stejného adresáře jako spustitelný soubor hry. Více o problematice např. [zde](//en.wikipedia.org/wiki/CWSDPMI) nebo [zde](https://sandmann.dotster.com/cwsdpmi/).  
Nativní 16-bit verze běží samostatně.

#### Použité kompilátory *(platformy)*:
- UN*X:
  - 🐧&nbsp;Ubuntu 24.04.2 LTS *(GCC 13.3.0) &ndash; [x86] [x86-64] [AArch64] [armhf] [riscv64]*
  - 🍎&nbsp;macOS 15.4 *(Apple Clang 17.0.0) &ndash; [x86-64] [AArch64] [Universal]*
  - 😈&nbsp;FreeBSD 14.3 *(Clang 19.1.7) &ndash; [x86] [x86-64] [AArch64]*
  - 🤖&nbsp;Android 14/Termux 0.118.3 *(Clang 20.1.7) &ndash; [AArch64]*
- 🪟&nbsp;Win:
  - Windows 11 *(MSVC 19.44.35211) &ndash; [x86] [x86-64] [AArch64]*
- 💾&nbsp;DOS:
  - FreeDOS 1.4:
    - *(Open Watcom 1.9) &ndash; [x86 16-bit]*
    - *(DJGPP GCC 12.2.0) &ndash; [x86 32-bit]*
- 🌐&nbsp;Web:
  - Wasm *(Emscripten 4.0.10) &ndash; [wasm32]*

### Snímky obrazovky

[Všechny dostupné screenshoty zde](https://github.com/ma-ta/hra-sibenice/tree/v1.3.1/res/screenshots)

<figure>
  <figcaption>macOS 15.4 (Sequoia) Terminal&nbsp;&ndash;&nbsp;obrazovka hry (v1.1.0):</figcaption>
  <img src="res/screenshots/hra-macos.png" alt="Šibenice - macOS 15.4 Terminal&nbsp;&ndash;&nbsp;obrazovka hry">
</figure>

<figure>
  <figcaption>Windows 11 (24H2)&nbsp;&ndash;&nbsp;obrazovka hry:</figcaption>
  <img src="res/screenshots/hra-win.png" alt="Šibenice - Windows 11 (24H2)&nbsp;&ndash;&nbsp;obrazovka hry">
</figure>

<figure>
  <figcaption>Webové rozhraní (Google Chrome 138):</figcaption>
  <img src="res/screenshots/hra-web.png" alt="Šibenice - webový prohlížeč">
</figure>

<figure>
  <figcaption>Herní statistiky:</figcaption>
  <img src="res/screenshots/kronika.png" alt="Šibenice - Statistiky">
</figure>

<figure>
  <figcaption>Informace o verzi (DOSBox-X 2025.05.03):</figcaption>
  <img src="res/screenshots/prepinace-dos.png" alt="Šibenice - MS-DOS">
</figure>

<figure>
  <figcaption>MS-DOS&nbsp;&ndash;&nbsp;hlavní menu:</figcaption>
  <img src="res/screenshots/menu-dos.png" alt="Šibenice - Menu">
</figure>

<figure>
  <figcaption>MS-DOS&nbsp;&ndash;&nbsp;obrazovka hry:</figcaption>
  <img src="res/screenshots/hra-dos.png" alt="Šibenice - MS-DOS">
</figure>

<figure>
  <figcaption>MS-DOS&nbsp;&ndash;&nbsp;slovo uhodnuto:</figcaption>
  <img src="res/screenshots/uhodl-dos.png" alt="Šibenice - MS-DOS">
</figure>

<figure>
  <figcaption>MS-DOS&nbsp;&ndash;&nbsp;tři a méně žitovů:</figcaption>
  <img src="res/screenshots/malozivotu-dos.png" alt="Šibenice - MS-DOS">
</figure>

<figure>
  <figcaption>MS-DOS&nbsp;&ndash;&nbsp;slovo neuhodnuto:</figcaption>
  <img src="res/screenshots/neuhodl-dos.png" alt="Šibenice - MS-DOS">
</figure>

<figure>
  <figcaption>MS-DOS&nbsp;&ndash;&nbsp;obrazovka prohry:</figcaption>
  <img src="res/screenshots/prohra2-dos.png" alt="Šibenice - MS-DOS">
</figure>

<figure>
  <figcaption>MS-DOS&nbsp;&ndash;&nbsp;konec programu:</figcaption>
  <img src="res/screenshots/konec-dos.png" alt="Šibenice - MS-DOS">
</figure>
