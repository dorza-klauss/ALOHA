#ifndef RANDOM_NUMBER_GENERATOR_HPP
#define RANDOM_NUMBER_GENERATOR_HPP

using namespace std;

#include <iostream>
#include <cmath>
#include <fstream>

class Random_Number_Generator{
    double seed;
    double rate;
    double previous_number;

    //LCG values
    int M; //2^31-1 (2147483648) (modulus)
    int a; //8*8192+3=65539 (Multiplier)

    public:

        Random_Number_Generator(): seed(1234.0), rate(-1), previous_number(1),
                                    M(2147483648), a(65539)
            {
                //initialize random seed
                srand ((int)seed);
            }

            //getters and setters
            void set_seed(double d) { seed = d; srand((int)seed); previous_number=seed;};
            double get_seed() const {return seed;};
            void set_rate(double d) {rate = d;};
            double get_rate() const {return rate;};

            void output(){

                cout<<"Random number generator with seed: "<<seed<<" and rate: "<<rate<<endl;

            };

            double next(){
                previous_number = fmod((a*previous_number), M);
                return previous_number;
            }

            double next_normalized(){
                previous_number = fmod((a*previous_number), M);
                return -previous_number/M;
            }

            double exponential(double n){
                //calculating F'(x) - using inverse transformation of exponential pdf
                return (-1/rate)*log(1-n);
            }

            double exponential(){
                return (-1/rate)*log(1-next_normalized());
            }

            double rand_exponential(){
                double rnd = rand()/(double)RAND_MAX;
                return (-1/rate)*log(1-rnd);
            }




};


#endif