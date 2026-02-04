#!/usr/bin/env python3
"""
Color Grading Tool - Build System
Compile avec clang++ sur Windows/macOS/Linux
"""

import subprocess
import sys
import os
from pathlib import Path
import shutil

class Builder:
    def __init__(self):
        self.platform = sys.platform
        self.compiler = "clang++"
        self.std = "-std=c++17"
        self.optimization = "-O2"
        self.debug = "-g"
        
        # Dossiers
        self.root = Path(__file__).parent
        self.src_dir = self.root / "src"
        self.thirdparty = self.root / "thirdparty"
        self.build_dir = self.root / "build"
        
        # Sortie
        self.output = "ColorGradingTool.exe" if self.platform == "win32" else "ColorGradingTool"
        
    def clean(self):
        """Supprime les fichiers de build"""
        print("Nettoyage...")
        if self.build_dir.exists():
            shutil.rmtree(self.build_dir)
        if Path(self.output).exists():
            os.remove(self.output)
        print("Nettoyage terminé")
        
    def find_sources(self):
        """Trouve tous les fichiers .cpp"""
        sources = []
        
        # Sources du projet
        sources.extend(self.src_dir.glob("*.cpp"))
        sources.extend(self.src_dir.glob("Core/*.cpp"))
        sources.extend(self.src_dir.glob("Utils/*.cpp"))
        
        # ImGui sources
        imgui_dir = self.thirdparty / "imgui"
        sources.append(imgui_dir / "imgui.cpp")
        sources.append(imgui_dir / "imgui_draw.cpp")
        sources.append(imgui_dir / "imgui_tables.cpp")
        sources.append(imgui_dir / "imgui_widgets.cpp")
        sources.append(imgui_dir / "imgui_demo.cpp")
        sources.append(imgui_dir / "backends/imgui_impl_sdl3.cpp")
        sources.append(imgui_dir / "backends/imgui_impl_sdlrenderer3.cpp")
        
        # tinyfiledialogs
        sources.append(self.thirdparty / "tinyfiledialogs.cpp")
        
        return [str(s) for s in sources]
        
    def get_includes(self):
        """Retourne les chemins d'inclusion"""
        includes = [
            f"-I{self.src_dir}",
            f"-I{self.thirdparty}",
            f"-I{self.thirdparty / 'imgui'}",
            f"-I{self.thirdparty / 'imgui/backends'}",
            f"-I{self.thirdparty / 'stb'}",
            f"-I{self.thirdparty / 'SDL3/include'}",
        ]
        return includes
        
    def get_libraries(self):
        """Retourne les bibliothèques à lier"""
        libs = []
        
        if self.platform == "win32":
            # Windows avec SDL3
            sdl_lib = self.thirdparty / "SDL3/lib/x64"
            libs.extend([
                f"-L{sdl_lib}",
                "-lSDL3",
                "-lshell32",
                "-luser32",
                "-lgdi32",
                "-limm32",
                "-lole32",
                "-loleaut32",
                "-lversion",
                "-luuid",
                "-ladvapi32",
                "-lsetupapi",
                "-lwinmm",
                "-lcomdlg32"
            ])
        elif self.platform == "darwin":
            # macOS
            libs.extend([
                "-framework", "Cocoa",
                "-framework", "IOKit",
                "-framework", "CoreVideo",
                "-lSDL3"
            ])
        else:
            # Linux
            libs.extend([
                "-lSDL3",
                "-ldl",
                "-lpthread"
            ])
            
        return libs
        
    def build(self):
        """Compile le projet"""
        print("Compilation du projet...")
        
        # Créer le dossier build
        self.build_dir.mkdir(exist_ok=True)
        
        # Trouver les sources
        sources = self.find_sources()
        print(f"{len(sources)} fichiers source trouvés")
        
        # Construire la commande
        cmd = [
            self.compiler,
            self.std,
            self.optimization,
            self.debug,
            *self.get_includes(),
            *sources,
            *self.get_libraries(),
            "-o", self.output
        ]
        
        # Afficher la commande (utile pour debug)
        print(f"\n Commande: {' '.join(cmd[:5])}... (+ {len(cmd)-5} arguments)\n")
        
        # Exécuter
        try:
            result = subprocess.run(cmd, check=True, capture_output=True, text=True)
            print("Compilation réussie!")
            return True
        except subprocess.CalledProcessError as e:
            print("Erreur de compilation:")
            print(e.stderr)
            return False
            
    def run(self):
        """Lance l'application"""
        print(f"\n Lancement de {self.output}...\n")
        
        # Sur Windows, copier SDL3.dll à côté de l'exe
        if self.platform == "win32":
            sdl_dll = self.thirdparty / "SDL3/lib/x64/SDL3.dll"
            if sdl_dll.exists():
                shutil.copy(sdl_dll, ".")
        
        try:
            subprocess.run([f"./{self.output}"], check=True)
        except FileNotFoundError:
            print(f"Fichier {self.output} introuvable. Compilez d'abord avec: python build.py build")
        except subprocess.CalledProcessError as e:
            print(f"L'application s'est terminée avec une erreur (code {e.returncode})")
            

def main():
    builder = Builder()
    
    if len(sys.argv) < 2:
        print("Usage:")
        print("  python build.py build   - Compile le projet")
        print("  python build.py run     - Lance l'application")
        print("  python build.py clean   - Nettoie les fichiers de build")
        print("  python build.py rebuild - Clean + Build")
        sys.exit(1)
        
    command = sys.argv[1]
    
    if command == "clean":
        builder.clean()
    elif command == "build":
        builder.build()
    elif command == "run":
        builder.run()
    elif command == "rebuild":
        builder.clean()
        builder.build()
    else:
        print(f"Commande inconnue: {command}")
        sys.exit(1)

if __name__ == "__main__":
    main()