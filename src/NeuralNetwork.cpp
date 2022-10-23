#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "MnistModel.h"

const int kArenaSize = 70*1024;

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

        model = tflite::GetModel(TfLiteModel_tflite);
        if (model->version() != TFLITE_SCHEMA_VERSION)
        {
            TF_LITE_REPORT_ERROR(errorReporter, "Model provided is schema version %d not equal to supported version %d.",
                                model->version(), TFLITE_SCHEMA_VERSION);
            return;
        }

        resolver = new tflite::MicroMutableOpResolver<10>();
        resolver->AddFullyConnected();
        resolver->AddSoftmax();


        tensorArea = (uint8_t *)malloc(kArenaSize);
        if (!tensorArea)
        {
            TF_LITE_REPORT_ERROR(errorReporter, "Could not allocate arena");
            return;
        }

        interpreter = new tflite::MicroInterpreter(
            model, *resolver, tensorArea, kArenaSize, errorReporter);

        TfLiteStatus allocate_status = interpreter->AllocateTensors();
        if (allocate_status != kTfLiteOk)
        {
            TF_LITE_REPORT_ERROR(errorReporter, "AllocateTensors() failed");
            return;
        }

        size_t used_bytes = interpreter->arena_used_bytes();
        TF_LITE_REPORT_ERROR(errorReporter, "Used bytes %d\n", used_bytes);

        input = interpreter->input(0);
        output = interpreter->output(0);
    }

    float* getInputBuffer(){
        return input->data.f;
    }

    float* predict(){
        interpreter->Invoke();
        return output->data.f;
    }

};