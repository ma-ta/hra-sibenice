## HOW-TO

256 px | 128 px | 64 px
---|---|---
![256px ikonka pro macOS](mac-ico.iconset/icon_256x256.png) | ![128px ikonka pro macOS](mac-ico.iconset/icon_128x128.png) | ![64px ikonka pro macOS](mac-ico.iconset/icon_32x32@2x.png)

1. stáhnout šablonu ze stránky
[https://developer.apple.com/design/resources/#macos-apps]
2. otevře např. Figma nebo GIMP (nutné převést na sRGB profil):
    - vytvořit vizuálně vyhovující sadu ikon
    - jednotlivé ikony vyexportovat jako PNG do složky [mac-ico.iconset]
      se správnými názvy pro macOS utilitu [iconutil]
3. PNG ideálně převést na barevný profil P3 pomocí ColorSync Utility v macOS
4. použít CLI utilitu [iconutil -c icns mac-ico.iconset] pro finální ikonu

### Použitý software
- Sketch Production Templates *(ver. January 3, 2025&nbsp;&ndash;&nbsp;macOS Sequoia)*
- Figma Desktop App *(ver. 125.3.5)*
- GitHub Action [workflow](//github.com/ma-ta/hra-sibenice/actions/workflows/build-macicon.yml)&nbsp;&ndash;&nbsp;vytvoření [mac-ico.icns](mac-ico.icns) ze zdrojových PNG v [mac-ico.iconset](mac-ico.iconset) *(runner: macOS 14.7.4 | Xcode CLT 16.2)*
