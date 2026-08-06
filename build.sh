#!/bin/bash
# ============================================================
# MW2 Revolution Mod - Build Script
# Compiles the DLL using MinGW on Linux/WSL/macOS
# ============================================================

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color
BOLD='\033[1m'

# Configuration
MOD_NAME="SleepTheGod's MW2 Revolution"
MOD_VERSION="3.0"
MOD_AUTHOR="SleepTheGod"
DLL_NAME="Revolution.dll"
SOURCE_FILE="../src/Revolution.cpp"
OUTPUT_DIR="../release"

# Function to print banner
print_banner() {
    echo -e "${PURPLE}"
    echo "╔══════════════════════════════════════════════════════════════╗"
    echo "║                                                              ║"
    echo "║   ${BOLD}${CYAN}███████╗██╗     ███████╗███████╗██████╗ ${PURPLE}║"
    echo "║   ${BOLD}${CYAN}██╔════╝██║     ██╔════╝██╔════╝██╔══██╗${PURPLE}║"
    echo "║   ${BOLD}${CYAN}███████╗██║     █████╗  █████╗  ██████╔╝${PURPLE}║"
    echo "║   ${BOLD}${CYAN}╚════██║██║     ██╔══╝  ██╔══╝  ██╔═══╝ ${PURPLE}║"
    echo "║   ${BOLD}${CYAN}███████║███████╗███████╗███████╗██║     ${PURPLE}║"
    echo "║   ${BOLD}${CYAN}╚══════╝╚══════╝╚══════╝╚══════╝╚═╝     ${PURPLE}║"
    echo "║                                                              ║"
    echo "║   ${BOLD}${GREEN}$MOD_NAME v$MOD_VERSION${PURPLE}                      ║"
    echo "║   ${BOLD}${YELLOW}By $MOD_AUTHOR${PURPLE}                                    ║"
    echo "║                                                              ║"
    echo "╚══════════════════════════════════════════════════════════════╝"
    echo -e "${NC}"
}

