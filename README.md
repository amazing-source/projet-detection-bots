# Projet de détection de bots Twitter en C

Projet universitaire de détection de bots au sein d'espaces commentaires Twitter, développé en langage C.

## Description

Ce projet implémente un système de détection de bots utilisant :
- Une analyse heuristique multi-critères
- Un test de connexion à l'API Twitter v2
- Un système de scoring pour identifier les commentaires suspects

## Fichiers du projet

- `simple_bot_detector.c` : Programme principal de détection de bots avec analyse de patterns
- `twitter_bot_detector.c` : Test de connexion et d'authentification avec l'API Twitter v2

## Compilation

### Prérequis
- Windows 11 avec MSYS2 MinGW-w64
- Bibliothèque libcurl installée

### Instructions de compilation

```bash
# Pour le détecteur de bots
gcc -o bot_detector.exe simple_bot_detector.c

# Pour le test API Twitter (nécessite libcurl)
gcc -o twitter_api_test.exe twitter_bot_detector.c -lcurl
