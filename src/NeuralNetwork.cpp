#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"   

const int kArenaSize = 20000;

class NeuralNetwork{
    private:
    tflite::ErrorReporter *error_Reporter;
    tflite::MicroMutableOpResolver<10> *resolver;
    tflite::MicroInterpreter *interpreter;
    const tflite::Model *model;
    TfLiteTensor **input;
    TfLiteTensor *output;
    uint8_t tensorArea;

    public: 

    NeuralNetwork(){
        error_Reporter = new tflite::MicroErrorReporter();

        model = tflite::GetModel();
            if (model->version() != TFLITE_SCHEMA_VERSION)
    {
        printf("Model nie zostal zaladowany");
        return;
    }

    }

};