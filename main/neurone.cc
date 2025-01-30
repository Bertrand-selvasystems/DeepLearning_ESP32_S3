/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// DECLARATION DES BIBLIOTHEQUES
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
#include "neurone.h"

#include "model.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// DECLARATION DES VARIABLES STATIQUES
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
static const tflite::Model *model = nullptr;
static tflite::MicroInterpreter *interpreter = nullptr;
static TfLiteTensor *input = nullptr;
static TfLiteTensor *output = nullptr;
static int inference_count = 0;
static constexpr int kTensorArenaSize = 40000;  // Ajuster si nécessaire en fonction de la taille de votre modèle
static uint8_t tensor_arena[kTensorArenaSize];
static const int kInferencesPerCycle = 20;  // Définition de la variable

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// DEFINITION DES FONCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

esp_err_t neurone_init() {
  // Charger le modèle dans la mémoire
  model = tflite::GetModel(g_model);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    MicroPrintf("Model provided is schema version %d not equal to supported version %d.", model->version(), TFLITE_SCHEMA_VERSION);
    return ESP_FAIL;
  }

  // Résoudre les opérations nécessaires (convolution, fully connected, etc.)
  static tflite::MicroMutableOpResolver<8> resolver;
  resolver.AddFullyConnected();  // Couches denses
  resolver.AddRelu();            // Activation ReLU
  resolver.AddLogistic();        // Fonction équivalente à Sigmoid
  resolver.AddQuantize();        // Si le modèle est quantifié (int8)
  resolver.AddDequantize();      // Pour conversion int8 <-> float32
  resolver.AddReshape();         // Si des reshape sont utilisés
  resolver.AddAdd();             // Si des additions sont présentes
  resolver.AddMul();             // Si des multiplications sont utilisées
  // Initialiser l'interpréteur
  static tflite::MicroInterpreter static_interpreter(model, resolver, tensor_arena, kTensorArenaSize);
  interpreter = &static_interpreter;

  // Allouer la mémoire pour les tenseurs
  if (interpreter->AllocateTensors() != kTfLiteOk) {
    MicroPrintf("AllocateTensors() failed");
    return ESP_FAIL;
  }

  // Obtenir les pointeurs vers les tenseurs d'entrée et de sortie
  input = interpreter->input(0);    // Entrée du modèle
  output = interpreter->output(0);  // Sortie du modèle

  // Vérification des pointeurs
  if (input == nullptr || output == nullptr) {
    MicroPrintf("Failed to get input or output tensor.");
    return ESP_FAIL;
  }

  inference_count = 0;
  return ESP_OK;
}

esp_err_t neurone_inference(const float *input_values, float *output_values) {
  // Remplir les données d'entrée dans le tenseur du modèle
  // Remplir les données d'entrée dans le tenseur du modèle
  for (int i = 0; i < 60; ++i) {
    input->data.f[i] = input_values[i];
  }

    // Exécuter l'inférence
  if (interpreter->Invoke() != kTfLiteOk) {
    MicroPrintf("Invoke failed");
    return ESP_FAIL;
  }

  // Récupérer les résultats de l'inférence
  for (int i = 0; i < 1; ++i) {
    output_values[i] = output->data.f[i];  // Récupérer les résultats directement en float
  }

  // Afficher ou traiter les résultats
  // HandleOutput(output_values[0], output_values[1], output_values[2]);
  return ESP_OK;
}
