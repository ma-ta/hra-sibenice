```
         +-----------------------------------------------------+
         |                                                     |
         |         Ma-TA {}       H R A                        |
         |            games            S I _ E _ I C E         |
         |                                                     |
         +--------------------------+--------------------------+
         |    (c) 2025  Freeware    |          v1.3.0          |
         +--------------------------+--------------------------+
         |    SESTAVENI PROJEKTU    |        2025-??-??        |
         +--------------------------+--------------------------+



+-------------------+-------------------------------+-------------------+
|                   |                               |                   |
|     ooooooooo     | +---+---+---+---+             |     ooooooooo     |
|   oo   o o   oo   | | G | C | C | / |             |   oo   O o   oo   |
|  ooo    |    ooo  | +---+---+---+---+---+---+---+ |  ooo    |    ooo  |
|  oo  ~~ - ~~  oo  |             | L | L | V | M | |  oo  ~~ O ~~  oo  |
|   ooooooooooooo   |             +---+---+---+---+ |   ooooooooooooo   |
|                   |                               |                   |
+-------------------+-------------------------------+-------------------+
+-----+-----------------------------------------------------------------+
|  1  |  UN*X (GNU/Linux, BSD, macOS, ...)                              |
+-----+-----------------------------------------------------------------+


  Spustit skript [make_unix.sh].


  Prerekvizity:
    Nastroje pro kompilaci zdrojovych kodu v C
      - typicky GNU GCC nebo LLVM Clang,
      - v macOS Xcode Command Line Tools (CLT) pro Apple Clang.

  Poznamky:
    - ohlidat ukoncovani radku v make.sh (nastavit na LF)
    - predem instalovat vyvojove nastroje:
      - Ubuntu  24.04 LTS  -> [sudo apt install build-essentials -y]
      - Fedora  42         -> [sudo dnf install gcc]
      - FreeBSD 14         -> [pkg install -y bash && bash]
      - macOS   15         -> [xcode-select --install]
    - spustit [sh make.sh]

    V macOS je mozne vyuzit prepinace:
      - [-arch arm64] pro Silicon,
      - [-arch x86_64] pro Intel.
    A nasledne vytvorit jedinou Universal binarku:
    [lipo -create -output sibenice-macUni sibenice-macArm64 sibenice-mac64]



+-------------------+-------------------------------+-------------------+
|                   |                               |                   |
|     ooooooooo     | +---+---+---+---+             |     ooooooooo     |
|   oo   o o   oo   | |   |   | M | S |             |   oo   O o   oo   |
|  ooo    |    ooo  | +---+---+---+---+---+---+---+ |  ooo    |    ooo  |
|  oo  ~~ - ~~  oo  |             | V | C |   |   | |  oo  ~~ O ~~  oo  |
|   ooooooooooooo   |             +---+---+---+---+ |   ooooooooooooo   |
|                   |                               |                   |
+-------------------+-------------------------------+-------------------+
+-----+-----------------------------------------------------------------+
|  2  |  WINDOWS 11                                                     |
+-----+-----------------------------------------------------------------+


  Spustit skript [make_win.bat] z vyvojarske konzole
  (tj. Native Tools Command Prompt for VS 2022).


  Prerekvizity:
    Vyvojovy nastroj Microsoft Visual Studio, resp. MSVC.

  Poznamky:
    - predem instalovat vyvojove nastroje:
      - Visual Studio Community s podporou vyvoje pro C/C++
         (tj. minimalne kompilator MSVC)
      - skript je treba spustit z vyvojarske konzole
         v nabidce Start:
           x86 Native Tools Command Prompt for VS (x86)
           x64 Native Tools Command Prompt for VS (x86_64)
             příp. pro cross-kompilaci AArch64 zavolat:
             [.\VC\Auxiliary\Build\vcvarsall.bat amd64_arm64]
    - dle zkusenosti dela problemy vestaveny antivirus Zabezpeceni Windows
      (tj. po vlastnim zvazeni, prip. kontrole zdrojovych kodu, pridat
      vyjimku nebo docasne vypnout antivirovou ochranu v realnem case)



+-------------------+-------------------------------+-------------------+
|                   |                               |                   |
|     ooooooooo     | +---+---+---+---+             |     ooooooooo     |
|   oo   o o   oo   | | 1 | 6 | / | 3 |             |   oo   O o   oo   |
|  ooo    |    ooo  | +---+---+---+---+---+---+---+ |  ooo    |    ooo  |
|  oo  ~~ - ~~  oo  |             | 2 | B | I | T | |  oo  ~~ O ~~  oo  |
|   ooooooooooooo   |             +---+---+---+---+ |   ooooooooooooo   |
|                   |                               |                   |
+-------------------+-------------------------------+-------------------+
+-----+-----------------------------------------------------------------+
|  3  |  FREEDOS 1.4                                                    |
+-----+-----------------------------------------------------------------+


  POZOR:
    DOS (resp. FAT16) vyzaduje konvenci max. 8 znaku pro nazev souboru
    plus dalsi max. 3 znaky za teckou jako priponu (typ souboru) !!!


  a) vytvorit a pripojit ISO se zdrojovymi soubory k virtualnimu PC,
  b) zkopirovat soubory z ISO na HDD napr. prikazem
     [xcopy /S D: C:\sibenice],
  c) vstoupit do slozky [cd C:\sibenice],
  d) sestavit projekt (viz nize),
  e) - pri pouziti VM vypnout FreeDOS prikazem [shutdown],
     - nalezt soubor virtualniho disku a extrahovat vytvorenou binarku
       (napr. disky .VMDK z VMware Workstation Pro lze otevrit pres 7-Zip).


  varianta 1:
    OPEN WATCOM (pro 16-bit binarku):  spustit skript [mk_dos16.bat]
  varianta 2:
    DJGPP GCC   (pro 32-bit binarku):  spustit skript [mk_dos32.bat]


  Prerekvizity:
    Idealne virtualni PC s FreeDOS [www.freedos.org/download].
    Cely system je vhodne aktualizovat on-line pomoci [fdnpkg update].

    varianta 1 - OPEN WATCOM
    {
      - pripojit BonusCD do virtualni mechaniky (FD14BNS.iso)
      - spustit [fdimples]
      - nainstalovat [Development -> WATCOMC]
      - idealne znovu aktualizovat system [fdnpkg update]
      - idealne [reboot]
    }

    varianta 2 - DJGPP GCC
    {
      - nainstalovat prekladac z on-line repozitare
        (od FreeDOS 1.4 se DJGPP nenachazi na BonusCD)
          [fdnpkg install djgpp]
          [fdnpkg install dj_gcc]
          [fdnpkg install dj_bin]
      - idealne [reboot]
    }



+-------------------+-------------------------------+-------------------+
|                   |                               |                   |
|     ooooooooo     | +---+---+---+---+             |     ooooooooo     |
|   oo   o o   oo   | | E | M | C | C |             |   oo   O o   oo   |
|  ooo    |    ooo  | +---+---+---+---+---+---+---+ |  ooo    |    ooo  |
|  oo  ~~ - ~~  oo  |             | H | T | T | P | |  oo  ~~ O ~~  oo  |
|   ooooooooooooo   |             +---+---+---+---+ |   ooooooooooooo   |
|                   |                               |                   |
+-------------------+-------------------------------+-------------------+
+-----+-----------------------------------------------------------------+
|  4  |  WEB (EMSCRIPTEN - WASM)                                        |
+-----+-----------------------------------------------------------------+


  Spustit skript [make_wasm.sh].


  Prerekvizity:
    - unixove prostredi (resp. Bash shell),
    - nainstalovane a nastavene EMSDK [source emsdk/emsdk_env.sh]
      pro sestavovani Wasm pres Emscripten (obsahuje i potrebne npm)
      [https://emscripten.org/docs/getting_started/downloads.html].


```