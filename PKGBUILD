# Maintainer: Hakan Göksu <hakan@goksu.me>
pkgname=quicktools-git
pkgver=r3.159c10d
pkgrel=1
pkgdesc="QuickTools Script Manager"
arch=('x86_64')
url="https://github.com/hakangoksu/quicktools"
license=('MIT')
depends=('gtk4' 'libadwaita')
makedepends=('git' 'gcc' 'pkg-config')
provides=('quicktools')
conflicts=('quicktools')
source=("git+https://github.com/hakangoksu/quicktools.git")
sha256sums=('SKIP')
options=('!debug')

pkgver() {
    cd "quicktools"
    printf "r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}

build() {
    cd "quicktools"
        gcc -o quicktools main.c \
            $CFLAGS \
            $LDFLAGS \
            $(pkg-config --cflags --libs gtk4 libadwaita-1)
}

package() {
    cd "quicktools"

    install -Dm755 quicktools "$pkgdir/usr/bin/quicktools"
    install -Dm644 quicktools.desktop "$pkgdir/usr/share/applications/quicktools.desktop"

    mkdir -p "$pkgdir/usr/share/quicktools/scripts"
    cp -r scripts/* "$pkgdir/usr/share/quicktools/scripts/"
    chmod -R +x "$pkgdir/usr/share/quicktools/scripts/"
}
