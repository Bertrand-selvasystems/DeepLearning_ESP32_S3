# Déploiement d’un réseau de neurones pour la classification sur un ESP32-S3 avec TensorFlow Lite

Ce projet implémente un modèle de deep learning sur un microcontrôleur ESP32-S3 en utilisant TensorFlow Lite Micro. 
Le modèle est entraîné sur PC avec Python, puis converti et déployé sur l'ESP32.

## 📌 Présentation

Ce projet utilise le jeu de données **Sonar** de l'UCI Machine Learning Repository.
L'objectif est de classer des signaux sonores en deux catégories : **roche** ou **mine**.
Le modèle est entraîné sous Python, puis converti en format TensorFlow Lite pour être utilisé sur l'ESP32-S3.

**Vidéo de démonstration** 📹 : [Voir sur YouTube](https://www.youtube.com/watch?v=eMNCVg_wVUQ)

---

## 📁 Structure du projet

```
deep_learning_mine/
│── main/                      # Code source pour l'ESP32
│   ├── main.cc                # Point d'entrée principal
│   ├── model.cc               # Gestion du modèle TensorFlow Lite
│   ├── model.h                # En-tête pour le modèle
│   ├── donnees.c              # Données d'entrée de test
│   ├── donnees.h              # Définition des données d'entrée
│   ├── neurone.cc             # Inférence du réseau de neurones
│   ├── neurone.h              # Déclaration des fonctions d'inférence
│   ├── tasks.cc               # Gestion des tâches FreeRTOS
│   ├── tasks.h                # Déclaration des tâches
│   ├── led.cc                 # Gestion des LEDs
│   ├── led.h                  # Définition des LEDs
│   ├── constantes.h           # Déclarations des constantes
│   ├── CMakeLists.txt         # Configuration du projet ESP-IDF
│── model/                     # Entraînement et conversion du modèle
│   ├── train.py               # Script d'entraînement du modèle
│   ├── convert.py             # Script de conversion en TensorFlow Lite
│   ├── mon_modele.h5          # Modèle entraîné en format Keras
│   ├── model.tflite           # Modèle converti en TensorFlow Lite
│── managed_components/        # Dépendances ESP-IDF
│── build/                     # Fichiers générés après compilation
│── sdkconfig                  # Configuration ESP-IDF
│── README.md                  # Ce fichier
```

---

## 🚀 Entraînement du modèle

L'entraînement est effectué sous Python avec TensorFlow et scikit-learn.

Vous trouverez le fichier ipynb dans le repertoire model : 
entrainement_extraction_modele.ipynb

---

## 🔄 Conversion vers TensorFlow Lite

Le modèle est converti en **TensorFlow Lite** à la suite de l'entrainement dans entrainement_extraction_modele.ipynb

Cela génère le fichier `model.tflite` utilisé sur l'ESP32.

---

## ⚡ Déploiement sur l'ESP32-S3

Le firmware est construit avec **ESP-IDF**. 

### Compilation et flash du firmware

```sh
idf.py set-target esp32s3
idf.py build
idf.py flash monitor
```

Cela charge le modèle TensorFlow Lite et exécute l'inférence sur l'ESP32.

---

## 📜 Licence

Ce projet est sous licence MIT. Vous êtes libre de le réutiliser et de l'améliorer.

---

**📹 Démonstration vidéo** : [YouTube](https://www.youtube.com/watch?v=eMNCVg_wVUQ)
