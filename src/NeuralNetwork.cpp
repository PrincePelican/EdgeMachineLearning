#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "MnistModel.h"

const int kArenaSize = 20*1024;

class NeuralNetwork{
    private:
    tflite::ErrorReporter *errorReporter;
    tflite::MicroMutableOpResolver<10> *resolver;
    tflite::MicroInterpreter *interpreter;
    const tflite::Model *model;
    TfLiteTensor *input;
    TfLiteTensor *output;
    uint8_t *tensorArea;

    public: 

    NeuralNetwork(){
        
        errorReporter = new tflite::MicroErrorReporter();

        tensorArea = (uint8_t*)malloc(kArenaSize);
        if (!tensorArea)
        {   
            errorReporter->Report("Could not allocate arena");
            return;
        }
    

        model = tflite::GetModel(TfLiteModel_tflite);

        if (model->version() != TFLITE_SCHEMA_VERSION){
            errorReporter->Report("Model didnt load");
            return;
        }

        resolver = new tflite::MicroMutableOpResolver<10>();

        resolver->AddFullyConnected();
        resolver->AddMul();
        resolver->AddAdd();
        resolver->AddLogistic();
        resolver->AddReshape();
        resolver->AddQuantize();
        resolver->AddDequantize();

        interpreter = new tflite::MicroInterpreter(model, *resolver, tensorArea, kArenaSize, errorReporter);

        TfLiteStatus tfliteStatus = interpreter->AllocateTensors();

        if(tfliteStatus = kTfLiteOk){
            errorReporter->Report("Allocate failed");
        }

        input = interpreter->input(0);
        output = interpreter->output(0);
    }

    float* getInputBuffer(){
        return input->data.f;
    }

    float predict(){
        interpreter->Invoke();
        return output->data.f[0];
    }

};