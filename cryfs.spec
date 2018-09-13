#
# Spec file for package cryfs
#
%define _prefix  /usr/local
Name:           cryfs
Version:        0.9.9
Release:        1%{?dist}
Summary:        cryfs encryption
License:        GPL-2.0+
Group:          Security
Source:         %{name}-%{version}-src.tar.gz
# Source:         https://github.com/cryfs/cryfs/releases/download/%{version}/cryfs-%{version}.tar.gz
URL:            https://github.com/cryfs/cryfs
#=================================
BuildRequires: cmake
BuildRequires: boost-devel
BuildRequires: boost-static
BuildRequires: cryptopp-devel
BuildRequires: curl-devel
BuildRequires: fuse-devel
BuildRequires: openssl-devel
#=================================

%description
CryFS encrypts your files, so you can safely store them anywhere.
It works well together with cloud services like Dropbox,
iCloud, OneDrive and others.

%global debug_package %{nil}

%prep
%setup -q

%build
mkdir cmake && cd cmake
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=off
%{__make}
cd ..

%install
rm -rf %{buildroot}
%make_install -C cmake

%clean
rm -rf %{buildroot}

%files
%defattr(0755,root,root)
%{_bindir}/cryfs*
%{_mandir}/man1/cryfs.1.gz

%changelog
* Thu Feb 15 2018 mnovotny 0.9.9-0.1
- update to 0.9.9

* Fri Sep 08 2017 mnovotny 0.9.7-0.1
- update to 0.9.7

* Sun Jul 03 2016 bb <bb> 0.9.5-0.5442662pclos2016
- create

