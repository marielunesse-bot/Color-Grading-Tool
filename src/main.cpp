#include "Main.h"
#include "Core/Application.h"
#include "Core/Log.h"
#include "Apps/ColorGradingApp.h"
#include  <string>

namespace ColorGradingTool 
{
    Application* CreateApplication(Args args)
    {
      // Creation des proprietes de l'application
      ApplicationProperties appProps;
      appProps.Name = "Color Grading Tool";
        appProps.Width = 1280;
        appProps.Height = 720;
        appProps.fullscreen = false;

        //Retourne une nouvelle instance de l'application
        return new ColorGradingApp(appProps);
    }
} // namespace ColorGradingTool
/* 
// Affiche l'utilisation du programme
static void afficherUsage(const char* prog) {
    std::cerr << "Utilisation : " << prog << " image_entree.jpg image_sortie.jpg [--exposure EV] [--contrast C] [--saturation S]\n"
              << "            [--temperature T] [--gamma G]\n"
              << "Valeurs par défaut : EV=0.0  C=1.0  S=1.0  T=0.0  G=1.0\n";
}

// Analyse les arguments passés en ligne de commande
static std::unordered_map<std::string, std::string> analyserArguments(int argc, char** argv) {
    std::unordered_map<std::string, std::string> m;
    for (int i = 1; i < argc; ++i) {
        std::string s = argv[i];
        if (s.rfind("--", 0) == 0 && i + 1 < argc) {
            m[s] = argv[++i];
        } else if (!m.count("input")) {
            m["input"] = s;
        } else if (!m.count("output")) {
            m["output"] = s;
        }
    }
    return m;
}

int main(int argc, char** argv) {
    if (argc < 3) { afficherUsage(argv[0]); return 1; }

    auto args = analyserArguments(argc, argv);
    if (!args.count("input") || !args.count("output")) { afficherUsage(argv[0]); return 1; }

    std::string cheminEntree = args["input"], cheminSortie = args["output"];
    float exposure = args.count("--exposure") ? std::stof(args["--exposure"]) : 0.f; // EV stops
    float contraste = args.count("--contrast") ? std::stof(args["--contrast"]) : 1.f;
    float saturation = args.count("--saturation") ? std::stof(args["--saturation"]) : 1.f;
    float temperature = args.count("--temperature") ? std::stof(args["--temperature"]) : 0.f; // -100..100
    float gamma = args.count("--gamma") ? std::stof(args["--gamma"]) : 1.f;

    // Lecture de l'image
    cv::Mat img = cv::imread(cheminEntree, cv::IMREAD_COLOR);
    if (img.empty()) { std::cerr << "Impossible de charger : " << cheminEntree << "\n"; return 2; }

    // Conversion en float (0..1)
    cv::Mat imgFloat;
    img.convertTo(imgFloat, CV_32F, 1.0f/255.0f); // BGR

    //  Exposure : multiplie la luminosité par 2^EV
    float facteurExposure = std::pow(2.0f, exposure);
    imgFloat *= facteurExposure;

    //  Contraste autour du milieu (0.5)
    imgFloat = (imgFloat - 0.5f) * contraste + 0.5f;

    // Température : ajuste le rouge et le bleu (chaud/froid)
    float t = std::clamp(temperature / 100.0f, -1.0f, 1.0f); // normalisation
    float facteurRouge = 1.0f + 0.5f * t;
    float facteurBleu  = 1.0f - 0.5f * t;
    std::vector<cv::Mat> canaux(3);
    cv::split(imgFloat, canaux);
    canaux[2] *= facteurRouge; // R
    canaux[0] *= facteurBleu;  // B
    cv::merge(canaux, imgFloat);

    // Saturation : passe en HSV, ajuste S, revient en BGR
    cv::Mat hsv;
    cv::cvtColor(imgFloat, hsv, cv::COLOR_BGR2HSV);
    std::vector<cv::Mat> hsvCh(3);
    cv::split(hsv, hsvCh);
    hsvCh[1] *= saturation; // canal Saturation
    cv::merge(hsvCh, hsv);
    cv::cvtColor(hsv, imgFloat, cv::COLOR_HSV2BGR);

    //  Gamma
    if (gamma > 0.0f && std::abs(gamma - 1.0f) > 1e-6f) {
        cv::pow(cv::max(imgFloat, 0.0f), 1.0f / gamma, imgFloat);
    }

    //  Clamp final : s'assure que toutes les valeurs sont entre 0 et 1
    cv::threshold(imgFloat, imgFloat, 0.0, 0.0, cv::THRESH_TOZERO); // min 0
    cv::min(imgFloat, 1.0f, imgFloat); // max 1

    // Conversion en 8-bit et écriture
    cv::Mat sortie;
    imgFloat.convertTo(sortie, CV_8U, 255.0);
    if (!cv::imwrite(cheminSortie, sortie)) {
        std::cerr << "Impossible d'écrire : " << cheminSortie << "\n";
        return 3;
    }

    return 0;
} */
