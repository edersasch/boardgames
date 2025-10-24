#!/bin/sh -xe

[ "$AQT_VERSION" ] || exit 1
[ "$QT_VERSION" ] || exit 2
[ "$QT_PATH" ] || exit 3

root_dir=$PWD
[ "$root_dir" != '/' ] || root_dir=""

curl -fso/tmp/aqt.exe -L "https://github.com/miurahr/aqtinstall/releases/download/v$AQT_VERSION/aqt.exe"

wine /tmp/aqt.exe install-qt -O "$QT_PATH" windows desktop "$QT_VERSION" win64_mingw
wine /tmp/aqt.exe install-tool -O "$QT_PATH" windows desktop tools_mingw1310
wine /tmp/aqt.exe install-tool -O "$QT_PATH" windows desktop tools_cmake
wine /tmp/aqt.exe install-tool -O "$QT_PATH" windows desktop tools_ninja

WDRIVEC="$HOME/.wine/drive_c"

# CPM expects git and uses patch.
# Only the portable version contains patch.exe and does not need graphical output to install.
curl -fso/tmp/git-binaries.exe -L "https://github.com/git-for-windows/git/releases/download/v2.51.1.windows.1/PortableGit-2.51.1-64-bit.7z.exe"
mkdir "$WDRIVEC/git"
7z x -o"$WDRIVEC/git" /tmp/git-binaries.exe
# Disable submodule support, because it hangs in wine and is called by CPM
echo '#!/bin/sh\ntrue' > "$WDRIVEC/git/mingw64/libexec/git-core/git-submodule"

WINEPATH="$(find "$WDRIVEC/Qt" "$WDRIVEC/git" -type d -name bin -o -name Ninja | sed "s|$WDRIVEC|C:|" | sed 's|/|\\\\|g' | tr '\n' ';')"

cat - <<EOF | sudo tee /usr/local/bin/qt-cmake
#!/bin/sh
WINEPATH="$WINEPATH" wine cmd /c qt-cmake "\$@"
EOF

for tool in cmake ninja windeployqt; do
cat - <<EOF | sudo -E tee /usr/local/bin/$tool
#!/bin/sh
WINEPATH="$WINEPATH" wine $tool "\$@"
EOF
done

sudo -E chmod +x /usr/local/bin/*
