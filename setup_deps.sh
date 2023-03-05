function download_chibi {
    mkdir -p deps
    cd deps
    git clone https://github.com/ashinn/chibi-scheme.git
    cd chibi-scheme
    git checkout da53f46c93477a920e2e6b55c96c793cbd955fb8
    cd ../../
}

function patch_chibi {
    echo "patch_chibi"
    cd deps/chibi-scheme/
    git apply ../../patches/chibi.patch
    cd ..
    mv chibi-scheme chibi-scheme-dynamic
    cp -r chibi-scheme-dynamic chibi-scheme-static
    cd ..
}

function build_dynamic_chibi {
    cd deps/chibi-scheme-dynamic/
    make -j8
    cd ../../
}
    

download_chibi
patch_chibi
build_dynamic_chibi

mkdir -p build/


