Name:           sibenice
Version:        1.4.0
Release:        1%{?dist}
Summary:        Hra Sibenice

License:        GPL
URL:            https://github.com/ma-ta/hra-sibenice
Source0:        %{name}-%{version}.tar.gz

%global debug_package %{nil}

%description
Hra Sibenice pro terminal.

%prep
%autosetup

%install
install -Dm755 sibenice %{buildroot}/usr/bin/sibenice
install -dm755 %{buildroot}/usr/share/sibenice/data
cp -r data/* %{buildroot}/usr/share/sibenice/data/

%files
#%license add-license-file-here
#%doc add-docs-here
/usr/bin/sibenice
/usr/share/sibenice/data/*

%changelog
* Sun Apr 19 2026 MaTab <13202912+ma-ta@users.noreply.github.com>
- Stabilní verze
