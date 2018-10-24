#include "Activation_Functions.h"
#include "Normalizer.h"
#include "Neural_Layer.h"
#include "Neural_Trainer.h"
#include "CSV_Importer.h"
#include <iostream>
#include <chrono>

int main(int argc, char *argv[]) {
  // Number of inputs
  int inp = 10;
  // Number of outputs
  int out = 2;

  // Create Layers
  std::shared_ptr<Neural_Layer> layer1 = std::make_shared<Neural_Layer>(10, inp, Logistic_Function);
  std::shared_ptr<Neural_Layer> layer2 = std::make_shared<Neural_Layer>(out, 10, layer1);

  // Import Data
  std::string path = "https://s3.amazonaws.com/files.themadphysicist.com/magic_data.csv";
  std::cout << "Downloading data from: " << path << std::endl;
  CSV_Importer imp(path, inp, out);
  std::vector<Evector> samples = imp.GetSamples();
  std::vector<Evector> targets = imp.GetTargets();

  // Normalize Data
  Normalizer samplen(samples, 0, 1);
  std::vector<Evector> normed_samples = samplen.get_batch_norm(samples);

  // Create Derivative Function Vector
  std::vector<function> derv_funs;
  derv_funs.push_back(Logistic_Function_D);
  derv_funs.push_back(Identity_Function_D);

  // Create Trainer
  Neural_Trainer trainer(layer2, derv_funs);

  // Train
  std::cout << "Starting to Train" << std::endl;
  auto start1 = std::chrono::steady_clock::now();
  for (int i = 0, sizei = 100; i < sizei; i++) {
    trainer.train_minibatch(normed_samples, targets, 1000);
  }
  auto end1 = std::chrono::steady_clock::now();

  // Calculate error
  float mse = layer2->mse(normed_samples, targets);
  float rmse = layer2->rmse(normed_samples, targets);
  float mae = layer2->mae(normed_samples, targets);
  float mpe = layer2->mpe(normed_samples, targets);   // mpe and mape return -nan and inf
  float mape = layer2->mape(normed_samples, targets); //  since targets contain zeros
  float r2 = layer2->r2(normed_samples, targets);
  auto end2 = std::chrono::steady_clock::now();

  std::cout << "MSE:            " << mse << std::endl;
  std::cout << "RMSE:           " << rmse << std::endl;
  std::cout << "MAE:            " << mae << std::endl;
  std::cout << "MPE:            " << mpe << std::endl;
  std::cout << "MAPE:           " << mape << std::endl;
  std::cout << "R^2:            " << r2 << std::endl;
  std::cout << "It took         " << std::chrono::duration_cast<std::chrono::seconds>(end1 - start1).count()
            << " seconds"
            << std::endl;
  std::cout << "Statistics took " << std::chrono::duration_cast<std::chrono::milliseconds>(end2 - end1).count()
            << " milliseconds";
  return (0.5 < mse);
}