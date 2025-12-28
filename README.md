# Color Grading Tool
## Fonctionalités implementées
Ce projet vise a concevoir un outil de color grading pour le traitement d'images numeririques. Pour se faire il sera question de developper un moteur de traitement d'images capable de:
- Charger une image depuis le disque
- Charger une Look-Up Table(.cube)
- Appliquer la LUT a l'image 
- Ajuster des courbes de couleurs RGB(Rouge,Vert,Bleu)
- Sauvegarder/exporter les images traitées
## Technologies utilisées
- **C++** langage principale du projet pour le traitement d'images en respectant les conventions de nommage
- **OpenCV** bibliotheque C++ pour permettre de charger et sauvegarder des images;manipuler les pixels,matrices de couleurs; appliquer les LUTs et ajuster les courbes RGB
- **CMake/g++** outil de compilation et de construction du projet CMake organise les fichiers et dependances; g++ compile le code C++ en executable 
- **Github** outil de gestion de version pour le suivie de l'evolution du projet
- **Betty Doc/Betty Style** norme de documentation et de style pour documenter chaque fonction,classe et methode afin d'assurer un code lisible 
## Orginisation du projet
ColorGradingTool/
│
├── sample_images/
│   └── test_image.jpg
│
├── luts/
│   └── cinematic.cube
│
├── src/
│   ├── ImageLoader.cpp
│   ├── ImageLoader.hpp
│   ├── LutProcessor.cpp
│   ├── LutProcessor.hpp
│   ├── ColorCurve.cpp
│   ├── ColorCurve.hpp
│   └── main.cpp
│
├── include/
│   └── Common.hpp
│
├── README.md
└── .gitignore
## Compilation et Execution
g++ src/*.cpp -o ColorGradingTool `pkg-config --cflags --libs opencv4`
./ColorGradingTool