# Function to check dependencies
check_dependencies() {
    echo -e "${YELLOW}[*] Checking dependencies...${NC}"
    
    local missing=()
    
    # Check for MinGW
    if ! command -v x86_64-w64-mingw32-g++ &> /dev/null; then
        missing+=("x86_64-w64-mingw32-g++")
    fi
    
    # Check for upx (optional)
    if ! command -v upx &> /dev/null; then
        echo -e "${YELLOW}[!] UPX not found (optional, skipping compression)${NC}"
    fi
    
    if [ ${#missing[@]} -ne 0 ]; then
        echo -e "${RED}[!] Missing dependencies:${NC}"
        for dep in "${missing[@]}"; do
            echo -e "    - $dep"
        done
        echo ""
        echo -e "${YELLOW}Install with:${NC}"
        echo "    sudo apt update"
        echo "    sudo apt install g++-mingw-w64-x86-64"
        echo "    sudo apt install upx-ucl  # optional"
        exit 1
    fi
    
    echo -e "${GREEN}[✓] All dependencies found${NC}"
}

# Function to compile the DLL
compile_dll() {
    echo -e "${YELLOW}[*] Compiling $DLL_NAME...${NC}"
    
    mkdir -p "$OUTPUT_DIR"
    
    CXXFLAGS="-O2 -Wall -Wextra -Wno-unused-parameter -Wno-ignored-attributes"
    CXXFLAGS="$CXXFLAGS -D_WIN32_WINNT=0x0600"
    CXXFLAGS="$CXXFLAGS -D_CRT_SECURE_NO_WARNINGS"
    CXXFLAGS="$CXXFLAGS -D_USE_MATH_DEFINES"
    
    LDFLAGS="-static-libgcc -static-libstdc++"
    LDFLAGS="$LDFLAGS -lkernel32 -luser32 -lgdi32 -lws2_32"
    LDFLAGS="$LDFLAGS -ld3d9 -ld3dx9 -lole32 -lshell32"
    LDFLAGS="$LDFLAGS -Wl,--dynamicbase -Wl,--nxcompat"
    LDFLAGS="$LDFLAGS -Wl,--enable-stdcall-fixup"
    
    echo -e "${CYAN}    Compiling...${NC}"
    x86_64-w64-mingw32-g++ -shared \
        $CXXFLAGS \
        -o "$OUTPUT_DIR/$DLL_NAME" \
        "$SOURCE_FILE" \
        $LDFLAGS
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}[✓] Compilation successful!${NC}"
    else
        echo -e "${RED}[✗] Compilation failed!${NC}"
        exit 1
    fi
}

# Function to strip debug symbols
strip_dll() {
    echo -e "${YELLOW}[*] Stripping debug symbols...${NC}"
    
    if command -v x86_64-w64-mingw32-strip &> /dev/null; then
        x86_64-w64-mingw32-strip "$OUTPUT_DIR/$DLL_NAME"
        echo -e "${GREEN}[✓] Debug symbols stripped${NC}"
    else
        echo -e "${YELLOW}[!] strip tool not found, skipping${NC}"
    fi
}

# Function to compress with UPX (optional)
compress_dll() {
    if command -v upx &> /dev/null; then
        echo -e "${YELLOW}[*] Compressing with UPX...${NC}"
        upx --best --ultra-brute "$OUTPUT_DIR/$DLL_NAME" 2>/dev/null || \
        upx --best "$OUTPUT_DIR/$DLL_NAME" 2>/dev/null || \
        echo -e "${YELLOW}[!] UPX compression failed, using uncompressed DLL${NC}"
        echo -e "${GREEN}[✓] DLL compressed${NC}"
    else
        echo -e "${YELLOW}[!] UPX not found, skipping compression${NC}"
    fi
}

# Function to create checksums
create_checksums() {
    echo -e "${YELLOW}[*] Creating checksums...${NC}"
    
    cd "$OUTPUT_DIR"
    
    if command -v md5sum &> /dev/null; then
        md5sum "$DLL_NAME" > "$DLL_NAME.md5"
        echo -e "${GREEN}[✓] MD5 checksum created${NC}"
    fi
    
    if command -v sha256sum &> /dev/null; then
        sha256sum "$DLL_NAME" > "$DLL_NAME.sha256"
        echo -e "${GREEN}[✓] SHA256 checksum created${NC}"
    fi
    
    cd - > /dev/null
}

# Function to show file info
show_file_info() {
    echo ""
    echo -e "${BLUE}════════════════════════════════════════════════════════════════${NC}"
    echo -e "${BOLD}${GREEN}✨ Build Complete!${NC}"
    echo -e "${BLUE}════════════════════════════════════════════════════════════════${NC}"
    
    if [ -f "$OUTPUT_DIR/$DLL_NAME" ]; then
        FILE_SIZE=$(du -h "$OUTPUT_DIR/$DLL_NAME" | cut -f1)
        FILE_DATE=$(stat -c '%y' "$OUTPUT_DIR/$DLL_NAME" 2>/dev/null || stat -f '%Sm' "$OUTPUT_DIR/$DLL_NAME" 2>/dev/null)
        
        echo -e "${CYAN}📁 Output:${NC} $OUTPUT_DIR/$DLL_NAME"
        echo -e "${CYAN}📊 Size:${NC} $FILE_SIZE"
        echo -e "${CYAN}📅 Built:${NC} $FILE_DATE"
        
        if [ -f "$OUTPUT_DIR/$DLL_NAME.md5" ]; then
            echo -e "${CYAN}🔑 MD5:${NC} $(cat "$OUTPUT_DIR/$DLL_NAME.md5" | cut -d' ' -f1)"
        fi
        
        if [ -f "$OUTPUT_DIR/$DLL_NAME.sha256" ]; then
            echo -e "${CYAN}🔑 SHA256:${NC} $(cat "$OUTPUT_DIR/$DLL_NAME.sha256" | cut -d' ' -f1)"
        fi
    fi
    
    echo ""
    echo -e "${BOLD}${GREEN}🚀 Ready to use!${NC}"
    echo ""
    echo -e "${YELLOW}To inject:${NC}"
    echo "  1. Copy $DLL_NAME to your MW2 folder"
    echo "  2. Use a DLL injector (Extreme Injector, Xenos, etc.)"
    echo "  3. Target iw4x.exe or iw4sp.exe"
    echo "  4. Inject and press F8 in-game"
    echo ""
    echo -e "${BLUE}════════════════════════════════════════════════════════════════${NC}"
}

# Main execution
main() {
    clear
    print_banner
    
    echo -e "${CYAN}╔══════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${CYAN}║${NC} ${BOLD}Building $MOD_NAME v$MOD_VERSION${NC}${CYAN}                          ║${NC}"
    echo -e "${CYAN}╚══════════════════════════════════════════════════════════════╝${NC}"
    echo ""
    
    if [ ! -f "$SOURCE_FILE" ]; then
        echo -e "${RED}[✗] Source file not found: $SOURCE_FILE${NC}"
        echo -e "${YELLOW}Make sure you're running this from the build directory${NC}"
        exit 1
    fi
    
    check_dependencies
    echo ""
    
    compile_dll
    echo ""
    
    strip_dll
    echo ""
    
    compress_dll
    echo ""
    
    create_checksums
    echo ""
    
    show_file_info
    
    echo -e "${PURPLE}💜 Thank you for using SleepTheGod's MW2 Revolution!${NC}"
    echo -e "${PURPLE}⭐ Don't forget to star the repo if you enjoy it!${NC}"
    echo ""
}

# Run main
main
