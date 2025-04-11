![64px ikonka pro macOS](mac-ico.iconset/icon_32x32@2x.png)

## HOW-TO

1. stáhnout šablonu ze stránky
[https://developer.apple.com/design/resources/#macos-apps]
2. otevře např. Figma nebo GIMP (nutné převést na sRGB profil):
    - vytvořit vizuálně vyhovující sadu ikon
    - jednotlivé ikony vyexportovat jako PNG do složky [mac-ico.iconset]
      se správnými názvy pro macOS utilitu [iconutil]
3. PNG ideálně převést na barevný profil P3 pomocí ColorSync Utility v macOS
4. použít CLI utilitu [iconutil -c icns mac-ico.iconset] pro finální ikonu

### Použitý software
- Sketch Production Templates pro macOS Sequoia (ver. January 3, 2025)
- Figma Desktop App (ver. 125.3.5)
